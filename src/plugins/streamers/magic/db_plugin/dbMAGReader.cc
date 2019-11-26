
/*

  KLayout Layout Viewer
  Copyright (C) 2006-2019 Matthias Koefferlein

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/



#include "dbMAGReader.h"
#include "dbStream.h"
#include "dbObjectWithProperties.h"
#include "dbArray.h"
#include "dbStatic.h"
#include "dbShapeProcessor.h"

#include "tlException.h"
#include "tlString.h"
#include "tlClassRegistry.h"
#include "tlFileUtils.h"
#include "tlUri.h"

#include <cctype>
#include <string>

namespace db
{

// ---------------------------------------------------------------
//  MAGReader


MAGReader::MAGReader (tl::InputStream &s)
  : m_stream (s),
    m_progress (tl::to_string (tr ("Reading MAG file")), 1000),
    m_lambda (1.0), m_dbu (0.001), m_merge (true)
{
  m_progress.set_format (tl::to_string (tr ("%.0fk lines")));
  m_progress.set_format_unit (1000.0);
  m_progress.set_unit (100000.0);

  mp_current_stream = 0;
}

MAGReader::~MAGReader ()
{
  //  .. nothing yet ..
}

const LayerMap &
MAGReader::read (db::Layout &layout)
{
  return read (layout, db::LoadLayoutOptions ());
}

const LayerMap &
MAGReader::read (db::Layout &layout, const db::LoadLayoutOptions &options)
{
  prepare_layers ();

  const db::MAGReaderOptions &specific_options = options.get_options<db::MAGReaderOptions> ();
  m_lambda = specific_options.lambda;
  m_dbu = specific_options.dbu;
  m_lib_paths = specific_options.lib_paths;
  m_merge = specific_options.merge;
  mp_current_stream = 0;

  db::LayerMap lm = specific_options.layer_map;
  lm.prepare (layout);
  set_layer_map (lm);
  set_create_layers (specific_options.create_other_layers);
  set_keep_layer_names (specific_options.keep_layer_names);

  db::cell_index_type top_cell = layout.add_cell (cell_name_from_path (m_stream.source ()).c_str ());

  layout.dbu (m_dbu);

  m_cells_to_read.clear ();
  m_cells_read.clear ();
  m_use_lib_paths.clear ();
  m_dbu_trans_inv = db::CplxTrans (m_dbu).inverted ();
  m_tech.clear ();

  {
    tl::SelfTimer timer (tl::verbosity () >= 21, "Reading MAGIC file tree");

    //  This is the seed
    do_read (layout, top_cell, m_stream);

    while (! m_cells_to_read.empty ()) {

      std::pair<std::string, std::pair<std::string, db::cell_index_type> > next = *m_cells_to_read.begin ();
      m_cells_to_read.erase (m_cells_to_read.begin ());

      tl::InputStream stream (next.second.first);
      tl::TextInputStream text_stream (stream);
      do_read (layout, next.second.second, text_stream);

    }
  }

  finish_layers (layout);
  return layer_map ();
}

void 
MAGReader::error (const std::string &msg)
{
  throw MAGReaderException (msg, mp_current_stream->line_number (), mp_current_stream->source ());
}

void 
MAGReader::warn (const std::string &msg)
{
  // TODO: compress
  tl::warn << msg 
           << tl::to_string (tr (" (line=")) << mp_current_stream->line_number ()
           << tl::to_string (tr (", file=")) << mp_current_stream->source ()
           << ")";
}

db::cell_index_type
MAGReader::cell_from_path (const std::string &path, db::Layout &layout)
{
  std::map<std::string, db::cell_index_type>::const_iterator c = m_cells_read.find (path);
  if (c != m_cells_read.end ()) {
    return c->second;
  }

// @@@ this can lead to cell variants if a cell is present with different library paths ... (L500_CHAR_p)
  db::cell_index_type ci = layout.add_cell (cell_name_from_path (path).c_str ());
  m_cells_read.insert (std::make_pair (path, ci));

  std::string cell_file;
  if (! resolve_path (path, cell_file)) {
    //  skip with a warning if the file can't be opened (TODO: better to raise an error?)
    tl::warn << tl::to_string (tr ("Unable to find a layout file for cell - skipping this cell: ")) << path;
    layout.cell (ci).set_ghost_cell (true);
  } else {
    m_cells_to_read.insert (std::make_pair (path, std::make_pair (cell_file, ci)));
  }

  return ci;
}

std::string
MAGReader::cell_name_from_path (const std::string &path)
{
  std::string file = tl::split (path, "/").back ();
  return tl::split (file, ".").front ();
}

static bool find_and_normalize_file (const tl::URI &uri, std::string &path)
{
  //  TODO: sync with plugin definition
  static const char *extensions[] = {
    ".mag", ".mag.gz", ".MAG", ".MAG.gz"
  };

  for (size_t e = 0; e < sizeof (extensions) / sizeof (extensions [0]); ++e) {

    if (uri.scheme ().empty () || uri.scheme () == "file") {

      std::string fp = uri.path () + extensions[e];

      if (tl::verbosity () >= 30) {
        tl::log << tl::to_string (tr ("Trying layout file: ")) << fp;
      }

      if (tl::file_exists (fp)) {
        path = fp;
        return true;
      }

    } else {

      //  TODO: this is not quite efficient, but the only thing we can do for now
      tl::URI uri_with_ext = uri;
      uri_with_ext.set_path (uri_with_ext.path () + extensions[e]);
      std::string us = uri_with_ext.to_string ();

      if (tl::verbosity () >= 30) {
        tl::log << tl::to_string (tr ("Trying layout URI: ")) << us;
      }

      try {
        tl::InputStream is (us);
        if (is.get (1)) {
          path = us;
          return true;
        }
      } catch (...) {
        //  .. nothing yet ..
      }

    }

  }

  return false;
}

bool
MAGReader::resolve_path (const std::string &path, std::string &real_path)
{
  tl::Eval expr;
/* @@@
  expr.set_var ("tech_dir", m_default_base_path);
  expr.set_var ("tech_file", m_lyt_file);
  expr.set_var ("tech_name", m_lyt_file);
*/
  expr.set_var ("tech_info", m_tech);

  tl::URI path_uri (path);

  //  absolute URIs are kept - we just try to figure out the suffix
  if (tl::is_absolute (path_uri.path ())) {
    return find_and_normalize_file (path_uri, real_path);
  }

  tl::URI source_uri (mp_current_stream->source ());
  source_uri.set_path (tl::dirname (source_uri.path ()));

  //  first attempt: try relative to source
  if (find_and_normalize_file (source_uri.resolved (tl::URI (path)), real_path)) {
    return true;
  }

  //  then try relative to library paths
  for (std::vector<std::string>::const_iterator lp = m_lib_paths.begin (); lp != m_lib_paths.end (); ++lp) {
    std::string lib_path = expr.interpolate (*lp);
    if (find_and_normalize_file (source_uri.resolved (tl::URI (lib_path).resolved (tl::URI (path))), real_path)) {
      return true;
    }
  }

  return false;
}

