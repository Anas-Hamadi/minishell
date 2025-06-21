#ifndef PARSE_H
#define PARSE_H
#define MAX_WORD_SIZE 1024


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

void	skip_spaces(char **cmd);
int		detect_invalid_metachar(char c);
char	*handle_word(char **cmd);
char	*expand_variable(char **cmd);
char	*handle_quote_block(char **cmd, char *quote_context);
int		get_last_exit_status(void);

typedef struct s_heredoc {
    char *delimiter;
    char *content;
    struct s_heredoc *next;
} t_heredoc;    

typedef struct s_redir {
    int type; // 0 for input, 1 for output, 2 for append, 3 for heredoc
    char *filename; // only used if type is not HEREDOC
    char quote_context; // 0, ', or "
    t_heredoc *heredoc; // Only used if type is HEREDOC
    struct s_redir *next;
} t_redir;

typedef struct s_cmdnode2 {
    t_heredoc *heredocs;
    t_redir *redirections;
    char    **argv;
    struct s_cmdnode *next;
} t_cmdnode2;


// new main struct
typedef struct s_cmdnode
{
    // t_redir *redirections;
    char    **argv;
    char	**reds;
	struct	s_cmdnode *next;
} t_cmdnode;


#endif