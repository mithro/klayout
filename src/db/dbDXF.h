
/*

  KLayout Layout Viewer
  Copyright (C) 2006-2016 Matthias Koefferlein

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


#ifndef HDR_dbDXF
#define HDR_dbDXF

#include "dbPoint.h"

#include "tlException.h"
#include "tlInternational.h"
#include "tlString.h"
#include "tlAssert.h"

#include <string>
#include <vector>

//  place this macro to force linking of DXF plugin
#define FORCE_LINK_DXF void force_link_DXF_f () { extern int force_link_DXF; force_link_DXF = 0; }

namespace db
{

/**
 *  @brief The diagnostics interface for reporting problems in the reader or writer
 */
class DXFDiagnostics
{
public:
  virtual ~DXFDiagnostics ();

  /**
   *  @brief Issue an error with positional informations
   */
  virtual void error (const std::string &txt) = 0;

  /**
   *  @brief Issue a warning with positional informations
   */
  virtual void warn (const std::string &txt) = 0;
};

}

#endif
