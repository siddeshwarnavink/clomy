#include <stdio.h>

#define CLOMY_IMPLEMENTATION
#define CLOMY_ARENA_DEFAULT_CAPACITY 64
#include "../clomy.h"

void _debugarena (arena *ar);

int
main ()
{
  arena ar = {0};
  char *buf, *name = (char *) aralloc (&ar, 16 * sizeof (char));
  int *age = (int *) aralloc (&ar, sizeof (int));

  if (!age)
    {
      fprintf (stderr, "Failed to allocate age\n");
      goto cleanup;
    }

  if (!name)
    {
      fprintf (stderr, "Failed to allocate name\n");
      goto cleanup;
    }

  _debugarena (&ar);

  printf ("What is your name?\n");
  scanf ("%s", name);

  printf ("What is your age?\n");
  scanf ("%d", age);

  printf ("Hello %s of age %d!\n", name, *age);

  printf ("Freeing name...\n");
  arfree (&ar, name);
  _debugarena (&ar);

  printf ("Allocating 20 bytes buf...\n");
  buf = aralloc (&ar, 20);
  if (!buf)
    {
      fprintf (stderr, "Failed to allocate buf\n");
      goto cleanup;
    }
  _debugarena (&ar);
  strcpy (buf, "Hello, World!");
  printf ("buf=\"%s\"\n", buf);

cleanup:
  arfold (&ar);

  return 0;
}

void
_debugarena (arena *ar)
{
  archunk *cnk = ar->head;

  printf ("\n-------------------\n");
  while (cnk)
    {
      printf ("Size: %d, Capacity %d\n", cnk->size, cnk->capacity);
      cnk = cnk->next;
    }
  printf ("-------------------\n\n");
}

