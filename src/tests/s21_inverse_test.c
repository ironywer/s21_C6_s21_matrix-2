#include "s21_test_matrix.h"

START_TEST(test_normal) {
  matrix_t m = {0};
  matrix_t expected = {0};
  s21_create_matrix(3, 3, &m);
  s21_create_matrix(3, 3, &expected);

  m.matrix[0][0] = 2;
  m.matrix[0][1] = 5;
  m.matrix[0][2] = 7;

  m.matrix[1][0] = 6;
  m.matrix[1][1] = 3;
  m.matrix[1][2] = 4;

  m.matrix[2][0] = 5;
  m.matrix[2][1] = -2;
  m.matrix[2][2] = -3;

  expected.matrix[0][0] = 1;
  expected.matrix[0][1] = -1;
  expected.matrix[0][2] = 1;

  expected.matrix[1][0] = -38;
  expected.matrix[1][1] = 41;
  expected.matrix[1][2] = -34;

  expected.matrix[2][0] = 27;
  expected.matrix[2][1] = -29;
  expected.matrix[2][2] = 24;
  matrix_t result = {0};
  int code = s21_inverse_matrix(&m, &result);

  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);
  ck_assert_int_eq(code, OK);
  s21_remove_matrix(&result);
  s21_remove_matrix(&m);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_error) {
  matrix_t m = {0};
  s21_create_matrix(3, 3, &m);
  m.matrix[0][0] = 1;
  m.matrix[0][1] = 2;
  m.matrix[0][2] = 3;

  m.matrix[1][0] = 2;
  m.matrix[1][1] = 4;
  m.matrix[1][2] = 6;

  m.matrix[2][0] = 3;
  m.matrix[2][1] = 6;
  m.matrix[2][2] = 9;

  matrix_t result = {0};
  int code = s21_inverse_matrix(&m, &result);

  ck_assert_int_eq(code, INCORRECT_CALC);
  s21_remove_matrix(&result);
  s21_remove_matrix(&m);
}
END_TEST

START_TEST(test_error2) {
  matrix_t m = {0};
  s21_create_matrix(3, 4, &m);

  matrix_t result = {0};
  int code = s21_inverse_matrix(&m, &result);

  ck_assert_int_eq(code, INCORRECT_CALC);
  s21_remove_matrix(&result);
  s21_remove_matrix(&m);
}
END_TEST

START_TEST(test_error3) {
  matrix_t m = {0};
  s21_create_matrix(-3, 4, &m);

  matrix_t result = {0};
  int code = s21_inverse_matrix(&m, &result);

  ck_assert_int_eq(code, INCORRECT_MATRIX);
  s21_remove_matrix(&result);
  s21_remove_matrix(&m);
}
END_TEST

Suite *suite_inverse_matrix(void) {
  Suite *s = suite_create("suite_inverse_matrix");
  TCase *tc = tcase_create("case_inverse_matrix");

  tcase_add_test(tc, test_normal);
  tcase_add_test(tc, test_error);
  tcase_add_test(tc, test_error2);
  tcase_add_test(tc, test_error3);
  suite_add_tcase(s, tc);
  return s;
}
