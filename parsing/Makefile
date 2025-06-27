NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDES = -I./include -I./libft

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SRCDIR = .
SOURCES = main.c parser.c utils.c heredoc.c

OBJDIR = obj
OBJECTS = $(SOURCES:%.c=$(OBJDIR)/%.o)

LIBS = -lreadline -L$(LIBFT_DIR) -lft

all: $(LIBFT) $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -rf $(OBJDIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
