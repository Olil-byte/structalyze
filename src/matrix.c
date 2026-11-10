#include "matrix.h"

#include <stdlib.h>

void matrix_init(matrix_t *matrix, int n)
{
	matrix->n = n;
	matrix->data = malloc(n * n * sizeof(*matrix->data));
}

int matrix_sum(const matrix_t *a, const matrix_t *b, matrix_t *c)
{
	int i, j;
	int n;

	if (!a || !b || !c) {
		return -1;
	}
	if (a->n != b->n || b->n != c->n) {
		return -1;
	}
	n = a->n;
	if (!a->data || !b->data || !c->data) {
		return -1;
	}

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			c->data[i * n + j] =
				a->data[i * n + j] + b->data[i * n + j];
		}
	}

	return 0;
}

int matrix_mul(const matrix_t *a, const matrix_t *b, matrix_t *c)
{
	int i, j, k;
	int n;

	int sum;

	if (a == c || b == c) {
		return -1;
	}
	if (a->n != b->n || b->n != c->n) {
		return -1;
	}
	n = a->n;
	if (!a->data || !b->data || !c->data) {
		return -1;
	}

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			sum = 0;
			for (k = 0; k < n; k++) {
				sum += a->data[i * n + k] * b->data[k * n + j];
			}
			c->data[i * n + j] = sum;
		}
	}

	return 0;
}

int matrix_pow(const matrix_t *a, int pow, matrix_t *b)
{
	int i, j;
	int n;

	matrix_t acc, tmp;

	int *t;

	if (a->n != b->n) {
		return -1;
	}
	n = a->n;

	acc.n = n;
	acc.data = calloc(acc.n * acc.n, sizeof(*acc.data));
	for (i = 0; i < n; i++) {
		acc.data[i * n + i] = 1;
	}

	tmp.n = n;
	tmp.data = malloc(tmp.n * tmp.n * sizeof(*tmp.data));

	for (i = 0; i < pow; i++) {
		matrix_mul(&acc, a, &tmp);
		t = acc.data;
		acc.data = tmp.data;
		tmp.data = t;
	}

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			b->data[i * n + j] = acc.data[i * n + j];
		}
	}

	free(acc.data);
	free(tmp.data);

	return 0;
}

int matrix_norm(matrix_t *matrix)
{
	int i, j;
	int n;

	if (!matrix || !matrix->data) {
		return -1;
	}
	n = matrix->n;

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			matrix->data[i * n + j] =
				matrix->data[i * n + j] > 0 ? 1 : 0;
		}
	}

	return 0;
}

int matrix_total_paths(const matrix_t *a, matrix_t *b)
{
	int i;
	int n;

	matrix_t tmp;

	if (!a || !b) {
		return -1;
	}
	if (a->n != b->n) {
		return -1;
	}
	n = a->n;
	if (!a->data || !b->data) {
		return -1;
	}
	memset(b->data, 0, n * n * sizeof(*b->data));

	tmp.n = n;
	tmp.data = malloc(tmp.n * tmp.n * sizeof(*tmp.data));

	for (i = 0; i <= n; i++) {
		matrix_pow(a, i, &tmp);

		matrix_sum(b, &tmp, b);
	}

	free(tmp.data);

	return 0;
}

