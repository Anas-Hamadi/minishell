/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 15:29:12 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/03 12:31:35 by ahamadi          ###   ########.fr       */
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
	char						**argv;
	t_redir						*redirs;
	struct s_cmdnode			*next;
	bool						cmd_has_quotes;
}								t_cmdnode;

/* Word buffer for parsing */
typedef struct s_word_buffer
{
	char						**buffer;
	size_t						*buffer_size;
	size_t						*buffer_len;
	bool						*found_quotes;
	int							*expand_flag;
}								t_word_buffer;

/* Quote buffer for parsing */
typedef struct s_quote_buffer
{
	char						**buffer;
	size_t						*buffer_size;
	size_t						*buffer_len;
}								t_quote_buffer;

/* Heredoc buffer for parsing */
typedef struct s_hd_buffer
{
	char						**buffer;
	size_t						*buffer_size;
	size_t						*buffer_len;
}								t_hd_buffer;

/* Function declarations */
void							skip_spaces(char **cmd);
int								detect_invalid_metachar(char c);
char							*handle_word(struct s_shell *shell, char **cmd,
									bool in_del, int *expand_in_hd);
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
									const char *delimiter, int *expand,
									char **out_filename);

/* Word parsing utility functions */
char							*process_quoted_content(struct s_shell *shell,
									char **cmd, bool in_del);
char							*process_expansion(struct s_shell *shell,
									char **cmd, t_word_buffer *wb);
int								handle_quote_char(struct s_shell *shell,
									char **cmd, bool in_del, t_word_buffer *wb);
int								handle_regular_char(char **cmd,
									t_word_buffer *wb);
int								process_word_char(struct s_shell *shell,
									char **cmd, bool in_del, t_word_buffer *wb);

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
int								validate_heredoc_delimiter(
									const char *delimiter);
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

/* Heredoc utility functions */
char							*heredoc_generate_filename(
									struct s_shell *shell,
									int *hdoc_count);
int								heredoc_child_process(const char *delimiter,
									int *expand, const char *filename,
									struct s_shell *shell);
char							*process_line_expansion(struct s_shell *shell,
									char *line, int *expand);
int								write_line_to_file(int fd, char *line);
int								check_delimiter_match(const char *line,
									const char *delimiter);
int								process_heredoc_line(int fd,
									const char *delimiter, int *expand,
									struct s_shell *shell);
int								read_heredoc_lines(int fd,
									const char *delimiter, int *expand,
									struct s_shell *shell);

/* Parse utility functions */
void							skip_spaces(char **cmd);
int								is_metachar(char c);
int								is_space(char c);
int								detect_invalid_metachar(char c);
bool							word_has_quotes(char *input);
int								count_args(char **argv);
char							**resize_argv(char **argv, int new_size);
void							print_parsing_error(const char *message);

/* Environment variable functions */
char							*get_env_value(struct s_shell *shell,
									const char *key);
int								get_last_exit_status(struct s_shell *shell);
char							*expand_variable(struct s_shell *shell,
									char **cmd);

/* Quote handling functions */
char							*handle_quote_block(struct s_shell *shell,
									char **cmd, char *quote_context,
									bool in_del);

/* Heredoc expansion functions */
char							*handle_hd_line(struct s_shell *shell,
									char **cmd);

#endif
