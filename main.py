#!/usr/bin/env python3

from board import Board

InGame = True
x = Board()

while(InGame):
    x.display()
    P1Piece = input("Where do you want to put your piece? ")
    if (x.putPiece((int(P1Piece[1:]) - 1), (ord(P1Piece[0]) - 97), 1) is False): # needs try catch to avoid crash
        print("Wrong move, P1Piece is not available please try again!")
    print(x.winCheck(1, "1"))
    ## AI player