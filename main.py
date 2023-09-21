#!/usr/bin/env python3

from board import Board

InGame = True
x = Board()

# while(InGame):
#     x.display()
#     P1Piece = input("Where do you want to put your piece? ").split()
#     if (len(P1Piece) != 2 # make sure that we have two coordinate.
#         or len(P1Piece[0]) != 1 # check that there is only one char where the letter should be for the column
#         or P1Piece[1].isdigit() is False # verify that we have a number for the row
#         or x.putPiece((int(P1Piece[1]) - 1), (ord(P1Piece[0]) - 97), 1) is False): # try to put the piece in place if it's possible
#         print("Wrong move, please try again!")
#     print(x.winCheck(1, "1"))
#     ## AI player

# from ctypes import *
# import pathlib

# if __name__ == "__main__":
#     # Load the shared library into ctypes
#     libname = pathlib.Path().absolute() / "libwinCheck.so"
#     c_lib = CDLL(libname)
#     c_lib.winCheck.argtypes = [POINTER(POINTER(c_int))]
#     # c_lib.winCheck.argtypes = None
#     c_lib.winCheck.restype = c_bool

# # # arr = (c_int * len(x.board[0]))(*x.board[0])
# # arr = (c_int * 15) * 15
# # for i in range(len(x.board)):
# #     for j in range(len(x.board[i])):
# #         arr[i][j] = x.board[i][j]
# cBoard = (c_int * 15) * 15
# arr = cBoard()
# for i in range(len(x.board)):
#     for j in range(len(x.board[i])):
#         arr[i][j] = x.board[i][j]
# answer = c_lib.winCheck(byref(arr))
# # print(answer)


from ctypes import *
import pathlib

if __name__ == "__main__":
    # Load the shared library into ctypes
    libname = pathlib.Path().absolute() / "libwinCheck.so"
    c_lib = CDLL(libname)
    c_lib.winCheck.argtypes = [POINTER(POINTER(c_int))]
    c_lib.winCheck.restype = c_bool

arr = (POINTER(c_int) * 15)()
for i in range(len(x.board)):
    arr[i] = (c_int * len(x.board[i]))(*x.board[i])
answer = c_lib.winCheck(*arr)
print(answer)