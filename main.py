#!/usr/bin/env python3

from board import Board

x = Board()

x.display()
# print("test")

P1Piece = input("Where do you want to put your piece? ")
x.putPiece((int(P1Piece[1]) - 1), (ord(P1Piece[0]) - 97), 1)
x.display()

P1Piece = input("Where do you want to put your piece? ")
x.putPiece((int(P1Piece[1]) - 1), (ord(P1Piece[0]) - 97), 1)
x.display()

P1Piece = input("Where do you want to put your piece? ")
x.putPiece((int(P1Piece[1]) - 1), (ord(P1Piece[0]) - 97), 1)
x.display()

P1Piece = input("Where do you want to put your piece? ")
x.putPiece((int(P1Piece[1]) - 1), (ord(P1Piece[0]) - 97), 1)
x.display()

P1Piece = input("Where do you want to put your piece? ")
x.putPiece((int(P1Piece[1]) - 1), (ord(P1Piece[0]) - 97), 1)
x.display()


print(x.winCheck(1, "1"))
# x.rowCheck(1, x.board)
# print(x.rowCheck(1, x.board))