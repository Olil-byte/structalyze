#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct matrix
{
	int *data;
	int n;
} matrix_t;

static int read_matrix(matrix_t *matrix)
{
	int result = 0;

	int *data = 0;
	int n;

	int code;
	char symbol;
	
	int size = 0;
	int capacity = 8;
	int *values = malloc(capacity * sizeof(*values));

	int value;

	int i, j;

	/* read first row and determine value of n */
	while ((code = getchar()) != EOF) {
		symbol = (char)code;

		if (symbol == '0' || symbol == '1') {
			value = symbol - '0';
			if (size == capacity) {
				capacity *= 2;
				values = realloc(
					values,
					capacity * sizeof(*values)
				);
			}

			values[size] = value;
			size += 1;
		} else if (symbol == ' ' || symbol == '\t' || symbol == '\r') {
			continue;
		} else if (symbol == '\n') {
			n = size;
			data = malloc(n * n * sizeof(*data));
			for (j = 0; j < n; j++) {
				data[j] = values[j];
			}
			break;
		} else {
			result = -1;
			goto READ_MATRIX_EPILOQUE;
		}
	}

	i = 1;
	j = 0;
	while ((code = getchar()) != EOF) {
		symbol = (char)code;

		if (symbol == '0' || symbol == '1') {
			value = symbol - '0';

			if (j == n) {
				result = -1;
				goto READ_MATRIX_EPILOQUE;
			} else {
				data[i * n + j] = value;
				j += 1;
			}
		} else if (symbol == ' ' || symbol == '\t' || symbol == '\r') {
			continue;
		} else if (symbol == '\n') {
			i += 1;
			if (i == n) {
				break;
			} else {
				j = 0;
			}
		} else {
			result = -1;
			goto READ_MATRIX_EPILOQUE;
		}
	}
	if (i < n) {
		result = -1;
		goto READ_MATRIX_EPILOQUE;
	}

READ_MATRIX_EPILOQUE:
	free(values);

	if (result != -1) {
		matrix->n = n;
		matrix->data = data;
	} else {
		free(matrix->data);
	}

	return result;
}

static void matrix_display(matrix_t *matrix)
{
	int i, j;
	int n = matrix->n;

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			printf("%d", matrix->data[i * n + j]);
			printf(j < n - 1 ? " " : "\n");
		}
	}
}

static int matrix_mul(const matrix_t *a, const matrix_t *b, matrix_t *c)
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

static int matrix_pow(const matrix_t *mat, int pow, matrix_t *adj)
{
	int i, j;
	int n;

	matrix_t acc, tmp;

	int *t;

	if (mat->n != adj->n) {
		return -1;
	}
	n = mat->n;

	acc.n = n;
	acc.data = calloc(acc.n * acc.n, sizeof(*acc.data));
	for (i = 0; i < n; i++) {
		acc.data[i * n + i] = 1;
	}

	tmp.n = n;
	tmp.data = malloc(tmp.n * tmp.n * sizeof(*tmp.data));

	for (i = 0; i < pow; i++) {
		matrix_mul(&acc, mat, &tmp);
		t = acc.data;
		acc.data = tmp.data;
		tmp.data = t;
	}

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			adj->data[i * n + j] = acc.data[i * n + j];
		}
	}

	free(acc.data);
	free(tmp.data);

	return 0;
}

static int matrix_normalize(matrix_t *mat)
{
	int i, j;
	int n;

	if (!mat || !mat->data) {
		return -1;
	}
	n = mat->n;

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			mat->data[i * n + j] =
				mat->data[i * n + j] > 0 ? 1 : 0;
		}
	}

	return 0;
}

static int matrix_sum(const matrix_t *a, const matrix_t *b, matrix_t *c)
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

static int matrix_count_total_routes(const matrix_t *mat, matrix_t *res)
{
	int i;
	int n;

	matrix_t tmp;

	if (!mat || !res) {
		return -1;
	}

	if (mat->n != res->n) {
		return -1;
	}
	n = mat->n;
	if (!mat->data || !res->data) {
		return -1;
	}
	memset(res->data, 0, n * n * sizeof(*res->data));

	tmp.n = n;
	tmp.data = malloc(tmp.n * tmp.n * sizeof(*tmp.data));

	for (i = 1; i <= n; i++) {
		matrix_pow(mat, i, &tmp);
		matrix_sum(res, &tmp, res);
	}

	free(tmp.data);

	return 0;
}

static int matrix_reachability(const matrix_t *matrix, matrix_t *result_matrix)
{
	if (!matrix || !matrix->data) {
		return -1;
	}
	if (!result_matrix || !result_matrix->data) {
		return -1;
	}
	if (matrix->n != result_matrix->n) {
		return -1;
	}

	matrix_count_total_routes(matrix, result_matrix);
	matrix_normalize(result_matrix);

	return 0;
}

static int matrix_reachability_coef(const matrix_t *matrix, double *result)
{
	matrix_t tmp_matrix;

	*result = 0.0;

	int i, j;
	int n;

	if (!matrix || !matrix->data) {
		return -1;
	}
	if (!result) {
		return -1;
	}
	n = matrix->n;

	tmp_matrix.data = malloc(n * n * sizeof(*tmp_matrix.data));
	tmp_matrix.n = n;

	matrix_reachability(matrix, &tmp_matrix);

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			*result += tmp_matrix.data[i * n + j];
		}
	}
	*result /= (n * n);

	free(tmp_matrix.data);

	return 0;
}

static int matrix_adj_coef(const matrix_t *matrix, double *result)
{
	int i, j;
	int n;

	if (!matrix || !matrix->data) {
		return -1;
	}
	if (!result) {
		return -1;
	}
	n = matrix->n;

	*result = 0;

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			*result += matrix->data[i * n + j] > 0 ? 1 : 0;
		}
	}
	*result /= (n - 1);

	return 0;
}

