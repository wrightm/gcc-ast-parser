// GCC header includes to get the parse tree
// declarations. The order is important and
// doesn't follow any kind of logic.
//
 
#include <stdlib.h>
#include <gmp.h>
 
#include <cstdlib> // Include before GCC poisons
                   // some declarations.

#include "gcc-plugin.h"
#include "plugin.h"
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
 
#include "tm.h"

#include "cp/cp-tree.h"

#include "diagnostic.h"

#include "c-family/c-common.h"
#include "c-family/c-pragma.h"
 
#include <set>
#include <string>
#include <iostream>

using namespace std;
 
int plugin_is_GPL_compatible;


struct decl_comparator
{
  bool
  operator() (tree x, tree y) const
  {
    location_t xl (DECL_SOURCE_LOCATION (x));
    location_t yl (DECL_SOURCE_LOCATION (y));

    return xl < yl;
  }
};

typedef std::multiset<tree, decl_comparator> decl_set;

void
collect (tree ns, decl_set& set)
{
  tree decl;
  cp_binding_level* level (NAMESPACE_LEVEL (ns));

  // Collect declarations.
  //
  for (decl = level->names; decl != 0; decl = TREE_CHAIN (decl))
  {
    if (DECL_IS_BUILTIN (decl))
      continue;

    set.insert (decl);
  }

  // Traverse namespaces.
  //
  for(decl = level->namespaces; decl != 0; decl = TREE_CHAIN (decl))
  {
    if (DECL_IS_BUILTIN (decl))
      continue;

    collect (decl, set);
  }
}

string
decl_namespace (tree decl)
{
  string s, tmp;

  for (tree scope (CP_DECL_CONTEXT (decl));
       scope != global_namespace;
       scope = CP_DECL_CONTEXT (scope))
  {
    tree id (DECL_NAME (scope));

    tmp = "::";
    tmp += (id != 0 ? IDENTIFIER_POINTER (id) : "<unnamed>");
    tmp += s;
    s.swap (tmp);
  }

  return s;
}

void
print_decl (tree decl)
{
  tree id (DECL_NAME (decl));
  const char* name (id ? IDENTIFIER_POINTER (id) : "<unnamed>");

  cerr << get_tree_code_name(TREE_CODE (decl)) << " "
       << decl_namespace (decl) << "::" << name
       << " at " << DECL_SOURCE_FILE (decl) << ":"
       << DECL_SOURCE_LINE (decl) << endl;
}

void
traverse (tree ns)
{
  decl_set set;
  collect (ns, set);

  for (decl_set::iterator i (set.begin ()), e (set.end ());
       i != e; ++i)
  {
    print_decl (*i);
  }
}

void
gate_callback (void*, void*)
{
  // If there were errors during compilation,
  // let GCC handle the exit.
  //
  if (errorcount || sorrycount)
    return;
 
  int r (0);
 
  //
  // Process AST. Issue diagnostics and set r
  // to 1 in case of an error.
  //
  cerr << "processing " << main_input_filename << endl;
  traverse (global_namespace);

  exit (r);
}
 
int
plugin_init (plugin_name_args* info,
             plugin_gcc_version* ver)
{
  int r (0);
 
  cerr << "starting " << info->base_name << endl;
 
  //
  // Parse options if any.
  //
 
  // Disable assembly output.
  //
  asm_file_name = HOST_BIT_BUCKET;
 
  // Register callbacks.
  //
  register_callback (info->base_name,
                     PLUGIN_OVERRIDE_GATE,
                     &gate_callback,
                     0);
  return r;
}

