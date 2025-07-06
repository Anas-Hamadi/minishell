CC = cc
FLAGS = -Wall -Wextra -Werror
NAME = libftprintf.a
SRCS = ft_putchar.c ft_putstr.c ft_putnbr.c ft_printf.c\
		ft_hexa.c ft_putptr.c

OFILES = $(SRCS:.c=.o)

$(NAME) : $(OFILES)

all : $(NAME)

%.o: %.c
	$(CC) -c $(FLAGS) $< -o $@
	ar rcs $(NAME) $@

clean:
	rm -rf  $(OFILES)

fclean:	clean
	rm -rf $(NAME)

re:	fclean all

.PHONY:	all clean