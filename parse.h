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

typedef struct s_cmdnode {
    t_heredoc *heredocs;
    t_redir *redirections;
    char    **argv;
    struct s_cmdnode *next;
} t_cmdnode;  