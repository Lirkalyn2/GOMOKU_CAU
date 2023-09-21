##
## EPITECH PROJECT, 2019
## Makefile
## File description:
## Makefile
##

.PHONY	:	$(NAME) clean fclean re

NAME	=	libwinCheck.so

SRC		=	winCheck.c \
			tools.c

OBJ		=	$(SRC:.c=.o)

$(NAME)	:	$(OBJ)
			gcc -c -Wall -Wextra -fpic $(SRC)
			gcc -shared -o $(NAME) $(OBJ)
			mv $(NAME) ../

clean	:
			rm -f $(OBJ)

fclean	:	clean
			rm -f ../$(NAME)

re		:	fclean $(NAME)