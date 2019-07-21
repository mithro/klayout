
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

/**
*  @file gsiDeclQXmlNamespaceSupport.cc 
*
*  DO NOT EDIT THIS FILE. 
*  This file has been created automatically
*/

#include <QXmlNamespaceSupport>
#include "gsiQt.h"
#include "gsiQtXmlCommon.h"
#include "gsiDeclQtXmlTypeTraits.h"
#include <memory>

// -----------------------------------------------------------------------
// class QXmlNamespaceSupport

//  Constructor QXmlNamespaceSupport::QXmlNamespaceSupport()


static void _init_ctor_QXmlNamespaceSupport_0 (qt_gsi::GenericStaticMethod *decl)
{
  decl->set_return_new<QXmlNamespaceSupport> ();
}

static void _call_ctor_QXmlNamespaceSupport_0 (const qt_gsi::GenericStaticMethod * /*decl*/, gsi::SerialArgs &args, gsi::SerialArgs &ret) 
{
  __SUPPRESS_UNUSED_WARNING(args);
  ret.write<QXmlNamespaceSupport *> (new QXmlNamespaceSupport ());
}


// void QXmlNamespaceSupport::popContext()


static void _init_f_popContext_0 (qt_gsi::GenericMethod *decl)
{
  decl->set_return<void > ();
}

static void _call_f_popContext_0 (const qt_gsi::GenericMethod * /*decl*/, void *cls, gsi::SerialArgs &args, gsi::SerialArgs &ret) 
{
  __SUPPRESS_UNUSED_WARNING(args);
  __SUPPRESS_UNUSED_WARNING(ret);
  ((QXmlNamespaceSupport *)cls)->popContext ();
}


// QString QXmlNamespaceSupport::prefix(const QString &)


static void _init_f_prefix_c2025 (qt_gsi::GenericMethod *decl)
{
  static gsi::ArgSpecBase argspec_0 ("arg1");
  decl->add_arg<const QString & > (argspec_0);
  decl->set_return<QString > ();
}

static void _call_f_prefix_c2025 (const qt_gsi::GenericMethod * /*decl*/, void *cls, gsi::SerialArgs &args, gsi::SerialArgs &ret) 
{
  __SUPPRESS_UNUSED_WARNING(args);
  tl::Heap heap;
  const QString &arg1 = gsi::arg_reader<const QString & >() (args, heap);
  ret.write<QString > ((QString)((QXmlNamespaceSupport *)cls)->prefix (arg1));
}


// QStringList QXmlNamespaceSupport::prefixes()


static void _init_f_prefixes_c0 (qt_gsi::GenericMethod *decl)
{
  decl->set_return<QStringList > ();
}

static void _call_f_prefixes_c0 (const qt_gsi::GenericMethod * /*decl*/, void *cls, gsi::SerialArgs &args, gsi::SerialArgs &ret) 
{
  __SUPPRESS_UNUSED_WARNING(args);
  ret.write<QStringList > ((QStringList)((QXmlNamespaceSupport *)cls)->prefixes ());
}


// QStringList QXmlNamespaceSupport::prefixes(const QString &)


static void _init_f_prefixes_c2025 (qt_gsi::GenericMethod *decl)
{
  static gsi::ArgSpecBase argspec_0 ("arg1");
  decl->add_arg<const QString & > (argspec_0);
  decl->set_return<QStringList > ();
}

static void _call_f_prefixes_c2025 (const qt_gsi::GenericMethod * /*decl*/, void *cls, gsi::SerialArgs &args, gsi::SerialArgs &ret) 
{
  __SUPPRESS_UNUSED_WARNING(args);
  tl::Heap heap;
  const QString &arg1 = gsi::arg_reader<const QString & >() (args, heap);
  ret.write<QStringList > ((QStringList)((QXmlNamespaceSupport *)cls)->prefixes (arg1));
}


// void QXmlNamespaceSupport::processName(const QString &, bool, QString &, QString &)


