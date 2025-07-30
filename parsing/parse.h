#ifndef PARSE_H
# define PARSE_H

# include <stddef.h>

/* Forward declarations */
struct s_shell;

# include <ctype.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

extern volatile sig_atomic_t	g_signal_num;

typedef struct s_list
{
	void						*content;
	struct s_list				*next;
}								t_list;

/* Type definitions */
typedef enum e_redir_type
{
	R_IN,    // "< file"
	R_OUT,   // "> file"
	R_APPEND // ">> file"
}								t_redir_type;

typedef struct s_redir
{
	t_redir_type				type;
	char *filename; // or temp-file from heredoc
	struct s_redir				*next;
}								t_redir;

/* one command (between pipes) */
typedef struct s_cmdnode
{
	char **argv;            // NULL-terminated list of args
	t_redir *redirs;        // list of <, >, >> (and heredoc temp files)
	struct s_cmdnode *next; // next in pipe
}								t_cmdnode;

/* Function declarations */
void							skip_spaces(char **cmd);
int								detect_invalid_metachar(char c);
char							*handle_word(struct s_shell *shell, char **cmd,
									bool in_del, bool *expand_in_hd);
char							*handle_hd_line(struct s_shell *shell, char **cmd);
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
t_redir							*create_redir(t_redir_type type,
									char *filename);
void							add_redir_to_cmd(t_cmdnode *cmd,
									t_redir *redir);
void							add_arg_to_cmd(t_cmdnode *cmd, char *arg);
void							free_cmdnode(t_cmdnode *cmd);
void							free_redir_list(t_redir *redirs);
void							free_cmd_list(t_cmdnode *cmd_list);
t_cmdnode						*parse_command_line(struct s_shell *shell,
									char *input);
char							*get_env_value(struct s_shell *shell,
									const char *key);
#endif
