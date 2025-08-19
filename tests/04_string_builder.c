#include <stdio.h>

#define CLOMY_IMPLEMENTATION
#define CLOMY_STRINGBUILDER_CAPACITY 8
#include "../clomy.h"
#include "../clomy_test.h"

int
main ()
{
  arena ar = { 0 };
  stringbuilder sb = { 0 };
  string *str;

  sbinit (&sb, &ar);

  sbappend (&sb, "Hello ");
  sbappend (&sb, "World!");
  sbinsert (&sb, "to the ", 6);

  sbrev (&sb);

  str = sbflush (&sb);
  printf ("Final String: \"%s\"\n", str->data);

  FAILFALSE (strcmp (str->data, "!dlroW eht ot olleH") == 0,
             "incorrect string constructed.");

  arfree (str);

  sbappend (&sb, " will build the string.");
  sbpush (&sb, "Flushing the string builder");
  sbappendch (&sb, '\n');

  str = sbflush (&sb);
  printf ("Final String: \"%s\"\n", str->data);

  FAILFALSE (
      strcmp (str->data, "Flushing the string builder will build the string.\n")
          == 0,
      "incorrect string constructed.");

  arfree (str);

  sbpushch (&sb, 'Y');
  sbappend (&sb, "ou can re-use the same");
  sbappend (&sb, " string builder after flush.");

  str = sbflush (&sb);
  printf ("Final String: \"%s\"\n", str->data);

  FAILFALSE (
      strcmp (str->data, "You can re-use the same string builder after flush.") == 0,
      "incorrect string constructed.");

  arfree (str);

  arfold (&ar);
}
