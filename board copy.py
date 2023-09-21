#!/usr/bin/env python3

class Board():
    board = [[0 for x in range(16)] for y in range(16)]

    # def __init__(self):

    def getBoard(self):
        return self.board

    def putPiece(self, x, y, piece):
        self.board[x][y] = piece

    def display(self):
        print("   a  b  c  d  f  g  h  i  j  k  l  m  n  o  p  q")
        print("  ================================================")
        for x in range(len(self.board)):
            line = (" " if (x + 1) < 10 else "") + str((x + 1)) + "|"
            for y in range(len(self.board[x])):
                line += "-" if self.board[x][y] == 0 else ("W" if self.board[x][y] == 1 else "B")
                if y != 15:
                    line += "  "
                else:
                    line += "|"
            if x != 15:
                print(line + "\n", end="")
            else:
                print("  ================================================")





    def winCheck(self, Piece_Number, Piece_Colour):
        if self.rowCheck(Piece_Number, self.board) or self.rowCheck(Piece_Number, self.transpose(self.board)) or self.rowCheck(Piece_Number, self.transposeDiagonalInc(self.board)) or self.rowCheck(Piece_Number, self.transposeDiagonalDec(self.board)):
            Winner = Piece_Colour
            return Winner

    def rowCheck(self, Piece_Number, board):
        for i in range(len(board)):
            if board[i].count(Piece_Number) >= 5:

                for z in range(len(board) - 3):
                    Connection = 0

                    for c in range(5):
                        if board[i][z + c] == Piece_Number:
                            Connection += 1

                        else:
                            break

                        if Connection == 5:
                            return True

    def getDiagonalDec(self, loa, digNum):
        lst=[]
        if digNum <= len(loa) - 1:
            index = len(loa) - 1
            for i in range(digNum, -1, -1):
                lst.append(loa[i][index])
                index -= 1
            return lst
        else:
            index = (len(loa) * 2 - 2) - digNum
            for i in range(len(loa) - 1, digNum - len(loa), -1):
                lst.append(loa[i][index])
                index -= 1
            return lst


    def transposeDiagonalDec(self, loa):
        lst = []
        for i in range(len(loa) * 2 - 1):
            lst.append(self.getDiagonalDec(loa, i))
        return lst

    def getDiagonalInc(self, loa, digNum):
        lst=[]
        if digNum <= len(loa) - 1:
            index = 0
            for i in range(digNum, -1, -1):
                lst.append(loa[i][index])
                index += 1
            return lst
        else:
            index =  digNum - len(loa) + 1
            for i in range(len(loa) - 1, digNum - len(loa), -1):
                lst.append(loa[i][index])
                index += 1
            return lst


    def transposeDiagonalInc(self, loa):
        lst = []
        for i in range(len(loa) * 2 - 1):
            lst.append(self.getDiagonalInc(loa, i))
        return lst

    def transpose(self, loa):
        lst = []
        for i in range(len(loa)):
            lst.append(self.getCol(loa, i))
        return lst

    def getCol(self, loa, colNum):
        lst = []
        for i in range(len(loa)):
            lst.append(loa[i][colNum])
        return lst
