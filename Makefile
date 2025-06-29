CC = cc

FLAGS = -Wall -Werror -Wextra -g

NAME = minishell

SRC = utils/ft_split.c utils/utils.c builtin/ft_pwd.c builtin/check_builtin.c builtin/ft_cd.c builtin/ft_echo.c \
	  main.c builtin/ft_env.c utils/ft_strchr.c builtin/ft_exit.c utils/ft_atoi.c utils/ft_memcpy.c utils/ft_strjoin.c \
	  builtin/ft_unset.c utils/ft_lstadd_back.c utils/ft_lstlast.c utils/ft_lstnew.c utils/envp_to_list.c builtin/ft_export.c \
	  utils/ft_substr.c utils/sorted_env.c utils/ft_lstsize.c utils/ft_strncmp.c executable/executable.c pipe/pipes.c parsing/parser.c parsing/utils.c parsing/heredoc.c

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
