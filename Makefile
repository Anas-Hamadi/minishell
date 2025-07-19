CC = cc

FLAGS = -Wall -Werror -Wextra -g

NAME = minishell

SRC = utils/ft_split.c utils/utils.c single_cmds/builtin/ft_pwd.c single_cmds/builtin/check_builtin.c single_cmds/builtin/ft_cd.c single_cmds/builtin/ft_echo.c \
	  main.c single_cmds/builtin/ft_env.c utils/ft_strchr.c single_cmds/builtin/ft_exit.c utils/ft_atoi.c utils/ft_memcpy.c utils/ft_strjoin.c \
	  single_cmds/builtin/ft_unset.c utils/ft_lstadd_back.c utils/ft_lstlast.c utils/ft_lstnew.c utils/envp_to_list.c single_cmds/builtin/ft_export.c \
	  utils/ft_substr.c utils/sorted_env.c utils/ft_lstsize.c utils/ft_strncmp.c single_cmds/executable/executable.c pipe/pipes.c parsing/parser.c parsing/utils.c parsing/heredoc.c \
	  utils/ft_putstr.c single_cmds/single_cmds.c utils/free.c utils/ft_lstclear.c handle_redirs.c

OBJ = $(SRC:.c=.o)

all : $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -lreadline -o $(NAME)

%.o : %.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY : all fclean
