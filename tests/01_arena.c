#define CLOMY_IMPLEMENTATION
#define CLOMY_ARENA_CAPACITY 32
#include "../clomy.h"
#define CLOMY_TEST_IMPLEMENTATION
#include "../clomy_test.h"

void _debugarena (arena *ar);

int
main ()
{
  arena ar = { 0 };
  void *buf1, *buf2, *buf3;
  const unsigned int cnk_headsize = sizeof (clomy_aralloc_hdr);

  printf ("Allocating buf1 16 bytes.\n");
  buf1 = (char *)aralloc (&ar, 16);
  FAILFALSE (buf1, "buf1 not allocated.");
  FAILFALSE (ar.head->capacity == 16 + cnk_headsize,
             "buf1 incorrect chunk capacity.");

  _debugarena (&ar);

  printf ("Allocating buf2 16 bytes.\n");
  buf2 = (char *)aralloc (&ar, 16);
  FAILFALSE (buf2, "buf2 not allocated.");
  FAILFALSE (ar.head->next->capacity == 16 + cnk_headsize,
             "buf1 incorrect chunk capacity.");

  _debugarena (&ar);

  printf ("Freeing buf1, buf2...\n");
  arfree (buf1);
  FAILFALSE (ar.head->size == 0, "buf1 didn't free.");
  arfree (buf2);
  FAILFALSE (ar.head->next->size == 0, "buf2 didn't free.");

  _debugarena (&ar);

  printf ("Allocating buf3 32 bytes.\n");
  buf3 = (char *)aralloc (&ar, 32);
  FAILFALSE (buf3, "buf3 not allocated.");
  FAILFALSE (ar.head->capacity == 2 * (16 + cnk_headsize),
             "arena didn't fold.");
  _debugarena (&ar);

  arfold (&ar);

  return 0;
}

void
_debugarena (arena *ar)
{
  archunk *cnk = ar->head;
  printf ("-------------------\n");
  while (cnk)
    {
      printf ("(%d/%d) %p \n", cnk->size, cnk->capacity, cnk->data);
      cnk = cnk->next;
    }
  printf ("-------------------\n");
}
