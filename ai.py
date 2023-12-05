# :)

from board import Board, BoardSize
import random, sys
from ctypes import *
from multiprocessing import Pool
from multiprocessing.dummy import Pool as ThreadPool
from itertools import repeat

MAX_DEPTH = 4
MAX_THREAD = 4
Infinity = 100000
NInfinity = -100000
move = 0
board = []
c_lib = 0

def set_board_clib(board_m, clib):
    global board
    global c_lib
    board = board_m
    c_lib = clib

def getNextMove(matrix):
    bestScore = NInfinity
    squares = getSquaresToCheck(matrix)
    y = 0
    x = 0
    best_y = 0
    best_x = 0

    print(f'Check {squares}')
    Multi_Pass = []

    # for i in range(len(squares)):
    #     [y, x] = squares[i]
    #     matrix[y][x] = -1
    #     score = alphabeta(matrix, 0, NInfinity, Infinity, False)
    #     matrix[y][x] = 0

    for i in range(len(squares)):
        [y, x] = squares[i]
        Multi_Pass.append([y, x])

    threadCount = MAX_THREAD
    if (len(Multi_Pass) < MAX_THREAD):
        threadCount = len(Multi_Pass)

    pool = ThreadPool(threadCount)
    results = pool.starmap(getNextMoveMulti, zip(Multi_Pass, repeat(matrix)))
    pool.close()
    pool.join()

    for pos in range(len(Multi_Pass)):
        # print(f'{[x, y]}`s bestscore of {results[pos]} evaluated to best {bestScore}')
        if results[pos] > bestScore:
            print("MAIS PUTAIN")
            bestScore = results[pos]
            best_y = y
            best_x = x
    # Recast the answer as a readable str. Cast y from int to minuscule letter aswell
    ans = f'{chr(97 + best_y)} {best_x + 1}'
    # print(f'Returning {best_y} {best_x} for {bestScore}')
    return [best_y, best_x]

def getNextMoveMulti(Multi_Pass, matrix):
    matrix[Multi_Pass[0]][Multi_Pass[1]] = -1
    tmp = alphabeta(matrix, 0, NInfinity, Infinity, False)
    matrix[Multi_Pass[0]][Multi_Pass[1]] = 0
    return tmp

# Get only the squares that need to be checked (aka empty squares next to placed ones)

def getSquaresToCheck(matrix):
    adjacent = []

    for i in range(len(matrix)):
        for j in range(len(matrix[i])):
            if not matrix[i][j] and isTouchingOccupied(matrix, i, j):
                adjacent.append([i, j])
#            matrix[i][j] = 0
#    for i in range(len(matrix)):
#        for j in range(len(matrix[i])):
#            matrix[i][j] = 0

    return adjacent


def isTouchingOccupied(matrix, i, j):
    return (occupied(matrix, i+1, j) or occupied(matrix, i-1, j) or occupied(matrix, i, j+1)
        or occupied(matrix, i, j-1) or occupied(matrix, i+1, j+1) or occupied(matrix, i-1, j+1)
        or occupied(matrix, i-1, j-1) or occupied(matrix, i+1, j-1))

def occupied(matrix, x, y):
    try:
        return matrix[x][y] # this bool only works with 1 not 2
    except:
        return False


# Alphabeta main function

def alphabeta(matrix, depth, alpha, beta, isAiTurn):
    global board
    global c_lib

    if depth >= MAX_DEPTH:
        # arrived at max depth, get the alphabeta values from this path and return
        eval = staticEval(matrix)
        return eval

    board.board = matrix
    arr = (POINTER(c_int) * 15)()
    for i in range(len(board.board)):
        arr[i] = (c_int * len(board.board[i]))(*board.board[i])
    winner = c_lib.winCheck(1 if not isAiTurn else 2, *arr)
    if winner:
        # print(f'For, {"AI" if winner != 1 else "Human"}: ', end='')
        # print(f'CATASTROPHIC FAILURE IMMINENT for sit {matrix}: {winner}')
        # winner should be 1 fro human and 2 for player, but here it either true or false.
        return 999999999 * (-1 if winner == 1 else 1) # winCheck always return true or false, not the color of the winnig side. should compare isAiTurn.

    best = NInfinity if isAiTurn else Infinity
    squares = getSquaresToCheck(matrix)
    for i in range(len(squares)):
        [y, x] = squares[i]
        matrix[y][x] = -1 if isAiTurn else 1

        score = alphabeta(matrix, depth + 1, alpha, beta, not isAiTurn)
#        print(f'{"max" if isAiTurn else "min"} btw score {score} é best {best} is...', end='')
        best = max(score, best) if isAiTurn else min(score, best)
#        print(f'{best}')

        if isAiTurn:
            alpha = max(alpha, best)
        else:
            beta = min(beta, best)

        matrix[y][x] = 0

        if alpha >= beta:
            break
    return best

def staticEval(matrix):
    a = horizontalScore(matrix) or 0 # why "or 0", if the function return != 0, the result is always this return.
    b = verticalScore(matrix) or 0 # why "or 0", if the function return != 0, the result is always this return.
    c = diagonalScore(matrix) or 0 # why "or 0", if the function return != 0, the result is always this return.

    return a + b + c

def horizontalScore(matrix):
    score = 0
    for i in range(len(matrix)):
        current = 0
        streak = 0

        for j in range(len(matrix)):
            (current, streak, score) = scoreConsecutive(matrix[i][j], current, streak, score)

        if current != 0:
            score += current * adjacentBlockScore(streak)

    return -1 * score

def verticalScore(matrix):
    score = 0

    for i in range(len(matrix)):
        current = 0
        streak = 0
        for j in range(len(matrix)):
            (current, streak, score) = scoreConsecutive(matrix[j][i], current, streak, score)

        if current != 0:
            score += current * adjacentBlockScore(streak)

    return -1 * score


def diagonalScore(matrix):
    lenght = len(matrix)
    score = 0
    res = { "d1": [], "d2": [], "d3": [], "d4": [] }

    for i in range(4, lenght):
        for key in res:
            res[key] = { "streak": 0, "current": 0, "score": 0 }

        for j in range(i):
                x = i-j
                y = j
                res["d1"] = process(matrix[x][y], res["d1"])

                x = lenght-1-j
                y = i-j
                res["d2"] = process(matrix[x][y], res["d2"])

                x = j
                y = lenght-1-i+j
                res["d3"] = process(matrix[x][y], res["d3"])

                x = lenght-1-i
                y = lenght - 1 - j
                res["d4"] = process(matrix[x][y], res["d4"])

        score += res["d1"]["score"] + res["d2"]["score"] + res["d3"]["score"] + res["d4"]["score"]
    return -1 * score # shouldn't it be one more to the left ?? (to return when "for i in range(4, lenght)" is done)


def scoreConsecutive(block, current, streak, score):
    if block != current:
        if current == 0:
            current = block
            streak = 1
        else:
            score += current * adjacentBlockScore(streak)
            current = block
            streak = 1
    else:
        if block != 0:
            streak += 1

    return (current, streak, score)

def adjacentBlockScore(count):
    scoreMatrix = [0, 2, 4, 8, 16, 32]
    try:
        return scoreMatrix[count]
    except:
        return -1

def process(block, obj): # why streak and current are swapped here?
    (a, b, c) = scoreConsecutive(block, obj["current"], obj["streak"], obj["score"])
    tock = { "current": a, "streak": b, "score": c }
    return tock

