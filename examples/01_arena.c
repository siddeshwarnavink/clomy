#include <stdio.h>
#include <string.h>

#define CLOMY_IMPLEMENTATION
#include "../clomy.h"

/* Print byte allocation of arena for debugging. */
void
arena_palloc (arena *ar)
{
  unsigned int i;
  for (i = 0; i < ar->capacity; ++i)
    printf ("%u-", ar->bytemap[i]);

  printf ("\n");
}

int
main ()
{
  arena ar = {0};
  char *name, *full_name;
  int *age;

  arinit (&ar, 16);

  name = (char *) aralloc (&ar, 8);
  printf ("Allocate Name 8 bytes %p\n", name);
  arena_palloc (&ar);
  printf ("\n");

  age = (int *) aralloc (&ar, 4);
  printf ("Allocate Age 4 bytes %p\n", age);
  arena_palloc (&ar);
  printf ("\n");

  strcpy (name, "Sid");
  *age = 21;

  printf ("Hello, %s of age %d!\n", name, *age);

  printf ("Free name\n");
  arfree (&ar, name, 8);
  arena_palloc (&ar);
  printf ("\n");

  full_name = (char *) aralloc (&ar, 16);
  printf ("Allocate Full Name 12 bytes %p\n", full_name);
  if (!full_name)
    {
      printf ("Successfully failed to allocate!\n\n");
    }

  printf ("Clear arena\n");
  arclear (&ar);
  arena_palloc (&ar);
  printf ("\n");

  full_name = (char *) aralloc (&ar, 12);
  printf ("Allocate Full Name 12 bytes %p\n", full_name);
  arena_palloc (&ar);
  printf ("\n");

  strcpy (full_name, "Siddeshwar");
  printf ("Full name is %s\n", full_name);

  arfold (&ar);

  return 0;
}
