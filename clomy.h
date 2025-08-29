/* clomy.h v1.0 - C library of my own - Siddeshwar

   A simple header-only C library for building cross-platform applications.

   Features:
     1. Arena
     2. Dynamic array
     3. Hash table
     4. String & String builder
     5. Assertions
     6. Essential Cross-platform API

   To use this library:
     #define CLOMY_IMPLEMENTATION
     #include "clomy.h"

   To add "clomy_" namespace prefix:
     #define CLOMY_NO_SHORT_NAMES */

#ifndef CLOMY_H
#define CLOMY_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(CLOMY_BACKEND_WINAPI)
#include <windows.h>
static HANDLE CLOMY__heap = NULL;

static inline char *
CLOMY_strncpy_ (char *dst, const char *src, size_t n)
{
  if (n == 0)
    return dst;

  size_t srclen = strlen (src);
  size_t copylen = (srclen < n) ? srclen : n;

  memcpy (dst, src, copylen);

  if (copylen < n)
    memset (dst + copylen, 0, n - copylen);

  return dst;
}

#define CLOMY_strcpy(dst, src, n) CLOMY_strncpy_ ((dst), (src), (n))
#elif defined(CLOMY_BACKEND_POSIX)
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define CLOMY_strcpy(dst, src, n) strncpy ((dst), (src), (n))
#else
#define CLOMY_strcpy(dst, src, n) strncpy ((dst), (src), (n))
#endif /* define(CLOMY_BACKEND_WINAPI) */

#ifndef CLOMY_ARENA_CAPACITY
#define CLOMY_ARENA_CAPACITY (8 * 1024)
#endif /* not CLOMY_ARENA_CAPACITY */

#ifndef CLOMY_STRINGBUILDER_CAPACITY
#define CLOMY_STRINGBUILDER_CAPACITY 1024
#endif /* not CLOMY_STRINGBUILDER_CAPACITY */

#ifndef CLOMY_ALLOC_MAGIC
#define CLOMY_ALLOC_MAGIC 0x00636E6B
#endif /* not CLOMY_ALLOC_MAGIC */

#ifndef NULL
#define NULL (void *)0
#endif /* not NULL */

#ifndef true
#define true 1
#endif /* not true */

#ifndef false
#define false 0
#endif /* not false */

typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;
typedef unsigned long U32l;
typedef unsigned long long U64;

typedef signed char S8;
typedef signed short S16;
typedef signed int S32;
typedef signed long long S64;

/*--------------------[ Assertion ]--------------------*/

#ifndef CLOMY_TEST_DISABLE

/* Utility to print error message. */
#define CLOMY_FAIL(msg)                                                        \
  do                                                                           \
    {                                                                          \
      fprintf (stderr, "%s:%d:0: Assertion failed: %s\n", __FILE__, __LINE__,  \
               (msg));                                                         \
      exit (1);                                                                \
    }                                                                          \
  while (0);

/* Utility to print error message if EXP is false. */
#define CLOMY_FAILFALSE(exp, msg)                                              \
  if (!(exp))                                                                  \
    CLOMY_FAIL ((msg));

/* Utility to print error message if EXP is true. */
#define CLOMY_FAILTRUE(exp, msg)                                               \
  if ((exp))                                                                   \
    CLOMY_FAIL ((msg));

#else

#define CLOMY_FAIL
#define CLOMY_FAILFALSE
#define CLOMY_FAILTRUE

#endif /* not CLOMY_TEST_DISABLE */

/*--------------------[ Arena ]--------------------*/

typedef struct clomy_arfree_block
{
  size_t size;
  struct clomy_arfree_block *next;
} clomy_arfree_block;

typedef struct clomy_archunk
{
  size_t size;
  size_t capacity;
  clomy_arfree_block *free_list;
  struct clomy_archunk *next;
  U8 data[];
} clomy_archunk;

typedef struct clomy_aralloc_hdr
{
  struct clomy_archunk *cnk;
  size_t size;
  U32 magic;
} clomy_aralloc_hdr;

typedef struct clomy_arena
{
  clomy_archunk *head, *tail;
} clomy_arena;

clomy_archunk *_clomy_newarchunk (size_t size);

clomy_arfree_block *_clomy_find_free_block (clomy_archunk *cnk,
                                            size_t needed_size,
                                            clomy_arfree_block **prev_ptr);

void _clomy_remove_free_block (clomy_archunk *cnk, clomy_arfree_block *block,
                               clomy_arfree_block *prev);

void _clomy_add_free_block (clomy_archunk *cnk, clomy_arfree_block *new_block);

/* Allocate memory inside arena. */
void *clomy_aralloc (clomy_arena *ar, size_t size);

/* Free the memory chunk inside arena. */
void clomy_arfree (void *value);

/* Free the entire arena. */
void clomy_arfold (clomy_arena *ar);

/*--------------------[ Dynamic Array ]--------------------*/

typedef struct clomy_da
{
  clomy_arena *ar;
  void *data;
  size_t data_size;
  size_t size;
  size_t capacity;
} clomy_da;

/* Initialize the dynamic array in arena. */
int clomy_dainit (clomy_da *da, clomy_arena *ar, size_t data_size,
                  size_t capacity);

/* Set the capacity of dynamic array. */
int clomy_dacap (clomy_da *da, size_t capacity);

/* Doubles the capacity of the array. */
int clomy_dagrow (clomy_da *da);

/* Get Ith element of dynamic array. */
void *clomy_daget (clomy_da *da, size_t i);
inline int clomy_daget_int (clomy_da *da, size_t i);
inline float clomy_daget_float (clomy_da *da, size_t i);
inline long clomy_daget_long (clomy_da *da, size_t i);
inline double clomy_daget_double (clomy_da *da, size_t i);
inline char clomy_daget_char (clomy_da *da, size_t i);
inline short clomy_daget_short (clomy_da *da, size_t i);
/**/

/* Append data at the end of dynamic array. */
int clomy_daappend (clomy_da *da, void *data);
inline int clomy_daappend_int (clomy_da *da, int data);
inline int clomy_daappend_float (clomy_da *da, float data);
inline int clomy_daappend_long (clomy_da *da, long data);
inline int clomy_daappend_double (clomy_da *da, double data);
inline int clomy_daappend_char (clomy_da *da, char data);
inline int clomy_daappend_short (clomy_da *da, short data);
/**/

/* Append data at the start of dynamic array.*/
int clomy_dapush (clomy_da *da, void *data);
inline int clomy_dapush_int (clomy_da *da, int data);
inline int clomy_dapush_float (clomy_da *da, float data);
inline int clomy_dapush_long (clomy_da *da, long data);
inline int clomy_dapush_double (clomy_da *da, double data);
inline int clomy_dapush_char (clomy_da *da, char data);
inline int clomy_dapush_short (clomy_da *da, short data);
/**/

