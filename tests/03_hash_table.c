#include <stdio.h>

#define CLOMY_IMPLEMENTATION
#include "../clomy.h"
#include "../clomy_test.h"

int
main ()
{
  arena ar = { 0 };
  ht strmap = { 0 }, nummap = { 0 };
  U32 i;

  /* --------- Hash table with stirng key --------- */
  htinit (&strmap, &ar, 8, sizeof (int));

  printf ("Inserting in string key table...\n");
  stput (&strmap, "foo", &(int){ 8 });
  stput (&strmap, "bar", &(int){ 16 });
  stput (&strmap, "foobar", &(int){ 24 });

  FAILFALSE (strmap.size == 3, "incorrect strmap size.");
  FAILFALSE (*((int *)stget (&strmap, "foo")) == 8,
             "incorrect value for FOO.");
  FAILFALSE (*((int *)stget (&strmap, "bar")) == 16,
             "incorrect value for BAR.");
  FAILFALSE (*((int *)stget (&strmap, "foobar")) == 24,
             "incorrect value for FOOBAR.");

  printf ("Deleting value of \"foo\"...\n");
  stdel (&strmap, "foo");
  FAILFALSE (strmap.size == 2, "key FOO not deleted.");
  FAILFALSE (stget (&strmap, "foo") == (void *)0, "key FOO not deleted.");

  printf ("Folding string key table...\n");
  stfold (&strmap);

  /* --------- Hash table with U32 key --------- */
  htinit (&nummap, &ar, 8, sizeof (int));

  printf ("Inserting in U32 key table...\n");

  for (i = 1; i <= 120; ++i)
    htput (&nummap, i, &(int){ i ^ 2 });

  FAILFALSE (nummap.size == 120, "incorrect nummap size.");

  printf ("Folding U32 key table...\n");
  htfold (&nummap);

  arfold (&ar);

  return 0;
}
