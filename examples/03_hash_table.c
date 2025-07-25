#include <stdio.h>

#define CLOMY_IMPLEMENTATION
#include "../clomy.h"

void _debught (ht *table);

int
main ()
{
  arena ar = { 0 };
  ht nummap = { 0 };
  unsigned int i, val, *ptr;

  srand ((unsigned)time (NULL));

  htinit (&nummap, &ar, 16, sizeof (int));
  /* htinit2 (&nummap, 16, sizeof (int)); */

  printf ("Inserting numbers...\n");
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

  _debught (&nummap);

  /* htfold (&nummap); */
  arfold (&ar);

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
