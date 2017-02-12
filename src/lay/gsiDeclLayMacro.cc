
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


#include "gsiDecl.h"
#include "gsiDeclBasic.h"
#include "gsiInterpreter.h"
#include "layMacroInterpreter.h"
#include "layMacro.h"
#include "layApplication.h"

#include "tlClassRegistry.h"

#include <cstdio>

namespace tl
{

//  type traits for BrowserDialog_Stub
template <> 
struct type_traits<lay::Macro> : public type_traits<void> {
  typedef tl::false_tag has_copy_constructor;
};

}

namespace gsi
{

class MacroExecutionContext
{
public:
  static void set_debugger_scope (const std::string &filename)
  {
    lay::Application::instance ()->ruby_interpreter ().set_debugger_scope (filename);
  }

  static void remove_debugger_scope ()
  {
    lay::Application::instance ()->ruby_interpreter ().remove_debugger_scope ();
  }

  static void ignore_next_exception ()
  {
    lay::Application::instance ()->ruby_interpreter ().ignore_next_exception ();
  }
};

Class<gsi::MacroExecutionContext> decl_MacroExecutionContext ("MacroExecutionContext", 
  gsi::method ("set_debugger_scope", &gsi::MacroExecutionContext::set_debugger_scope,
    "@brief Sets a debugger scope (file level which shall appear in the debugger)\n"
    "@args filename\n"
    "If a debugger scope is set, back traces will be produced starting from that scope. "
    "Setting a scope is useful for implementing DSL interpreters and giving a proper hint about "
    "the original location of an error."
  ) +
  gsi::method ("remove_debugger_scope", &gsi::MacroExecutionContext::remove_debugger_scope,
    "@brief Removes a debugger scope previously set with \\set_debugger_scope\n"
  ) +
  gsi::method ("ignore_next_exception", &gsi::MacroExecutionContext::ignore_next_exception,
    "@brief Ignores the next exception in the debugger\n"
    "The next exception thrown will be ignored in the debugger. That feature is useful when "
    "reraising exceptions if those new exception shall not appear in the debugger."
  ),
  "@brief Support for various debugger features\n"
  "\n"
  "This class implements some features that allow customization of the debugger behavior, specifically "
  "the generation of back traces and the handling of exception. These functions are particular useful "
  "for implementing DSL interpreters and providing proper error locations in the back traces or to "
  "suppress exceptions when reraising them."
);

class MacroInterpreter
  : public lay::MacroInterpreter
{
public:
  MacroInterpreter ()
    : lay::MacroInterpreter (), 
      mp_registration (0) 
  {
    m_suffix = lay::MacroInterpreter::suffix ();
    m_description = lay::MacroInterpreter::description ();
    m_storage_scheme = lay::MacroInterpreter::storage_scheme ();
    m_syntax_scheme = lay::MacroInterpreter::syntax_scheme ();
    m_debugger_scheme = lay::MacroInterpreter::debugger_scheme ();
  }

  ~MacroInterpreter ()
  {
    delete mp_registration;
    mp_registration = 0;
    for (std::vector<lay::Macro *>::const_iterator t = m_templates.begin (); t != m_templates.end (); ++t) {
      delete *t;
    }
    m_templates.clear ();
  }

  void register_gsi (const char *name)
  {
    //  makes the object owned by the C++ side
    keep ();

    //  cancel any previous registration and register (again)
    delete mp_registration;
    mp_registration = new tl::RegisteredClass<lay::MacroInterpreter> (this, 0 /*position*/, name, false /*does not own object*/);

    m_name = name;
  }

  virtual void execute (const lay::Macro *macro) const
  {
    if (f_execute.can_issue ()) {
      f_execute.issue<MacroInterpreter, const lay::Macro *> (&MacroInterpreter::execute, macro);
    }
  }

  void set_storage_scheme (int scheme)
  {
    m_storage_scheme = lay::Macro::Format (scheme);
  }

  virtual lay::Macro::Format storage_scheme () const
  {
    return m_storage_scheme;
  }

  void set_debugger_scheme (int scheme)
  {
    m_debugger_scheme = lay::Macro::Interpreter (scheme);
  }

  virtual lay::Macro::Interpreter debugger_scheme () const
  {
    return m_debugger_scheme;
  }

  void set_syntax_scheme (const std::string &s)
  {
    m_syntax_scheme = s;
  }

  virtual std::string syntax_scheme () const
  {
    return m_syntax_scheme;
  }

  void set_description (const std::string &s)
  {
    m_description = s;
  }

  virtual std::string description () const
  {
    return m_description;
  }

  void set_suffix (const std::string &s)
  {
    m_suffix = s;
  }

  virtual std::string suffix () const
  {
    return m_suffix;
  }

  lay::Macro *create_template (const std::string &name) 
  {
    if (! mp_registration) {
      throw std::runtime_error (tl::to_string (QObject::tr ("MacroInterpreter::create_template must be called after register")));
    }

    lay::Macro *m = new lay::Macro ();

    m->rename (name);
    m->set_readonly (true);
    m->set_dsl_interpreter (m_name);
    m->set_interpreter (lay::Macro::DSLInterpreter);
    m->set_format (storage_scheme ());

    m_templates.push_back (m);
    return m;
  }

  virtual void get_templates (std::vector<lay::Macro *> &tt) const
  {
    for (std::vector<lay::Macro *>::const_iterator t = m_templates.begin  (); t != m_templates.end (); ++t) {
      tt.push_back (new lay::Macro ());
      tt.back ()->rename ((*t)->name ());
      tt.back ()->assign (**t);
    }
  }

  gsi::Callback f_execute;

private:
  tl::RegisteredClass <lay::MacroInterpreter> *mp_registration;
  std::string m_name;
  std::vector<lay::Macro *> m_templates;
  std::string m_syntax_scheme;
  lay::Macro::Format m_storage_scheme;
  lay::Macro::Interpreter m_debugger_scheme;
  std::string m_suffix;
  std::string m_description;
};

int const_PlainTextFormat ()
{
  return int (lay::Macro::PlainTextFormat);
}

int const_PlainTextWithHashAnnotationsFormat ()
{
  return int (lay::Macro::PlainTextWithHashAnnotationsFormat);
}

int const_MacroFormat ()
{
  return int (lay::Macro::MacroFormat);
}

int const_RubyDebugger ()
{
  return int (lay::Macro::Ruby);
}

int const_NoDebugger ()
{
  return int (lay::Macro::None);
}

Class<gsi::MacroInterpreter> decl_MacroInterpreter ("MacroInterpreter", 
  gsi::method ("PlainTextFormat", &const_PlainTextFormat,
    "@brief Indicates plain text format for \\storage_scheme\n"
  ) +
  gsi::method ("PlainTextWithHashAnnotationsFormat", &const_PlainTextWithHashAnnotationsFormat,
    "@brief Indicates plain text format for \\storage_scheme\n"
    "This format is identical to \\PlainTextFormat but indicates that it is possible "
    "to insert annotations (properties) into the text in a hash-commented header."
  ) +
  gsi::method ("MacroFormat", &const_MacroFormat,
    "@brief Indicates macro (XML) format for \\storage_scheme\n"
  ) +
  gsi::method ("RubyDebugger", &const_RubyDebugger,
    "@brief Indicates Ruby debugger for \\debugger_scheme\n"
  ) +
  gsi::method ("NoDebugger", &const_NoDebugger,
    "@brief Indicates no debugging for \\debugger_scheme\n"
  ) +
  gsi::method ("register", &MacroInterpreter::register_gsi,
    "@brief Registers the macro interpreter\n"
    "@args name\n"
    "@param name The interpreter name. This is an arbitrary string which should be unique.\n"
    "\n"
    "Registration of the interpreter makes the object known to the system. After registration, macros whose interpreter "
    "is set to 'dsl' can use this object to run the script. For executing a script, the system will "
    "call the interpreter's \\execute method.\n"
  ) + 
  gsi::method ("create_template", &MacroInterpreter::create_template,
    "@brief Creates a new macro template\n"
    "@args name\n"
    "@param name The template name. This is an arbitrary string which should be unique.\n"
    "\n"
    "This method will create a register a new macro template. It returns a \\Macro object which "
    "can be modified in order to adjust the template (for example to set description, add a content, "
    "menu binding, autorun flags etc.)\n"
    "\n"
    "This method must be called after \\register has called.\n"
  ) + 
  gsi::method ("syntax_scheme=", &gsi::MacroInterpreter::set_syntax_scheme, gsi::arg ("scheme"),
    "@brief Sets a string indicating the syntax highlighter scheme\n"
    "\n"
    "The scheme string can be empty (indicating no syntax highlighting), \"ruby\" for the Ruby syntax "
    "highlighter or another string. In that case, the highlighter will look for a syntax definition "
    "under the resource path \":/syntax/<scheme>.xml\".\n"
    "\n"
    "Use this attribute setter in the initializer before registering the interpreter.\n"
    "\n"
    "Before version 0.25 this attribute was a reimplementable method. It has been turned into an attribute for "
    "performance reasons in version 0.25.\n"
  ) +
  gsi::method ("debugger_scheme=", &gsi::MacroInterpreter::set_debugger_scheme, gsi::arg ("scheme"),
    "@brief Sets the debugger scheme (which debugger to use for the DSL macro)\n"
    "\n"
    "The value can be one of the constants \\RubyDebugger or \\NoDebugger.\n"
    "\n"
    "Use this attribute setter in the initializer before registering the interpreter.\n"
    "\n"
    "Before version 0.25 this attribute was a reimplementable method. It has been turned into an attribute for "
    "performance reasons in version 0.25.\n"
  ) +
  gsi::method ("storage_scheme=", &gsi::MacroInterpreter::set_storage_scheme, gsi::arg ("scheme"),
    "@brief Sets the storage scheme (the format as which the macro is stored)\n"
    "\n"
    "This value indicates how files for this DSL macro type shall be stored. "
    "The value can be one of the constants \\PlainTextFormat, \\PlainTextWithHashAnnotationsFormat and \\MacroFormat.\n"
    "\n"
    "Use this attribute setter in the initializer before registering the interpreter.\n"
    "\n"
    "Before version 0.25 this attribute was a reimplementable method. It has been turned into an attribute for "
    "performance reasons in version 0.25.\n"
  ) +
  gsi::method ("description=", &gsi::MacroInterpreter::set_description, gsi::arg ("description"),
    "@brief Sets a description string\n"
    "\n"
    "This string is used for showing the type of DSL macro in the file selection box together with the "
    "suffix for example. "
    "\n"
    "Use this attribute setter in the initializer before registering the interpreter.\n"
    "\n"
    "Before version 0.25 this attribute was a reimplementable method. It has been turned into an attribute for "
    "performance reasons in version 0.25.\n"
  ) +
  gsi::method ("suffix=", &gsi::MacroInterpreter::set_suffix, gsi::arg ("suffix"),
    "@brief Sets the file suffix\n"
    "\n"
    "This string defines which file suffix to associate with the DSL macro. If an empty string is given (the default) "
    "no particular suffix is assciated with that macro type and \"lym\" is assumed. "
    "\n"
    "Use this attribute setter in the initializer before registering the interpreter.\n"
    "\n"
    "Before version 0.25 this attribute was a reimplementable method. It has been turned into an attribute for "
    "performance reasons in version 0.25.\n"
  ) +
  gsi::callback ("execute", &gsi::MacroInterpreter::execute, &gsi::MacroInterpreter::f_execute,
    "@brief Gets called to execute a macro\n"
    "@args macro\n"
    "This method must be reimplemented to execute the macro. "
    "The system will call this script when a macro with interpreter type 'dsl' and the "
    "name of this interpreter is run."
    "\n"
    "@param macro The macro to execute\n"
  ),
  "@brief A custom interpreter for a DSL (domain specific language)\n"
  "\n"
  "DSL interpreters are a way to provide macros written in a language specific for the "
  "application. One example are DRC scripts which are written in some special language "
  "optimized for DRC ruledecks. Interpreters for such languages "
  "can be built using scripts itself by providing the interpreter implementation through "
  "this object.\n"
  "\n"
  "An interpreter implementation involves at least these steps:\n"
  "\n"
  "@ul\n"
  "@li Derive a new object from RBA::MacroInterpreter @/li\n"
  "@li Reimplement the \\execute method for the actual execution of the code @/li\n"
  "@li In the initialize method configure the object using the attribute setters like \\suffix= and register the object as DSL interpreter (in that order) @/li\n"
  "@li Create at least one template macro in the initialize method @/li\n"
  "@/ul\n"
  "\n"
  "Template macros provide a way for the macro editor to present macros for the new interpreter in the "
  "list of templates. Template macros can provide menu bindings, shortcuts and some initial text for example\n"
  "\n"
  "The simple implementation can be enhanced by providing more information, i.e. syntax highlighter "
  "information, the debugger to use etc. This involves reimplementing further methods, i.e. \"syntax_scheme\".\n"
  "\n"
  "This is a simple example for an interpreter in Ruby. Is is registered under the name 'simple-dsl' and "
  "just evaluates the script text:\n"
  "\n"
  "@code\n"
  "class SimpleInterpreter < RBA::MacroInterpreter\n"
  "\n"
  "  # Constructor\n"
  "  def initialize\n"
  "    self.description = \"A test interpreter\"\n"
  "    # Registers the new interpreter\n"
  "    register(\"simple-dsl\")\n"
  "    # create a template for the macro editor:\n"
  "    # Name is \"new_simple\", the description will be \"Simple interpreter macro\"\n"
  "    # in the \"Special\" group.\n"
  "    mt = create_template(\"new_simple\")\n"
  "    mt.description = \"Special;;Simple interpreter macro\"\n" 
  "  end\n"
  "  \n"
  "  # Implements the execute method\n"
  "  def execute(macro)\n"
  "    eval(macro.text, nil, macro.path)\n"
  "  end\n"
  "\n"
  "end\n"
  "\n"
  "# Register the new interpreter\n"
  "SimpleInterpreter::new\n"
  "\n"
  "@/code\n"
  "\n"
  "Please note that such an implementation is dangerous because the evaluation of the script "
  "happens in the context of the interpreter object. In this implementation the script could redefine "
  "the execute method for example. This implementation is provided as an example only.\n"
  "A real implementation should add execution of prolog and epilog code inside the execute method "
  "and proper error handling.\n"
  "\n"
  "In order to make the above code effective, store the code in an macro, set \"early auto-run\" and restart KLayout.\n"
  "\n"
  "This class has been introduced in version 0.23.\n"
);

Class<lay::Macro> decl_Macro ("Macro", 
  gsi::method ("path", &lay::Macro::path,
    "@brief Gets the path of the macro\n"
    "\n"
    "The path is the path where the macro is stored, starting with an abstract group identifier. "
    "The path is used to identify the macro in the debugger for example."
  ) + 
  gsi::method ("description", &lay::Macro::description,
    "@brief Gets the description text\n"
    "\n"
    "The description text of a macro will appear in the macro list. If used as a macro template, "
    "the description text can have the format \"Group;;Description\". In that case, the macro "
    "will appear in a group with title \"Group\"."
  ) + 
  gsi::method ("description=", &lay::Macro::set_description,
    "@brief Sets the description text\n"
    "@args description\n"
    "@param description The description text.\n"
    "See \\description for details.\n"
  ) +
  gsi::method ("prolog", &lay::Macro::prolog,
    "@brief Gets the prolog code\n"
    "\n"
    "The prolog is executed before the actual code is executed. Interpretation depends on the "
    "implementation of the DSL interpreter for DSL macros."
  ) + 
  gsi::method ("prolog=", &lay::Macro::set_prolog,
    "@brief Sets the prolog\n"
    "@args string\n"
    "See \\prolog for details.\n"
  ) +
  gsi::method ("epilog", &lay::Macro::epilog,
    "@brief Gets the epilog code\n"
    "\n"
    "The epilog is executed after the actual code is executed. Interpretation depends on the "
    "implementation of the DSL interpreter for DSL macros."
  ) + 
  gsi::method ("epilog=", &lay::Macro::set_epilog,
    "@brief Sets the epilog\n"
    "@args string\n"
    "See \\epilog for details.\n"
  ) +
  gsi::method ("category", &lay::Macro::category,
    "@brief Gets the category tags\n"
    "\n"
    "The category tags string indicates to which categories a macro will belong to. This string "
    "is only used for templates currently and is a comma-separated list of category names."
  ) + 
  gsi::method ("category=", &lay::Macro::set_category,
    "@brief Sets the category tags string\n"
    "@args string\n"
    "See \\category for details.\n"
  ) +
  gsi::method ("text", &lay::Macro::text,
    "@brief Gets the macro text\n"
    "\n"
    "The text is the code executed by the macro interpreter. "
    "Depending on the DSL interpreter, the text can be any kind of code."
  ) + 
  gsi::method ("text=", &lay::Macro::set_text,
    "@brief Sets the macro text\n"
    "@args string\n"
    "See \\text for details.\n"
  ) +
  gsi::method ("show_in_menu?", &lay::Macro::show_in_menu,
    "@brief Gets a value indicating whether the macro shall be shown in the menu\n"
  ) + 
  gsi::method ("show_in_menu=", &lay::Macro::set_show_in_menu,
    "@brief Sets a value indicating whether the macro shall be shown in the menu\n"
    "@args flag\n"
  ) +
  gsi::method ("group_name", &lay::Macro::group_name,
    "@brief Gets the menu group name\n"
    "\n"
    "If a group name is specified and \\show_in_menu? is true, the macro will appear in "
    "a separate group (separated by a separator) together with other macros sharing the same group."
  ) + 
  gsi::method ("group_name=", &lay::Macro::set_group_name,
    "@brief Sets the menu group name\n"
    "@args string\n"
    "See \\group_name for details.\n"
  ) +
  gsi::method ("menu_path", &lay::Macro::menu_path,
    "@brief Gets the menu path\n"
    "\n"
    "If a menu path is specified and \\show_in_menu? is true, the macro will appear in "
    "the menu at the specified position."
  ) + 
  gsi::method ("menu_path=", &lay::Macro::set_menu_path,
    "@brief Sets the menu path\n"
    "@args string\n"
    "See \\menu_path for details.\n"
  ),
  "@brief A macro class\n"
  "\n"
  "This class is provided mainly to support generation of template macros in the "
  "DSL interpreter framework provided by \\MacroInterpreter. The implementation may be "
  "enhanced in future versions and provide access to macros stored inside KLayout's macro repository."
);

}