/* Insert data at Ith position of dynamic array. */
int clomy_dainsert (clomy_da *da, size_t i, void *data);
inline int clomy_dainsert_int (clomy_da *da, size_t i, int data);
inline int clomy_dainsert_float (clomy_da *da, size_t i, float data);
inline int clomy_dainsert_long (clomy_da *da, size_t i, long data);
inline int clomy_dainsert_double (clomy_da *da, size_t i, double data);
inline int clomy_dainsert_char (clomy_da *da, size_t i, char data);
inline int clomy_dainsert_short (clomy_da *da, size_t i, short data);
/**/

/* Delete data at Ith position of dynamic array. */
void clomy_dadel (clomy_da *da, size_t i);

/* Delete data at front of the dynamic array and returns the data. */
void *clomy_dapop (clomy_da *da);
inline int clomy_dapop_int (clomy_da *da);
inline float clomy_dapop_float (clomy_da *da);
inline long clomy_dapop_long (clomy_da *da);
inline double clomy_dapop_double (clomy_da *da);
inline char clomy_dapop_char (clomy_da *da);
inline short clomy_dapop_short (clomy_da *da);
/**/

/* Get Ith element of dynamic array. */
void *clomy_daget (clomy_da *da, size_t i);
inline int clomy_daget_int (clomy_da *da, size_t i);
inline float clomy_daget_float (clomy_da *da, size_t i);
inline long clomy_daget_long (clomy_da *da, size_t i);
inline double clomy_daget_double (clomy_da *da, size_t i);
inline char clomy_daget_char (clomy_da *da, size_t i);
inline short clomy_daget_short (clomy_da *da, size_t i);
/**/

/* Get first element of dynamic array. */
void *clomy_dafirst (clomy_da *da);
inline int clomy_dafirst_int (clomy_da *da);
inline float clomy_dafirst_float (clomy_da *da);
inline long clomy_dafirst_long (clomy_da *da);
inline double clomy_dafirst_double (clomy_da *da);
inline char clomy_dafirst_char (clomy_da *da);
inline short clomy_dafirst_short (clomy_da *da);
/**/

/* Get last element of dynamic array. */
void *clomy_dalast (clomy_da *da);
inline int clomy_dalast_int (clomy_da *da);
inline float clomy_dalast_float (clomy_da *da);
inline long clomy_dalast_long (clomy_da *da);
inline double clomy_dalast_double (clomy_da *da);
inline char clomy_dalast_char (clomy_da *da);
inline short clomy_dalast_short (clomy_da *da);
/**/

/* Free the dynamic array. */
void clomy_dafold (clomy_da *da);

/*--------------------[ Hash Table ]--------------------*/

typedef struct clomy_htdata
{
  int key;
  struct clomy_htdata *next;
  U8 data[];
} clomy_htdata;

typedef struct clomy_stdata
{
  char *key;
  struct clomy_stdata *next;
  U8 data[];
} clomy_stdata;

typedef struct clomy_ht
{
  clomy_arena *ar;
  void **data;
  size_t data_size;
  size_t size;
  size_t capacity;
  U32 a;
} clomy_ht;

/* Loop through each item of hash table. */
#define clomy_ht_foreach(t, k, body)                                           \
  for (U32 __i = 0; __i < (t)->capacity; ++__i)                                \
    for (clomy_htdata *__data = (t)->data[__i]; __data; __data = __data->next) \
      {                                                                        \
        (k) = __data->key;                                                     \
        body                                                                   \
      }
#define clomy_st_foreach(t, k, body)                                           \
  for (U32 __i = 0; __i < (t)->capacity; ++__i)                                \
    for (clomy_stdata *__data = (t)->data[__i]; __data; __data = __data->next) \
      {                                                                        \
        (k) = __data->key;                                                     \
        body                                                                   \
      }
U32 _clomy_hash_int (clomy_ht *ht, int x);
U32 _clomy_hash_str (clomy_ht *ht, char *x);

/* Initialize hash table. */
int clomy_htinit (clomy_ht *ht, clomy_arena *ar, size_t capacity, size_t dsize);

/* Put value in KEY in hash table. */

int clomy_htput (clomy_ht *ht, int key, void *value);
inline int clomy_htput_int (clomy_ht *table, int key, int val);
inline int clomy_htput_float (clomy_ht *table, int key, float val);
inline int clomy_htput_long (clomy_ht *table, int key, long val);
inline int clomy_htput_double (clomy_ht *table, int key, double val);
inline int clomy_htput_char (clomy_ht *table, int key, char val);
inline int clomy_htput_short (clomy_ht *table, int key, short val);

int clomy_stput (clomy_ht *st, char *key, void *value);
inline int clomy_stput_int (clomy_ht *table, char *key, int val);
inline int clomy_stput_float (clomy_ht *table, char *key, float val);
inline int clomy_stput_long (clomy_ht *table, char *key, long val);
inline int clomy_stput_double (clomy_ht *table, char *key, double val);
inline int clomy_stput_char (clomy_ht *table, char *key, char val);
inline int clomy_stput_short (clomy_ht *table, char *key, short val);
/**/

/* Set integer value 1 if key doesn't exist, increment it otherwise. */
inline int clomy_htinc_int (clomy_ht *ht, int key);
inline int clomy_stinc_int (clomy_ht *ht, char *key);

/* Get value for KEY Hash table. */

void *clomy_htget (clomy_ht *ht, int key);
inline int *clomy_htget_int (clomy_ht *ht, int key);
inline float *clomy_htget_float (clomy_ht *ht, int key);
inline long *clomy_htget_long (clomy_ht *ht, int key);
inline double *clomy_htget_double (clomy_ht *ht, int key);
inline char *clomy_htget_char (clomy_ht *ht, int key);
inline short *clomy_htget_short (clomy_ht *ht, int key);

void *clomy_stget (clomy_ht *st, char *key);
inline int *clomy_stget_int (clomy_ht *st, char *key);
inline float *clomy_stget_float (clomy_ht *st, char *key);
inline long *clomy_stget_long (clomy_ht *st, char *key);
inline double *clomy_stget_double (clomy_ht *st, char *key);
inline char *clomy_stget_char (clomy_ht *st, char *key);
inline short *clomy_stget_short (clomy_ht *st, char *key);
/**/

/* Delete from hash table. */
void clomy_htdel (clomy_ht *ht, int key);
void clomy_stdel (clomy_ht *ht, char *key);

/* Free the hash table. */
void clomy_htfold (clomy_ht *ht);
void clomy_stfold (clomy_ht *ht);