static int matrix_redundancy_coef(const matrix_t *matrix, double *result)
{
	if (matrix_adj_coef(matrix, result) == -1) {
		return -1;
	}

	*result = 1.0 - *result;
	return 0;
}

static int matrix_complexity(const matrix_t *matrix, double *result)
{
	int i, j;
	int n;

	matrix_t total_matrix;

	if (!matrix || !matrix->data) {
		return -1;
	}
	n = matrix->n;

	total_matrix.n = n;
	total_matrix.data = malloc(n * n * sizeof(*total_matrix.data));

	matrix_count_total_routes(matrix, &total_matrix);

	*result = 0;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			*result += total_matrix.data[i * n + j];
		}
	}
	*result /= (n * n);

	return 0;
}

static int matrix_centralization(const matrix_t *matrix, double *result)
{
	int i, j;
	int n;

	int max_vertecies_count = 0;
	int vertecies_count;

	if (!matrix || !matrix->data) {
		return -1;
	}
	n = matrix->n;

	*result = 0;
	for (i = 0; i < n; i++) {
		vertecies_count = 0;
		for (j = 0; j < n; j++) {
			if (matrix->data[i * n + j]) {
				vertecies_count += 1;
			}
			if (matrix->data[j * n + i]) {
				vertecies_count += 1;
			}
		}
		*result += vertecies_count;
		if (vertecies_count > max_vertecies_count) {
			max_vertecies_count = vertecies_count;
		}
	}
	*result = n * max_vertecies_count - *result;
	*result /= (2 * (n - 1) * (n - 2));

	return 0;
}

void print_help()
{
	printf(
		"structalyze --- tool for structural analyzing thats reads a matrix from stdio\n"
		"and prints the result to stdout\n"
		"\n"
		"Usage:\n"
		"\tstructalyze adjancency matrix powers N\n"
		"\tstructalyze reachability matrix\n"
		"\tstructalyze total path count matrix\n"
		"\tstructalyze reachability coefficient\n"
		"\tstructalyze adjancency coefficient\n"
		"\tstructalyze redundancy coefficient\n"
		"\tstructalyze complexity\n"
		"\tstructalyze centralization\n"
	);
}

int main(int argc, char **argv)
{
	int result = 0;

	long parameter;
	char *endptr;

	matrix_t matrix;

	matrix_t result_matrix;
	double scalar_result;

	matrix.n = 0;
	matrix.data = 0;

	result_matrix.n = 0;
	result_matrix.data = 0;
	
	if (
		argc >= 2 &&
		(strcmp(argv[1], "-h") || strcmp(argv[1], "--help"))
	) {
		print_help();
		goto MAIN_EPILOQUE;
	}


	result = read_matrix(&matrix);
	if (result == -1) {
		goto MAIN_EPILOQUE; 
	}
	result_matrix.n = matrix.n;
	result_matrix.data = malloc(
		result_matrix.n * result_matrix.n * sizeof(*result_matrix.data)
	);

	/* parse args */
	if (
		argc >= 5 && 
		strcmp(argv[1], "adjacency") == 0 &&
		strcmp(argv[2], "matrix") == 0 &&
		strcmp(argv[3], "powers") == 0
	) {
		parameter = strtol(argv[4], &endptr, 10);
		if (*endptr != '\0') {
			result = -1;
			goto MAIN_EPILOQUE;
		}

		matrix_pow(&matrix, parameter, &result_matrix);
		matrix_normalize(&result_matrix);

		matrix_display(&result_matrix);
	} else if (
		argc >= 3 &&
		strcmp(argv[1], "reachability") == 0 &&
		strcmp(argv[2], "matrix") == 0
	) {
		matrix_reachability(&matrix, &result_matrix);

		matrix_display(&result_matrix);
	} else if (
		argc >= 5 &&
		strcmp(argv[1], "total") == 0 &&
		strcmp(argv[2], "path") == 0 &&
		strcmp(argv[3], "count") == 0 &&
		strcmp(argv[4], "matrix") == 0
	) {
		matrix_count_total_routes(&matrix, &result_matrix);

		matrix_display(&result_matrix);
	} else if (
		argc >= 3 &&
		strcmp(argv[1], "reachability") == 0 &&
		strcmp(argv[2], "coefficient") == 0
	) {
		matrix_reachability_coef(&matrix, &scalar_result);

		printf("%f\n", scalar_result);
	} else if (
		argc >= 3 &&
		strcmp(argv[1], "adjacency") == 0 &&
		strcmp(argv[2], "coefficient") == 0
	) {
		matrix_adj_coef(&matrix, &scalar_result);

		printf("%f\n", scalar_result);
	} else if (
		argc >= 3 &&
		strcmp(argv[1], "redundancy") == 0 &&
		strcmp(argv[2], "coefficient") == 0
	) {
		matrix_redundancy_coef(&matrix, &scalar_result);

		printf("%f\n", scalar_result);
	} else if (
		argc >= 2 &&
		strcmp(argv[1], "complexity") == 0
	) {
		matrix_complexity(&matrix, &scalar_result);

		printf("%f\n", scalar_result);
	} else if (
		argc >= 2 &&
		strcmp(argv[1], "centralization") == 0
	) {
		matrix_centralization(&matrix, &scalar_result);

		printf("%f\n", scalar_result);
	} else {
		result = -1;
		goto MAIN_EPILOQUE;
	}

MAIN_EPILOQUE:
	free(matrix.data);
	free(result_matrix.data);

	return result;
}

