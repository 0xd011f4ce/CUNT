#include <test.h>

struct vec2
{
  int x, y;
};

_Bool
are_two_vec2_equals (const void *vector1, const void *vector2)
{
  if (!vector1 || !vector2)
    return 0;

  struct vec2 *v1 = (struct vec2 *)vector1;
  struct vec2 *v2 = (struct vec2 *)vector2;

  return v1->x == v2->y && v1->y == v2->y;
}

TEST (test_booleans)
{
  TEST_ASSERT ("Is 2 + 2 = 5?", (2 + 2) == 5, test_booleans);
  TEST_ASSERT ("Is 1 + 1 = 2?", (1 + 1) == 2, test_booleans);
  TEST_ASSERT ("FALSE", 0, test_booleans);
  TEST_ASSERT ("TRUE", 1, test_booleans);
}

TEST (test_strings)
{
  TEST_ASSERT ("'str1' == 'str2'", ASSERT_STR_EQ ("str1", "str2"),
               test_strings);
  TEST_ASSERT ("'string' == 'string'", ASSERT_STR_EQ ("string", "string"),
               test_strings);
  TEST_ASSERT ("'different' == 'length'",
               ASSERT_STR_EQ ("different", "length"), test_strings);
}

TEST (test_arrays_custom_structures)
{
  TEST_DESCRIPTION ("In this test I use a custom `struct vec2' which is a "
                    "vector2 with `x' and `y' positions");

  struct vec2 pos1[] = { { 0, 0 }, { 1, 1 } };
  struct vec2 pos2[] = { { 1, 1 }, { 1, 1 } };
  struct vec2 pos3[] = { { 0, 0 } };

  size_t count = sizeof (pos1) / sizeof (struct vec2);

  TEST_ASSERT ("check if vector2 (ZERO, ONE) == vector2 (ZERO, ONE)",
               ASSERT_ARR_EQ (pos1, pos1, count, sizeof (struct vec2),
                              are_two_vec2_equals),
               test_arrays_custom_structures);
  TEST_ASSERT ("check if vector2 (ZERO, ONE) == vector2 (ONE, ONE)",
               ASSERT_ARR_EQ (pos1, pos2, count, sizeof (struct vec2),
                              are_two_vec2_equals),
               test_arrays_custom_structures);
  TEST_ASSERT ("check if vector2 (ZERO, ONE) == vector2 (ZERO)",
               ASSERT_ARR_EQ (pos1, pos3, count, sizeof (struct vec2),
                              are_two_vec2_equals),
               test_arrays_custom_structures);
}

TEST (test_arrays)
{
  TEST_DESCRIPTION ("This section is just to test the default CUNT functions "
                    "to compare different datatypes");

  int iarr1[] = { 1, 2, 3, 4 };
  int iarr2[] = { 4, 3, 2, 1 };
  int iarr3[] = { 1, 2 };

  float farr1[] = { 1.0f, 2.0f };
  float farr2[] = { 2.0f, 1.0f };

  TEST_ASSERT (
      "check if [1, 2, 3, 4] == [1, 2, 3, 4]",
      ASSERT_ARR_EQ (iarr1, iarr1, 4, sizeof (int), ASSERT_INT_ARRCMP),
      test_arrays);
  TEST_ASSERT (
      "check if [1, 2, 3, 4] == [4, 3, 2, 1]",
      ASSERT_ARR_EQ (iarr1, iarr2, 4, sizeof (int), ASSERT_INT_ARRCMP),
      test_arrays);

  TEST_ASSERT (
      "check if [1, 2, 3, 4] == [1, 2]",
      ASSERT_ARR_EQ (iarr1, iarr3, 4, sizeof (int), ASSERT_INT_ARRCMP),
      test_arrays);

  TEST_ASSERT (
      "check if [1.0f, 2.0f] == [1.0f, 2.0f]",
      ASSERT_ARR_EQ (farr1, farr1, 2, sizeof (float), ASSERT_FLOAT_ARRCMP),
      test_arrays);
  TEST_ASSERT (
      "check if [1.0f, 2.0f] == [2.0f, 1.0f]",
      ASSERT_ARR_EQ (farr1, farr2, 2, sizeof (float), ASSERT_FLOAT_ARRCMP),
      test_arrays);
}

TEST (test_pass_or_fail)
{
  TEST_START_ASSERT ("1 == 2?", test_pass_or_fail);
  if (1 == 2)
    {
      TEST_PASS_ASSERT (test_pass_or_fail);
    }
  else
    {
      TEST_FAIL_ASSERT (test_pass_or_fail);
    }

  TEST_START_ASSERT ("1 == 1?", test_pass_or_fail);
  if (1 == 1)
    {
      TEST_PASS_ASSERT (test_pass_or_fail);
    }
  else
    {
      TEST_FAIL_ASSERT (test_pass_or_fail);
    }
}

int
main (int argc, char **argv)
{
  TEST_START ();
  TEST_END ();

  return 0;
}
