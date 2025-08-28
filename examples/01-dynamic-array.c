/* By default CLOMY uses LIBC as backend but you can change to platform
   specific for better performance. */

/* Use native WINAPI on Windows. */
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define CLOMY_BACKEND_WINAPI
#endif

/* Adding this will magically include all the implementation. You don't need
   to link with anything. Make sure you only have this line in one central
   place.*/
#define CLOMY_IMPLEMENTATION

#include "../build/clomy.h"

typedef struct
{
  char name[16];
  U32 age;
} Person;

inline Person *daget_person (da *people, size_t i);

#define daappend_person(people, ...)                                          \
  daappend ((people), &(Person){ __VA_ARGS__ })

#define dapush_person(people, ...) dapush ((people), &(Person){ __VA_ARGS__ })

#define dainsert_person(people, i, ...)                                       \
  dainsert ((people), (i), &(Person){ __VA_ARGS__ })

int
main ()
{
  /* Everything in CLOMY is stored inside area. */
  arena ar = { 0 };
  da numbers = { 0 }, people = { 0 };
  Person *p;
  size_t i;
  int num;

  /* Initialize the dynamic array of INT with CAPACITY 16. When it exceeds this
     CAPACITY, the array will re-allocate with double it's size. */
  if (dainit (&numbers, &ar, sizeof (int), 16) == 1)
    {
      fprintf (stderr, "Failed to initialize numbers.");
      arfold (&ar);
      return 1;
    }

  /* This is the primitive function to add value to end of the array. */
  num = 69;
  if (daappend (&numbers, &num) == 1) /* It returns 1 when it fails. */
    {
      fprintf (stderr, "Buy more RAM!");
      arfold (&ar);
      return 1;
    }

  /* As passing pointer is inconvenient, we have wrapper functions for all
     built-in data types.

     daappend_<datatype> (*ar, val);

     I recommended you to create such wrapper functions for your custom data
     types. */
  daappend_int (&numbers, 420);

  /* In a similar fashion we have dapush, dainsert to add to beginning and
     in-between of the array. */

  /* Looping through all elements is straight pretty forward. */
  printf ("[");
  for (i = 0; i < numbers.size; ++i)
    {
      /* daget is a primitive function to get Ith value of a dynamic array. */
      num = *((int *)daget (&numbers, i));
      printf ("%d,", num);
    }
  printf ("]\n");

  /* We have wrapper function for easier use. */ 
  printf ("i=1 has %d\n", daget_int (&numbers, 1));

  /* Usually you don't need to de-allocate the dynamic array manually - as you
     can just de-allocate the arena after use. But you can manually do it to
     free up space inside arena like so. */
  dafold (&numbers);

  /* Here is an example with custom datatype. */
  dainit (&people, &ar, sizeof (Person), 16);
  daappend_person (&people, .name = "Siddeshwar", .age = 21);
  daappend_person (&people, .name = "Jane doe", .age = 34);
  dapush_person (&people, .name = "John doe", .age = 23);

  for (i = 0; i < people.size; ++i)
    {
      p = daget_person (&people, i);
      printf ("Name: %s  Age: %d\n", p->name, p->age);
    }

  /* Free the entire arena, including dynamic array. */
  arfold (&ar);

  return 0;
}

Person *
daget_person (da *people, size_t i)
{
  return (Person *)daget (people, i);
}
