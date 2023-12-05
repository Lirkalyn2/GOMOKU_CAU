#!/usr/bin/env python3

from ctypes import *
import pathlib
from board import Board
from ai import *
import copy

libname = pathlib.Path().absolute() / "libwinCheckPP.so"
c_lib = CDLL(libname)
c_lib.winCheck.argtypes = [c_int, POINTER(POINTER(c_int))]
c_lib.winCheck.restype = c_bool
c_lib.ai.argtypes = [POINTER(POINTER(c_int))]
c_lib.ai.restype = c_int

Done = False
x = Board()

while(not Done):
    set_board_clib(x, c_lib)
    no_move = False
    x.display()
    P1Piece = input("Where do you want to put your piece? ").split()
    if (len(P1Piece) != 2 # make sure that we have two coordinate.
        or len(P1Piece[0]) != 1 # check that there is only one char where the letter should be for the column
        or P1Piece[1].isdigit() is False # verify that we have a number for the row
        or x.putPiece((int(P1Piece[1]) - 1), (ord(P1Piece[0]) - 97), 1) is False): # try to put the piece in place if it's possible
        no_move = True
        print("Wrong move, please try again!")

    """Converts the board for the C++"""
    arr = (POINTER(c_int) * 15)()
    for i in range(len(x.board)):
        arr[i] = (c_int * len(x.board[i]))(*x.board[i])
    # aa = c_lib.ai(*arr)
    # print("aa")
    # print(aa)
    # print(int(aa/100))
    # print(int(aa%100))
    if c_lib.winCheck(1, *arr):
        print("You won!")
        sys.exit(0)

    import datetime

    ## AI player
    if not no_move:
        """OLD AI"""
        # start_time = datetime.datetime.now()
        # [y, z] = getNextMove(copy.deepcopy(x.board))
        # end_time = datetime.datetime.now()

        # time_diff = (end_time - start_time)
        # execution_time = time_diff.total_seconds() * 1000
        # print(execution_time, end='')
        # print("ms")
        # print("y = " + str(y) + ", z = " + str(z))

        """NEW AI"""
        """Converts the board for the C++"""
        arr = (POINTER(c_int) * 15)()
        for i in range(len(x.board)):
            arr[i] = (c_int * len(x.board[i]))(*x.board[i])
        ai_rsl = c_lib.ai(*arr)
        y = int(ai_rsl / 100)
        z = int(ai_rsl % 100)

        if not (x.putPiece(y, z, 2)):
            print("AI did something wrong")
        arr = (POINTER(c_int) * 15)()
        for i in range(len(x.board)):
            arr[i] = (c_int * len(x.board[i]))(*x.board[i])
        Done = c_lib.winCheck(2, *arr)
print("Oh no, you lost!")
