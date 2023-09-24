##
## EPITECH PROJECT, 2019
## Makefile
## File description:
## Makefile
##

.PHONY	:	$(NAME) comp clean fclean re

NAME	=	libwinCheckPP.so

SRC		=	WinChecker.cpp

OBJ		=	$(SRC:.cpp=.o)

$(NAME)	:	$(OBJ)
			g++ -c -Wall -Wextra -fpic $(SRC)
			g++ -shared -o $(NAME) $(OBJ)
			mv $(NAME) ../

comp	:	$(OBJ)
			g++ -g -Wall -Wextra $(SRC)

clean	:
			rm -f $(OBJ)

fclean	:	clean
			rm -f ../$(NAME)

re		:	fclean $(NAME)