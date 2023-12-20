#include "test.h"

#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_RESET "\x1b[0m"

static struct test *tests = NULL;
static unsigned int total_tests = 0;

static struct test_assertion *current_assert = NULL;

/*****************************************************************************/
/*                          Internal functions only                          */
/*****************************************************************************/
static void
test_init ()
{
  if (tests)
    return;

  tests = malloc (sizeof (struct test));
  if (!tests)
    {
      fprintf (
          stderr,
          "test_init: there's been an error allocating memory for `tests'.\n");
      return;
    }
}

static void
test_reallocate_if_necessary ()
{
  if (sizeof (tests) > sizeof (struct test) * total_tests + 1)
    return;

  tests = realloc (tests, sizeof (struct test) * total_tests + 1);
  if (!tests)
    {
      fprintf (stderr, "test_reallocate_if_necessary: there's been an error "
                       "reallocating the memory.\n");
      return;
    }
}

static void
test_copy_name (struct test *const t, const char *name)
{
  if (!t || !name)
    return;

  size_t len = strlen (name);
  t->name = malloc (sizeof (char) * (len + 1));
  strncpy (t->name, name, len);
  t->name[len] = '\0';
}

static void
assert_copy_name (struct test_assertion *const ass, const char *name)
{
  if (!ass || !name)
    return;

  size_t len = strlen (name);
  ass->name = malloc (sizeof (char) * (len + 1));
  strncpy (ass->name, name, len);
  ass->name[len] = '\0';
}

static struct test *
test_get_test_by_func (void (*func) ())
{
  if (!func)
    return NULL;

  struct test *t = NULL;
  for (unsigned int i = 0; i < total_tests; i++)
    {
      struct test current = tests[i];
      if (current.func == func)
        {
          t = &tests[i];
          break;
        }
    }

  return t;
}

static void
test_realloc_asserts (struct test *t)
{
  if (!t || !t->asserts)
    return;

  t->total_asserts++;
  if (sizeof (t->total_asserts)
      < sizeof (struct test_assertion) * t->total_asserts + 1)
    {
      t->asserts = realloc (
          t->asserts, sizeof (struct test_assertion) * t->total_asserts + 1);

      if (!t->asserts)
        return;
    }
}

/*****************************************************************************/
/*                    These are mostly aesthetic things :/                   */
/*****************************************************************************/
void
print_cute_header (const char *name)
{
  size_t len = strlen (name);
  int width = 80;

  int padding = (width - len) / 2;

  printf ("%s*", COLOR_MAGENTA);
  for (int i = 0; i < width - 2; i++)
    printf ("*");
  printf ("*\n");

  printf ("*%*s%s%*s*\n", padding, "", name,
          (int)(padding + (width - len) % 2) - 2, "");

  printf ("*");
  for (int i = 0; i < width - 2; i++)
    printf ("*");
  printf ("*%s\n", COLOR_RESET);
}

void
test_add_newline_at_80_cols (const char *str)
{
  if (!str || strlen (str) <= 80)
    return;

  char *new_str = strdup (str);

  for (size_t i = 80; i > 0; i--)
    {
      if (str[i] == ' ')
        {
          new_str[i] = '\n';
          break;
        }
    }

  printf ("%s\n", new_str);
  free (new_str);
}

/*****************************************************************************/
/*                            Accessible functions                           */
/*****************************************************************************/
void
test_register (const char *name, void (*func) ())
{
  if (!tests)
    test_init ();

  total_tests++;
  test_reallocate_if_necessary ();

  tests[total_tests - 1].func = func;
  test_copy_name (&tests[total_tests - 1], name);
  tests[total_tests - 1].asserts = malloc (sizeof (struct test_assertion));
}

void
test_add_assert (const char *name, _Bool expr, void (*test) ())
{
  struct test *t = test_get_test_by_func (test);
  if (!t)
    return;

  test_realloc_asserts (t);

  struct test_assertion *ass = &t->asserts[t->total_asserts - 1];
  assert_copy_name (ass, name);
  ass->passed = expr;
}

void
test_start_assert (const char *name, void (*test) ())
{
  struct test *t = test_get_test_by_func (test);
  if (!t)
    return;

  if (current_assert)
    {
      fprintf (stderr,
               "test_start_assert: there's already an assert running\n");
      return;
    }

  test_realloc_asserts (t);
  current_assert = &t->asserts[t->total_asserts - 1];
  assert_copy_name (current_assert, name);
}

void
test_pass_assert (void (*test) ())
{
  if (!current_assert)
    {
      fprintf (stderr, "test_pass_assert: there's no assert running\n");
      return;
    }

  current_assert->passed = 1;
  current_assert = NULL;
}

void
test_fail_assert (void (*test) ())
{
  if (!current_assert)
    {
      fprintf (stderr, "test_fail_assert: there's no assert running\n");
      return;
    }

  current_assert->passed = 0;
  current_assert = NULL;
}

void
test_run_tests ()
{
  if (!tests)
    return;

  for (unsigned int i = 0; i < total_tests; i++)
    {
      if (i)
        printf ("\n\n");

      print_cute_header (tests[i].name);
      tests[i].func ();

      for (unsigned int j = 0; j < tests[i].total_asserts; j++)
        {
          struct test_assertion ass = tests[i].asserts[j];
          const char *result_txt = ass.passed ? "PASSED" : "FAILED";
          const char *result_col = ass.passed ? COLOR_GREEN : COLOR_RED;

          printf ("%s%s: %s%s\n", result_col, ass.name, result_txt,
                  COLOR_RESET);
        }
    }
}

void
test_end ()
{
  for (unsigned int i = 0; i < total_tests; i++)
    {
      for (unsigned int j = 0; j < tests[i].total_asserts; j++)
        {
          free (tests[i].asserts[j].name);
          tests[i].asserts[j].name = NULL;
        }

      free (tests[i].name);
      tests[i].name = NULL;

      free (tests[i].asserts);
      tests[i].asserts = NULL;
    }

  free (tests);
  tests = NULL;
}

/*****************************************************************************/
/*                              Assertion utils                              */
/*****************************************************************************/
_Bool
test_are_two_strings_equal (const char *str1, const char *str2)
{
  if (!str1 || !str2)
    return 0;

  size_t len1 = strlen (str1);
  size_t len2 = strlen (str2);

  if (len1 != len2)
    return 0;

  return strncmp (str1, str2, len1) == 0;
}

_Bool
test_are_two_arrays_equal (const void *arr1, const void *arr2, size_t size,
                           size_t child_size,
                           _Bool (*fn) (const void *, const void *))
{
  const char *p1 = arr1;
  const char *p2 = arr2;

  for (size_t i = 0; i < size; i++)
    {
      if (!fn (p1, p2))
        return 0;

      p1 += child_size;
      p2 += child_size;
    }

  return 1;
}

/*****************************************************************************/
/*                        Generic comparing functions                        */
/*****************************************************************************/
_Bool
test_generic_arrcmp_int (const void *int1, const void *int2)
{
  if (!int1 || !int2)
    return 0;

  int *i1 = (int *)int1;
  int *i2 = (int *)int2;

  return *i1 == *i2;
}

_Bool
test_generic_arrcmp_float (const void *float1, const void *float2)
{
  if (!float1 || !float2)
    return 0;

  float *f1 = (float *)float1;
  float *f2 = (float *)float2;

  return *f1 == *f2;
}
