#define CLOMY_IMPLEMENTATION
#include "../build/clomy.h"

typedef struct
{
  char name[16];
  U32 age;
} Person;

inline Person *daget_person (da *people, U32 i);

#define daappend_person(people, ...)                                          \
  daappend ((people), &(Person){ __VA_ARGS__ })

#define dapush_person(people, ...) dapush ((people), &(Person){ __VA_ARGS__ })

#define dainsert_person(people, i, ...)                                       \
  dainsert ((people), (i), &(Person){ __VA_ARGS__ })

int
main ()
{
  arena ar = { 0 };
  da stk = { 0 }, people = { 0 };
  Person *p;

  /* --------- Stack --------- */
  printf ("Initialising dynamic array (stack)...\n");
  dainit (&stk, &ar, sizeof (int), 16);

  dapush_int (&stk, 2);
  dapush_int (&stk, 4);
  dapush_int (&stk, 6);

  FAILFALSE (stk.size == 3, "invalid stack size.");

  FAILFALSE (dapop_int (&stk) == 6, "invalid pop value.");
  FAILFALSE (stk.size == 2, "invalid stack size.");

  FAILFALSE (dafirst_int (&stk) == 4, "invalid stack value.");
  FAILFALSE (daget_int (&stk, 0) == 4, "invalid stack value.");

  FAILFALSE (dalast_int (&stk) == 2, "invalid stack value.");
  FAILFALSE (daget_int (&stk, 1) == 2, "invalid stack value.");

  /* --------- Custom dynamic array --------- */
  printf ("Initialising dynamic array (custom)...\n");
  dainit (&people, &ar, sizeof (Person), 16);

  printf ("Appending 2 people...\n");

  daappend_person (&people, .name = "John doe", .age = 23);
  daappend_person (&people, .name = "Jane doe", .age = 26);
  FAILFALSE (people.size == 2, "failed to append 3 items.");

  p = daget_person (&people, 0);
  FAILFALSE (p, "person 1 is NULL.");
  FAILFALSE (strcmp ("John doe", p->name) == 0, "person 1 name incorrect.");

  p = daget_person (&people, 1);
  FAILFALSE (p, "person 2 is NULL.");
  FAILFALSE (strcmp ("Jane doe", p->name) == 0, "person 1 name incorrect.");

  printf ("Push a person...\n");
  dapush_person (&people, .name = "Sue Cortez", .age = 32);
  FAILFALSE (people.size == 3, "failed to push.");

  p = daget_person (&people, 0);
  FAILFALSE (p, "person 3 is NULL.");
  FAILFALSE (strcmp ("Sue Cortez", p->name) == 0, "person 1 name incorrect.");

  p = daget_person (&people, 1);
  FAILFALSE (p, "person 1 is NULL.");
  FAILFALSE (strcmp ("John doe", p->name) == 0, "person 2 name incorrect.");

  printf ("Insert a person I=2...\n");
  dainsert_person (&people, 2, .name = "Dwain Thomas", .age = 19);
  FAILFALSE (people.size == 4, "failed to insert.");

  p = daget_person (&people, 1);
  FAILFALSE (p, "person 1 is NULL.");
  FAILFALSE (strcmp ("John doe", p->name) == 0, "person 1 name incorrect.");

  p = daget_person (&people, 2);
  FAILFALSE (p, "person 4 is NULL.");
  FAILFALSE (strcmp ("Dwain Thomas", p->name) == 0,
             "person 4 name incorrect.");

  p = daget_person (&people, 3);
  FAILFALSE (p, "person 2 is NULL.");
  FAILFALSE (strcmp ("Jane doe", p->name) == 0, "person 2 name incorrect.");

  dadel (&people, 1);
  FAILFALSE (people.size == 3, "failed to insert.");

  p = daget_person (&people, 0);
  FAILFALSE (p, "person 3 is NULL.");
  FAILFALSE (strcmp ("Sue Cortez", p->name) == 0, "person 3 name incorrect.");

  p = daget_person (&people, 1);
  FAILFALSE (p, "person 4 is NULL.");
  FAILFALSE (strcmp ("Dwain Thomas", p->name) == 0,
             "person 4 name incorrect.");

  p = daget_person (&people, 2);
  FAILFALSE (p, "person 2 is NULL.");
  FAILFALSE (strcmp ("Jane doe", p->name) == 0, "person 2 name incorrect.");

  arfold (&ar);
  return 0;
}

Person *
daget_person (da *people, U32 i)
{
  return (Person *)daget (people, i);
}