void
MAGReader::do_read (db::Layout &layout, db::cell_index_type cell_index, tl::TextInputStream &stream)
{
  try {

    mp_current_stream = &stream;
    do_read_part (layout, cell_index, stream);

    if (m_merge) {
      do_merge_part (layout, cell_index);
    }

  } catch (tl::Exception &ex) {
    error (ex.msg ());
  }
}

void 
MAGReader::do_read_part (db::Layout &layout, db::cell_index_type cell_index, tl::TextInputStream &stream)
{
  tl::SelfTimer timer (tl::verbosity () >= 31, "File read");

  if (tl::verbosity () >= 30) {
    tl::log << "Reading layout file: " << stream.source ();
  }

  std::string l = stream.get_line ();
  if (l != "magic") {
    error (tl::to_string (tr ("Could not find 'magic' header line - is this a MAGIC file?")));
  }

  bool valid_layer = false;
  unsigned int current_layer = 0;
  bool in_labels = false;

  while (! stream.at_end ()) {

    std::string l = stream.get_line ();
    tl::Extractor ex (l.c_str ());

    if (ex.at_end () || ex.test ("#")) {

      //  skip empty lines and comments
      continue;

    } else if (ex.test ("tech")) {

      ex.read_word_or_quoted (m_tech);

      if (&m_stream == &stream) {
        //  initial file - store technology
        layout.add_meta_info (db::MetaInfo ("magic_tech", "MAGIC technology string", m_tech));
      }

      ex.expect_end ();

    } else if (ex.test ("timestamp")) {

      size_t ts = 0;
      ex.read (ts);

      if (&m_stream == &stream) {
        //  initial file - store technology
        layout.add_meta_info (db::MetaInfo ("magic_timestamp", "MAGIC main file timestamp", tl::to_string (ts)));
      }

      ex.expect_end ();

    } else if (ex.test ("<<")) {

      std::string lname;
      ex.read_word_or_quoted (lname);

      if (lname == "end") {
        in_labels = false;
        valid_layer = false;
      } else if (lname == "labels") {
        in_labels = true;
      } else {
        in_labels = false;
        std::pair<bool, unsigned int> ll = open_layer (layout, lname);
        valid_layer = ll.first;
        current_layer = ll.second;
      }

      ex.expect (">>");
      ex.expect_end ();

    } else if (ex.test ("rect")) {

      if (in_labels) {
        error (tl::to_string (tr ("'rect' statement inside labels section")));
      } else if (valid_layer) {
        read_rect (ex, layout, cell_index, current_layer);
      }

    } else if (ex.test ("rlabel")) {

      if (! in_labels) {
        error (tl::to_string (tr ("'rlabel' statement outside labels section")));
      } else {
        read_rlabel (ex, layout, cell_index);
      }

    } else if (ex.test ("use")) {

      read_cell_instance (ex, stream, layout, cell_index);

    }

  }
}

