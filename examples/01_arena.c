/* An arena is memory allocator which pre-allocates a large chunk of memory and
   sub-allocate parts of it as and when it is needed.

   Why use an arena?
     1. Less number of syscall(s).
     2. De-allocate the whole arean after use.

   I recommend using an area for every module of the code rather than one arena
   for the entire application (unless it's a very simple application of
   course).

   I also recommend adjusting CLOMY_ARENA_CAPACITY according to the need. */
#include <stdio.h>

#define DEBUG 0 /* Set to 1 to inspect arena. */

#define CLOMY_IMPLEMENTATION
#define CLOMY_ARENA_CAPACITY 8 /* Optional. Size of each chunk in bytes. */
#include "../clomy.h"

/* For inspecting the arena. */
void _debugarena (arena *ar);

/* Helper macro to check if allocated successfully. */
#define CHECKALLOC(ptr)                                                       \
  do                                                                          \
    {                                                                         \
      if (!(ptr))                                                             \
        {                                                                     \
          fprintf (stderr, "Failed to allocate age\n");                       \
          goto cleanup;                                                       \
        }                                                                     \
    }                                                                         \
  while (0)

int
main ()
{
  arena ar = { 0 };
  char *buf, *name = (char *)aralloc (&ar, 16);
  int *age = (int *)aralloc (&ar, sizeof (int));

  CHECKALLOC (age);
  CHECKALLOC (name);

#if DEBUG
  _debugarena (&ar);
#endif /* DEBUG */

  printf ("What is your name?\n");
  scanf ("%s", name);

  printf ("What is your age?\n");
  scanf ("%d", age);

  printf ("Hello %s of age %d!\n", name, *age);

  printf ("Freeing name...\n");
  arfree (name);

#if DEBUG
  _debugarena (&ar);
#endif /* DEBUG */

  printf ("Allocating 20 bytes buf...\n");
  buf = aralloc (&ar, 20);
  CHECKALLOC (buf);

#if DEBUG
  _debugarena (&ar);
#endif /* DEBUG */

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
