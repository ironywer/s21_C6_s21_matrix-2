#include "s21_matrix.h"

#include <math.h>
// typedef struct matrix_struct {
//     double** matrix;
//     int rows;
//     int columns;
// } matrix_t;
int s21_correct_matrix(matrix_t *A) {
  int flag = OK;
  if (A == NULL || A->matrix == NULL || A->rows <= 0 || A->columns <= 0)
    flag = INCORRECT_MATRIX;
  for (int i = 0; flag == OK && i < A->rows; i++) {
    if (A->matrix[i] == NULL) flag = INCORRECT_MATRIX;
  }
  return flag;
}

void s21_print_matrix(matrix_t *A) {
  printf("\n");
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      printf(" %lf", A->matrix[i][j]);
    }
    printf("\n");
  }
}

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int flag = OK;
  if (rows <= 0 || columns <= 0 || result == NULL)
    flag = INCORRECT_MATRIX;
  else {
    result->matrix = (double **)malloc(rows * sizeof(double *));
    if (result->matrix == NULL) flag = INCORRECT_CALC;
    for (int i = 0; flag == OK && i < rows; i++) {
      result->matrix[i] = (double *)malloc(columns * sizeof(double));
      if (result->matrix[i] == NULL) flag = INCORRECT_CALC;
      for (int j = 0; flag == OK && j < columns; j++) result->matrix[i][j] = 0;
    }
    result->rows = rows;
    result->columns = columns;
  }
  return flag;
}

void s21_remove_matrix(matrix_t *A) {
  for (int i = 0; A != NULL && A->matrix != NULL && i < A->rows; i++) {
    if (A->matrix[i] != NULL) {
      free(A->matrix[i]);
    }
  }
  if (A != NULL && A->matrix != NULL) {
    free(A->matrix);
  }
  A->matrix = NULL;
  A->rows = 0;
  A->columns = 0;
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  if (A->rows != B->rows || A->columns != B->columns || A->matrix == NULL ||
      B->matrix == NULL || A->rows == 0 || A->columns == 0 || B->rows == 0 ||
      B->columns == 0) {
    return FAILURE;
  }
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      if (fabs(A->matrix[i][j] - B->matrix[i][j]) > EPS) {
        return FAILURE;
      }
    }
  }
  return SUCCESS;
}

int s21_sum_minus_matrix(matrix_t *A, matrix_t *B, matrix_t *result,
                         char sign) {
  int flag = OK;
  int sgn = (sign == '-') ? -1 : 1;
  flag = s21_correct_matrix(A);
  flag = s21_correct_matrix(B);
  if (result == NULL || B->rows != A->rows || B->columns != A->columns)
    flag = 2;
  else {
    // printf("--1111111111--");
    flag = s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows && flag == OK; i++) {
      for (int j = 0; j < A->columns && flag == OK; j++) {
        result->matrix[i][j] = A->matrix[i][j] + sgn * B->matrix[i][j];
      }
    }
  }
  // printf("--%d %d %d %d %d--", A->rows, A->columns, B->rows, B->columns,
  // flag);
  return flag;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int flag = OK;
  flag = s21_sum_minus_matrix(A, B, result, '+');
  return flag;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int flag = OK;
  flag = s21_sum_minus_matrix(A, B, result, '-');
  return flag;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int flag = OK;
  flag = s21_create_matrix(A->rows, A->columns, result);
  if (s21_correct_matrix(A) != OK || result == NULL) {
    flag = INCORRECT_MATRIX;
  }
  for (int i = 0; flag == OK && i < A->rows; i++) {
    for (int j = 0; flag == OK && j < A->columns; j++) {
      result->matrix[i][j] = number * A->matrix[i][j];
    }
  }
  return flag;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int flag = OK;
  flag = s21_create_matrix(A->rows, B->columns, result);
  if (s21_correct_matrix(A) != OK || s21_correct_matrix(B) != OK) {
    flag = INCORRECT_MATRIX;
  } else if (A->columns != B->rows || result == NULL)
    flag = 2;
  else {
    // for (int i = 0; flag == OK && i < A->rows; i++) {
    //   for (int k = 0; flag == OK && k < B->columns; k++) {
    //     int summ = 0;
    //     for (int j = 0; flag == OK && j < B->columns; j++) {
    //       summ += A->matrix[i][j] * B->matrix[j][k];
    //     }
    //     result->matrix[i][k] = summ;
    //   }
    // }
    for (int i = 0; flag == OK && i < A->rows; i++) {
      for (int j = 0; flag == OK && j < B->columns; j++) {
        result->matrix[i][j] = 0;
        for (int k = 0; flag == OK && k < A->columns; k++)
          result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
        // result->matrix[i][j] += 1;
      }
    }
  }
  return flag;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  int flag = OK;
  if (s21_correct_matrix(A) != OK || result == NULL)
    flag = INCORRECT_MATRIX;
  else {
    flag = s21_create_matrix(A->columns, A->rows, result);
    for (int i = 0; flag == OK && i < A->rows; i++)
      for (int j = 0; flag == OK && j < A->columns; j++) {
        result->matrix[j][i] = A->matrix[i][j];
      }
  }
  return flag;
}

