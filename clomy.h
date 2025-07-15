/* clomy.h - C library of my own - Siddeshwar

   To use this library:
     #define CLOMY_IMPLEMENTATION
     #include "clomy.h"

   To add "clomy_" namespace prefix:
     #define CLOMY_NO_SHORT_NAMES */

#ifndef CLOMY_H
#define CLOMY_H

#include <stdlib.h>

struct clomy_arena
{
  void *ptr;
  unsigned char *bytemap;
  unsigned int size;
  unsigned int capacity;
};
typedef struct clomy_arena clomy_arena;

/* Initialize the arena. Returns 0 if success and 1 if failed. */
int clomy_arinit (clomy_arena *ar, unsigned int size);

/* Allocate memory inside arena.  */
void *clomy_aralloc (clomy_arena *ar, unsigned int size);

/* De-allocate memory inside arena. */
/* TODO: After implementing hashtable, use it to store size. */
void clomy_arfree (clomy_arena *ar, void *ptr, unsigned int size);

/* Clear all memory inside arena. */
void clomy_arclear (clomy_arena *ar);

/* Free the entire arena. */
void clomy_arfold (clomy_arena *ar);

#ifndef CLOMY_NO_SHORT_NAMES

#define arena clomy_arena
#define arinit clomy_arinit
#define aralloc clomy_aralloc
#define arfree clomy_arfree
#define arclear clomy_arclear
#define arfold clomy_arfold

#endif /* not CLOMY_NO_SHORT_NAMES */

#endif /* not CLOMY_H */

#ifdef CLOMY_IMPLEMENTATION

int
clomy_arinit (clomy_arena *ar, unsigned int size)
{
  ar->ptr = (void *) malloc (size);
  if (!ar->ptr)
    return 1;

  /* TODO: Make this bitwise */
  ar->bytemap = (unsigned char *) malloc (size * sizeof (unsigned char));
  if (!ar->bytemap)
    return 1;

  ar->size = 0;
  ar->capacity = size;
  return 0;
}

void *
clomy_aralloc (clomy_arena *ar, unsigned int size)
{
  unsigned int i, j, start = 0, found_size = 0;

  if (ar->capacity - ar->size < size)
    return (void *)0;

  /* Simple first-fit approach. */
  for (i = 0; i <= ar->capacity; ++i)
    {
      if (found_size >= size)
        {
          for (j = start; j < start + size; ++j)
              ar->bytemap[j] = 1;

          ar->size += size;
          return (void *) ar->ptr + start + size;
        }

      if (!ar->bytemap[i])
        {
          if (found_size < 1)
            start = i;
          ++found_size;
        }
      else if (found_size < 1)
        {
          found_size = 0;
        }
    }

  return (void *) 0;
}

void
clomy_arfree (clomy_arena *ar, void *ptr, unsigned int size)
{
  unsigned long i = (unsigned long) ptr - (unsigned long) ar->ptr - size;
  unsigned long end = i + size;

  for ( ; i < end; ++i)
    {
      ar->bytemap[i] = 0;
    }

  ar->size -= size;
}

void
clomy_arclear (clomy_arena *ar)
{
  unsigned int i;
  for (i = 0; i < ar->capacity; ++i)
    {
      ar->bytemap[i] = 0;
    }

  ar->size = 0;
}

void
clomy_arfold (clomy_arena *ar)
{
  if (ar->ptr)
    {
      free (ar->ptr);
      ar->ptr = (void *) 0;
    }

  if (ar->bytemap)
    {
      free (ar->bytemap);
      ar->bytemap = (void *) 0;
    }

  ar->size = 0;
  ar->capacity = 0;
}

#endif /* CLOMY_IMPLEMENTATION */
