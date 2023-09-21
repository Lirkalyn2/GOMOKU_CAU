#!/usr/bin/env python3

BoardSize = 15

class Board():
    board = [[0 for x in range(BoardSize)] for y in range(BoardSize)]

    # def __init__(self):

    def getBoard(self):
        return self.board

    def putPiece(self, x, y, piece):
        if (self.board[x][y] == 0):
            self.board[x][y] = piece
            return True
        return False

    def display(self):
        print("   a  b  c  d  e  f  g  h  i  j  k  l  m  n  o")
        print("  =============================================")
        for x in range(len(self.board)):
            line = (" " if (x + 1) < 10 else "") + str((x + 1)) + "|"
            for y in range(len(self.board[x])):
                line += "-" if self.board[x][y] == 0 else ("W" if self.board[x][y] == 1 else "B")
                if y != (BoardSize - 1):
                    line += "  "
                else:
                    line += "|"
            if x != (BoardSize - 1):
                print(line + "\n", end="")
            else:
                print("  =============================================")





    def winCheck(self, Piece_Number, Piece_Colour):
        if self.rowCheck(Piece_Number, self.board) or self.rowCheck(Piece_Number, self.transpose())or self.rowCheck(Piece_Number, self.transposeDiagonalInc()) or self.rowCheck(Piece_Number, self.transposeDiagonalDec()):
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


    def transposeDiagonalDec(self):
        lst = []
        for i in range(len(self.board) * 2 - 1):
            lst.append(self.getDiagonalDec(self.board, i))
        return lst

    def getDiagonalDec(self, localBoard, digNum):
        lst=[]
        if digNum <= len(localBoard) - 1:
            index = len(localBoard) - 1
            for i in range(digNum, -1, -1):
                lst.append(localBoard[i][index])
                index -= 1
            return lst
        else:
            index = (len(localBoard) * 2 - 2) - digNum
            for i in range(len(localBoard) - 1, digNum - len(localBoard), -1):
                lst.append(localBoard[i][index])
                index -= 1
            return lst


    def transposeDiagonalInc(self):
        lst = []
        for i in range(len(self.board) * 2 - 1):
            lst.append(self.getDiagonalInc(self.board, i))
        return lst

    def getDiagonalInc(self, localBoard, digNum):
        lst=[]
        if digNum <= len(localBoard) - 1:
            index = 0
            for i in range(digNum, -1, -1):
                lst.append(localBoard[i][index])
                index += 1
            return lst
        else:
            index =  digNum - len(localBoard) + 1
            for i in range(len(localBoard) - 1, digNum - len(localBoard), -1):
                lst.append(localBoard[i][index])
                index += 1
            return lst


    def transpose(self):
        lst = []
        for i in range(len(self.board)):
            lst.append(self.getCol(self.board, i))
        return lst

    def getCol(self, localBoard, colNum):
        lst = []
        for i in range(len(localBoard)):
            lst.append(localBoard[i][colNum])
        return lst