double s21_minor(matrix_t *A, int row, int column) {
  double result = 0;
  if (A->rows == 1 && A->columns == 1)
    result = 1;
  else if (A->rows == 2 && A->columns == 2) {
    result =
        A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
  } else {
    matrix_t temp = {0};
    s21_create_matrix(A->rows - 1, A->columns - 1, &temp);
    int k = 0;
    for (int i = 0; i < A->rows; i++) {
      int n = 0;
      for (int j = 0; j < A->columns; j++) {
        if (i != row && j != column) {
          temp.matrix[k][n] = A->matrix[i][j];
          n += 1;
        }
      }
      if (i != row) k += 1;
    }
    // s21_print_matrix(&temp);
    // printf("%d %d\n", temp.rows, temp.columns);
    if (temp.columns == 2 && temp.rows == 2)
      result = s21_minor(&temp, 0, 0);
    else {
      for (int i = 0; i < temp.columns; i++) {
        result += temp.matrix[0][i] * s21_minor(&temp, 0, i) *
                  ((i % 2 == 0) ? 1 : -1);
      }
    }
    s21_remove_matrix(&temp);
  }
  return result;
}  // Доделать минор

int s21_determinant(matrix_t *A, double *result) {
  int flag = OK;
  if (s21_correct_matrix(A) != OK)
    flag = INCORRECT_MATRIX;
  else if (A->rows != A->columns || result == NULL)
    flag = 2;
  else if (A->rows == 2 && A->columns == 2)
    *result = s21_minor(A, 0, 0);
  else {
    *result = 0;
    for (int i = 0; i < A->columns; i++) {
      double minor = s21_minor(A, 0, i);
      *result += A->matrix[0][i] * minor * ((i % 2 == 0) ? 1 : -1);
    }
  }
  return flag;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int flag = OK;
  if (s21_correct_matrix(A) != OK || result == NULL)
    flag = INCORRECT_MATRIX;
  else if (A->rows != A->columns || A->rows < 2 || A->columns < 2)
    flag = 2;
  else if (A->rows == 2 && A->columns == 2) {
    flag = s21_create_matrix(A->rows, A->columns, result);
    result->matrix[0][0] = A->matrix[1][1];
    result->matrix[0][1] = -A->matrix[1][0];
    result->matrix[1][0] = -A->matrix[0][1];
    result->matrix[1][1] = A->matrix[0][0];
  } else {
    flag = s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; flag == OK && i < A->rows; i++) {
      for (int j = 0; flag == OK && j < A->columns; j++) {
        result->matrix[i][j] =
            (((i + j) % 2 == 0) ? 1 : -1) * s21_minor(A, i, j);
      }
    }
  }
  return flag;
}

// 311.000000 63.000000 -422.000000 55.000000
// 217.000000 61.000000 -274.000000 -115.000000
// -36.000000 112.000000 192.000000 -20.000000
// 81.000000 13.000000 98.000000 45.000000

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int flag = OK;
  if (s21_correct_matrix(A) != OK || result == NULL)
    flag = INCORRECT_MATRIX;
  else if (A->rows != A->columns)
    flag = 2;
  else {
    matrix_t temp = {0};
    flag = (flag == OK) ? s21_calc_complements(A, &temp) : flag;
    matrix_t temp2 = {0};
    flag = (flag == OK) ? s21_transpose(&temp, &temp2) : flag;

    double detA = 0;
    flag = (flag == OK) ? s21_determinant(A, &detA) : flag;
    if (detA == 0)
      flag = 2;
    else
      flag = (flag == OK) ? s21_mult_number(&temp2, 1. / detA, result) : flag;
    s21_remove_matrix(&temp);
    s21_remove_matrix(&temp2);
  }
  return flag;
}