void
MAGReader::do_merge_part (Layout &layout, cell_index_type cell_index)
{
  tl::SelfTimer timer (tl::verbosity () >= 31, "Merge step");

  db::Cell &cell = layout.cell (cell_index);
  db::ShapeProcessor sp;
  if (tl::verbosity () >= 40) {
    sp.enable_progress (tl::to_string (tr ("Merging shapes for MAG reader")));
  }

  for (db::Layout::layer_iterator l = layout.begin_layers (); l != layout.end_layers (); ++l) {
    unsigned int li = (unsigned int) (*l).first;
    sp.merge (layout, cell, li, cell.shapes (li), false);
  }
}

void
MAGReader::read_rect (tl::Extractor &ex, Layout &layout, cell_index_type cell_index, unsigned int layer)
{
  double l, b, r, t;
  ex.read (l);
  ex.read (b);
  ex.read (r);
  ex.read (t);
  ex.expect_end ();

  db::DBox box (l, b, r, t);
  layout.cell (cell_index).shapes (layer).insert ((box * m_lambda).transformed (m_dbu_trans_inv));
}

void
MAGReader::read_rlabel (tl::Extractor &ex, Layout &layout, cell_index_type cell_index)
{
  std::string lname;
  ex.read (lname);

  double l, b, r, t;
  ex.read (l);
  ex.read (b);
  ex.read (r);
  ex.read (t);

  int pos = 0;
  ex.read (pos);

  ex.skip ();
  db::DText text (db::DTrans (), *ex);

  double x = 0.5 * (l + r);
  double y = 0.5 * (b + t);
  if (pos == 2 || pos == 3 || pos == 4) {
    x = r;
  } else if (pos == 6 || pos == 7 || pos == 8) {
    x = l;
  }
  if (pos == 1 || pos == 2 || pos == 8) {
    y = t;
  } else if (pos == 4 || pos == 5 || pos == 6) {
    y = b;
  }

  text.move (db::DVector (x, y));

  if (lname != "space") {   //  really? "space"?
    std::pair<bool, unsigned int> ll = open_layer (layout, lname);
    if (ll.first) {
      layout.cell (cell_index).shapes (ll.second).insert ((text * m_lambda).transformed (m_dbu_trans_inv));
    }
  }
}

