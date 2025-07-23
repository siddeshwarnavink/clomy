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
#include <time.h>

#ifndef CLOMY_ARENA_CAPACITY
#define CLOMY_ARENA_CAPACITY (8 * 1024)
#endif // not CLOMY_ARENA_CAPACITY

#define CLOMY_ALIGN_UP(n, a) (((n) + ((a) - 1)) & ~((a) - 1))

struct clomy_archunk
{
  unsigned int size;
  unsigned int capacity;
  struct clomy_archunk *next;
  unsigned char data[];
};
typedef struct clomy_archunk clomy_archunk;

struct clomy_aralloc_hdr
{
  struct clomy_archunk *cnk;
  unsigned short size;
};
typedef struct clomy_aralloc_hdr clomy_aralloc_hdr;

struct clomy_arena
{
  clomy_archunk *head, *tail;
};
typedef struct clomy_arena clomy_arena;

/* Allocate memory inside arena. */
void *clomy_aralloc (clomy_arena *ar, unsigned int size);

/* Free the memory chunk inside arena. */
void clomy_arfree (void *value);

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

struct clomy_htdata
{
  int key;
  struct clomy_htdata *next;
  unsigned char data[];
};
typedef struct clomy_htdata clomy_htdata;

struct clomy_ht
{
  clomy_arena *ar;
  clomy_htdata **data;
  unsigned int data_size;
  unsigned int size;
  unsigned int capacity;
  unsigned int a;
};
typedef struct clomy_ht clomy_ht;

unsigned int _clomy_hash_int (clomy_ht *ht, unsigned int x);

/* Initialize hash table with unsigned integer key. */
int clomy_htinit (clomy_ht *ht, clomy_arena *ar, unsigned int capacity,
                  unsigned int dsize);

/* Put value in key in hash table. */
int clomy_htput (clomy_ht *ht, int key, void *value);

/* Get value for key hash table. */
void *clomy_htget (clomy_ht *ht, int key);

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

#define ht clomy_ht
#define htdata clomy_htdata
#define htinit clomy_htinit
#define htput clomy_htput
#define htget clomy_htget

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
  clomy_archunk *cnk;
  clomy_aralloc_hdr *hdr;
  unsigned int cnk_size;

  size = CLOMY_ALIGN_UP (size, 8);

  /* Initialize arena */
  if (!ar->head)
    {
      ar->head = _clomy_newarchunk (CLOMY_ARENA_CAPACITY);
      if (!ar->head)
        return (void *)0;

      ar->tail = ar->head;
    }

  /* Trying first-fit exising chunk. */
  cnk = ar->head;
  cnk_size = size + sizeof (clomy_aralloc_hdr);
  while (cnk)
    {
      if (cnk->capacity - cnk->size >= cnk_size)
        {
          hdr = (clomy_aralloc_hdr *)(cnk->data + cnk->size);
          hdr->cnk = cnk;
          hdr->size = size;

          cnk->size += cnk_size;
          return (void *)((char *)hdr + sizeof (clomy_aralloc_hdr));
        }

      cnk = cnk->next;
    }

  /* Allocate new memory. */
  cnk_size = cnk_size > CLOMY_ARENA_CAPACITY ? cnk_size : CLOMY_ARENA_CAPACITY;
  cnk = _clomy_newarchunk (cnk_size);
  if (!cnk)
    return (void *)0;

  cnk->size = cnk_size;

  ar->tail->next = cnk;
  ar->tail = cnk;

  hdr = (clomy_aralloc_hdr *)cnk->data;
  hdr->cnk = cnk;
  hdr->size = size;

  return (void *)((char *)hdr + sizeof (clomy_aralloc_hdr));
}

void
clomy_arfree (void *value)
{
  clomy_archunk *cnk;
  clomy_aralloc_hdr *hdr;

  if (!value)
    return;

  hdr = (clomy_aralloc_hdr *)((char *)value - sizeof (clomy_aralloc_hdr));
  cnk = hdr->cnk;
  cnk->size -= hdr->size + sizeof (clomy_aralloc_hdr);
}

void
clomy_arfold (clomy_arena *ar)
{
  clomy_archunk *cnk = ar->head, *next;
  while (cnk)
    {
      next = cnk->next;
      free (cnk);
      cnk = next;
    }

  ar->head = (void *)0;
  ar->tail = (void *)0;
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
          arfree (da->data);
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

  if (clomy_dagrow (da))
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
        clomy_arfree (da->data);
      else
        free (da->data);
      da->data = (void *)0;
    }
}

/*----------------------------------------------------------------------*/

unsigned int
_clomy_hash_int (clomy_ht *ht, unsigned int x)
{
  unsigned int p = ht->a ^ x;
  return p % ht->capacity;
}

int
clomy_htinit (clomy_ht *ht, clomy_arena *ar, unsigned int capacity,
              unsigned int dsize)
{
  srand ((unsigned)time (NULL));
  ht->a = ((unsigned int)rand () << 1) | 1;

  ht->ar = ar;
  ht->data_size = dsize;
  ht->size = 0;
  ht->capacity = CLOMY_ALIGN_UP (capacity, 8);

  ht->data = clomy_aralloc (ar, capacity * sizeof (clomy_htdata *));
  if (!ht->data)
    return 1;

  return 0;
}

int
clomy_htput (clomy_ht *ht, int key, void *value)
{
  clomy_htdata *data;
  unsigned int i = _clomy_hash_int (ht, key);

  data = clomy_aralloc (ht->ar, sizeof (clomy_htdata) + ht->data_size);
  if (!data)
    return 1;

  data->key = key;
  data->next = (void *)0;
  memcpy (data->data, value, ht->data_size);

  if (!ht->data[i])
    {
      ht->data[i] = data;
    }
  else
    {
      data->next = ht->data[i];
      ht->data[i] = data;
    }

  return 0;
}

void *
clomy_htget (clomy_ht *ht, int key)
{
  clomy_htdata *ptr;
  unsigned int i = _clomy_hash_int (ht, key);

  ptr = ht->data[i];
  if (!ptr)
    return (void *)0;

  while (ptr)
    {
      if (ptr->key == key)
        return ptr->data;
      ptr = ptr->next;
    }

  return (void *)0;
}

#endif /* CLOMY_IMPLEMENTATION */
