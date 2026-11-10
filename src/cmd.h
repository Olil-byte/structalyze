#ifndef CMD_H
#define CMD_H

typedef int (*cmd_arg_parser_t)(const char *token, void **value);
typedef int (*cmd_action_t)(int argc, void **args);

typedef struct cmd
{
	int path_len;
	const char *const *path;
	cmd_arg_parser_t *arg_parsers;

	cmd_action_t action;
} cmd_t;

struct cmd_trie;

typedef struct cmd_trie_edge
{
	const char *token;
	struct cmd_trie *node;

	cmd_arg_parser_t arg_parser;
} cmd_trie_edge_t;

typedef struct cmd_trie
{
	int edge_count;
	cmd_trie_edge_t *edges;

	const cmd_t *cmd;
} cmd_trie_t;

void cmd_trie_init(cmd_trie_t *trie);
void cmd_trie_free(cmd_trie_t *trie);
void cmd_trie_free_children(cmd_trie_t *trie);

int cmd_trie_append(cmd_trie_t *trie, cmd_t *cmd);

int cmd_handle(const cmd_trie_t *trie, int argc, const char **argv);

int cmd_arg_parse_int(const char *token, void **value);

#endif

