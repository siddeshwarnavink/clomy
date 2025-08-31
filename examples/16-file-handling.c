/* Example to read and write to file in a cross-platfom way. */

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
  string *data, *read_data;
  const char *file_path = "file.txt";

  data = stringnew (&ar, "This is the file content!");
  if (!data)
    {
      fprintf (stderr, "Failed to allocate string.");
      arfold (&ar);
      return 1;
    }

  printf ("Content: %s\n", data->data);

  /* Write to file. */
  if (file_put_content (data, file_path) == 1)
    {
      fprintf (stderr, "Failed to write to file.");
      arfold (&ar);
      return 1;
    }

  printf ("Wrote to %s\n", file_path);

  /* Read from file. */
  read_data = file_get_content (&ar, file_path);
  if (!read_data)
    {
      fprintf (stderr, "Failed to read file.");
      arfold (&ar);
      return 1;
    }
  printf ("Read from file: %s\n", read_data->data);

  /* Delete file. */
  if (file_delete (file_path) == 0)
    {
      printf ("Deleted file: %s\n", file_path);
    }
  else
    {
      fprintf (stderr, "Failed to delete file.");
      arfold (&ar);
      return 1;
    }

  arfold (&ar);
  return 0;
}
