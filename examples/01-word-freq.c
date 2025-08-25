#define CLOMY_IMPLEMENTATION
#include "../clomy.h"

int
main ()
{
  arena ar = { 0 };
  ht wc = { 0 };
  string *poem, *word;
  char *max_word;
  int max_count = 0, cur_count;

  htinit (&wc, &ar, 16, sizeof (U32));

  poem = file_get_content (&ar, "poem.txt");
  if (!poem)
    return 1;

  while ((word = string_split (poem)))
    {
      string_trim (word);
      string_lower (word);
      stinc_int (&wc, word->data);
    }

  st_foreach (&wc, {
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
