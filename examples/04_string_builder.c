/* String builder helps to build and manipulate string of dynamic size.

   After string is built use sbflush() to get the whole null-terminated string.
   The memory of this string should be managed by user. Calling sbflush() also
   reset the string builder. */
#include <stdio.h>

#define CLOMY_IMPLEMENTATION
#define CLOMY_STRINGBUILDER_CAPACITY 8 /* Optional. Capacity of chunk. */
#include "../clomy.h"

#define IN_ARENA 1 /* Set to 0 to allocate in heap. */
#define DEBUG 0    /* Set to 1 to inspect dynamic array. */

/* Helper macro to append to string builder and check for error */
#define SBAPPEND(sb, str)                                                     \
  do                                                                          \
    {                                                                         \
      if (sbappend (&(sb), (str)) > 0)                                        \
        {                                                                     \
          fprintf (stderr, "Error in appending string\n");                    \
          goto cleanup;                                                       \
        }                                                                     \
    }                                                                         \
  while (0)

/* Helper function to inspect string builder. */
void _debugsb (stringbuilder *sb);

int
main ()
{
#if IN_ARENA
  arena ar = { 0 };
#endif /* IN_ARENA */
  stringbuilder sb = { 0 };
  char *str;

#if IN_ARENA
  sbinit (&sb, &ar);
#else
  sbinit2 (&sb);
#endif /* IN_ARENA */

  SBAPPEND (sb, "Hello, ");
  SBAPPEND (sb, "World!");
  SBAPPEND (sb, " Flushing the string builder will build the string.");

#if DEBUG
  _debugsb (&sb);
#endif /* DEBUG */

  str = sbflush (&sb);
  printf ("Final String: \"%s\"\n", str);

#if IN_ARENA
  arfree (str);
#else
  free (str);
#endif /* IN_ARENA */

  SBAPPEND (sb, "You can re-use the same string builder after flush.");

#if DEBUG
  _debugsb (&sb);
#endif /* DEBUG */

  str = sbflush (&sb);
  printf ("Final String: \"%s\"\n", str);

#if IN_ARENA
  arfree (str);
#else
  free (str);
#endif /* IN_ARENA */

cleanup:
#if IN_ARENA
  arfold (&ar);
#else
  sbfold (&sb);
#endif /* IN_ARENA */
}

void
_debugsb (stringbuilder *sb)
{
  clomy_sbchunk *ptr = sb->head;

  printf ("\n-------------------\n");
  printf ("Size: %d\n", sb->size);
  while (ptr)
    {
      printf ("(Size: %d) => \"%.*s\"\n", ptr->size, ptr->size, ptr->data);
      ptr = ptr->next;
    }
  printf ("-------------------\n\n");
}
