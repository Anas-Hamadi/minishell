/* parser.c */

#include "parse.h"

/* Create a new command node */
t_cmdnode *create_cmdnode(void) {
    t_cmdnode *cmd = malloc(sizeof(t_cmdnode));
    if (!cmd)
	return (NULL);
    cmd->argv = malloc(sizeof(char *));
    if (!cmd->argv) {
	free(cmd);
	return NULL;
    }
    cmd->argv[0] = NULL;
    cmd->redirs = NULL;
    cmd->next = NULL;
    return (cmd);
}

/* Create a new redirection */
t_redir *create_redir(t_redir_type type, char *filename) {
    t_redir *redir = malloc(sizeof(t_redir));
    if (!redir)
	return NULL;
    redir->type = type;
    redir->filename = strdup(filename);
    if (!redir->filename) {
	free(redir);
	return NULL;
    }
    redir->next = NULL;
    return redir;
}

/* Add redirection to command */
void add_redir_to_cmd(t_cmdnode *cmd, t_redir *redir) {
    t_redir *current;

    if (!cmd || !redir)
	return;
    if (!cmd->redirs) {
	cmd->redirs = redir;
	return;
    }
    current = cmd->redirs;
    while (current->next) current = current->next;
    current->next = redir;
}

/* Add argument to command */
void add_arg_to_cmd(t_cmdnode *cmd, char *arg) {
    int count = 0;
    char **new_argv;

    if (!cmd || !arg)
	return;

    // Count current arguments
    while (cmd->argv[count]) count++;

    // Realloc for one more argument + NULL terminator
    new_argv = malloc(sizeof(char *) * (count + 2));
    if (!new_argv)
	return;

    // Copy existing arguments
    for (int i = 0; i < count; i++) new_argv[i] = cmd->argv[i];

    // Add new argument
    new_argv[count] = strdup(arg);
    if (!new_argv[count]) {
	free(new_argv);
	return;
    }
    new_argv[count + 1] = NULL;

    // Replace old argv
    free(cmd->argv);
    cmd->argv = new_argv;
}

/* Free redirection list */
void free_redir_list(t_redir *redirs) {
    t_redir *next;

    while (redirs) {
	next = redirs->next;
	free(redirs->filename);
	free(redirs);
	redirs = next;
    }
}

/* Free single command node */
void free_cmdnode(t_cmdnode *cmd) {
    int i = 0;

    if (!cmd)
	return;

    // Free argv
    if (cmd->argv) {
	while (cmd->argv[i]) {
	    free(cmd->argv[i]);
	    i++;
	}
	free(cmd->argv);
    }

    // Free redirections
    free_redir_list(cmd->redirs);
    free(cmd);
}

/* Free entire command list */
void free_cmd_list(t_cmdnode *cmd_list) {
    t_cmdnode *next;

    while (cmd_list) {
	next = cmd_list->next;
	free_cmdnode(cmd_list);
	cmd_list = next;
    }
}

// static void add_arg(t_cmdnode *node, char *word)
// {
//     add_arg_to_cmd(node, word);
// }

// static void add_redir(t_cmdnode *node, t_redir_type type, char *filename)
// {
//     t_redir *r = create_redir(type, filename);
//     if (r)
//         add_redir_to_cmd(node, r);
// }

/* Parses one full line into a pipeline of cmdnodes */
t_cmdnode *parse_command_line(struct s_shell *shell, char *input) {
    char *cmd = input;
    t_cmdnode *head = create_cmdnode();
    t_cmdnode *cur = head;
    bool expand_hd = true;

    if (!head)
	return NULL;
    while (*cmd) {
	skip_spaces(&cmd);
	if (!*cmd)
	    break;

	// Error on unsupported metacharacters
	if (detect_invalid_metachar(*cmd)) {
	    write(STDERR_FILENO, "Error: invalid char '", 21);
	    write(STDERR_FILENO, cmd, 1);
	    write(STDERR_FILENO, "'\n", 2);
	    free_cmd_list(head);
	    return NULL;
	}

	if (*cmd == '<' && cmd[1] == '<') {
	    /* heredoc */
	    cmd += 2;
	    skip_spaces(&cmd);
	    if (!*cmd) {
		write(STDERR_FILENO,
		      "syntax error: expected heredoc delimiter\n", 42);
		free_cmd_list(head);
		return NULL;
	    }

	    expand_hd = true;
	    bool has_quotes = false;
	    char *delim = handle_word(shell, &cmd, true, &expand_hd);
	    if (!delim) {
		write(STDERR_FILENO, "syntax error: bad heredoc delimiter\n",
		      37);
		free_cmd_list(head);
		return NULL;
	    }

	    // Check if delimiter had quotes (disable expansion if so)
	    if (!expand_hd)
		has_quotes = true;

	    char *tmp;
	    if (handle_heredoc(shell, delim, expand_hd && !has_quotes, &tmp) <
	        0) {
		free(delim);
		// Check if heredoc was interrupted by signal
		if (g_signal_num == SIGINT) {
		    free_cmd_list(head);
		    return NULL;  // Return NULL to indicate interruption
		}
		perror("heredoc");
		free_cmd_list(head);
		return NULL;
	    }
	    t_redir *r = create_redir(R_IN, tmp);
	    if (r)
		add_redir_to_cmd(cur, r);
	    free(delim);
	    free(tmp);
	} else if (*cmd == '<' || *cmd == '>') {
	    /* <, >>, > */
	    t_redir_type t = (*cmd == '<') ? R_IN : R_OUT;
	    char op = *cmd++;
	    if (*cmd == op) {
		t = R_APPEND;
		cmd++;
	    }
	    skip_spaces(&cmd);
	    if (!*cmd) {
		write(STDERR_FILENO,
		      "syntax error: expected filename after redirection\n",
		      51);
		free_cmd_list(head);
		return (NULL);
	    }

	    char *fn = handle_word(shell, &cmd, 0, 0);
	    if (!fn) {
		write(STDERR_FILENO, "syntax error: bad filename\n", 28);
		free_cmd_list(head);
		return NULL;
	    }
	    t_redir *r = create_redir(t, fn);
	    if (r)
		add_redir_to_cmd(cur, r);
	    free(fn);
	} else if (*cmd == '|') {
	    /* new pipe segment */
	    cmd++;
	    cur->next = create_cmdnode();
	    if (!cur->next) {
		free_cmd_list(head);
		return NULL;
	    }
	    cur = cur->next;
	} else {
	    /* a normal word */
	    char *w = handle_word(shell, &cmd, 0, 0);
	    if (!w) {
		write(STDERR_FILENO, "Parse error\n", 12);
		free_cmd_list(head);
		return NULL;
	    }

	    // Trim trailing spaces
	    char *end = w + strlen(w) - 1;
	    while (end >= w && (*end == ' ' || *end == '\t')) *end-- = '\0';

	    add_arg_to_cmd(cur, w);
	    free(w);
	}
    }

    return (head);
}