/*--------------------[ String ]--------------------*/

typedef struct clomy_string
{
  clomy_arena *ar;
  size_t size; /* Size of string excluding NULL. */
  char *data;  /* NULL-terminated string. */
} clomy_string;

/* New string. */
clomy_string *clomy_stringnew (clomy_arena *ar, const char *s);

/* Copy string. */
clomy_string *clomy_stringcpy (clomy_string *s);

/* Convert to lower case. */
void clomy_string_lower (clomy_string *s);

/* Convert to upper case. */
void clomy_string_upper (clomy_string *s);

/* Remove the first character of the string. */
char clomy_string_chop_head (clomy_string *s);

/* Split string by space (default delimiter). */
clomy_string *clomy_string_split_delim (clomy_string *s, char delim);
inline clomy_string *clomy_string_split (clomy_string *s);

/* Trim string. */
void clomy_string_trim (clomy_string *s);

/* Free string. */
void clomy_stringfold (clomy_string *s);

/*--------------------[ String Builder ]--------------------*/

typedef struct clomy_sbchunk
{
  size_t size;
  size_t capacity;
  struct clomy_sbchunk *next;
  char data[];
} clomy_sbchunk;

typedef struct clomy_stringbuilder
{
  clomy_arena *ar;
  size_t size;
  clomy_sbchunk *head, *tail;
} clomy_stringbuilder;

/* Initialize string builder. */
void clomy_sbinit (clomy_stringbuilder *sb, clomy_arena *ar);

/* Append string to the end of string builder. */
int clomy_sbappend (clomy_stringbuilder *sb, char *val);

/* Append character to the end of string builder. */
int clomy_sbappendch (clomy_stringbuilder *sb, char val);

/* Insert string at Ith position of string builder. */
int clomy_sbinsert (clomy_stringbuilder *sb, char *val, size_t i);

/* Push string to the beginning of string builder. */
int clomy_sbpush (clomy_stringbuilder *sb, char *val);

/* Push character to the beginning of string builder. */
int clomy_sbpushch (clomy_stringbuilder *sb, char val);

/* Reverse the string. */
void clomy_sbrev (clomy_stringbuilder *sb);

/* Returns the constructed string and flushes the string builder. */
clomy_string *clomy_sbflush (clomy_stringbuilder *sb);

/* Reset the string builder. */
void clomy_sbreset (clomy_stringbuilder *sb);

/* Free the string builder. */
void clomy_sbfold (clomy_stringbuilder *sb);

/*--------------------[ Cross Platform API ]--------------------*/

/* Read entire file into single buffer. */
clomy_string *clomy_file_get_content (clomy_arena *ar, const char *file_path);

/*--------------------------------------------------------------*/

#ifndef CLOMY_NO_SHORT_NAMES

#define FAIL CLOMY_FAIL
#define FAILFALSE CLOMY_FAILFALSE
#define FAILTRUE CLOMY_FAILTRUE

#define arena clomy_arena
#define archunk clomy_archunk
#define aralloc clomy_aralloc
#define arfree clomy_arfree
#define arfold clomy_arfold

#define da clomy_da
#define dainit clomy_dainit
#define daget clomy_daget
#define daget_int clomy_daget_int
#define daget_float clomy_daget_float
#define daget_long clomy_daget_long
#define daget_double clomy_daget_double
#define daget_char clomy_daget_char
#define daget_short clomy_daget_short
#define dafirst clomy_dafirst
#define dafirst_int clomy_dafirst_int
#define dafirst_float clomy_dafirst_float
#define dafirst_long clomy_dafirst_long
#define dafirst_double clomy_dafirst_double
#define dafirst_char clomy_dafirst_char
#define dafirst_short clomy_dafirst_short
#define dalast clomy_dalast
#define dalast_int clomy_dalast_int
#define dalast_float clomy_dalast_float
#define dalast_long clomy_dalast_long
#define dalast_double clomy_dalast_double
#define dalast_char clomy_dalast_char
#define dalast_short clomy_dalast_short
#define dapush clomy_dapush
#define dapush_int clomy_dapush_int
#define dapush_float clomy_dapush_float
#define dapush_long clomy_dapush_long
#define dapush_double clomy_dapush_double
#define dapush_char clomy_dapush_char
#define dapush_short clomy_dapush_short
/**/
#define daappend clomy_daappend
#define daappend_int clomy_daappend_int
#define daappend_float clomy_daappend_float
#define daappend_long clomy_daappend_long
#define daappend_double clomy_daappend_double
#define daappend_char clomy_daappend_char
#define daappend_short clomy_daappend_short
/**/
#define dainsert clomy_dainsert
#define dainsert_int clomy_dainsert_int
#define dainsert_float clomy_dainsert_float
#define dainsert_long clomy_dainsert_long
#define dainsert_double clomy_dainsert_double
#define dainsert_char clomy_dainsert_char
#define dainsert_short clomy_dainsert_short
/**/

#define dadel clomy_dadel
#define dapop clomy_dapop
#define dapop_int clomy_dapop_int
#define dapop_float clomy_dapop_float
#define dapop_long clomy_dapop_long
#define dapop_double clomy_dapop_double
#define dapop_char clomy_dapop_char
#define dapop_short clomy_dapop_short
#define dafold clomy_dafold

#define ht clomy_ht
#define htdata clomy_htdata
#define htinit clomy_htinit
#define ht_foreach clomy_ht_foreach
#define htput clomy_htput
#define htput_int clomy_htput_int
#define htput_float clomy_htput_float
#define htput_long clomy_htput_long
#define htput_double clomy_htput_double
#define htput_char clomy_htput_char
#define htput_short clomy_htput_short
/**/
#define htget clomy_htget
#define htget_int clomy_htget_int
#define htget_float clomy_htget_float
#define htget_long clomy_htget_long
#define htget_double clomy_htget_double
#define htget_char clomy_htget_char
#define htget_short clomy_htget_short
/**/
#define htinc_int clomy_htinc_int
#define htdel clomy_htdel
#define htfold clomy_htfold
#define st_foreach clomy_st_foreach
#define stput clomy_stput
#define stput_int clomy_stput_int
#define stput_float clomy_stput_float
#define stput_long clomy_stput_long
#define stput_double clomy_stput_double
#define stput_char clomy_stput_char
#define stput_short clomy_stput_short
/**/
#define stget clomy_stget
#define stget_int clomy_stget_int
#define stget_float clomy_stget_float
#define stget_long clomy_stget_long
#define stget_double clomy_stget_double
#define stget_char clomy_stget_char
#define stget_short clomy_stget_short
/**/
#define stinc_int clomy_stinc_int
#define stdel clomy_stdel
#define stfold clomy_stfold
#define string clomy_string
#define stringnew clomy_stringnew
#define stringcpy clomy_stringcpy
#define string_lower clomy_string_lower
#define string_upper clomy_string_upper
#define string_chop_head clomy_string_chop_head
#define string_split_delim clomy_string_split_delim
#define string_split clomy_string_split
#define string_trim clomy_string_trim
#define stringfold clomy_stringfold

