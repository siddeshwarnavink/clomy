#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define CLOMY_BACKEND_WINAPI
#endif

#define CLOMY_IMPLEMENTATION
#include "../build/clomy.h"

#define CHECK_NUMBER(i)                                                       \
  do                                                                          \
    {                                                                         \
      val = *((int *)htget (&nummap, (i)));                                   \
      printf ("Value of %d is %d\n", (i), val);                               \
      FAILFALSE (val == (i) * (i), "incorrect square value");                 \
    }                                                                         \
  while (0)

int
main ()
{
  arena ar = { 0 };
  ht strmap = { 0 }, nummap = { 0 };
  U32 i;
  int val;

  /* --------- Hash table with stirng key --------- */
  htinit (&strmap, &ar, 8, sizeof (int));

  printf ("Inserting in string key table...\n");
  stput (&strmap, "foo", &(int){ 8 });
  stput_int (&strmap, "bar", 6);
  stput_int (&strmap, "foobar", 24);

  FAILFALSE (strmap.size == 3, "incorrect strmap size.");
  FAILFALSE (*((int *)stget (&strmap, "foo")) == 8,
             "incorrect value for FOO.");
  FAILFALSE (*stget_int (&strmap, "bar") == 6, "incorrect value for BAR.");
  FAILFALSE (*stget_int (&strmap, "foobar") == 24,
             "incorrect value for FOOBAR.");

  printf ("Deleting value of \"foo\"...\n");
  stdel (&strmap, "foo");
  FAILFALSE (strmap.size == 2, "key FOO not deleted.");
  FAILFALSE (stget_int (&strmap, "foo") == NULL, "key FOO not deleted.");

  printf ("Folding string key table...\n");
  stfold (&strmap);

  /* --------- Hash table with int key --------- */
  htinit (&nummap, &ar, 8, sizeof (int));

  printf ("Inserting in U32 key table...\n");

  for (i = 1; i <= 120; ++i)
    htput (&nummap, i, &(int){ i * i });

  FAILFALSE (nummap.size == 120, "incorrect nummap size.");

  CHECK_NUMBER (4);
  CHECK_NUMBER (27);
  CHECK_NUMBER (50);
  CHECK_NUMBER (73);
  CHECK_NUMBER (111);

  printf ("Folding U32 key table...\n");
  htfold (&nummap);

  arfold (&ar);

  return 0;
}
