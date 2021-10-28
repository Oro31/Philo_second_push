CFLAGS = -Wall -Wextra

NAME = philosopher

SRC = $(wildcard *.c)

OBJ = $(SRC:%.c=%.o)

all: $(NAME)

$(NAME): $(OBJ)
	gcc $(CFLAGS) -o $(NAME) $(OBJ) -D_REENTRANT

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

flemme: re
	make clean

.PHONY: clean fclean all re
