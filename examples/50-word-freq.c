/* Program to find the maximum occurred word in a document. And report how many
   times it occurred. */

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
  ht wc = { 0 };
  string *poem, *word;
  char *max_word, *key;
  int max_count = 0, cur_count;

  if (htinit (&wc, &ar, 16, sizeof (U32)) == 1)
    {
      printf ("Failed to initialize table.");
      arfold (&ar);
      return 1;
    }

  /* Load entire file into a string buffer. */
  poem = file_get_content (&ar, "poem.txt");
  if (!poem)
    {
      printf ("Failed to open poem.txt");
      arfold (&ar);
      return 1;
    }

  while ((word = string_split (poem))) /* Split string by SPACE */
    {
      string_trim (word);
      string_lower (word);

      /* stinc_int is a second level function that is built using stget_int
         and stput_int */
      stinc_int (&wc, word->data); /* If key doesn't exit puts value 1.
                                      If key already exists increment it. */
    }

  st_foreach (&wc, key, {
    cur_count = *stget_int (&wc, key);
    if (cur_count > max_count)
      {
        max_word = key;
        max_count = cur_count;
      }
  });

  printf ("\"%s\" - %d times\n", max_word, max_count);

  arfold (&ar);

  return 0;
}
