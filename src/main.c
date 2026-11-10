#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "cmd.h"
#include "matrix.h"

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
			goto EPILOQUE;
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
				goto EPILOQUE;
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
			goto EPILOQUE;
		}
	}
	if (i < n) {
		result = -1;
		goto EPILOQUE;
	}

EPILOQUE:
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
	int tmp;
	int width = 0;

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			tmp = (int)log10(matrix->data[i * n + j]) + 1;
			if (tmp > width) {
				width = tmp;
			}
		}
	}

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			printf("%-*d", width, matrix->data[i * n + j]);
			printf(j < n - 1 ? " " : "\n");
		}
	}
}

static int matrix_powers_cmd_action(int argc, void **args);
static int matrix_reachability_cmd_action(int argc, void **args);
static int matrix_total_paths_cmd_action(int argc, void **args);

static int coeff_reachability_cmd_action(int argc, void **args);
static int coeff_adjacency_cmd_action(int argc, void **args);
static int coeff_redundancy_cmd_action(int argc, void **args);
static int coeff_complexity_cmd_action(int argc, void **args);
static int coeff_centralization_cmd_action(int argc, void **args);

static void print_help(const cmd_trie_t *trie);

int main(int argc, const char **argv)
{
	int result = 0;

	cmd_trie_t trie;

	const char *matrix_powers_cmd_path[] = {"matrix", "powers", "<n>"};
	cmd_arg_parser_t matrix_powers_cmd_parsers[] = {0, 0, cmd_arg_parse_int}; 
	cmd_t matrix_powers_cmd;

	const char *matrix_reachability_cmd_path[] = {"matrix", "reachability"};
	cmd_arg_parser_t matrix_reachability_cmd_parsers[] = {0, 0};
	cmd_t matrix_reachability_cmd;

	const char *matrix_total_paths_cmd_path[] = {"matrix", "total-paths"};
	cmd_arg_parser_t matrix_total_paths_cmd_parsers[] = {0, 0};
	cmd_t matrix_total_paths_cmd;


	const char *coeff_reachability_cmd_path[] = {"coeff", "reachability"};
	cmd_arg_parser_t coeff_reachability_cmd_parsers[] = {0, 0};
	cmd_t coeff_reachability_cmd;

	const char *coeff_adjacency_cmd_path[] = {"coeff", "adjacency"};
	cmd_arg_parser_t coeff_adjacency_cmd_parsers[] = {0, 0};
	cmd_t coeff_adjacency_cmd;

	const char *coeff_redundancy_cmd_path[] = {"coeff", "redundancy"};
	cmd_arg_parser_t coeff_redundancy_cmd_parsers[] = {0, 0};
	cmd_t coeff_redundancy_cmd;

	const char *coeff_complexity_cmd_path[] = {"coeff", "complexity"};
	cmd_arg_parser_t coeff_complexity_cmd_parsers[] = {0, 0};
	cmd_t coeff_complexity_cmd;

	const char *coeff_centralization_cmd_path[] = {"coeff", "centralization"};
	cmd_arg_parser_t coeff_centralization_cmd_parsers[] = {0, 0};
	cmd_t coeff_centralization_cmd;


	cmd_trie_init(&trie);

	matrix_powers_cmd.path_len = 3;
	matrix_powers_cmd.path = matrix_powers_cmd_path;
	matrix_powers_cmd.arg_parsers = matrix_powers_cmd_parsers;
	matrix_powers_cmd.action = matrix_powers_cmd_action;

	cmd_trie_append(&trie, &matrix_powers_cmd);

	matrix_reachability_cmd.path_len = 2;
	matrix_reachability_cmd.path = matrix_reachability_cmd_path;
	matrix_reachability_cmd.arg_parsers = matrix_reachability_cmd_parsers;
	matrix_reachability_cmd.action = matrix_reachability_cmd_action;

	cmd_trie_append(&trie, &matrix_reachability_cmd);

	matrix_total_paths_cmd.path_len = 2;
	matrix_total_paths_cmd.path = matrix_total_paths_cmd_path;
	matrix_total_paths_cmd.arg_parsers = matrix_total_paths_cmd_parsers;
	matrix_total_paths_cmd.action = matrix_total_paths_cmd_action;

	cmd_trie_append(&trie, &matrix_total_paths_cmd);


	coeff_reachability_cmd.path_len = 2;
	coeff_reachability_cmd.path = coeff_reachability_cmd_path;
	coeff_reachability_cmd.arg_parsers = coeff_reachability_cmd_parsers;
	coeff_reachability_cmd.action = coeff_reachability_cmd_action;

	cmd_trie_append(&trie, &coeff_reachability_cmd);

	coeff_adjacency_cmd.path_len = 2;
	coeff_adjacency_cmd.path = coeff_adjacency_cmd_path;
	coeff_adjacency_cmd.arg_parsers = coeff_adjacency_cmd_parsers;
	coeff_adjacency_cmd.action = coeff_adjacency_cmd_action;

	cmd_trie_append(&trie, &coeff_adjacency_cmd);

	coeff_redundancy_cmd.path_len = 2;
	coeff_redundancy_cmd.path = coeff_redundancy_cmd_path;
	coeff_redundancy_cmd.arg_parsers = coeff_redundancy_cmd_parsers;
	coeff_redundancy_cmd.action = coeff_redundancy_cmd_action;

	cmd_trie_append(&trie, &coeff_redundancy_cmd);

	coeff_complexity_cmd.path_len = 2;
	coeff_complexity_cmd.path = coeff_complexity_cmd_path;
	coeff_complexity_cmd.arg_parsers = coeff_complexity_cmd_parsers;
	coeff_complexity_cmd.action = coeff_complexity_cmd_action;

	cmd_trie_append(&trie, &coeff_complexity_cmd);

	coeff_centralization_cmd.path_len = 2;
	coeff_centralization_cmd.path = coeff_centralization_cmd_path;
	coeff_centralization_cmd.arg_parsers = coeff_centralization_cmd_parsers;
	coeff_centralization_cmd.action = coeff_centralization_cmd_action;

	cmd_trie_append(&trie, &coeff_centralization_cmd);

	/* hack for help */
	if (argc >= 2) {
		if (
			strcmp(argv[1], "-h") == 0 || 
			strcmp(argv[1], "--help") == 0
		) {
			print_help(&trie);
			goto EPILOQUE;
		}
	}

	cmd_handle(&trie, argc, argv);


EPILOQUE:
	cmd_trie_free_children(&trie);

	return result;
}

