#if defined(CLOMY_BACKEND_WINAPI)
#define WIN32_LEAN_AND_MEAN
#define CLOMY_BACKEND_WINAPI
#elif defined(CLOMY_BACKEND_POSIX)
#define CLOMY_BACKEND_POSIX
#endif

#define CLOMY_IMPLEMENTATION
#include "../build/clomy.h"

int
main ()
{
  arena ar = { 0 };
  da stack = { 0 };
  stringbuilder sb = { 0 };
  string *str1, *str2, *word;
  char ch;

  /* This is a string. */
  str1 = stringnew (&ar, "this is a string");
  if (!str1)
    {
      fprintf (stderr, "Failed to allocate str1");
      arfold (&ar);
      return 1;
    }

  /* You can parse character-by-character. Here is a example of a bracket
     validator. */
  str2 = stringnew (&ar, "{}{{}}");
  dainit (&stack, &ar, sizeof (char), 16);

  while ((ch = string_chop_head (str2)) != '\0')
    {
      if (ch == '{')
        dapush_char (&stack, ch);
      else
        dapop (&stack);
    }

  if (stack.size == 0)
    printf ("VALID\n");
  else
    printf ("INVALID\n");

  dafold (&stack);
  stringfold (str2);

  /* You can split it by delimiter (default SPACE). You can use other delimiter
     by using string_split_delim () instead. */
  while ((word = string_split (str1)))
    {
      printf ("%s-", word->data); /* Access raw NULL-terminated character
                                      array. */
    }
  printf ("\n");

  /* In order to build a string, we have stringbuilder. */
  sbinit (&sb, &ar);

  /* You can perform various operation like so. */
  sbappend (&sb, "ello ");
  sbappend (&sb, "World!");
  sbinsert (&sb, "to the ", 6);
  sbpushch (&sb, 'H');

  sbrev (&sb); /* Reverse the sring. */

  /* After you're done building, you can simplfy flush the stringbuilder. */
  str2 = sbflush (&sb);
  printf ("%s\n", str2->data);

  /* You can re-use the same stringbuilder after flush. */
  sbappend (&sb, "You can re-use");
  sbappend (&sb, " the same stringbuilder!");

  str2 = sbflush (&sb);
  printf ("%s\n", str2->data);

  arfold (&ar);
}
