#include "s21_test_matrix.h"

START_TEST(test_one_by_one) {
  matrix_t m = {0};
  matrix_t result = {0};
  s21_create_matrix(1, 1, &m);
  int code = s21_calc_complements(&m, &result);
  ck_assert_int_eq(code, INCORRECT_CALC);
  s21_remove_matrix(&m);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_incorrect) {
  matrix_t m = {0};
  matrix_t result = {0};
  int code = s21_calc_complements(&m, &result);
  ck_assert_int_eq(code, INCORRECT_MATRIX);
  s21_remove_matrix(&m);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_not_sqare) {
  matrix_t m = {0};
  matrix_t result = {0};
  int codec = s21_create_matrix(3, 4, &m);
  if (codec == OK) {
    int code = s21_calc_complements(&m, &result);
    ck_assert_int_eq(code, INCORRECT_CALC);
  }
  s21_remove_matrix(&m);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_normal) {
  matrix_t expected = {0};
  matrix_t m = {0};
  matrix_t result = {0};
  s21_create_matrix(3, 3, &m);
  s21_create_matrix(3, 3, &expected);
  m.matrix[0][0] = 1;
  m.matrix[0][1] = 2;
  m.matrix[0][2] = 3;

  m.matrix[1][0] = 0;
  m.matrix[1][1] = 4;
  m.matrix[1][2] = 2;

  m.matrix[2][0] = 5;
  m.matrix[2][1] = 2;
  m.matrix[2][2] = 1;

  expected.matrix[0][0] = 0;
  expected.matrix[0][1] = 10;
  expected.matrix[0][2] = -20;

  expected.matrix[1][0] = 4;
  expected.matrix[1][1] = -14;
  expected.matrix[1][2] = 8;

  expected.matrix[2][0] = -8;
  expected.matrix[2][1] = -2;
  expected.matrix[2][2] = 4;

  int code = s21_calc_complements(&m, &result);

  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);
  ck_assert_int_eq(code, OK);
  s21_remove_matrix(&m);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

// START_TEST(test_error) {
//   matrix_t m = {0};
//   s21_create_matrix(3, 3, &m);
//   m.matrix[0][0] = 1;
//   m.matrix[0][1] = 2;
//   m.matrix[0][2] = 3;

//   m.matrix[1][0] = 2;
//   m.matrix[1][1] = 4;
//   m.matrix[1][2] = 6;

//   m.matrix[2][0] = 3;
//   m.matrix[2][1] = 6;
//   m.matrix[2][2] = 9;

//   matrix_t result = {0};
//   int code = s21_calc_complements(&m, &result);

//   ck_assert_int_eq(code, INCORRECT_CALC);
//   s21_remove_matrix(&result);
//   s21_remove_matrix(&m);
// }
// END_TEST

Suite *suite_calc_complements(void) {
  Suite *s = suite_create("suite_calc_complements");
  TCase *tc = tcase_create("case_calc_complements");

  tcase_add_test(tc, test_normal);
  tcase_add_test(tc, test_not_sqare);
  tcase_add_test(tc, test_incorrect);
  tcase_add_test(tc, test_one_by_one);
  // tcase_add_test(tc, test_error);

  suite_add_tcase(s, tc);
  return s;
}
