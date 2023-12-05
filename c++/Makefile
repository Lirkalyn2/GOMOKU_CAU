##
## EPITECH PROJECT, 2019
## Makefile
## File description:
## Makefile
## -O3

.PHONY	:	$(NAME) comp clean fclean re

NAME	=	libwinCheckPP.so

SRC		=	WinChecker.cpp	\
			ai.cpp

OBJ		=	$(SRC:.cpp=.o)

$(NAME)	:	$(OBJ)
			g++ -g -c -Wall -Wextra -fpic $(SRC)
			g++ -o test $(OBJ)
			g++ -shared -o $(NAME) $(OBJ)
			mv $(NAME) ../

comp	:	$(OBJ)
			g++ -g -Wall -Wextra $(SRC)

clean	:
			rm -f $(OBJ)

fclean	:	clean
			rm -f ../$(NAME)

re		:	fclean $(NAME)