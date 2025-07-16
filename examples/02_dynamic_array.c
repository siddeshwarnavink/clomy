#include <stdio.h>

#define CLOMY_IMPLEMENTATION
#include "../clomy.h"

void
display_nums (da *nums)
{
  unsigned int i, *ptr;
  printf ("The numbers are:\n");
  for (i = 0; i < nums->size; ++i)
    {
      ptr = dageti (nums, i);
      printf ("%d\n", *ptr);
    }
}

int
main ()
{
  da nums = {0};
  unsigned int i, count, n;

  printf ("How many numbers?\n");
  scanf ("%d", &count);

  dainit (&nums, sizeof (int), count);
  printf ("Array capacity: %d\n", nums.capacity);

  printf ("Enter those numbers:\n");

  for (i = 0; i < count; ++i)
    {
      scanf ("%d", &n);
      daappend (&nums, &n);
    }

  printf ("Adding 69, 420 and 25...\n");

  /* Insert at beginning of array. */
  n = 69;
  if (dapush (&nums, &n) == 1)
    {
      fprintf (stderr, "Failed to add %d to array\n", n);
      goto cleanup;
    }

  /* Insert at end of array. */
  n = 420;
  if (daappend (&nums, &n) == 1)
    {
      fprintf (stderr, "Failed to add %d to array\n", n);
      goto cleanup;
    }

  /* Insert at Ith position of array. */
  n = 25;
  if (clomy_dainsert (&nums, &n, 2) == 1)
    {
      fprintf (stderr, "Failed to add %d to array\n", n);
      goto cleanup;
    }

  printf ("Array capacity: %d\n", nums.capacity);
  display_nums (&nums);

  dadel (&nums, 1);
  printf ("Delete at position 1...\n");
  display_nums (&nums);

cleanup:
  dafold (&nums);

  return 0;
}
