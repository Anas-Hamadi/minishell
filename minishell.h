/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molamham <molamham@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 13:00:59 by molamham          #+#    #+#             */
/*   Updated: 2025/07/26 13:03:56 by molamham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
# include <stdbool.h>
# include <sys/wait.h>
# include <signal.h>
# include "parsing/parse.h"

typedef struct s_shell
{
	t_list		*envp_list;
	t_list		*envp;
	t_cmdnode	*cmds;
	char		*input;
	int			exit_code;
}	t_shell;

// /*==========================New struct===============================*/
//
// /* Type definitions */
// typedef enum e_redir_type {
//     R_IN,       // "< file"
//     R_OUT,      // "> file"
//     R_APPEND    // ">> file"
// } t_redir_type;
//
// typedef struct s_redir {
//     t_redir_type       type;
//     char              *filename;    // or temp-file from heredoc
//     struct s_redir    *next;
// } t_redir;
//
// /* one command (between pipes) */
// typedef struct s_cmdnode {
//     t_list	*t_envp;
//     char       **argv;        // NULL-terminated list of args
//     t_redir     *redirs;      // list of <, >, >> (and heredoc temp files)
//     struct s_cmdnode *next;   // next in pipe
// } t_cmdnode;

/*==========================New struct===============================*/

# define RESET "\033[0m"
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define CYAN "\033[36m"
# define RESET "\033[0m"

void	ft_export(t_list **t_envp, char **input);
void	ft_cd(t_shell *shell);
int		check_builtin(t_shell *shell);
int		ft_strcmp(const char *s1, const char *s2);
char	**ft_split(char const *s, char c);
void	ft_free(char **arr);
void	ft_echo(char **s_input);
int		ft_strlen(char *str);
void	ft_pwd(char **s_input);
char	*ft_strchr(const char *s, int c);
void	ft_env(t_list *t_envp);
char	*ft_strchr(const char *s, int c);
int		ft_atoi(const char *str);
void	ft_putchar(int c, int fd);
void	ft_putstr_fd(char *str, int fd);
void	ft_exit(t_shell *shell);
void	*ft_memcpy(void *dest, const void *src, size_t n);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strrchr(const char *s, int c);
char	*ft_strdup(const char *s);
void	ft_unset(char **s_input, t_list *t_envp);
void	ft_lstfree(t_list *lst);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	**sorted_env(t_list *env);
void	ft_putendl_fd(char *s, int fd);

t_list	*ft_lstnew(void *content);
void	ft_lstadd_back(t_list **lst, t_list *t_new);
t_list	*ft_lstlast(t_list *lst);
t_list	*envp_to_list(char **envp);
int		ft_lstsize(t_list *lst);
void	ft_lstclear(t_list **lst, void (*del)(void *));

char	*get_path_value(t_list *t_envp);
char	*find_cmd_path(char *cmd, t_list *t_envp);
void	check_exec(t_shell *shell);

char	**list_to_array(t_list *t_envp);
void	handle_pipes(t_shell *shell);
void	handle_redirs(t_shell *shell);
void	handle_single_cmd(t_shell *shell);
void	update_env(t_list **envp, char *key, char *value);

void	free_shell(t_shell *shell);

void	handle_single_redir(char *filename, int flags, int std_fd);
void	handle_redirs(t_shell *shell);

#endif
