/* #define CLOMY_ARENA_CAPACITY 64 */
#define CLOMY_IMPLEMENTATION

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define CLOMY_BACKEND_WINAPI
#endif

#include "../build/clomy.h"

int
main ()
{
  arena ar = { 0 };
  void *buf1, *buf2, *buf3;

  printf ("Allocating buf1 16 bytes.\n");
  buf1 = (char *)aralloc (&ar, 16);
  FAILFALSE (buf1, "buf1 not allocated.");
  printf ("buf1=%p\n", buf1);

  printf ("Allocating buf2 16 bytes.\n");
  buf2 = (char *)aralloc (&ar, 16);
  FAILFALSE (buf2, "buf2 not allocated.");
  printf ("buf2=%p\n", buf2);

  printf ("Freeing buf1...\n");
  arfree (buf1);
  /* FAILFALSE (ar.head->free_list->size == 32, "buf1 didn't free."); */
  FAILFALSE (ar.head->free_list->size == 40, "buf1 didn't free.");

  printf ("Freeing buf2...\n"); arfree (buf2);
  /* FAILFALSE (ar.head->free_list->size == 64, "buf2 didn't free."); */
  FAILFALSE (ar.head->free_list->size == 80, "buf2 didn't free.");

  printf ("Allocating buf3 32 bytes.\n");
  buf3 = (char *)aralloc (&ar, 32);
  FAILFALSE (buf3, "buf3 not allocated.");

  arfold (&ar);

  return 0;
}