#define stringbuilder clomy_stringbuilder
#define sbinit clomy_sbinit
#define sbappend clomy_sbappend
#define sbappendch clomy_sbappendch
#define sbinsert clomy_sbinsert
#define sbpush clomy_sbpush
#define sbpushch clomy_sbpushch
#define sbrev clomy_sbrev
#define sbflush clomy_sbflush
#define sbfold clomy_sbfold
#define sbreset clomy_sbreset

#define file_get_content clomy_file_get_content

#endif /* not CLOMY_NO_SHORT_NAMES */

#define CLOMY_ALIGN_UP(n, a) (((n) + ((a) - 1)) & ~((a) - 1))

#ifdef CLOMY_IMPLEMENTATION

clomy_archunk *
_clomy_newarchunk (size_t size)
{
  size_t cnksize = sizeof (clomy_archunk) + size;
  clomy_archunk *cnk;

#if defined(CLOMY_BACKEND_WINAPI)
  cnk = HeapAlloc (CLOMY__heap ? CLOMY__heap
                               : (CLOMY__heap = GetProcessHeap ()),
                   HEAP_ZERO_MEMORY, cnksize);
#elif defined(CLOMY_BACKEND_POSIX)
  cnk = mmap (NULL, cnksize, PROT_READ | PROT_WRITE,
              MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

  if (cnk == MAP_FAILED)
    return NULL;
#else
  cnk = (clomy_archunk *)malloc (cnksize);
#endif /* defined(CLOMY_BACKEND_WINAPI) */

  if (!cnk)
    return NULL;

  cnk->size = 0;
  cnk->capacity = size;
  cnk->next = NULL;
  cnk->free_list = NULL;

  return cnk;
}

clomy_arfree_block *
_clomy_find_free_block (clomy_archunk *cnk, size_t needed_size,
                        clomy_arfree_block **prev_ptr)
{
  clomy_arfree_block *prev = NULL;
  clomy_arfree_block *current = cnk->free_list;

  while (current)
    {
      if (current->size >= needed_size)
        {
          if (prev_ptr)
            *prev_ptr = prev;
          return current;
        }
      prev = current;
      current = current->next;
    }

  if (prev_ptr)
    *prev_ptr = NULL;
  return NULL;
}

void
_clomy_remove_free_block (clomy_archunk *cnk, clomy_arfree_block *block,
                          clomy_arfree_block *prev)
{
  if (prev)
    prev->next = block->next;
  else
    cnk->free_list = block->next;
}

void
_clomy_add_free_block (clomy_archunk *cnk, clomy_arfree_block *new_block)
{
  clomy_arfree_block *prev = NULL;
  clomy_arfree_block *current = cnk->free_list;

  while (current && current < new_block)
    {
      prev = current;
      current = current->next;
    }

  new_block->next = current;
  if (prev)
    prev->next = new_block;
  else
    cnk->free_list = new_block;

  if (current && ((char *)new_block + new_block->size) == (char *)current)
    {
      new_block->size += current->size;
      new_block->next = current->next;
    }

  if (prev && ((char *)prev + prev->size) == (char *)new_block)
    {
      prev->size += new_block->size;
      prev->next = new_block->next;
    }
}

void *
clomy_aralloc (clomy_arena *ar, size_t size)
{
  clomy_archunk *cnk;
  clomy_aralloc_hdr *hdr;
  clomy_arfree_block *free_blk, *prev_free, *rem;
  size_t cnk_size;
  const size_t hdr_size = CLOMY_ALIGN_UP (sizeof (clomy_aralloc_hdr), 8);

  size = CLOMY_ALIGN_UP (size, 8);

  /* Initialize arena */
  if (!ar->head)
    {
      ar->head = _clomy_newarchunk (CLOMY_ARENA_CAPACITY);
      if (!ar->head)
        return NULL;

      ar->tail = ar->head;
    }

  /* Trying first-fit exising chunk. */
  cnk = ar->head;
  cnk_size = size + hdr_size;
  cnk_size = CLOMY_ALIGN_UP (cnk_size, 8);

  while (cnk)
    {
      free_blk = _clomy_find_free_block (cnk, cnk_size, &prev_free);
      if (free_blk)
        {
          _clomy_remove_free_block (cnk, free_blk, prev_free);

          if (free_blk->size >= cnk_size + sizeof (clomy_arfree_block))
            {
              rem = (clomy_arfree_block *)((char *)free_blk + cnk_size);
              rem->size = free_blk->size - cnk_size;
              _clomy_add_free_block (cnk, rem);
            }

          hdr = (clomy_aralloc_hdr *)free_blk;
          hdr->cnk = cnk;
          hdr->size = size;
          hdr->magic = CLOMY_ALLOC_MAGIC;

          return (void *)((char *)hdr + hdr_size);
        }

      if (cnk->capacity - cnk->size >= cnk_size)
        {
          hdr = (clomy_aralloc_hdr *)(cnk->data + cnk->size);
          hdr->cnk = cnk;
          hdr->size = size;
          hdr->magic = CLOMY_ALLOC_MAGIC;

          cnk->size += cnk_size;
          return (void *)((char *)hdr + hdr_size);
        }

      cnk = cnk->next;
    }

  /* Allocate new memory. */
  cnk_size = cnk_size > CLOMY_ARENA_CAPACITY ? cnk_size : CLOMY_ARENA_CAPACITY;
  cnk = _clomy_newarchunk (cnk_size);
  if (!cnk)
    return NULL;

  cnk->size = cnk_size;

  ar->tail->next = cnk;
  ar->tail = cnk;

  hdr = (clomy_aralloc_hdr *)cnk->data;
  hdr->cnk = cnk;
  hdr->size = size;
  hdr->magic = CLOMY_ALLOC_MAGIC;

  return (void *)((char *)hdr + hdr_size);
}

void
clomy_arfree (void *value)
{
  clomy_archunk *cnk;
  clomy_aralloc_hdr *hdr;
  clomy_arfree_block *blk;
  const size_t hdr_size = CLOMY_ALIGN_UP (sizeof (clomy_aralloc_hdr), 8);

  if (!value)
    return;

  hdr = (clomy_aralloc_hdr *)((char *)value - hdr_size);
  if (hdr->magic != CLOMY_ALLOC_MAGIC)
    return;

  cnk = hdr->cnk;

  hdr->magic = 0;

  blk = (clomy_arfree_block *)hdr;
  blk->size = hdr->size + hdr_size;
  blk->next = NULL;

  _clomy_add_free_block (cnk, blk);
}

void
clomy_arfold (clomy_arena *ar)
{
  clomy_archunk *cnk = ar->head, *next;
  while (cnk)
    {
      next = cnk->next;
#if defined(CLOMY_BACKEND_WINAPI)
      HeapFree (CLOMY__heap ? CLOMY__heap : (CLOMY__heap = GetProcessHeap ()),
                0, cnk);
#elif defined(CLOMY_BACKEND_POSIX)
      munmap (cnk, sizeof (clomy_archunk) + cnk->capacity);
#else
      free (cnk);
#endif /* defined(CLOMY_BACKEND_WINAPI) */
      cnk = next;
    }

  ar->head = NULL;
  ar->tail = NULL;
}

/*----------------------------------------------------------------------*/

int
clomy_dainit (clomy_da *da, clomy_arena *ar, size_t data_size, size_t capacity)
{
  CLOMY_FAILFALSE (ar, "Arena is required.");

  da->ar = ar;
  da->data = clomy_aralloc (ar, data_size * capacity);
  if (!da->data)
    return 1;

  da->data_size = data_size;
  da->size = 0;
  da->capacity = CLOMY_ALIGN_UP (capacity, 8);

  return 0;
}

int
clomy_dacap (clomy_da *da, size_t capacity)
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
clomy_daget (clomy_da *da, size_t i)
{
  return (char *)da->data + i * da->data_size;
}

int
clomy_daget_int (clomy_da *da, size_t i)
{
  return *(int *)clomy_daget (da, i);
}
float
clomy_daget_float (clomy_da *da, size_t i)
{
  return *(float *)clomy_daget (da, i);
}
long
clomy_daget_long (clomy_da *da, size_t i)
{
  return *(long *)clomy_daget (da, i);
}
double
clomy_daget_double (clomy_da *da, size_t i)
{
  return *(double *)clomy_daget (da, i);
}
char
clomy_daget_char (clomy_da *da, size_t i)
{
  return *(char *)clomy_daget (da, i);
}
short
clomy_daget_short (clomy_da *da, size_t i)
{
  return *(short *)clomy_daget (da, i);
}
/**/

void *
clomy_dafirst (clomy_da *da)
{
  return clomy_daget (da, 0);
}

int
clomy_dafirst_int (clomy_da *da)
{
  return *(int *)clomy_daget (da, 0);
}
float
clomy_dafirst_float (clomy_da *da)
{
  return *(float *)clomy_daget (da, 0);
}
long
clomy_dafirst_long (clomy_da *da)
{
  return *(long *)clomy_daget (da, 0);
}
double
clomy_dafirst_double (clomy_da *da)
{
  return *(double *)clomy_daget (da, 0);
}
char
clomy_dafirst_char (clomy_da *da)
{
  return *(char *)clomy_daget (da, 0);
}
short
clomy_dafirst_short (clomy_da *da)
{
  return *(short *)clomy_daget (da, 0);
}
/**/

void *
clomy_dalast (clomy_da *da)
{
  return clomy_daget (da, da->size - 1);
}

int
clomy_dalast_int (clomy_da *da)
{
  return *(int *)clomy_daget (da, da->size - 1);
}
float
clomy_dalast_float (clomy_da *da)
{
  return *(float *)clomy_daget (da, da->size - 1);
}
long
clomy_dalast_long (clomy_da *da)
{
  return *(long *)clomy_daget (da, da->size - 1);
}
double
clomy_dalast_double (clomy_da *da)
{
  return *(double *)clomy_daget (da, da->size - 1);
}
char
clomy_dalast_char (clomy_da *da)
{
  return *(char *)clomy_daget (da, da->size - 1);
}
short
clomy_dalast_short (clomy_da *da)
{
  return *(short *)clomy_daget (da, da->size - 1);
}
/**/

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
clomy_daappend_int (clomy_da *da, int data)
{
  return clomy_daappend (da, &data);
}
int
clomy_daappend_float (clomy_da *da, float data)
{
  return clomy_daappend (da, &data);
}
int
clomy_daappend_long (clomy_da *da, long data)
{
  return clomy_daappend (da, &data);
}
int
clomy_daappend_double (clomy_da *da, double data)
{
  return clomy_daappend (da, &data);
}
int
clomy_daappend_char (clomy_da *da, char data)
{
  return clomy_daappend (da, &data);
}
int
clomy_daappend_short (clomy_da *da, short data)
{
  return clomy_daappend (da, &data);
}
/**/

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
clomy_dapush_int (clomy_da *da, int data)
{
  return clomy_dapush (da, &(int){ data });
}
int
clomy_dapush_float (clomy_da *da, float data)
{
  return clomy_dapush (da, &(float){ data });
}
int
clomy_dapush_long (clomy_da *da, long data)
{
  return clomy_dapush (da, &(long){ data });
}
int
clomy_dapush_double (clomy_da *da, double data)
{
  return clomy_dapush (da, &(double){ data });
}
int
clomy_dapush_char (clomy_da *da, char data)
{
  return clomy_dapush (da, &(char){ data });
}
int
clomy_dapush_short (clomy_da *da, short data)
{
  return clomy_dapush (da, &(short){ data });
}
/**/

int
clomy_dainsert (clomy_da *da, size_t i, void *data)
{
  void *pos;

  if (i > da->size)
    return 1;

  if (clomy_dagrow (da))
    return 1;

  pos = (char *)da->data + i * da->data_size;
  memmove ((char *)pos + da->data_size, pos, (da->size - i) * da->data_size);
  memcpy ((char *)pos, data, da->data_size);
  ++da->size;

  return 0;
}

int
clomy_dainsert_int (clomy_da *da, size_t i, int data)
{
  return clomy_dainsert (da, i, &(int){ data });
}
int
clomy_dainsert_float (clomy_da *da, size_t i, float data)
{
  return clomy_dainsert (da, i, &(float){ data });
}
int
clomy_dainsert_long (clomy_da *da, size_t i, long data)
{
  return clomy_dainsert (da, i, &(long){ data });
}
int
clomy_dainsert_double (clomy_da *da, size_t i, double data)
{
  return clomy_dainsert (da, i, &(double){ data });
}
int
clomy_dainsert_char (clomy_da *da, size_t i, char data)
{
  return clomy_dainsert (da, i, &(char){ data });
}
int
clomy_dainsert_short (clomy_da *da, size_t i, short data)
{
  return clomy_dainsert (da, i, &(short){ data });
}
/**/

void
clomy_dadel (clomy_da *da, size_t i)
{
  void *pos = (char *)da->data + i * da->data_size;
  memmove (pos, (char *)pos + da->data_size,
           da->size * da->data_size - i * da->data_size);
  --da->size;
}

void *
clomy_dapop (clomy_da *da)
{
  void *data = aralloc (da->ar, da->data_size);
  memcpy (data, clomy_daget (da, 0), da->data_size);
  clomy_dadel (da, 0);
  return data;
}

int
clomy_dapop_int (clomy_da *da)
{
  return *(int *)clomy_dapop (da);
}
float
clomy_dapop_float (clomy_da *da)
{
  return *(float *)clomy_dapop (da);
}
long
clomy_dapop_long (clomy_da *da)
{
  return *(long *)clomy_dapop (da);
}
double
clomy_dapop_double (clomy_da *da)
{
  return *(double *)clomy_dapop (da);
}
char
clomy_dapop_char (clomy_da *da)
{
  return *(char *)clomy_dapop (da);
}
short
clomy_dapop_short (clomy_da *da)
{
  return *(short *)clomy_dapop (da);
}
/**/

void
clomy_dafold (clomy_da *da)
{
  if (da->data)
    {
      clomy_arfree (da->data);
      da->data = NULL;
    }
}

/*----------------------------------------------------------------------*/

U32
_clomy_hash_int (clomy_ht *ht, int x)
{
  U32 p = ht->a ^ x;
  return p % ht->capacity;
}

U32
_clomy_hash_str (clomy_ht *ht, char *x)
{
  U32 hash = ht->a;
  int c;

  while ((c = *x++))
    hash = ((hash << 5) + hash) + c;

  return hash % ht->capacity;
}

int
clomy_htinit (clomy_ht *ht, clomy_arena *ar, size_t capacity, size_t dsize)
{
  CLOMY_FAILFALSE (ar, "Arena is required.");

  size_t size;

  capacity = CLOMY_ALIGN_UP (capacity, 8);
  size = capacity * sizeof (clomy_htdata *);
  srand ((unsigned)time (NULL));
  ht->a = ((U32)rand () << 1) | 1;

  ht->ar = ar;
  ht->data_size = dsize;
  ht->size = 0;
  ht->capacity = capacity;
  ht->data = clomy_aralloc (ar, size);
  if (!ht->data)
    return 1;

  memset (ht->data, 0, size);

  return 0;
}

int
clomy_htput (clomy_ht *ht, int key, void *value)
{
  clomy_htdata *data;
  size_t i = _clomy_hash_int (ht, key);
  size_t size = sizeof (clomy_htdata) + ht->data_size;

  data = clomy_aralloc (ht->ar, size);
  if (!data)
    return 1;

  data->key = key;
  data->next = NULL;
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
clomy_htput_int (clomy_ht *table, int key, int val)
{
  return clomy_htput (table, key, &val);
}
int
clomy_htput_float (clomy_ht *table, int key, float val)
{
  return clomy_htput (table, key, &val);
}
int
clomy_htput_long (clomy_ht *table, int key, long val)
{
  return clomy_htput (table, key, &val);
}
int
clomy_htput_double (clomy_ht *table, int key, double val)
{
  return clomy_htput (table, key, &val);
}
int
clomy_htput_char (clomy_ht *table, int key, char val)
{
  return clomy_htput (table, key, &val);
}
int
clomy_htput_short (clomy_ht *table, int key, short val)
{
  return clomy_htput (table, key, &val);
}
/**/
int
clomy_htinc_int (clomy_ht *ht, int key)
{
  int *ptr = clomy_htget (ht, key);
  if (ptr)
    return clomy_htput_int (ht, key, *ptr + 1);
  else
    return clomy_htput_int (ht, key, 1);
}
/**/

int
clomy_stput_int (clomy_ht *table, char *key, int val)
{
  return clomy_stput (table, key, &val);
}
int
clomy_stput_float (clomy_ht *table, char *key, float val)
{
  return clomy_stput (table, key, &val);
}
int
clomy_stput_long (clomy_ht *table, char *key, long val)
{
  return clomy_stput (table, key, &val);
}
int
clomy_stput_double (clomy_ht *table, char *key, double val)
{
  return clomy_stput (table, key, &val);
}
int
clomy_stput_char (clomy_ht *table, char *key, char val)
{
  return clomy_stput (table, key, &val);
}
int
clomy_stput_short (clomy_ht *table, char *key, short val)
{
  return clomy_stput (table, key, &val);
}
/**/
int
clomy_stinc_int (clomy_ht *st, char *key)
{
  int *ptr = clomy_stget (st, key);
  if (ptr)
    return clomy_stput_int (st, key, *ptr + 1);
  else
    return clomy_stput_int (st, key, 1);
}
/**/
int
clomy_stput (clomy_ht *ht, char *key, void *value)
{
  clomy_stdata *data;
  size_t i = _clomy_hash_str (ht, key);
  size_t size = sizeof (clomy_stdata) + ht->data_size, keylen = strlen (key);

  data = clomy_aralloc (ht->ar, size);
  if (!data)
    return 1;

  data->key = clomy_aralloc (ht->ar, keylen + 1);
  if (!data->key)
    return 1;

  CLOMY_strcpy (data->key, key, keylen);
  data->next = NULL;
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
  size_t i = _clomy_hash_int (ht, key);

  ptr = ht->data[i];
  if (!ptr)
    return NULL;

  while (ptr)
    {
      if (ptr->key == key)
        return ptr->data;
      ptr = ptr->next;
    }

  return NULL;
}

void *
clomy_stget (clomy_ht *ht, char *key)
{
  clomy_stdata *ptr;

  size_t i = _clomy_hash_str (ht, key);

  ptr = ht->data[i];
  if (!ptr)
    return NULL;

  while (ptr)
    {
      if (strcmp (ptr->key, key) == 0)
        return ptr->data;
      ptr = ptr->next;
    }

  return NULL;
}

int *
clomy_htget_int (clomy_ht *ht, int key)
{
  return (int *)clomy_htget (ht, key);
}
float *
clomy_htget_float (clomy_ht *ht, int key)
{
  return (float *)clomy_htget (ht, key);
}
long *
clomy_htget_long (clomy_ht *ht, int key)
{
  return (long *)clomy_htget (ht, key);
}
double *
clomy_htget_double (clomy_ht *ht, int key)
{
  return (double *)clomy_htget (ht, key);
}
char *
clomy_htget_char (clomy_ht *ht, int key)
{
  return (char *)clomy_htget (ht, key);
}
short *
clomy_htget_short (clomy_ht *ht, int key)
{
  return (short *)clomy_htget (ht, key);
}

int *
clomy_stget_int (clomy_ht *st, char *key)
{
  return (int *)clomy_stget (st, key);
}
float *
clomy_stget_float (clomy_ht *st, char *key)
{
  return (float *)clomy_stget (st, key);
}
long *
clomy_stget_long (clomy_ht *st, char *key)
{
  return (long *)clomy_stget (st, key);
}
double *
clomy_stget_double (clomy_ht *st, char *key)
{
  return (double *)clomy_stget (st, key);
}
char *
clomy_stget_char (clomy_ht *st, char *key)
{
  return (char *)clomy_stget (st, key);
}
short *
clomy_stget_short (clomy_ht *st, char *key)
{
  return (short *)clomy_stget (st, key);
}
/**/

void
clomy_htdel (clomy_ht *ht, int key)
{
  clomy_htdata *ptr, *prev = NULL;
  size_t i = _clomy_hash_int (ht, key);

  ptr = ht->data[i];

  while (ptr)
    {
      if (ptr->key == key)
        {
          if (prev)
            prev->next = ptr->next;

          arfree (ptr);

          if (!prev)
            ht->data[i] = ptr->next;

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
  clomy_stdata *ptr, *prev = NULL;
  size_t i = _clomy_hash_str (ht, key);

  ptr = ht->data[i];

  while (ptr)
    {
      if (strcmp (ptr->key, key) == 0)
        {
          if (prev)
            prev->next = ptr->next;

          arfree (ptr->key);
          arfree (ptr);

          if (!prev)
            ht->data[i] = NULL;

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
  size_t i;

  for (i = 0; i < ht->capacity; ++i)
    {
      ptr = ht->data[i];

      while (ptr)
        {
          next = ptr->next;
          arfree (ptr);
          --ht->size;
          ptr = next;
        }
    }

  arfree (ht->data);
}

void
clomy_stfold (clomy_ht *ht)
{
  clomy_stdata *ptr, *next;
  size_t i;

  for (i = 0; i < ht->capacity; ++i)
    {
      ptr = ht->data[i];

      while (ptr)
        {
          next = ptr->next;
          arfree (ptr->key);
          arfree (ptr);
          --ht->size;
          ptr = next;
        }
    }

  arfree (ht->data);
}

/*----------------------------------------------------------------------*/

clomy_string *
clomy_stringnew (clomy_arena *ar, const char *s)
{
  clomy_string *new = clomy_aralloc (ar, sizeof (clomy_string));
  if (!new)
    return NULL;

  new->ar = ar;
  new->size = strlen (s);

  new->data = clomy_aralloc (ar, new->size + 1);
  if (!new->data)
    return NULL;

  CLOMY_strcpy (new->data, s, new->size);

  return new;
}

clomy_string *
clomy_stringcpy (clomy_string *s)
{
  string *str = clomy_aralloc (s->ar, sizeof (clomy_string));
  if (!str)
    return NULL;

  str->data = clomy_aralloc (s->ar, s->size + 1);
  str->ar = s->ar;
  str->size = s->size;
  CLOMY_strcpy (str->data, s->data, str->size);
  return str;
}

void
clomy_string_lower (clomy_string *s)
{
  U8 i;
  for (i = 0; i < s->size; ++i)
    s->data[i] = tolower (s->data[i]);
}

void
clomy_string_upper (clomy_string *s)
{
  U8 i;
  for (i = 0; i < s->size; ++i)
    s->data[i] = toupper (s->data[i]);
}

char
clomy_string_chop_head (clomy_string *s)
{
  char ch;

  if (s->size <= 0)
    return '\0';

  ch = s->data[0];
  memmove (s->data, s->data + 1, --s->size);
  s->data[s->size + 1] = '\0';
  return ch;
}

clomy_string *
clomy_string_split_delim (clomy_string *s, char delim)
{
  clomy_stringbuilder sb = { 0 };
  string *res;
  char ch;

  sbinit (&sb, s->ar);

  while (s->size > 0 && (ch = clomy_string_chop_head (s)) != '\0'
         && ch != delim)
    {
      sbappendch (&sb, ch);
    }

  res = sbflush (&sb);
  return res;
}

clomy_string *
clomy_string_split (clomy_string *s)
{
  return clomy_string_split_delim (s, ' ');
}

void
clomy_string_trim (clomy_string *s)
{
  size_t a = 0, b = s->size - 1;

  while (isspace ((U8)s->data[a]))
    ++a;
  while (b > a && isspace ((U8)s->data[b]))
    --b;

  if (a > 0 || b < s->size)
    {
      memmove (s->data, s->data + a, b - a + 1);
      s->data[b - a + 1] = '\0';
    }
}

void
clomy_stringfold (clomy_string *s)
{
  arfree (s->data);
  arfree (s);
}

/*----------------------------------------------------------------------*/

clomy_sbchunk *
_clomy_newsbchunk (clomy_stringbuilder *sb, size_t capacity)
{
  clomy_sbchunk *cnk;
  size_t cnksize = sizeof (clomy_sbchunk) + capacity;

  cnk = (clomy_sbchunk *)aralloc (sb->ar, cnksize);
  if (!cnk)
    return NULL;

  cnk->size = 0;
  cnk->capacity = capacity;
  cnk->next = NULL;
  return cnk;
}

void
clomy_sbinit (clomy_stringbuilder *sb, clomy_arena *ar)
{
  sb->ar = ar;
  sb->size = 0;
}

int
clomy_sbappend (clomy_stringbuilder *sb, char *val)
{
  clomy_sbchunk *ptr;
  size_t i = 0, j = 0;

  if (!sb->head)
    {
      sb->head = _clomy_newsbchunk (sb, CLOMY_STRINGBUILDER_CAPACITY);
      if (!sb->head)
        return 1;

      sb->tail = sb->head;
    }

  ptr = sb->tail;

  do
    {
      if (j >= ptr->capacity)
        {
          if (!ptr->next)
            {
              ptr->next = _clomy_newsbchunk (sb, CLOMY_STRINGBUILDER_CAPACITY);
              if (!ptr->next)
                return 1;

              ptr = ptr->next;
            }
          else
            {
              ptr = ptr->next;
            }

          sb->tail = ptr;
        }

      j = ptr->size;

      while (val[i] != '\0' && j < ptr->capacity)
        {
          ptr->data[j++] = val[i++];
          ++ptr->size;
          ++sb->size;
        }
    }
  while (val[i] != '\0');

  return 0;
}

int
clomy_sbappendch (clomy_stringbuilder *sb, char val)
{
  clomy_sbchunk *ptr, *cnk;

  if (!sb->head)
    {
      sb->head = _clomy_newsbchunk (sb, CLOMY_STRINGBUILDER_CAPACITY);
      if (!sb->head)
        return 1;

      sb->tail = sb->head;
    }

  ptr = sb->tail;
  if (ptr->size + 1 > ptr->capacity)
    {
      cnk = _clomy_newsbchunk (sb, CLOMY_STRINGBUILDER_CAPACITY);
      if (!cnk)
        return 1;

      cnk->data[0] = val;
      cnk->size = 1;
      cnk->next = ptr->next;
      ptr->next = cnk;
      sb->tail = cnk;
    }
  else
    {
      ptr->data[ptr->size++] = val;
    }
  ++sb->size;

  return 0;
}

int
clomy_sbinsert (clomy_stringbuilder *sb, char *val, size_t i)
{
  clomy_sbchunk *ptr = sb->head, *prev, *cnk;
  int index = (int)i;
  size_t len = strlen (val), offset;

  do
    {
      if (index - (int)ptr->size < 0)
        break;

      index -= (int)ptr->size;
      ptr = ptr->next;
    }
  while (ptr);

  if (index == 0 || !ptr->next)
    {
      cnk = _clomy_newsbchunk (sb, len);
      if (!cnk)
        return 1;

      CLOMY_strcpy (cnk->data, val, len);
      cnk->size = len;
      cnk->next = ptr->next;

      ptr->next = cnk;
      sb->size += len;
    }
  else
    {
      prev = ptr;
      offset = ptr->size - index;

      cnk = _clomy_newsbchunk (sb, len - offset);
      if (!cnk)
        return 1;

      CLOMY_strcpy (cnk->data, &val[offset], len - offset);
      cnk->size = len - offset;
      cnk->next = ptr->next;
      ptr->next = cnk;
      ptr = cnk;

      cnk = _clomy_newsbchunk (sb, offset);
      if (!cnk)
        return 1;

      CLOMY_strcpy (cnk->data, &((char *)prev->data)[index], offset);
      cnk->size = offset;
      cnk->next = ptr->next;
      ptr->next = cnk;

      CLOMY_strcpy (&((char *)prev->data)[index], val, offset);
      sb->size += len;
    }

  return 0;
}

int
clomy_sbpush (clomy_stringbuilder *sb, char *val)
{
  clomy_sbchunk *cnk;
  size_t len = strlen (val);

  cnk = _clomy_newsbchunk (sb, len);
  if (!cnk)
    return 1;

  CLOMY_strcpy (cnk->data, val, len);

  cnk->size = len;
  cnk->next = sb->head;

  sb->head = cnk;
  sb->size += len;

  return 0;
}

int
clomy_sbpushch (clomy_stringbuilder *sb, char val)
{
  clomy_sbchunk *cnk;

  cnk = _clomy_newsbchunk (sb, 1);
  if (!cnk)
    return 1;

  cnk->data[0] = val;
  cnk->size = 1;
  cnk->next = sb->head;

  sb->head = cnk;
  ++sb->size;

  return 0;
}

void
clomy_sbrev (clomy_stringbuilder *sb)
{
  clomy_sbchunk *ptr = sb->head, *prev = NULL, *next;
  size_t a, b;
  char tmp;

  if (!ptr)
    return;

  sb->tail = sb->head;

  while (ptr)
    {
      a = 0;
      b = ptr->size - 1;
      while (b > a)
        {
          tmp = ptr->data[a];
          ptr->data[a] = ptr->data[b];
          ptr->data[b] = tmp;
          ++a;
          --b;
        }

      next = ptr->next;
      ptr->next = prev;
      prev = ptr;
      ptr = next;
    }

  sb->head = prev;
}

string *
clomy_sbflush (clomy_stringbuilder *sb)
{
  clomy_string *str;
  clomy_sbchunk *ptr = sb->head;
  size_t size = sb->size + 1, i, j = 0;

  if (!ptr)
    return (clomy_string *)0;

  str = clomy_aralloc (sb->ar, sizeof (clomy_string));
  if (!str)
    return NULL;

  str->data = clomy_aralloc (sb->ar, size);

  while (ptr)
    {
      for (i = 0; i < ptr->size; ++i)
        {
          str->data[j++] = ptr->data[i];
          ptr->data[i] = 0;
        }

      ptr->size = 0;
      ptr = ptr->next;
    }

  clomy_sbreset (sb);

  str->data[j] = '\0';
  str->size = size - 1;
  str->ar = sb->ar;

  return str;
}

void
clomy_sbreset (clomy_stringbuilder *sb)
{
  sb->size = 0;
  sb->tail = sb->head;
}

void
clomy_sbfold (clomy_stringbuilder *sb)
{
  clomy_sbchunk *ptr = sb->head, *next;

  while (ptr)
    {
      next = ptr->next;
      clomy_arfree (ptr);
      ptr = next;
    }

  sb->size = 0;
  sb->head = NULL;
  sb->tail = NULL;
}

/*----------------------------------------------------------------------*/

clomy_string *
clomy_file_get_content (clomy_arena *ar, const char *file_path)
{
  stringbuilder sb = { 0 };
#if defined(CLOMY_BACKEND_WINAPI)
  HANDLE hFile;
  DWORD dwBytesRead;
#elif defined(CLOMY_BACKEND_POSIX)
  int fd;
  ssize_t bytes_read;
#else
  FILE *file;
#endif /* defined(CLOMY_BACKEND_WINAPI) */
  clomy_string *res;
  char ch;

  clomy_sbinit (&sb, ar);

#if defined(CLOMY_BACKEND_WINAPI)
  hFile = CreateFile (file_path, GENERIC_READ, 0, NULL, OPEN_EXISTING,
                      FILE_ATTRIBUTE_NORMAL, NULL);

  if (hFile == INVALID_HANDLE_VALUE)
    return NULL;

  while (ReadFile (hFile, &ch, 1, &dwBytesRead, NULL) && dwBytesRead > 0)
    sbappendch (&sb, ch);

  CloseHandle (hFile);
#elif defined(CLOMY_BACKEND_POSIX)
  fd = open (file_path, O_RDONLY);
  if (fd == -1)
    return NULL;

  while ((bytes_read = read (fd, &ch, 1)) > 0)
    sbappendch (&sb, ch);

  if (bytes_read == -1)
    {
      close (fd);
      return NULL;
    }

  close (fd);
#else
  file = fopen (file_path, "r");
  if (!file)
    return NULL;

  while ((ch = fgetc (file)) != EOF)
    sbappendch (&sb, ch);

  fclose (file);
#endif /* defined(CLOMY_BACKEND_WINAPI) */

  res = sbflush (&sb);

  return res;
}

#endif /* CLOMY_IMPLEMENTATION */

#endif /* not CLOMY_H */