/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 13:00:59 by molamham          #+#    #+#             */
/*   Updated: 2025/08/01 15:07:21 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "parsing/parse.h"
# include <asm-generic/signal-defs.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

extern volatile sig_atomic_t	g_signal_num;

typedef struct s_shell
{
	t_list						*envp_list;
	t_list						*envp;
	t_cmdnode					*cmds;
	char						*input;
	int							exit_code;
	char						**temp_files;
	int							temp_count;
	int							temp_capacity;
}								t_shell;

void							add_temp_file(t_shell *shell,
									const char *filename);
void							cleanup_temp_files(t_shell *shell);
void							remove_temp_file(t_shell *shell,
									const char *filename);

# define RESET "\033[0m"
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define CYAN "\033[36m"

void							ft_export(t_shell *shell);
void							ft_cd(t_shell *shell);
int								check_builtin(t_shell *shell);
int								ft_strcmp(const char *s1, const char *s2);
char							**ft_split(char const *s, char c);
void							ft_free(char **arr);
void							ft_echo(t_shell *shell);
int								ft_strlen(char *str);
void							ft_pwd(t_shell *shell);
char							*ft_strchr(const char *s, int c);
void							ft_env(t_shell *shell);
int								ft_atoi(const char *str);
void							ft_putchar(int c, int fd);
void							ft_putstr_fd(char *str, int fd);
void							ft_exit(t_shell *shell);
void							*ft_memcpy(void *dest, const void *src,
									size_t n);
char							*ft_strjoin(char const *s1, char const *s2);
char							*ft_strdup(const char *s);
void							ft_unset(t_shell *shell);
void							ft_lstfree(t_list *lst);
int								ft_strncmp(const char *s1, const char *s2,
									size_t n);
char							*ft_substr(char const *s, unsigned int start,
									size_t len);
char							**sorted_env(t_list *env);
void							ft_putendl_fd(char *s, int fd);

t_list							*ft_lstnew(void *content);
void							ft_lstadd_back(t_list **lst, t_list *t_new);
t_list							*ft_lstlast(t_list *lst);
t_list							*envp_to_list(char **envp);
int								ft_lstsize(t_list *lst);
void							ft_lstclear(t_list **lst, void (*del)(void *));

char							*get_path_value(t_list *t_envp);
char							*find_cmd_path(char *cmd, t_list *t_envp);
void							check_exec(t_shell *shell);

char							**list_to_array(t_list *t_envp);
void							handle_pipes(t_shell *shell);
int								handle_redirs(t_shell *shell);
void							handle_single_cmd(t_shell *shell);
void							update_env(t_list **envp, char *key,
									char *value);

void							free_shell(t_shell *shell);

void							setup_signals_interactive(void);
void							setup_signals_heredoc(void);
void							setup_signals_child(void);

int								handle_single_redir(char *filename, int flags,
									int std_fd);

void							handle_parse_error(t_shell *shell);
void							restore_stdio(int saved_in, int saved_out);
int								handle_input(t_shell *shell);
void							check_signals_after_execution(t_shell *shell);
void							start(t_shell *shell);

#endif
