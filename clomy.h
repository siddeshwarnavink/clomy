/* clomy.h - C library of my own - Siddeshwar

   To use this library:
     #define CLOMY_IMPLEMENTATION
     #include "clomy.h"

   To add "clomy_" namespace prefix:
     #define CLOMY_NO_SHORT_NAMES */

#ifndef CLOMY_H
#define CLOMY_H

#include <stdlib.h>
#include <string.h>

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

/*----------------------------------------------------------------------*/

struct clomy_da
{
  void *data;
  unsigned data_size;
  unsigned int size;
  unsigned int capacity;
};
typedef struct clomy_da clomy_da;

/* Initialize the dynamic array in heap. Returns 0 if success and 1 if
failed. */
int clomy_dainit (clomy_da *da, unsigned int data_size,
                  unsigned int capacity);

/* Set the capacity of dynamic array. Returns 0 if success and 1 if failed. */
int clomy_dacap (clomy_da *da, unsigned int capacity);

/* Append data at the start of dynamic array. Returns 0 if success and 1 if
failed. */
int clomy_dapush (clomy_da *da, void *data);

/* Append data at the end of dynamic array. Returns 0 if success and 1 if
failed. */
int clomy_daappend (clomy_da *da, void *data);

/* Insert data at Ith position of dynamic array. Returns 0 if success and 1 if
failed. */
int clomy_dainsert (clomy_da *da, void *data, unsigned int i);

/* Delete data at Ith position of dynamic array. */
void clomy_dadel (clomy_da *da, unsigned int i);

/* Get Ith element of dynamic array. */
void *clomy_dageti (clomy_da *da, unsigned int i);

/* Free the dynamic array. */
void clomy_dafold (clomy_da *da);

#ifndef CLOMY_NO_SHORT_NAMES

#define arena clomy_arena
#define arinit clomy_arinit
#define aralloc clomy_aralloc
#define arfree clomy_arfree
#define arclear clomy_arclear
#define arfold clomy_arfold

#define da clomy_da
#define dainit clomy_dainit
#define dacap clomy_dacap
#define dapush clomy_dapush
#define daappend clomy_daappend
#define dainsert clomy_dainsert
#define dadel clomy_dadel
#define dageti clomy_dageti
#define dafold clomy_dafold

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

/*----------------------------------------------------------------------*/

int
clomy_dainit (clomy_da *da, unsigned int data_size, unsigned int capacity)
{
  da->data = malloc (data_size * capacity);
  if (!da->data)
    return 1;

  da->data_size = data_size;
  da->size = 0;
  da->capacity = capacity;

  return 0;
}

int
clomy_dapush (clomy_da *da, void *data)
{
  int res;
  if (da->size + 1 > da->capacity)
    {
      res = clomy_dacap (da, da->capacity * 2);
      if (res > 0)
        return 1;
    }

  memmove (da->data + da->data_size, da->data, da->size * da->data_size);
  memcpy (da->data, data, da->data_size);
  ++da->size;

  return 0;
}

int
clomy_daappend (clomy_da *da, void *data)
{
  int res;
  if (da->size + 1 > da->capacity)
    {
      res = clomy_dacap (da, da->capacity * 2);
      if (res > 0)
        return 1;
    }

  memcpy (da->data + da->size++ * da->data_size, data, da->data_size);

  return 0;
}

int
clomy_dainsert (clomy_da *da, void *data, unsigned int i)
{
  int res;
  void *pos;
  if (da->size + 1 > da->capacity)
    {
      res = clomy_dacap (da, da->capacity * 2);
      if (res > 0)
        return 1;
    }

  pos = da->data + i * da->data_size;
  memmove (pos + da->size, pos, da->size * da->data_size - i * da->data_size);
  memcpy (pos, data, da->data_size);
  ++da->size;

  return 0;
}

void
clomy_dadel (clomy_da *da, unsigned int i)
{
  void *pos = da->data + i * da->data_size;
  memmove (pos, pos + da->data_size,
           da->size * da->data_size - i * da->data_size);
  --da->size;
}

int
clomy_dacap (clomy_da *da, unsigned int capacity)
{
  void *newarr = reallocarray (da->data, capacity, da->data_size);
  if (!newarr)
    return 1;

  da->data = newarr;
  da->capacity = capacity;

  return 0;
}

void *
clomy_dageti (clomy_da *da, unsigned int i)
{
  return da->data + i * da->data_size;
}

void
clomy_dafold (clomy_da *da)
{
  if (da->data)
    {
      free (da->data);
      da->data = (void *) 0;
    }
}

#endif /* CLOMY_IMPLEMENTATION */
