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
  arena ar = { 0 };
  da nums = { 0 };
  unsigned int i, count, n;

  printf ("How many numbers?\n");
  scanf ("%d", &count);

  if (dainit (&nums, &ar, sizeof (int), count) == 1)
  /* if (dainit2 (&nums, sizeof (int), count) == 1) */
    {
      fprintf (stderr, "Failed to initialize dynamic array.\n");
      goto cleanup;
    }

  printf ("Array capacity: %d\n", nums.capacity);

  printf ("Enter those numbers:\n");

  for (i = 0; i < count; ++i)
    {
      scanf ("%d", &n);
      daappend (&nums, &n);
    }

  display_nums (&nums);

  printf ("Adding 69, 420 and 25...\n");

  n = 69;
  if (dapush (&nums, &n) == 1)
    {
      fprintf (stderr, "Failed to add %d to array\n", n);
      goto cleanup;
    }

  n = 420;
  if (daappend (&nums, &n) == 1)
    {
      fprintf (stderr, "Failed to add %d to array\n", n);
      goto cleanup;
    }

  n = 25;
  if (clomy_dainsert (&nums, &n, 1) == 1)
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
  /* We don't need to explicitly fold the array when
     allocated inside arena */
  /* dafold (&nums); */
  arfold (&ar);

  return 0;
}
