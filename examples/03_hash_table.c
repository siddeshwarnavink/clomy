#include <stdio.h>

#define CLOMY_IMPLEMENTATION
#include "../clomy.h"

void _debught (ht *table);

int
main ()
{
  arena ar = { 0 };
  ht nummap = { 0 };
  int val, *ptr;

  htinit (&nummap, &ar, 16, sizeof (int));

  printf ("Inserting numbers...\n");

  val = 2;
  htput (&nummap, 1, &val);

  val = 4;
  htput (&nummap, 2, &val);

  val = 6;
  htput (&nummap, 3, &val);

  _debught (&nummap);

  printf ("Value of 2 is ");
  ptr = (int *)htget (&nummap, 2);
  if (ptr)
    printf ("%d\n", *ptr);
  else
    printf ("Not found!\n");

  arfold (&ar);

  return 0;
}

void
_debught (ht *table)
{
  clomy_htdata *ptr;
  unsigned int i;

  printf ("\n-------------------\n");

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
