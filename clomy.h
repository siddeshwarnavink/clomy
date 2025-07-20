/* clomy.h - C library of my own - Siddeshwar

   To use this library:
     #define CLOMY_IMPLEMENTATION
     #include "clomy.h"

   To add "clomy_" namespace prefix:
     #define CLOMY_NO_SHORT_NAMES */

#ifndef CLOMY_H
#define CLOMY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef CLOMY_ARENA_DEFAULT_CAPACITY
#define CLOMY_ARENA_DEFAULT_CAPACITY (8 * 1024)
#endif // not CLOMY_ARENA_DEFAULT_CAPACITY

struct clomy_archunk
{
  unsigned int size;
  unsigned int capacity;
  struct clomy_archunk *next;
  unsigned char data[];
};
typedef struct clomy_archunk clomy_archunk;

struct clomy_arena
{
  clomy_archunk *head, *tail;
};
typedef struct clomy_arena clomy_arena;

/* Allocate memory inside arena. */
void *clomy_aralloc (clomy_arena *ar, unsigned int size);

/* Free the memory chunk inside arena. */
void clomy_arfree (clomy_arena *ar, void *value);

/* Free the entire arena. */
void clomy_arfold (clomy_arena *ar);

/*----------------------------------------------------------------------*/

struct clomy_da
{
  clomy_arena *ar;
  void *data;
  unsigned data_size;
  unsigned int size;
  unsigned int capacity;
};
typedef struct clomy_da clomy_da;

/* Initialize the dynamic array in arena. */
int clomy_dainit (clomy_da *da, clomy_arena *ar, unsigned int data_size,
                  unsigned int capacity);

/* Initialize the dynamic array in heap.*/
int clomy_dainit2 (clomy_da *da, unsigned int data_size,
                   unsigned int capacity);

/* Set the capacity of dynamic array. */
int clomy_dacap (clomy_da *da, unsigned int capacity);

/* Doubles the capacity of the array. */
int clomy_dagrow (clomy_da *da);

/* Get Ith element of dynamic array. */
void *clomy_dageti (clomy_da *da, unsigned int i);

/* Append data at the end of dynamic array. */
int clomy_daappend (clomy_da *da, void *data);

/* Append data at the start of dynamic array.*/
int clomy_dapush (clomy_da *da, void *data);

/* Insert data at Ith position of dynamic array. */
int clomy_dainsert (clomy_da *da, void *data, unsigned int i);

/* Delete data at Ith position of dynamic array. */
void clomy_dadel (clomy_da *da, unsigned int i);

/* Get Ith element of dynamic array. */
void *clomy_dageti (clomy_da *da, unsigned int i);

/* Free the dynamic array. */
void clomy_dafold (clomy_da *da);

/*----------------------------------------------------------------------*/

#ifndef CLOMY_NO_SHORT_NAMES

#define arena clomy_arena
#define archunk clomy_archunk
#define aralloc clomy_aralloc
#define arfree clomy_arfree
#define arfold clomy_arfold

#define da clomy_da
#define dainit clomy_dainit
#define dainit2 clomy_dainit2
#define dageti clomy_dageti
#define dapush clomy_dapush
#define daappend clomy_daappend
#define dainsert clomy_dainsert
#define dadel clomy_dadel
#define dafold clomy_dafold

#endif /* not CLOMY_NO_SHORT_NAMES */

#endif /* not CLOMY_H */

#ifdef CLOMY_IMPLEMENTATION

clomy_archunk *
_clomy_newarchunk (unsigned int size)
{
  unsigned int cnksize = sizeof (clomy_archunk) + size;
  clomy_archunk *cnk = (clomy_archunk *)malloc (cnksize);
  if (!cnk)
    return (clomy_archunk *)0;

  cnk->size = 0;
  cnk->capacity = size;
  cnk->next = (void *)0;
  return cnk;
}

