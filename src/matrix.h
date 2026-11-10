#ifndef MATRIX_H
#define MATRIX_H

typedef struct matrix
{
	int *data;
	int n;
} matrix_t;

void matrix_init(matrix_t *matrix, int n);

int matrix_sum(const matrix_t *a, const matrix_t *b, matrix_t *c);
int matrix_mul(const matrix_t *a, const matrix_t *b, matrix_t *c);
int matrix_pow(const matrix_t *a, int pow, matrix_t *b);

int matrix_norm(matrix_t *matrix);

int matrix_total_paths(const matrix_t *a, matrix_t *b);

#endif

