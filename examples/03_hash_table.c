/* Hash table is a data structure to store key-value pairs.

   Value of hash table can be anything but key must be either unsigned integer
   or string. */
#include <stdio.h>

#define CLOMY_IMPLEMENTATION
#include "../clomy.h"

#define IN_ARENA 1 /* Set to 0 to allocate in heap. */
#define DEBUG 0    /* Set to 1 to inspect dynamic array. */

/* Helper function to inspect hash table. */
void _debught (ht *table);

int
main ()
{
#if IN_ARENA
  arena ar = { 0 };
#endif /* IN_ARENA */

  ht nummap = { 0 }, strmap = { 0 };
  unsigned int i, val, *ptr;

  srand ((unsigned)time (NULL));

  /* Here SIZEOF (INT) denotes size individual element of table. */
#if IN_ARENA
  htinit (&nummap, &ar, 8, sizeof (int));
  htinit (&strmap, &ar, 16, sizeof (int));
#else
  htinit2 (&nummap, 16, sizeof (int));
  htinit2 (&strmap, 16, sizeof (int));
#endif /* IN_ARENA */

  /* --------- Hash table with stirng key --------- */

  printf ("Inserting in string key table...\n");

  val = rand () % 100;
  stput (&strmap, "foo", &val);

  val = rand () % 100;
  stput (&strmap, "bar", &val);

  val = rand () % 100;
  stput (&strmap, "baz", &val);

  val = rand () % 100;
  stput (&strmap, "foobarbaz", &val);

#if DEBUG
  _debught (&strmap);
#endif /* DEBUG */

  printf ("Value of \"foo\" is ");
  ptr = (unsigned int *)stget (&strmap, "foo");
  if (ptr)
    {
      printf ("%d\n", *ptr);
      printf ("Deleting value of \"foo\"...\n");
      stdel (&nummap, "foo");
    }

  /* --------- Hash table with unsigned int key --------- */

  printf ("Inserting in uint key table...\n");
  for (i = 0; i < 100; ++i)
    {
      val = rand () % 100;
      htput (&nummap, i, &val);
    }

  printf ("Value of 2 is ");
  ptr = (unsigned int *)htget (&nummap, 2);
  if (ptr)
    {
      printf ("%d\n", *ptr);
      printf ("Deleting value of 2...\n");
      htdel (&nummap, 2);
    }
  else
    {
      printf ("Not found!\n");
    }

#if DEBUG
  _debught (&strmap);
#endif /* DEBUG */

#if IN_ARENA
  arfold (&ar);
#else
  stfold (&strmap);
  htfold (&nummap);
#endif

  return 0;
}

void
_debught (ht *table)
{
  clomy_htdata *ptr;
  unsigned int i;

  printf ("\n-------------------\n");
  printf ("Size = %d\n", table->size);

  for (i = 0; i < table->capacity; ++i)
    {
      ptr = table->data[i];
      printf ("%d => ", i);
      if (!ptr)
        {
          printf ("NULL\n");
          continue;
        }

      while (ptr)
        {
          printf ("%d, ", *((int *)ptr->data));
          ptr = ptr->next;
        }
      printf ("\n");
    }

  printf ("-------------------\n\n");
}
