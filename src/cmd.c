#include "cmd.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void cmd_trie_init(cmd_trie_t *trie)
{
	trie->edge_count = 0;
	trie->edges = 0;

	trie->cmd = 0;
}

void cmd_trie_free(cmd_trie_t *trie)
{
	int i;
	cmd_trie_edge_t *edge;
	cmd_trie_t *node;

	if (!trie) {
		return;
	}

	for (i = 0; i < trie->edge_count; i++) {
		edge = &trie->edges[i];
		node = edge->node;

		cmd_trie_free(node);
	}

	free(trie->edges);
	free(trie);
}

void cmd_trie_free_children(cmd_trie_t *trie)
{
	int i;
	cmd_trie_edge_t *edge;
	cmd_trie_t *node;

	if (!trie) {
		return;
	}

	for (i = 0; i < trie->edge_count; i++) {
		edge = &trie->edges[i];
		node = edge->node;

		cmd_trie_free(node);
	}

	free(trie->edges);
}

int cmd_trie_append(cmd_trie_t *trie, cmd_t *cmd)
{
	int result = 0;

	int i, j;

	const char *token = 0;

	cmd_arg_parser_t arg_parser;

	cmd_trie_edge_t *edge = 0;
	cmd_trie_t *node = 0;

	for (i = 0; i < cmd->path_len; i++) {
		token = cmd->path[i];
		arg_parser = cmd->arg_parsers[i];

		node = 0;
		for (j = 0; j < trie->edge_count; j++) {
			edge = &trie->edges[j];

			if (strcmp(token, edge->token) == 0) {
				if (arg_parser != edge->arg_parser) {
					result = 1;
					goto CMD_TRIE_APPEND_EPILOQUE;
				}
				node = edge->node;
				break;
			}
		}
		if (!node) {
			trie->edge_count += 1;
			trie->edges = realloc(
				trie->edges,
				trie->edge_count * sizeof(cmd_trie_edge_t)
			);

			edge = &trie->edges[trie->edge_count - 1];

			edge->token = token;

			edge->node = malloc(sizeof(*edge->node));
			cmd_trie_init(edge->node);

			edge->arg_parser = arg_parser;

			node = edge->node;
		}
		trie = node;
	}
	if (node) {
		node->cmd = cmd;
	}

CMD_TRIE_APPEND_EPILOQUE:
	return result;
}

static void free_args(int n, void **args);
static void cmd_invalid_token(const char *token, const cmd_trie_t *context);

int cmd_handle(const cmd_trie_t *trie, int argc, const char** argv)
{
	int result = 0;

	int i, j;
	const char* token;

	cmd_trie_edge_t *edge;
	cmd_arg_parser_t arg_parser;

	int n = 0;
	void **args = 0;

	const cmd_trie_t *node;
	
	const cmd_t *cmd;
	cmd_action_t action;

	node = trie;
	for (i = 1; i < argc; i++) {
		token = argv[i];

		edge = 0;
		for (j = 0; j < node->edge_count; j++) {
			edge = &node->edges[j];

			arg_parser = edge->arg_parser;

			if (arg_parser) {
				n += 1;
				args = realloc(args, n * sizeof(*args));
				args[n - 1] = 0;
				if (arg_parser(token, &args[n - 1]) == 0) {
					break;
				}
			} else if (strcmp(token, edge->token) == 0) {
				break;
			} else {
				edge = 0;
			}
		}
		if (!edge) {
			cmd_invalid_token(token, trie);
			result = -1;

			goto CMD_HANDLE_EPILOQUE;
		}
		node = edge->node;
	}
	if (!node || !node->cmd) {
		cmd_invalid_token("", node);
		result = -1;

		goto CMD_HANDLE_EPILOQUE;
	}
	cmd = node->cmd;

	action = cmd->action;
	action(n, args);

CMD_HANDLE_EPILOQUE:
	free_args(n, args);
	
	return result;
}

static void free_args(int n, void **args)
{
	int i;
	void *arg;

	for (i = 0; i < n; i++)
	{
		arg = args[i];
		free(arg);
	}
	free(args);
}

static void cmd_invalid_token(const char *token, const cmd_trie_t *context)
{
	int i;

	const cmd_trie_edge_t *edge;

	fprintf(stderr, "structalyze: invalid token '%s' (expected: ", token);
	for (i = 0; i < context->edge_count - 1; i++) {
		edge = &context->edges[i];

		fprintf(stderr, "'%s', ", edge->token);
	}
	edge = &context->edges[context->edge_count - 1];
	fprintf(stderr, "'%s')\n", edge->token);
}

int cmd_arg_parse_int(const char *token, void **value)
{
	int result = 0;

	char symbol;
	int digit;

	int number = 0;

	int *casted_value;

	while ((symbol = *token) != '\0') {
		if (symbol >= '0' && symbol <= '9') {
			digit = symbol - '0';
			number *= 10;
			number += digit;
		} else {
			result = -1;

			goto CMD_ARG_PARSE_INT_EPILOQUE;
		}

		++token;
	}
	*value = malloc(sizeof(int));
	casted_value = (int*)*value;
	*casted_value = number;

CMD_ARG_PARSE_INT_EPILOQUE:
	return result;
}