static int matrix_powers_cmd_action(int argc, void **args)
{
	int result = 0;

	int status;

	matrix_t matrix;
	matrix_t result_matrix;

	void *arg = *args;
	int power = *(int*)arg;

	status = read_matrix(&matrix);
	if (status != 0) {
		result = -1;

		goto EPILOQUE;
	}

	matrix_init(&result_matrix, matrix.n);

	matrix_pow(&matrix, power, &result_matrix);
	matrix_norm(&result_matrix);

	matrix_display(&result_matrix);

EPILOQUE:
	free(matrix.data);
	free(result_matrix.data);

	return result;
}

static int matrix_reachability_cmd_action(int argc, void **args)
{
	int result = 0;

	int status;

	matrix_t matrix;
	matrix_t result_matrix;

	status = read_matrix(&matrix);
	if (status != 0) {
		result = -1;

		goto EPILOQUE;
	}

	matrix_init(&result_matrix, matrix.n);

	matrix_total_paths(&matrix, &result_matrix);
	matrix_norm(&result_matrix);

	matrix_display(&result_matrix);

EPILOQUE:
	free(matrix.data);
	free(result_matrix.data);

	return result;
}

static int matrix_total_paths_cmd_action(int argc, void **args)
{
	int result = 0;

	int status;

	matrix_t matrix;
	matrix_t result_matrix;

	status = read_matrix(&matrix);
	if (status != 0) {
		result = -1;

		goto EPILOQUE;
	}

	matrix_init(&result_matrix, matrix.n);

	matrix_total_paths(&matrix, &result_matrix);

	matrix_display(&result_matrix);

EPILOQUE:
	free(matrix.data);
	free(result_matrix.data);

	return result;
}

static int coeff_reachability_cmd_action(int argc, void **args)
{
	int result = 0;

	int status;

	matrix_t matrix;
	matrix_t tmp_matrix;

	double scalar_result = 0.0;

	int i, j;
	int n;

	status = read_matrix(&matrix);
	if (status != 0) {
		result = -1;

		goto EPILOQUE;
	}

	n = matrix.n;
	matrix_init(&tmp_matrix, n);

	matrix_total_paths(&matrix, &tmp_matrix);
	matrix_norm(&tmp_matrix);

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			scalar_result += tmp_matrix.data[i * n + j];
		}
	}
	scalar_result /= (n * n);

	printf("%f\n", scalar_result);

