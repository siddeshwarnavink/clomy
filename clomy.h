/* clomy.h - C library of my own - Siddeshwar

   A header-only universal C library.

   Features:
     1. Arena
     2. Dynamic array
     3. Hash table
     4. String builder

   To use this library:
     #define CLOMY_IMPLEMENTATION
     #include "clomy.h"

   To add "clomy_" namespace prefix:
     #define CLOMY_NO_SHORT_NAMES

   To learn how to use this library I would recommend checking examples/ codes
   which is sorted in increasing complexity and detail explanation of the
   feature.

   Common conventions:
     1. Initialising function "*init" always initializes in arena whereas
        "*init2" allocates in heap.
     2. Any function involving memory allocation will return 0 if no error.  */

#ifndef CLOMY_H
#define CLOMY_H

#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef CLOMY_ARENA_CAPACITY
#define CLOMY_ARENA_CAPACITY (8 * 1024)
#endif // not CLOMY_ARENA_CAPACITY

#ifndef CLOMY_STRINGBUILDER_CAPACITY
#define CLOMY_STRINGBUILDER_CAPACITY 1024
#endif // not CLOMY_STRINGBUILDER_CAPACITY

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

struct clomy_stdata
{
  char *key;
  struct clomy_stdata *next;
  unsigned char data[];
};
typedef struct clomy_stdata clomy_stdata;

struct clomy_ht
{
  clomy_arena *ar;
  void **data;
  unsigned int data_size;
  unsigned int size;
  unsigned int capacity;
  unsigned int a;
};
typedef struct clomy_ht clomy_ht;

unsigned int _clomy_hash_int (clomy_ht *ht, unsigned int x);

unsigned int _clomy_hash_str (clomy_ht *ht, char *x);

/* Initialize hash table with either unsigned integer or string key. */
int clomy_htinit (clomy_ht *ht, clomy_arena *ar, unsigned int capacity,
                  unsigned int dsize);

/* Initialize hash table with either unsigned integer or string key in heap. */
int clomy_htinit2 (clomy_ht *ht, unsigned int capacity, unsigned int dsize);

/* Put value in unsigned integer key in hash table. */
int clomy_htput (clomy_ht *ht, int key, void *value);

/* Put value in string key in hash table. */
int clomy_stput (clomy_ht *ht, char *key, void *value);

/* Get value for unsigned integer key hash table. */
void *clomy_htget (clomy_ht *ht, int key);

/* Get value for string key hash table. */
void *clomy_stget (clomy_ht *ht, char *key);

/* Delete unsigned integer key from hash table. */
void clomy_htdel (clomy_ht *ht, int key);

/* Delete string key from hash table. */
void clomy_stdel (clomy_ht *ht, char *key);

/* Free the hash table with unsigned integer key. */
void clomy_htfold (clomy_ht *ht);

/* Free the hash table with string key. */
void clomy_stfold (clomy_ht *ht);

/*----------------------------------------------------------------------*/

struct clomy_sbchunk
{
  unsigned int size;
  struct clomy_sbchunk *next;
  char data[];
};
typedef struct clomy_sbchunk clomy_sbchunk;

struct clomy_stringbuilder
{
  clomy_arena *ar;
  unsigned int size;
  clomy_sbchunk *head, *tail;
};
typedef struct clomy_stringbuilder clomy_stringbuilder;

/* Initialize string builder. */
void clomy_sbinit (clomy_stringbuilder *sb, clomy_arena *ar);

/* Initialize string builder in heap. */
void clomy_sbinit2 (clomy_stringbuilder *sb);

/* Append string to the end of string builder. */
int clomy_sbappend (clomy_stringbuilder *sb, char *val);

/* Returns the constructed string and flushes the string builder. */
char *clomy_sbflush (clomy_stringbuilder *sb);

/* Free the string builder. */
void clomy_sbfold (clomy_stringbuilder *sb);

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
#define htinit2 clomy_htinit2
#define htput clomy_htput
#define stput clomy_stput
#define htget clomy_htget
#define stget clomy_stget
#define htdel clomy_htdel
#define stdel clomy_stdel
#define htfold clomy_htfold
#define stfold clomy_stfold