void
MAGReader::read_cell_instance (tl::Extractor &ex, tl::TextInputStream &stream, Layout &layout, cell_index_type cell_index)
{
  const char *include_chars_in_files = "$_,.-$+#:;[]()<>|/\\";

  std::string filename, use_id, lib_path;
  ex.read_word_or_quoted (filename, include_chars_in_files);
  if (! ex.at_end ()) {
    ex.read_word_or_quoted (use_id);
  }
  if (! ex.at_end ()) {
    ex.read_word_or_quoted (lib_path, include_chars_in_files);
  }

  if (lib_path.empty ()) {
    std::map<std::string, std::string>::const_iterator lp = m_use_lib_paths.find (filename);
    if (lp != m_use_lib_paths.end ()) {
      lib_path = lp->second;
    }
  } else {
    //  give precendence to lib_path
    filename = tl::filename (filename);
    //  save for next use
    m_use_lib_paths.insert (std::make_pair (filename, lib_path));
  }

  if (! lib_path.empty ()) {
    //  NOTE: we don't use the system separator because it looks like MAG files use "/".
    filename = lib_path + "/" + filename;
  }

  //  read more lines until box

  db::DVector a, b, p;
  unsigned long na = 1, nb = 1;

  db::DCplxTrans trans;

  while (! stream.at_end ()) {

    std::string l = stream.get_line ();
    tl::Extractor ex2 (l.c_str ());

    if (ex2.at_end () || ex2.test ("#")) {
      continue;
    } else if (ex2.test ("array")) {

      int xlo = 0, xhi = 0, ylo = 0, yhi = 0;
      double xsep = 0.0, ysep = 0.0;

      ex2.read (xlo);
      ex2.read (xhi);
      ex2.read (xsep);

      ex2.read (ylo);
      ex2.read (yhi);
      ex2.read (ysep);

      na = (unsigned long) std::max (0, xhi - xlo + 1);
      a = db::DVector (xsep, 0);
      nb = (unsigned long) std::max (0, yhi - ylo + 1);
      b = db::DVector (ysep, 0);

    } else if (ex2.test ("timestamp")) {
      //  ignored
    } else if (ex2.test ("transform")) {

      double m11 = 0.0, m12 = 0.0, m21 = 0.0, m22 = 0.0;
      double dx = 0.0, dy = 0.0;

      ex2.read (m11);
      ex2.read (m12);
      ex2.read (dx);
      ex2.read (m21);
      ex2.read (m22);
      ex2.read (dy);

      trans = db::DCplxTrans (db::Matrix2d (m11, m12, m21, m22), db::DVector (dx, dy));

    } else if (ex2.test ("box")) {
      //  ignored
      break;
    }

  }

  //  create the instance

  db::cell_index_type ci = cell_from_path (filename, layout);

  db::ICplxTrans itrans = m_dbu_trans_inv * trans * db::CplxTrans (m_dbu);

  if (na == 1 && nb == 1) {
    layout.cell (cell_index).insert (db::CellInstArray (db::CellInst (ci), itrans));
  } else {
    layout.cell (cell_index).insert (db::CellInstArray (db::CellInst (ci), itrans, m_dbu_trans_inv * a, m_dbu_trans_inv * b, na, nb));
  }
}

}
