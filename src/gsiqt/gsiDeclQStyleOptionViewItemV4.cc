
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

/**
*  @file gsiDeclQStyleOptionViewItemV4.cc 
*
*  DO NOT EDIT THIS FILE. 
*  This file has been created automatically
*/

#include <QStyleOptionViewItemV4>
#include <QStyleOptionViewItem>
#include <QStyleOptionViewItemV2>
#include <QWidget>
#include "gsiQt.h"
#include "gsiQtCommon.h"
#include "gsiDeclQtTypeTraits.h"
#include <memory>

// -----------------------------------------------------------------------
// class QStyleOptionViewItemV4

//  Constructor QStyleOptionViewItemV4::QStyleOptionViewItemV4()


static void _init_ctor_QStyleOptionViewItemV4_0 (qt_gsi::GenericStaticMethod *decl)
{
  decl->set_return_new<QStyleOptionViewItemV4> ();
}

static void _call_ctor_QStyleOptionViewItemV4_0 (const qt_gsi::GenericStaticMethod * /*decl*/, gsi::SerialArgs &args, gsi::SerialArgs &ret) 
{
  __SUPPRESS_UNUSED_WARNING(args);
  ret.write<QStyleOptionViewItemV4 *> (new QStyleOptionViewItemV4 ());
}


//  Constructor QStyleOptionViewItemV4::QStyleOptionViewItemV4(const QStyleOptionViewItemV4 &other)


static void _init_ctor_QStyleOptionViewItemV4_3504 (qt_gsi::GenericStaticMethod *decl)
{
  static gsi::ArgSpecBase argspec_0 ("other");
  decl->add_arg<const QStyleOptionViewItemV4 & > (argspec_0);
  decl->set_return_new<QStyleOptionViewItemV4> ();
}

static void _call_ctor_QStyleOptionViewItemV4_3504 (const qt_gsi::GenericStaticMethod * /*decl*/, gsi::SerialArgs &args, gsi::SerialArgs &ret) 
{
  __SUPPRESS_UNUSED_WARNING(args);
  tl::Heap heap;
  const QStyleOptionViewItemV4 &arg1 = args.read<const QStyleOptionViewItemV4 & > (heap);
  ret.write<QStyleOptionViewItemV4 *> (new QStyleOptionViewItemV4 (arg1));
}


//  Constructor QStyleOptionViewItemV4::QStyleOptionViewItemV4(const QStyleOptionViewItem &other)


static void _init_ctor_QStyleOptionViewItemV4_3366 (qt_gsi::GenericStaticMethod *decl)
{
  static gsi::ArgSpecBase argspec_0 ("other");
  decl->add_arg<const QStyleOptionViewItem & > (argspec_0);
  decl->set_return_new<QStyleOptionViewItemV4> ();
}

static void _call_ctor_QStyleOptionViewItemV4_3366 (const qt_gsi::GenericStaticMethod * /*decl*/, gsi::SerialArgs &args, gsi::SerialArgs &ret) 
{
  __SUPPRESS_UNUSED_WARNING(args);
  tl::Heap heap;
  const QStyleOptionViewItem &arg1 = args.read<const QStyleOptionViewItem & > (heap);
  ret.write<QStyleOptionViewItemV4 *> (new QStyleOptionViewItemV4 (arg1));
}


// (const QStyleOptionViewItem &)


static void _init_f_operator_eq__3366 (qt_gsi::GenericMethod *decl)
{
  static gsi::ArgSpecBase argspec_0 ("other");
  decl->add_arg<const QStyleOptionViewItem & > (argspec_0);
  decl->set_return<QStyleOptionViewItemV4 & > ();
}

static void _call_f_operator_eq__3366 (const qt_gsi::GenericMethod * /*decl*/, void *cls, gsi::SerialArgs &args, gsi::SerialArgs &ret) 
{
  __SUPPRESS_UNUSED_WARNING(args);
  tl::Heap heap;
  const QStyleOptionViewItem &arg1 = args.read<const QStyleOptionViewItem & > (heap);
  ret.write<QStyleOptionViewItemV4 & > ((QStyleOptionViewItemV4 &)((QStyleOptionViewItemV4 *)cls)->operator = (arg1));
}



namespace gsi
{

static gsi::Methods methods_QStyleOptionViewItemV4 () {
  gsi::Methods methods;
  methods += new qt_gsi::GenericStaticMethod ("new", "@brief Constructor QStyleOptionViewItemV4::QStyleOptionViewItemV4()\nThis method creates an object of class QStyleOptionViewItemV4.", &_init_ctor_QStyleOptionViewItemV4_0, &_call_ctor_QStyleOptionViewItemV4_0);
  methods += new qt_gsi::GenericStaticMethod ("new", "@brief Constructor QStyleOptionViewItemV4::QStyleOptionViewItemV4(const QStyleOptionViewItemV4 &other)\nThis method creates an object of class QStyleOptionViewItemV4.", &_init_ctor_QStyleOptionViewItemV4_3504, &_call_ctor_QStyleOptionViewItemV4_3504);
  methods += new qt_gsi::GenericStaticMethod ("new", "@brief Constructor QStyleOptionViewItemV4::QStyleOptionViewItemV4(const QStyleOptionViewItem &other)\nThis method creates an object of class QStyleOptionViewItemV4.", &_init_ctor_QStyleOptionViewItemV4_3366, &_call_ctor_QStyleOptionViewItemV4_3366);
  methods += new qt_gsi::GenericMethod ("assign", "@brief Method (const QStyleOptionViewItem &)\n", false, &_init_f_operator_eq__3366, &_call_f_operator_eq__3366);
  return methods;
}

gsi::Class<QStyleOptionViewItemV3> &qtdecl_QStyleOptionViewItemV3 ();

gsi::Class<QStyleOptionViewItemV4> decl_QStyleOptionViewItemV4 (qtdecl_QStyleOptionViewItemV3 (), "QStyleOptionViewItemV4",
  methods_QStyleOptionViewItemV4 (),
  "@qt\n@brief Binding of QStyleOptionViewItemV4");


GSIQT_PUBLIC gsi::Class<QStyleOptionViewItemV4> &qtdecl_QStyleOptionViewItemV4 () { return decl_QStyleOptionViewItemV4; }

}
