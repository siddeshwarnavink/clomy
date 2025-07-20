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
#define CLOMY_ARENA_DEFAULT_CAPACITY (8*1024)
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

#ifndef CLOMY_NO_SHORT_NAMES

#define arena clomy_arena
#define archunk clomy_archunk
#define aralloc clomy_aralloc
#define arfree clomy_arfree
#define arfold clomy_arfold

#endif /* not CLOMY_NO_SHORT_NAMES */

#endif /* not CLOMY_H */

#ifdef CLOMY_IMPLEMENTATION

clomy_archunk *
_clomy_newarchunk (unsigned int size)
{
  unsigned int cnksize = sizeof (clomy_archunk) + size;
  clomy_archunk *cnk = (clomy_archunk *) malloc (cnksize);
  if (!cnk)
    return (clomy_archunk *) 0;

  cnk->size = 0;
  cnk->capacity= size;
  cnk->next = (void *) 0;
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
        return (void *) 0;
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
        return (void *) 0;
      ar->tail->next = ptr;
      ar->tail = ptr;

      dfsize = ptr->capacity - ptr->size;
    }

  newcnk = (clomy_archunk *) &ptr->data[dfsize - size - sizeof (clomy_archunk)];
  newcnk->size = size;
  newcnk->capacity = size;
  newcnk->next = (void *) 0;
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

#endif /* CLOMY_IMPLEMENTATION */