void *
clomy_aralloc (clomy_arena *ar, unsigned int size)
{
  clomy_archunk *ptr, *newcnk;
  unsigned int dfsize = 0;

  /* Initialize arena */
  if (!ar->head)
    {
      ar->head = _clomy_newarchunk (CLOMY_ARENA_DEFAULT_CAPACITY);
      ar->tail = ar->head;
      if (!ar->head)
        return (void *)0;
    }

  /* Trying first-fit exising chunk. */
  ptr = ar->head;
  do
    {
      /* TODO: Implement folding adjacent memory. */
      dfsize = ptr->capacity - ptr->size;
      if (dfsize >= size)
        break;

      ptr = ptr->next;
    }
  while (ptr->next);

  /* Allocate new memory. */
  if (dfsize < size)
    {
      ptr = _clomy_newarchunk (CLOMY_ARENA_DEFAULT_CAPACITY);
      if (!ptr)
        return (void *)0;
      ar->tail->next = ptr;
      ar->tail = ptr;

      dfsize = ptr->capacity - ptr->size;
    }

  newcnk = (clomy_archunk *)&ptr->data[dfsize - size - sizeof (clomy_archunk)];
  newcnk->size = size;
  newcnk->capacity = size;
  newcnk->next = (void *)0;
  ar->tail->next = newcnk;
  ar->tail = newcnk;

  ptr->capacity = dfsize - size - sizeof (clomy_archunk);

  return newcnk->data;
}

void
clomy_arfree (clomy_arena *ar, void *value)
{
  clomy_archunk *ptr = ar->head;
  while (ptr)
    {
      if (ptr->data == value)
        {
          ptr->size = 0;
          break;
        }
      ptr = ptr->next;
    }
}

void
clomy_arfold (clomy_arena *ar)
{
  unsigned char prevzero = 0;
  clomy_archunk *ptr = ar->head, *ptr2;

  if (!ar->head)
    return;

  do
    {
      if (!prevzero && ptr->size == 0)
        {
          ptr2 = ptr->next;
          free (ptr);
          ptr = ptr2;
          prevzero = 1;
        }
      else
        {
          ptr = ptr->next;
          prevzero = 0;
        }
    }
  while (ptr->next);
}

/*----------------------------------------------------------------------*/

int
clomy_dainit (clomy_da *da, clomy_arena *ar, unsigned int data_size,
              unsigned int capacity)
{
  da->ar = ar;
  da->data = clomy_aralloc (ar, data_size * capacity);
  if (!da->data)
    return 1;

  da->data_size = data_size;
  da->size = 0;
  da->capacity = capacity;

  return 0;
}

int
clomy_dainit2 (clomy_da *da, unsigned int data_size, unsigned int capacity)
{
  da->ar = (void *)0;
  da->data = malloc (data_size * capacity);
  if (!da->data)
    return 1;

  da->data_size = data_size;
  da->size = 0;
  da->capacity = capacity;

  return 0;
}

int
clomy_dacap (clomy_da *da, unsigned int capacity)
{
  void *newarr;
  if (da->ar)
    {
      newarr = aralloc (da->ar, capacity * da->data_size);
      if (newarr)
        {
          /* TODO: Implement realloc in arena. */
          memcpy (newarr, da->data, da->capacity * da->data_size);
          arfree (da->ar, da->data);
        }
    }
  else
    {
      newarr = realloc (da->data, capacity * da->data_size);
    }

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

int
clomy_dagrow (clomy_da *da)
{
  if (da->size + 1 > da->capacity)
    return clomy_dacap (da, da->capacity * 2);
  return 0;
}

int
clomy_daappend (clomy_da *da, void *data)
{
  if (clomy_dagrow (da))
    return 1;

  memcpy ((char *)da->data + da->size * da->data_size, data, da->data_size);
  ++da->size;

  return 0;
}

int
clomy_dapush (clomy_da *da, void *data)
{
  if (clomy_dagrow (da))
    return 1;

  memmove ((char *)da->data + da->data_size, da->data,
           da->size * da->data_size);
  memcpy ((char *)da->data, data, da->data_size);
  ++da->size;

  return 0;
}

int
clomy_dainsert (clomy_da *da, void *data, unsigned int i)
{
  void *pos;

  if (i > da->size)
    return 1;

  if (clomy_dagrow(da))
      return 1;

  pos = da->data + i * da->data_size;
  memmove ((char *)pos + da->data_size, pos, (da->size - i) * da->data_size);
  memcpy ((char *)pos, data, da->data_size);
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

void
clomy_dafold (clomy_da *da)
{
  if (da->data)
    {
      if (da->ar)
        clomy_arfree (da->ar, da->data);
      else
        free (da->data);
      da->data = (void *)0;
    }
}

#endif /* CLOMY_IMPLEMENTATION */