#define stringbuilder clomy_stringbuilder
#define sbinit clomy_sbinit
#define sbinit2 clomy_sbinit2
#define sbappend clomy_sbappend
#define sbflush clomy_sbflush
#define sbfold clomy_sbfold

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

  if (ar)
    da->data = clomy_aralloc (ar, data_size * capacity);
  else
    da->data = malloc (data_size * capacity);

  if (!da->data)
    return 1;

  da->data_size = data_size;
  da->size = 0;
  da->capacity = CLOMY_ALIGN_UP (capacity, 8);

  return 0;
}

int
clomy_dainit2 (clomy_da *da, unsigned int data_size, unsigned int capacity)
{
  return clomy_dainit (da, (void *)0, data_size, capacity);
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

unsigned int
_clomy_hash_str (clomy_ht *ht, char *x)
{
  unsigned int hash = ht->a;
  int c;

  while ((c = *x++))
    hash = ((hash << 5) + hash) + c;

  return hash % ht->capacity;
}

int
clomy_htinit (clomy_ht *ht, clomy_arena *ar, unsigned int capacity,
              unsigned int dsize)
{
  unsigned int size;

  capacity = CLOMY_ALIGN_UP (capacity, 8);
  size = capacity * sizeof (clomy_htdata *);

  srand ((unsigned)time (NULL));
  ht->a = ((unsigned int)rand () << 1) | 1;

  ht->ar = ar;
  ht->data_size = dsize;
  ht->size = 0;
  ht->capacity = capacity;

  if (ar)
    ht->data = clomy_aralloc (ar, size);
  else
    ht->data = malloc (size);

  if (!ht->data)
    return 1;

  return 0;
}

int
clomy_htinit2 (clomy_ht *ht, unsigned int capacity, unsigned int dsize)
{
  return clomy_htinit (ht, (void *)0, capacity, dsize);
}

int
clomy_htput (clomy_ht *ht, int key, void *value)
{
  clomy_htdata *data;
  unsigned int i = _clomy_hash_int (ht, key),
               size = sizeof (clomy_htdata) + ht->data_size;

  if (ht->ar)
    data = clomy_aralloc (ht->ar, size);
  else
    data = malloc (size);

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
  ++ht->size;

  return 0;
}

int
clomy_stput (clomy_ht *ht, char *key, void *value)
{
  clomy_stdata *data;
  unsigned int i = _clomy_hash_str (ht, key),
               size = sizeof (clomy_stdata) + ht->data_size,
               keylen = strlen (key);

  if (ht->ar)
    data = clomy_aralloc (ht->ar, size);
  else
    data = malloc (size);

  if (!data)
    return 1;

  if (ht->ar)
    data->key = clomy_aralloc (ht->ar, keylen);
  else
    data->key = malloc (keylen);

  if (!data->key)
    return 1;

  strcpy (data->key, key);
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
  ++ht->size;

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

void *
clomy_stget (clomy_ht *ht, char *key)
{
  clomy_stdata *ptr;

  unsigned int i = _clomy_hash_str (ht, key);

  ptr = ht->data[i];
  if (!ptr)
    return (void *)0;

  while (ptr)
    {
      if (strcmp (ptr->key, key) == 0)
        return ptr->data;
      ptr = ptr->next;
    }

  return (void *)0;
}

void
clomy_htdel (clomy_ht *ht, int key)
{
  clomy_htdata *ptr, *prev = (void *)0;
  unsigned int i = _clomy_hash_int (ht, key);

  ptr = ht->data[i];

  while (ptr)
    {
      if (ptr->key == key)
        {
          if (prev)
            prev->next = ptr->next;

          if (ht->ar)
            arfree (ptr);
          else
            free (ptr);

          if (!prev)
            ht->data[i] = (void *)0;

          --ht->size;
          break;
        }

      prev = ptr;
      ptr = ptr->next;
    }
}

void
clomy_stdel (clomy_ht *ht, char *key)
{
  clomy_stdata *ptr, *prev = (void *)0;
  unsigned int i = _clomy_hash_str (ht, key);

  ptr = ht->data[i];

  while (ptr)
    {
      if (ptr->key == key)
        {
          if (prev)
            prev->next = ptr->next;

          if (ht->ar)
            {
              arfree (ptr->key);
              arfree (ptr);
            }
          else
            {
              free (ptr->key);
              free (ptr);
            }

          if (!prev)
            ht->data[i] = (void *)0;

          --ht->size;
          break;
        }

      prev = ptr;
      ptr = ptr->next;
    }
}

void
clomy_htfold (clomy_ht *ht)
{
  clomy_htdata *ptr, *next;
  unsigned int i;

  for (i = 0; i < ht->capacity; ++i)
    {
      ptr = ht->data[i];

      while (ptr)
        {
          next = ptr->next;

          if (ht->ar)
            arfree (ptr);
          else
            free (ptr);

          --ht->size;
          ptr = next;
        }
    }

  if (ht->ar)
    arfree (ht->data);
  else
    free (ht->data);
}

void
clomy_stfold (clomy_ht *ht)
{
  clomy_stdata *ptr, *next;
  unsigned int i;

  for (i = 0; i < ht->capacity; ++i)
    {
      ptr = ht->data[i];

      while (ptr)
        {
          next = ptr->next;

          if (ht->ar)
            {
              arfree (ptr->key);
              arfree (ptr);
            }
          else
            {
              free (ptr->key);
              free (ptr);
            }

          --ht->size;
          ptr = next;
        }
    }

  if (ht->ar)
    arfree (ht->data);
  else
    free (ht->data);
}

/*----------------------------------------------------------------------*/

clomy_sbchunk *
_clomy_newsbchunk (clomy_arena *ar)
{
  unsigned int cnksize = sizeof (clomy_sbchunk) + CLOMY_STRINGBUILDER_CAPACITY;

  clomy_sbchunk *cnk;

  if (ar)
    cnk = (clomy_sbchunk *)clomy_aralloc (ar, cnksize);
  else
    cnk = (clomy_sbchunk *)malloc (cnksize);

  if (!cnk)
    return (clomy_sbchunk *)0;

  cnk->size = 0;
  cnk->next = (void *)0;
  return cnk;
}

void
clomy_sbinit (clomy_stringbuilder *sb, clomy_arena *ar)
{
  sb->ar = ar;
  sb->size = 0;
}

void
clomy_sbinit2 (clomy_stringbuilder *sb)
{
  clomy_sbinit (sb, (void *)0);
}

int
clomy_sbappend (clomy_stringbuilder *sb, char *val)
{
  clomy_sbchunk *ptr;
  unsigned int i = 0, j = 0;

  if (!sb->head)
    {
      sb->head = _clomy_newsbchunk (sb->ar);
      if (!sb->head)
        return 1;

      sb->tail = sb->head;
    }

  ptr = sb->tail;

  do
    {
      if (j >= CLOMY_STRINGBUILDER_CAPACITY)
        {
          if (!ptr->next)
            {
              ptr->next = _clomy_newsbchunk (sb->ar);
              if (!ptr->next)
                return 1;

              ptr = ptr->next;
              sb->tail = ptr;
            }
          else
            {
              ptr = ptr->next;
            }
        }

      j = ptr->size;

      while (val[i] != '\0' && j < CLOMY_STRINGBUILDER_CAPACITY)
        {
          ptr->data[j++] = val[i++];
          ++ptr->size;
          ++sb->size;
        }
    }
  while (val[i] != '\0');

  return 0;
}

char *
clomy_sbflush (clomy_stringbuilder *sb)
{
  clomy_sbchunk *ptr = sb->head;
  unsigned int size = sb->size + 1, i, j = 0;
  char *str;

  if (!ptr)
    return (char *)0;

  if (sb->ar)
    str = clomy_aralloc (sb->ar, size);
  else
    str = malloc (size);

  if (!str)
    return (char *)0;

  while (ptr)
    {
      for (i = 0; i < ptr->size; ++i)
        {
          str[j++] = ptr->data[i];
          ptr->data[i] = 0;
        }

      ptr->size = 0;
      ptr = ptr->next;
    }

  sb->size = 0;
  sb->tail = sb->head;

  str[j] = '\0';

  return str;
}

void
clomy_sbfold (clomy_stringbuilder *sb)
{
  clomy_sbchunk *ptr = sb->head, *next;

  while (ptr)
    {
      next = ptr->next;

      if (sb->ar)
        clomy_arfree (ptr);
      else
        free (ptr);

      ptr = next;
    }

  sb->size = 0;
  sb->head = (void *)0;
  sb->tail = (void *)0;
}

#endif /* CLOMY_IMPLEMENTATION */