static void _init_f_processName_c5225 (qt_gsi::GenericMethod *decl)
{
  static gsi::ArgSpecBase argspec_0 ("arg1");
  decl->add_arg<const QString & > (argspec_0);
  static gsi::ArgSpecBase argspec_1 ("arg2");
  decl->add_arg<bool > (argspec_1);
  static gsi::ArgSpecBase argspec_2 ("arg3");
  decl->add_arg<QString & > (argspec_2);
  static gsi::ArgSpecBase argspec_3 ("arg4");
  decl->add_arg<QString & > (argspec_3);
  decl->set_return<void > ();
}

static void _call_f_processName_c5225 (const qt_gsi::GenericMethod * /*decl*/, void *cls, gsi::SerialArgs &args, gsi::SerialArgs &ret) 
{
  __SUPPRESS_UNUSED_WARNING(args);
  tl::Heap heap;
  const QString &arg1 = gsi::arg_reader<const QString & >() (args, heap);
  bool arg2 = gsi::arg_reader<bool >() (args, heap);
  QString &arg3 = gsi::arg_reader<QString & >() (args, heap);
  QString &arg4 = gsi::arg_reader<QString & >() (args, heap);
  __SUPPRESS_UNUSED_WARNING(ret);
  ((QXmlNamespaceSupport *)cls)->processName (arg1, arg2, arg3, arg4);
}


// void QXmlNamespaceSupport::pushContext()


static void _init_f_pushContext_0 (qt_gsi::GenericMethod *decl)
{
  decl->set_return<void > ();
}

static void _call_f_pushContext_0 (const qt_gsi::GenericMethod * /*decl*/, void *cls, gsi::SerialArgs &args, gsi::SerialArgs &ret) 
{
  __SUPPRESS_UNUSED_WARNING(args);
  __SUPPRESS_UNUSED_WARNING(ret);
  ((QXmlNamespaceSupport *)cls)->pushContext ();
}


// void QXmlNamespaceSupport::reset()


static void _init_f_reset_0 (qt_gsi::GenericMethod *decl)
{
  decl->set_return<void > ();
}

static void _call_f_reset_0 (const qt_gsi::GenericMethod * /*decl*/, void *cls, gsi::SerialArgs &args, gsi::SerialArgs &ret) 
{
  __SUPPRESS_UNUSED_WARNING(args);
  __SUPPRESS_UNUSED_WARNING(ret);
  ((QXmlNamespaceSupport *)cls)->reset ();
}


// void QXmlNamespaceSupport::setPrefix(const QString &, const QString &)


static void _init_f_setPrefix_3942 (qt_gsi::GenericMethod *decl)
{
  static gsi::ArgSpecBase argspec_0 ("arg1");
  decl->add_arg<const QString & > (argspec_0);
  static gsi::ArgSpecBase argspec_1 ("arg2");
  decl->add_arg<const QString & > (argspec_1);
  decl->set_return<void > ();
}

static void _call_f_setPrefix_3942 (const qt_gsi::GenericMethod * /*decl*/, void *cls, gsi::SerialArgs &args, gsi::SerialArgs &ret) 
{
  __SUPPRESS_UNUSED_WARNING(args);
  tl::Heap heap;
  const QString &arg1 = gsi::arg_reader<const QString & >() (args, heap);
  const QString &arg2 = gsi::arg_reader<const QString & >() (args, heap);
  __SUPPRESS_UNUSED_WARNING(ret);
  ((QXmlNamespaceSupport *)cls)->setPrefix (arg1, arg2);
}


// void QXmlNamespaceSupport::splitName(const QString &, QString &, QString &)


static void _init_f_splitName_c4469 (qt_gsi::GenericMethod *decl)
{
  static gsi::ArgSpecBase argspec_0 ("arg1");
  decl->add_arg<const QString & > (argspec_0);
  static gsi::ArgSpecBase argspec_1 ("arg2");
  decl->add_arg<QString & > (argspec_1);
  static gsi::ArgSpecBase argspec_2 ("arg3");
  decl->add_arg<QString & > (argspec_2);
  decl->set_return<void > ();
}

static void _call_f_splitName_c4469 (const qt_gsi::GenericMethod * /*decl*/, void *cls, gsi::SerialArgs &args, gsi::SerialArgs &ret) 
{
  __SUPPRESS_UNUSED_WARNING(args);
  tl::Heap heap;
  const QString &arg1 = gsi::arg_reader<const QString & >() (args, heap);
  QString &arg2 = gsi::arg_reader<QString & >() (args, heap);
  QString &arg3 = gsi::arg_reader<QString & >() (args, heap);
  __SUPPRESS_UNUSED_WARNING(ret);
  ((QXmlNamespaceSupport *)cls)->splitName (arg1, arg2, arg3);
}


