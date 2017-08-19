
/*

  KLayout Layout Viewer
  Copyright (C) 2006-2017 Matthias Koefferlein

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

#include "bdWriterOptions.h"
#include "dbLayout.h"
#include "dbSaveLayoutOptions.h"
#include "tlCommandLineParser.h"
#include "tlGlobPattern.h"

namespace bd
{

GenericWriterOptions::GenericWriterOptions ()
  : m_scale_factor (1.0), m_dbu (0.0),
    m_dont_write_empty_cells (false), m_keep_instances (false), m_write_context_info (false)
{
  //  .. nothing yet ..
}

void
GenericWriterOptions::add_options (tl::CommandLineOptions &cmd, const std::string &format)
{
  const std::string gds2_format_name = m_gds2_writer_options.format_name ();
  const std::string gds2text_format_name = gds2_format_name + "Text";  //  no special options
  const std::string oasis_format_name = m_oasis_writer_options.format_name ();
  const std::string dxf_format_name = m_dxf_writer_options.format_name ();
  const std::string cif_format_name = m_cif_writer_options.format_name ();

  std::string group ("[Output options - General]");

  cmd << tl::arg (group +
                  "-os|--scale-factor=factor", &m_scale_factor, "Scales the layout upon writing",
                  "Specifies layout scaling. If given, the saved layout will be scaled by the "
                  "given factor."
                 );

  if (format.empty () || format == gds2_format_name || format == gds2text_format_name || format == oasis_format_name) {
    cmd << tl::arg (group +
                    "-od|--dbu-out=dbu",    &m_dbu, "Uses the specified database unit",
                    "Specifies the database unit to save the layout in. The database unit is given "
                    "in micron units. By default, the original unit is used. The layout will not "
                    "change physically because internally, the coordinates are scaled to match the "
                    "new database unit."
                   );
  }

  cmd << tl::arg (group +
                  "#--drop-empty-cells",    &m_dont_write_empty_cells, "Drops empty cells",
                  "If given, empty cells won't be written. See --keep-instances for more options."
                 );

  if (format.empty () || format == gds2_format_name || format == gds2text_format_name) {
    cmd << tl::arg (group +
                    "#--keep-instances",      &m_keep_instances, "Keeps instances of dropped cells",
                    "If given, instances of dropped cell's won't be removed. Hence, ghost cells are "
                    "produced. The resulting layout may not be readable by consumers that require "
                    "all instantiated cells to be present as actual cells.\n"
                    "Dropped cells are those which are removed by a negative cell selection (see "
                    "--write-cells) "
                   );
  }

  if (format.empty () || format == gds2_format_name || format == gds2text_format_name || format == oasis_format_name) {
    cmd << tl::arg (group +
                    "#--write-context-info",  &m_write_context_info, "Writes context information",
                    "Include context information for PCell instances and other information in a format-specific "
                    "way. The resulting layout may show unexpected features for other consumers."
                   );
  }

  cmd << tl::arg (group +
                  "#--write-cells=sel",       &m_cell_selection, "Specifies cells to write",
                  "This option specifies the cells to write. The value of this option is a sequence of "
                  "positive and negative cell select operations. "
                  "A select operation is an optional plus (+) or minus sign (-), followed by "
                  "a cell filter. By default a select operation is positive, with a minus sign, the "
                  "select operation is negative and will unselect the matching cells."
                  "A cell filter is a plain cell name or a glob pattern (using '*' and '?' for placeholders). "
                  "If a cell filter is enclosed in round brackets, it will apply only to the matching cells. "
                  "Otherwise it will apply to these cells plus their children.\n"
                  "\n"
                  "Multiple operations can be specified by combining them with a comma. "
                  "Positive and negative selection happens in the order given. Hence it's possible "
                  "to select a cell with it's children and then unselect some children of this cell.\n"
                  "\n"
                  "Examples:\n\n"
                  "* \"TOP1,TOP2\" - Select cells TOP1 and TOP2 with all of their children\n"
                  "* \"(TOP)\" - Select only cell TOP, but none of it's child cells\n"
                  "* \"TOP,-A\" - Select cell TOP (plus children), then remove A (with children)"
                 );

  if (format.empty () || format == gds2_format_name || format == gds2text_format_name) {

    //  Add GDS2 and GDS2Text format options
    std::string group = "[Output options - GDS2 specific]";

    cmd << tl::arg (group +
                    "-ov|--max-vertex-count=count", &m_gds2_writer_options.max_vertex_count, "Specifies the maximum number of points per polygon",
                    "If this number is given, polygons are cut into smaller parts if they have more "
                    "than the specified number of points. If not given, the maximum number of points will be used. "
                    "This is 8190 unless --multi-xy-records is given."
                   )
        << tl::arg (group +
                    "#--multi-xy-records", &m_gds2_writer_options.multi_xy_records, "Allows unlimited number of points",
                    "If this option is given, multiple XY records will be written to accomodate an unlimited number "
                    "of points per polygon or path. However, such files may not be compatible with some consumers."
                   )
        << tl::arg (group +
                    "#--no-zero-length-paths", &m_gds2_writer_options.no_zero_length_paths, "Converts zero-length paths to polygons",
                    "If this option is given, zero-length paths (such with one point) are not written as paths "
                    "but converted to polygons. This avoids compatibility issues with consumers of this layout file."
                   )
        << tl::arg (group +
                    "-on|--cellname-length=length", &m_gds2_writer_options.max_cellname_length, "Limits cell names to the given length",
                    "If this option is given, long cell names will truncated if their length exceeds the given length."
                   )
        << tl::arg (group +
                    "-ol|--libname=libname", &m_gds2_writer_options.libname, "Uses the given library name",
                    "This option can specify the GDS2 LIBNAME for the output file. By default, the original LIBNAME is "
                    "written."
                   )
        << tl::arg (group +
                    "#--user-units=unit", &m_gds2_writer_options.user_units, "Specifies the user unit to use",
                    "Specifies the GDS2 user unit. By default micrometers are used for the user unit."
                   )
        << tl::arg (group +
                    "#!--no-timestamps", &m_gds2_writer_options.write_timestamps, "Don't write timestamps",
                    "Writes a dummy time stamp instead of the actual time. With this option, GDS2 files become "
                    "bytewise indentical even if written at different times. This option is useful if binary "
                    "identity is important (i.e. in regression scenarios)."
                   )
        << tl::arg (group +
                    "#--write-cell-properties", &m_gds2_writer_options.write_cell_properties, "Write cell properties",
                    "This option enables a GDS2 extension that allows writing of cell properties to GDS2 files. "
                    "Consumers that don't support this feature, may not be able to read such a GDS2 files."
                   )
        << tl::arg (group +
                    "#--write-file-properties", &m_gds2_writer_options.write_file_properties, "Write file properties",
                    "This option enables a GDS2 extension that allows writing of file properties to GDS2 files. "
                    "Consumers that don't support this feature, may not be able to read such a GDS2 files."
                   )
      ;

  }

  if (format.empty () || format == oasis_format_name) {

    //  Add OASIS format options
    std::string group = "[Output options - OASIS specific]";

    cmd << tl::arg (group +
                    "-ok|--compression-level=level", &m_oasis_writer_options.compression_level, "Specifies the OASIS compression level",
                    "This level describes how hard the OASIS writer will try to compress the shapes "
                    "using shape arrays. Building shape arrays may take some time and requires some memory.\n"
                    "* 0 - no shape array building\n"
                    "* 1 - nearest neighbor shape array formation\n"
                    "* 2++ - enhanced shape array search algorithm using 2nd and further neighbor distances as well\n"
                   )
        << tl::arg (group +
                    "-ob|--cblocks", &m_oasis_writer_options.write_cblocks, "Uses CBLOCK compression"
                   )
        << tl::arg (group +
                    "-ot|--strict-mode", &m_oasis_writer_options.strict_mode, "Uses strict mode"
                   )
        << tl::arg (group +
                    "#--recompress", &m_oasis_writer_options.recompress, "Compresses shape arrays again",
                    "With this option, shape arrays will be expanded and recompressed. This may result in a better "
                    "compression ratio, but at the cost of slower execution."
                   )
        << tl::arg (group +
                    "#--write-std-properties", &m_oasis_writer_options.write_std_properties, "Writes some global standard properties",
                    "This is an integer describing what standard properties shall be written. 0 is \"none\" (the default), "
                    "1 means \"global standard properties such as S_TOP_CELL\" are produced. With 2 also per-cell bounding "
                    "boxes are produced."
                   )
        << tl::arg (group +
                    "#--subst-char=char", this, &GenericWriterOptions::set_oasis_substitution_char, "Specifies the substitution character for non-standard characters",
                    "The first character of the string specified with this option will be used in placed of illegal "
                    "characters in n-strings and a-strings."
                   )
      ;

  }

  if (format.empty () || format == dxf_format_name) {

    //  Add DXF format options
    std::string group = "[Output options - DXF specific]";

    cmd << tl::arg (group +
                    "-op|--polygon-mode=mode", &m_dxf_writer_options.polygon_mode, "Specifies how to write polygons",
                    "This option specifies how to write polygons:\n"
                    "* 0: create POLYLINE\n"
                    "* 1: create LWPOLYLINE\n"
                    "* 2: decompose into SOLID\n"
                    "* 3: create HATCH"
                   )
      ;

  }

  if (format.empty () || format == cif_format_name) {

    //  Add CIF format options
    std::string group = "[Output options - CIF specific]";

    cmd << tl::arg (group +
                    "#--dummy-calls",         &m_cif_writer_options.dummy_calls,       "Produces dummy calls",
                    "If this option is given, the writer will produce dummy cell calls on global level for all top cells"
                   )
        << tl::arg (group +
                    "#--blank-separator",     &m_cif_writer_options.blank_separator,   "Uses blanks as x/y separators",
                    "If this option is given, blank characters will be used to separate x and y values. "
                    "Otherwise comma characters will be used.\n"
                    "Use this option if your CIF consumer cannot read comma characters as x/y separators."
                   )
      ;

  }
}

void GenericWriterOptions::set_oasis_substitution_char (const std::string &text)
{
  if (! text.empty ()) {
    m_oasis_writer_options.subst_char = text[0];
  }
}

static void get_selected_cells (tl::Extractor &ex, const db::Layout &layout, std::set<db::cell_index_type> &selected)
{
  while (! ex.at_end ()) {

    bool remove = ex.test ("-");
    ex.test ("+");

    bool without_children = ex.test ("(");
    std::string filter;
    ex.read_word_or_quoted (filter, "_-.*?{}$[]");

    if (without_children) {
      ex.expect (")");
    }

    if (! ex.at_end ()) {
      ex.expect (",");
    }

    tl::GlobPattern pat (filter);
    for (db::Layout::const_iterator c = layout.begin (); c != layout.end (); ++c) {

      if (pat.match (layout.cell_name (c->cell_index ()))) {

        std::set<db::cell_index_type> cells;
        cells.insert (c->cell_index ());
        if (! without_children) {
          c->collect_called_cells (cells);
        }

        if (! remove) {
          selected.insert (cells.begin (), cells.end ());
        } else {
          selected.erase (cells.begin (), cells.end ());
        }

      }

    }

  }
}

void
GenericWriterOptions::configure (db::SaveLayoutOptions &save_options, const db::Layout &layout) const
{
  save_options.set_scale_factor (m_scale_factor);
  save_options.set_dbu (m_dbu);
  save_options.set_dont_write_empty_cells (m_dont_write_empty_cells);
  save_options.set_keep_instances (m_keep_instances);
  save_options.set_write_context_info (m_write_context_info);

  save_options.set_options (m_gds2_writer_options);
  save_options.set_options (m_oasis_writer_options);
  save_options.set_options (m_dxf_writer_options);
  save_options.set_options (m_cif_writer_options);

  if (!m_cell_selection.empty ()) {

    std::set<db::cell_index_type> selected;
    tl::Extractor ex (m_cell_selection.c_str ());
    get_selected_cells (ex, layout, selected);

    save_options.clear_cells ();
    for (std::set<db::cell_index_type>::const_iterator s = selected.begin (); s != selected.end (); ++s) {
      save_options.add_this_cell (*s);
    }

  }
}

}