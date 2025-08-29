#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define CLOMY_BACKEND_WINAPI
#elif defined(_POSIX_VERSION)
#define CLOMY_BACKEND_POSIX
#endif /* defined(_WIN32) */

#define CLOMY_IMPLEMENTATION
#include "../build/clomy.h"

int
main ()
{
  arena ar = { 0 };
  ht cube_nums = { 0 }, student_gpa = { 0 };
  float gpa;
  int i, *numptr, num, key;
  char *skey;

  /* CLOMY only hash table with int and string as a key. All the general and
     int key hash table function prefix with 'ht'. The string key hash table
     function prefix with 'st'. */

  /* Here is an example for int key hash table. Here we are creating hash table
     of bucket size 8 and data of integer. */
  htinit (&cube_nums, &ar, 8, sizeof (int));

  /* Here we are setting value of 8 to key 2. */
  num = 8;
  if (htput (&cube_nums, 2, &num) == 1)
    {
      fprintf (stderr, "Failed to insert key 2.");
      arfold (&ar);
      return 1;
    }

  /* Throughout the library we follow the same pattern of primitive functions
     and helper wrapper function. */
  for (i = 3; i <= 10; ++i)
    {
      if (htput_int (&cube_nums, i, i * i * i) == 1)
        {
          fprintf (stderr, "Failed to insert key %d.", i);
          arfold (&ar);
          return 1;
        }
    }

  /* Getting value by key. */
  numptr = (int *)htget (&cube_nums, 2);
  if (numptr == NULL)
    {
      fprintf (stderr, "Key 2 not found.");
      arfold (&ar);
      return 1;
    }
  printf ("Value of 2 is %d\n", *numptr);

  printf ("Value of 8 is %d\n", *htget_int (&cube_nums, 8));

  /* Delete a key. */
  htdel (&cube_nums, 5);

  /* He have this special macro to loop through each key. */
  clomy_ht_foreach (&cube_nums, key, {
    num = *htget_int (&cube_nums, key);
    printf ("%d -> %d\n", key, num);
  });

  htfold (&cube_nums);

  /* Here is a quick example of hash table with string key */
  htinit (&student_gpa, &ar, 8, sizeof (float));

  if (stput (&student_gpa, "JohnDoe", &(float){ 8.2f }) == 1)
    {
      fprintf (stderr, "Failed to insert JohnDoe.");
      arfold (&ar);
      return 1;
    }

  if (stput_float (&student_gpa, "Sid", 9.8f) == 1)
    {
      fprintf (stderr, "Failed to insert Sid.");
      arfold (&ar);
      return 1;
    }

  if (stput_float (&student_gpa, "Dave", 7.2f) == 1)
    {
      fprintf (stderr, "Failed to insert Dave.");
      arfold (&ar);
      return 1;
    }

  st_foreach (&student_gpa, skey, {
    gpa = *stget_float (&student_gpa, skey);
    printf ("%s got %.1f GPA\n", skey, gpa);
  });

  stfold (&student_gpa);

  arfold (&ar);
}
