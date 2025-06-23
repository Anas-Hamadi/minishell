#ifndef PARSE_H
#define PARSE_H
#define MAX_WORD_SIZE 1024


# include <errno.h>
# include <stdlib.h>
# include <limits.h>
# include <stdbool.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>

char	*handle_hd_line(char **cmd);
void	skip_spaces(char **cmd);
int		detect_invalid_metachar(char c);
char	*handle_word(char **cmd, bool in_del, bool *expand_in_hd);
char	*expand_variable(char **cmd);
char	*handle_quote_block(char **cmd, char *quote_context, bool in_del);
int		get_last_exit_status(void);
int		is_metachar(char c);
int		handle_heredoc(const char *delimiter, int expand, char **out_filename);

// OLD STRUCTS 
// typedef struct s_heredoc {
//     char *delimiter;
//     char *content;
//     struct s_heredoc *next;
// } t_heredoc;    

// typedef struct s_redir {
//     int type; // 0 for input, 1 for output, 2 for append, 3 for heredoc
//     char *filename; // only used if type is not HEREDOC
//     char quote_context; // 0, ', or "
//     t_heredoc *heredoc; // Only used if type is HEREDOC
//     struct s_redir *next;
// } t_redir;

// typedef struct s_cmdnode2 {
//     t_heredoc *heredocs;
//     t_redir *redirections;
//     char    **argv;
//     struct s_cmdnode *next;
// } t_cmdnode2;


// // new main struct
// typedef struct s_cmdnode
// {
//     // t_redir *redirections;
//     // char	**reds;
// 	// as char** or actual t_redir linked list? ☝️
//     char    **argv;
// 	struct	s_cmdnode *next;
// 	// what i else i need?
// } t_cmdnode;


typedef enum e_redir_type {
    R_IN,       // "< file"
    R_OUT,      // "> file"
    R_APPEND    // ">> file"
} t_redir_type;

typedef struct s_redir {
    t_redir_type       type;
    char              *filename;    // or temp-file from heredoc
    struct s_redir    *next;
} t_redir;

/* one command (between pipes) */
typedef struct s_cmdnode {
    char       **argv;        // NULL-terminated list of args
    t_redir     *redirs;      // list of <, >, >> (and heredoc temp files)
    struct s_cmdnode *next;   // next in pipe
} t_cmdnode;


#endif