#ifndef __TEST_H
#define __TEST_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST(func)                                                            \
  void func ();                                                               \
  __attribute__ ((constructor)) void register_##func ()                       \
  {                                                                           \
    test_register (#func, func);                                              \
  }                                                                           \
  void func ()

/* test related */
#define TEST_START test_run_tests
#define TEST_END test_end

/* assert related */
#define TEST_ASSERT(name, expr, test) test_add_assert (name, expr, test);
#define TEST_START_ASSERT(name, test) test_start_assert (name, test);
#define TEST_PASS_ASSERT(test) test_pass_assert (test);
#define TEST_FAIL_ASSERT(test) test_fail_assert (test);

/* some other useful things */
#define TEST_DESCRIPTION(desc) test_add_newline_at_80_cols (desc);

/* `expr' assert functions */
#define ASSERT_STR_EQ(str1, str2) test_are_two_strings_equal (str1, str2)
#define ASSERT_ARR_EQ(arr1, arr2, size, csize, fn)                            \
  test_are_two_arrays_equal (arr1, arr2, size, csize, fn)

/* generic array functions */
#define ASSERT_INT_ARRCMP test_generic_arrcmp_int
#define ASSERT_FLOAT_ARRCMP test_generic_arrcmp_float

struct test
{
  char *name;
  unsigned int total_asserts;
  struct test_assertion *asserts;

  void (*func) ();
};

struct test_assertion
{
  char *name;
  _Bool passed;
};

void test_register (const char *name, void (*func) ());
void test_add_assert (const char *name, _Bool expr, void (*test) ());
void test_start_assert (const char *name, void (*test) ());
void test_pass_assert (void (*test) ());
void test_fail_assert (void (*test) ());

_Bool test_are_two_strings_equal (const char *str1, const char *str2);
_Bool test_are_two_arrays_equal (const void *arr1, const void *arr2,
                                 size_t size, size_t child_size,
                                 _Bool (*fn) (const void *, const void *));

_Bool test_generic_arrcmp_int (const void *int1, const void *int2);
_Bool test_generic_arrcmp_float (const void *float1, const void *float2);

void test_add_newline_at_80_cols (const char *str);
void test_run_tests ();
void test_end ();

#endif
