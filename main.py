#!/usr/bin/env python3

# from board import Board

# InGame = True
# x = Board()

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

import ctypes
import pathlib

if __name__ == "__main__":
    # Load the shared library into ctypes
    libname = pathlib.Path().absolute() / "libwinCheck.so"
    c_lib = ctypes.CDLL(libname)

answer = c_lib.main()
print(answer)