#include <stdio.h>

#define CLOMY_IMPLEMENTATION
#include "../clomy.h"

struct Person
{
  char name[16];
  U32 age;
};
typedef struct Person Person;

int
main ()
{
  arena ar = { 0 };
  da people = { 0 };

  printf ("Initialising dynamic array...\n");
  dainit (&people, &ar, sizeof (Person), 16);

  printf ("Appending 2 people...\n");

  daappend (&people, &(Person){ "John doe", 23 });
  daappend (&people, &(Person){ "Jane doe", 26 });
  FAILFALSE (people.size == 2, "failed to append 3 items.");
  FAILFALSE (strcmp ("John doe", ((Person *)dageti (&people, 0))->name) == 0,
             "person 1 name incorrect.");
  FAILFALSE (strcmp ("Jane doe", ((Person *)dageti (&people, 1))->name) == 0,
             "person 2 name incorrect.");

  printf ("Push a person...\n");
  dapush (&people, &(Person){ "Sue Cortez", 32 });
  FAILFALSE (people.size == 3, "failed to push.");
  FAILFALSE (strcmp ("Sue Cortez", ((Person *)dageti (&people, 0))->name) == 0,
             "person 1 name incorrect.");
  FAILFALSE (strcmp ("John doe", ((Person *)dageti (&people, 1))->name) == 0,
             "person 2 name incorrect.");

  printf ("Insert a person I=2...\n");
  dainsert (&people, &(Person){ "Dwain Thomas", 19 }, 2);
  FAILFALSE (people.size == 4, "failed to insert.");
  FAILFALSE (strcmp ("John doe", ((Person *)dageti (&people, 1))->name) == 0,
             "person 2 name incorrect.");
  FAILFALSE (strcmp ("Dwain Thomas", ((Person *)dageti (&people, 2))->name)
                 == 0,
             "person 3 name incorrect.");
  FAILFALSE (strcmp ("Jane doe", ((Person *)dageti (&people, 3))->name) == 0,
             "person 4 name incorrect.");

  dadel (&people, 1);
  FAILFALSE (people.size == 3, "failed to insert.");
  FAILFALSE (strcmp ("Sue Cortez", ((Person *)dageti (&people, 0))->name) == 0,
             "person 1 name incorrect.");
  FAILFALSE (strcmp ("Dwain Thomas", ((Person *)dageti (&people, 1))->name)
                 == 0,
             "person 2 name incorrect.");
  FAILFALSE (strcmp ("Jane doe", ((Person *)dageti (&people, 2))->name) == 0,
             "person 3 name incorrect.");

  arfold (&ar);
  return 0;
}