EPILOQUE:
	free(matrix.data);
	free(tmp_matrix.data);

	return result;
}

static int coeff_adjacency_cmd_action(int argc, void **args)
{
	int result = 0;

	int status;

	matrix_t matrix;
	matrix_t tmp_matrix;

	double scalar_result = 0.0;

	int i, j;
	int n;

	status = read_matrix(&matrix);
	if (status != 0) {
		result = -1;

		goto EPILOQUE;
	}

	n = matrix.n;
	matrix_init(&tmp_matrix, n);

	matrix_total_paths(&matrix, &tmp_matrix);
	matrix_norm(&tmp_matrix);

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			scalar_result += matrix.data[i * n + j] > 0 ? 1 : 0;
		}
	}
	scalar_result /= (n - 1);


	printf("%f\n", scalar_result);

EPILOQUE:
	free(matrix.data);
	free(tmp_matrix.data);

	return result;
}

static int coeff_redundancy_cmd_action(int argc, void **args)
{
	int result = 0;

	int status;

	matrix_t matrix;
	matrix_t tmp_matrix;

	double scalar_result = 0.0;

	int i, j;
	int n;

	status = read_matrix(&matrix);
	if (status != 0) {
		result = -1;

		goto EPILOQUE;
	}

	n = matrix.n;
	matrix_init(&tmp_matrix, n);

	matrix_total_paths(&matrix, &tmp_matrix);
	matrix_norm(&tmp_matrix);

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			scalar_result += matrix.data[i * n + j] > 0 ? 1 : 0;
		}
	}
	scalar_result /= (n - 1);
	scalar_result = scalar_result - 1.0;

	printf("%f\n", scalar_result);

EPILOQUE:
	free(matrix.data);
	free(tmp_matrix.data);

	return result;
}

static int coeff_complexity_cmd_action(int argc, void **args)
{
	int result = 0;

	int status;

	matrix_t matrix;
	matrix_t tmp_matrix;

	double scalar_result = 0.0;

	int i, j;
	int n;

	status = read_matrix(&matrix);
	if (status != 0) {
		result = -1;

		goto EPILOQUE;
	}

	n = matrix.n;
	matrix_init(&tmp_matrix, n);

	matrix_total_paths(&matrix, &tmp_matrix);

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			scalar_result += tmp_matrix.data[i * n + j];
		}
	}
	scalar_result /= (n * n);
	printf("%f\n", scalar_result);

EPILOQUE:
	free(matrix.data);
	free(tmp_matrix.data);

	return result;
}

static int coeff_centralization_cmd_action(int argc, void **args)
{
	int result = 0;

	int status;

	matrix_t matrix;

	double scalar_result = 0.0;

	int i, j;
	int n;

	int max_vertecies_count = 0;
	int vertecies_count;

	status = read_matrix(&matrix);
	if (status != 0) {
		result = -1;

		goto EPILOQUE;
	}

	n = matrix.n;

	scalar_result = 0.0;
	for (i = 0; i < n; i++) {
		vertecies_count = 0;
		for (j = 0; j < n; j++) {
			if (matrix.data[i * n + j]) {
				vertecies_count += 1;
			}
			if (matrix.data[j * n + i]) {
				vertecies_count += 1;
			}
		}
		scalar_result += vertecies_count;
		if (vertecies_count > max_vertecies_count) {
			max_vertecies_count = vertecies_count;
		}
	}
	scalar_result = n * max_vertecies_count - scalar_result;
	scalar_result /= (2 * (n - 1) * (n - 2));
	printf("%f\n", scalar_result);

EPILOQUE:
	free(matrix.data);

	return result;
}

const void print_help(const cmd_trie_t *trie)
{
	printf(
		"structalyze - structural analysis of a matrix\n"
		"\n"
		"Reads a matrix from stdio and prints the result to stdout\n"
		"\n"
	
		"Usage:\n"
		"	structalyze matrix powers N\n"
		"	structalyze matrix reachability\n"
		"	structalyze matrix total-paths\n"
		"\n"
		"	structalyze coeff reachability\n"
		"	structalyze coeff adjacency\n"
		"	structalyze coeff redundancy\n"
		"	structalyze coeff complexity\n"
		"	structalyze coeff centralization\n"
		"\n"
		"Options:\n"
		"	-h, --help	show this help and exit\n"
		"\n"
	);
}

