#!/usr/bin/env python3

from ctypes import *
import pathlib
from board import Board

libname = pathlib.Path().absolute() / "libwinCheckPP.so"
c_lib = CDLL(libname)
c_lib.winCheck.argtypes = [c_int, POINTER(POINTER(c_int))]
c_lib.winCheck.restype = c_bool

Done = False
x = Board()

while(not Done):
    x.display()
    P1Piece = input("Where do you want to put your piece? ").split()
    if (len(P1Piece) != 2 # make sure that we have two coordinate.
        or len(P1Piece[0]) != 1 # check that there is only one char where the letter should be for the column
        or P1Piece[1].isdigit() is False # verify that we have a number for the row
        or x.putPiece((int(P1Piece[1]) - 1), (ord(P1Piece[0]) - 97), 1) is False): # try to put the piece in place if it's possible
        print("Wrong move, please try again!")

    arr = (POINTER(c_int) * 15)()
    for i in range(len(x.board)):
        arr[i] = (c_int * len(x.board[i]))(*x.board[i])
    Done = c_lib.winCheck(1, *arr)
    print("1 ", end="")
    print(Done)

    # print(x.winCheck(1, "1"))
    ## AI player