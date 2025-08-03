CC = cc

FLAGS = -Wall -Werror -Wextra -g

NAME = minishell

LIBFT_DIR = parsing/libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC = utils/ft_split.c utils/utils.c single_cmds/builtin/ft_pwd.c single_cmds/builtin/check_builtin.c single_cmds/builtin/ft_cd.c single_cmds/builtin/cd_utils.c single_cmds/builtin/ft_echo.c \
	  main.c main_utils.c single_cmds/builtin/ft_env.c utils/ft_strchr.c single_cmds/builtin/ft_exit.c utils/ft_atoi.c utils/ft_memcpy.c utils/ft_strjoin.c \
	  single_cmds/builtin/ft_unset.c utils/ft_lstadd_back.c utils/ft_lstlast.c utils/ft_lstnew.c utils/envp_to_list.c single_cmds/builtin/ft_export.c single_cmds/builtin/ft_export_utils.c \
	  utils/ft_substr.c utils/sorted_env.c utils/ft_lstsize.c utils/ft_strncmp.c single_cmds/executable/executable.c pipe/pipes.c pipe/pipe_utils.c parsing/parser.c parsing/utils.c parsing/heredoc.c parsing/memory_utils.c \
	  utils/ft_putstr.c single_cmds/single_cmds.c utils/free.c utils/ft_lstclear.c handle_redirs.c single_cmds/executable/executable_utils.c single_cmds/executable/exec_utils.c signals.c signals_setup.c temp_files.c \
	  parsing/parser_utils.c parsing/parse_heredoc.c parsing/parse_redirs.c parsing/parse_pipeline.c parsing/parser_memory.c parsing/parse_tokens.c \
	  parsing/heredoc_utils.c parsing/heredoc_process.c parsing/heredoc_input.c parsing/parse_env.c parsing/expand_var.c \
	  parsing/parse_quotes.c parsing/parse_heredoc_expand.c parsing/parse_word.c parsing/parse_word_utils.c

OBJ = $(SRC:.c=.o)

all : $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(FLAGS) $(OBJ) $(LIBFT) -lreadline -o $(NAME)

%.o : %.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -rf $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY : all fclean