// QString QXmlNamespaceSupport::uri(const QString &)


static void _init_f_uri_c2025 (qt_gsi::GenericMethod *decl)
{
  static gsi::ArgSpecBase argspec_0 ("arg1");
  decl->add_arg<const QString & > (argspec_0);
  decl->set_return<QString > ();
}

static void _call_f_uri_c2025 (const qt_gsi::GenericMethod * /*decl*/, void *cls, gsi::SerialArgs &args, gsi::SerialArgs &ret) 
{
  __SUPPRESS_UNUSED_WARNING(args);
  tl::Heap heap;
  const QString &arg1 = gsi::arg_reader<const QString & >() (args, heap);
  ret.write<QString > ((QString)((QXmlNamespaceSupport *)cls)->uri (arg1));
}



namespace gsi
{

static gsi::Methods methods_QXmlNamespaceSupport () {
  gsi::Methods methods;
  methods += new qt_gsi::GenericStaticMethod ("new", "@brief Constructor QXmlNamespaceSupport::QXmlNamespaceSupport()\nThis method creates an object of class QXmlNamespaceSupport.", &_init_ctor_QXmlNamespaceSupport_0, &_call_ctor_QXmlNamespaceSupport_0);
  methods += new qt_gsi::GenericMethod ("popContext", "@brief Method void QXmlNamespaceSupport::popContext()\n", false, &_init_f_popContext_0, &_call_f_popContext_0);
  methods += new qt_gsi::GenericMethod ("prefix", "@brief Method QString QXmlNamespaceSupport::prefix(const QString &)\n", true, &_init_f_prefix_c2025, &_call_f_prefix_c2025);
  methods += new qt_gsi::GenericMethod ("prefixes", "@brief Method QStringList QXmlNamespaceSupport::prefixes()\n", true, &_init_f_prefixes_c0, &_call_f_prefixes_c0);
  methods += new qt_gsi::GenericMethod ("prefixes", "@brief Method QStringList QXmlNamespaceSupport::prefixes(const QString &)\n", true, &_init_f_prefixes_c2025, &_call_f_prefixes_c2025);
  methods += new qt_gsi::GenericMethod ("processName", "@brief Method void QXmlNamespaceSupport::processName(const QString &, bool, QString &, QString &)\n", true, &_init_f_processName_c5225, &_call_f_processName_c5225);
  methods += new qt_gsi::GenericMethod ("pushContext", "@brief Method void QXmlNamespaceSupport::pushContext()\n", false, &_init_f_pushContext_0, &_call_f_pushContext_0);
  methods += new qt_gsi::GenericMethod ("reset", "@brief Method void QXmlNamespaceSupport::reset()\n", false, &_init_f_reset_0, &_call_f_reset_0);
  methods += new qt_gsi::GenericMethod ("setPrefix", "@brief Method void QXmlNamespaceSupport::setPrefix(const QString &, const QString &)\n", false, &_init_f_setPrefix_3942, &_call_f_setPrefix_3942);
  methods += new qt_gsi::GenericMethod ("splitName", "@brief Method void QXmlNamespaceSupport::splitName(const QString &, QString &, QString &)\n", true, &_init_f_splitName_c4469, &_call_f_splitName_c4469);
  methods += new qt_gsi::GenericMethod ("uri", "@brief Method QString QXmlNamespaceSupport::uri(const QString &)\n", true, &_init_f_uri_c2025, &_call_f_uri_c2025);
  return methods;
}

gsi::Class<QXmlNamespaceSupport> decl_QXmlNamespaceSupport ("QtXml", "QXmlNamespaceSupport",
  methods_QXmlNamespaceSupport (),
  "@qt\n@brief Binding of QXmlNamespaceSupport");


GSI_QTXML_PUBLIC gsi::Class<QXmlNamespaceSupport> &qtdecl_QXmlNamespaceSupport () { return decl_QXmlNamespaceSupport; }

}
