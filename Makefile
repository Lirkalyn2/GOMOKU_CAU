##
## EPITECH PROJECT, 2019
## Makefile
## File description:
## Makefile
##

.PHONY	:	all $(NAME) clean fclean re

NAME	=	ok

all	:
			make -C ./c++

$(NAME)	:
			make -C ./c++

clean	:
			make clean -C ./c++

fclean	:
			make fclean -C ./c++

re		:	fclean $(NAME)