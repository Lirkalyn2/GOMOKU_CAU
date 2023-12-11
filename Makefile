##
## EPITECH PROJECT, 2019
## Makefile
## File description:
## Makefile
## -O3/-g

.PHONY	:	$(NAME) comp clean fclean re

NAME	=	Gomoku

STATIC_LIB_NAME	=	./uint256/libuint256.a

STATIC_SRC = 	./uint256/uint128_t.cpp 	\
				./uint256/uint256_t.cpp

SRC		=	WinChecker.cpp				\
			ai.cpp						\
			ML.cpp						\
			Game.cpp					\
			main.cpp

OBJ		=	$(SRC:.cpp=.o)
STATIC_OBJ		=	$(STATIC_SRC:.cpp=.o)

$(NAME)	:	$(OBJ)
		g++ -O3 -Wall -Wextra -fPIC -pthread -o $(NAME) $(SRC) $(STATIC_SRC) ./xxhashlib/xxhash.c


# $(NAME)	:	$(OBJ)
# 			gcc -O3 -c -fPIC $(STATIC_SRC)
# 			ar -rv $(STATIC_LIB_NAME) uint128_t.o uint256_t.o
# 			g++ -g -c -Wall -Wextra -fPIC $(SRC) ./xxhashlib/xxhash.c
# 			g++ -o test ./xxhash.o $(OBJ) $(STATIC_LIB_NAME)
# 			# g++ -shared -o $(NAME) ./xxhash.o $(OBJ) $(STATIC_LIB_NAME)
# # gcc -o $(NAME) -Wl,--whole-archive $(STATIC_LIB_NAME) -Wl,--no-whole-archive -shared  $(OBJ)
# #			g++ -shared -o $(NAME) $(OBJ)
# 			mv $(NAME) ../

# gcc -o test.so -Wl,--whole-archive ./uint256/libuint256.a -Wl,--no-whole-archive -shared WinChecker.o ai.o

# 		g++ -o test $(OBJ)
# 		g++ -shared -o $(NAME) $(OBJ)
# 		mv $(NAME) ../

comp	:	$(OBJ)
			g++ -g -Wall -Wextra $(SRC)

clean	:
			rm -f $(OBJ)
			rm -f $(STATIC_OBJ)
			rm -f ./xxhash.o
			rm -f *.o

fclean	:	clean
			rm -f $(NAME)
#			rm -f $(STATIC_LIB_NAME)

re		:	fclean $(NAME)