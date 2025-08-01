/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 15:29:12 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/01 17:31:05 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include "libft/libft.h"
# include <ctype.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

extern volatile sig_atomic_t	g_signal_num;

struct							s_shell;

typedef enum e_redir_type
{
	R_IN,
	R_OUT,
	R_APPEND
}								t_redir_type;

typedef struct s_redir
{
	t_redir_type				type;
	char						*filename;
	struct s_redir				*next;
}								t_redir;

/* one command (between pipes) */
typedef struct s_cmdnode
{
	char				**argv;
	t_redir				*redirs;
	struct s_cmdnode	*next;
}								t_cmdnode;

/* Function declarations */
void							skip_spaces(char **cmd);
int								detect_invalid_metachar(char c);
char							*handle_word(struct s_shell *shell, char **cmd,
									bool in_del, bool *expand_in_hd);
bool							word_has_quotes(char *input);
char							*handle_hd_line(struct s_shell *shell,
									char **cmd);
char							*expand_variable(struct s_shell *shell,
									char **cmd);
int								get_last_exit_status(struct s_shell *shell);
char							*handle_quote_block(struct s_shell *shell,
									char **cmd, char *quote_context,
									bool in_del);
int								is_metachar(char c);
int								handle_heredoc(struct s_shell *shell,
									const char *delimiter, int expand,
									char **out_filename);

/* Memory utility functions */
char							*safe_strcat_realloc(char **dest,
									size_t *dest_size, size_t *dest_len,
									const char *src);
char							*safe_charcat_realloc(char **dest,
									size_t *dest_size, size_t *dest_len,
									char c);
char							*ft_itoa_simple(int n);

/* Signal handling functions */
void							setup_signals_interactive(void);
void							setup_signals_heredoc(void);
void							setup_signals_child(void);

/* Structure management functions */
t_cmdnode						*create_cmdnode(void);
struct s_redir					*create_redir(t_redir_type type,
									char *filename);
void							add_redir_to_cmd(t_cmdnode *cmd,
									t_redir *redir);
void							add_arg_to_cmd(t_cmdnode *cmd, char *arg);
void							free_cmdnode(t_cmdnode *cmd);
void							free_redir_list(t_redir *redirs);
void							free_cmd_list(t_cmdnode *cmd_list);
struct s_cmdnode				*parse_command_line(struct s_shell *shell,
									char *input);
char							*get_env_value(struct s_shell *shell,
									const char *key);

/* Helper functions */
void							copy_argv_array(char **new_argv,
									char **old_argv, int count);
int								validate_heredoc_delimiter(const char
									*delimiter);
int								handle_heredoc_parsing(struct s_shell *shell,
									char **cmd_ptr, t_cmdnode *cmd);
int								handle_redirection_parsing(
									struct s_shell *shell,
									char **cmd_ptr, t_cmdnode *cmd);
int								handle_pipe_token(char **cmd_ptr,
									t_cmdnode **cur);
int								handle_word_token(struct s_shell *shell,
									char **cmd_ptr, t_cmdnode *cur);
int								handle_heredoc_token(struct s_shell *shell,
									char **cmd_ptr, t_cmdnode *cur);
int								handle_redirection_token(struct s_shell *shell,
									char **cmd_ptr, t_cmdnode *cur);
int								handle_token_types(struct s_shell *shell,
									char **cmd, t_cmdnode **cur);
#endif
