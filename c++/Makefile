##
## EPITECH PROJECT, 2019
## Makefile
## File description:
## Makefile
## -O3/-g

.PHONY	:	$(NAME) comp clean fclean re

NAME	=	libwinCheckPP.so

STATIC_LIB_NAME	=	./uint256/libuint256.a

STATIC_SRC = 	./uint256/uint128_t.cpp 	\
				./uint256/uint256_t.cpp

SRC		=	WinChecker.cpp				\
			ai.cpp						\

OBJ		=	$(SRC:.cpp=.o)
STATIC_OBJ		=	$(STATIC_SRC:.cpp=.o)

$(NAME)	:	$(OBJ)
			gcc -O3 -c -fPIC $(STATIC_SRC)
			ar -rv $(STATIC_LIB_NAME) $(STATIC_OBJ)
			g++ -O3 -c -Wall -Wextra -fPIC $(SRC)
			g++ -o test $(OBJ) $(STATIC_LIB_NAME)
			g++ -shared -o $(NAME) $(OBJ) $(STATIC_LIB_NAME)
# gcc -o $(NAME) -Wl,--whole-archive $(STATIC_LIB_NAME) -Wl,--no-whole-archive -shared  $(OBJ)
#			g++ -shared -o $(NAME) $(OBJ)
			mv $(NAME) ../

# gcc -o test.so -Wl,--whole-archive ./uint256/libuint256.a -Wl,--no-whole-archive -shared WinChecker.o ai.o

# 		g++ -o test $(OBJ)
# 		g++ -shared -o $(NAME) $(OBJ)
# 		mv $(NAME) ../

comp	:	$(OBJ)
			g++ -g -Wall -Wextra $(SRC)

clean	:
			rm -f $(OBJ)

fclean	:	clean
			rm -f ../$(NAME)

re		:	fclean $(NAME)