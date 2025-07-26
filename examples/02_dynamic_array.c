/* Dynamic array is an array whose can be adjusted at runtime. For dynamic
   array of characters use String Builder instead. */

#include <stdio.h>

#define CLOMY_IMPLEMENTATION
#include "../clomy.h"

#define IN_ARENA 1 /* Set to 0 to allocate in heap. */
#define DEBUG 0    /* Set to 1 to inspect dynamic array. */

/* Helper macro to check if any error. */
#define CHECKERR(res)                                                         \
  if ((res) != 0)                                                             \
    {                                                                         \
      fprintf (stderr, "An error occurred!\n");                               \
      goto cleanup;                                                           \
    }

/* Helper function to print dynamic array of integers. */
void _display_nums (da *nums);

int
main ()
{
#if IN_ARENA
  arena ar = { 0 };
#endif
  da nums = { 0 };
  unsigned int i, capacity, n;

  printf ("How many numbers?\n");
  scanf ("%d", &capacity);

  /* Here SIZEOF (INT) denotes size individual element of array. */
#if IN_ARENA
  CHECKERR (dainit (&nums, &ar, sizeof (int), capacity));
#else
  CHECKERR (dainit2 (&nums, sizeof (int), capacity));
#endif

#if DEBUG
  printf ("Array capacity: %d\n", nums.capacity);
#endif

  printf ("Enter those numbers:\n");

  for (i = 0; i < capacity; ++i)
    {
      scanf ("%d", &n);
      daappend (&nums, &n);
    }

  _display_nums (&nums);

  /* Pass pointer of element data to add to dynamic array. Internally it'll
     copy the element data. */
  printf ("Adding 16 to front of array...\n");
  n = 16;
  CHECKERR (dapush (&nums, &n));

  printf ("Adding 32 to end of array...\n");
  n = 32;
  CHECKERR (daappend (&nums, &n));

  printf ("Adding 64 to I=1 of array...\n");
  n = 64;
  CHECKERR (clomy_dainsert (&nums, &n, 1));

#if DEBUG
  printf ("Array capacity: %d\n", nums.capacity);
#endif

  _display_nums (&nums);

  printf ("Delete at position I=1 of array...\n");
  dadel (&nums, 1);
  _display_nums (&nums);

cleanup:
#if IN_ARENA
  arfold (&ar);
#else
  dafold (&nums);
#endif

  return 0;
}

void
_display_nums (da *nums)
{
  unsigned int i, *ptr;

  printf ("The numbers are: \n");
  for (i = 0; i < nums->size; ++i)
    {
      ptr = dageti (nums, i);
      printf ("%d, ", *ptr);
    }
  printf ("\n");
}
