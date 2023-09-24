# :)

from board import Board, BoardSize
import random, sys

MAX_DEPTH = 4
Infinity = sys.float_info.max
NInfinity = sys.float_info.min
move = 0


def getNextMove(board):
    matrix = board.getRawMatrix()
    (matrix, off) = board.pruneMatrix(matrix, 5)
    print(matrix)
    print(off)

    bestScore = NInfinity
    squares = getSquaresToCheck(matrix)
    y = 0
    x = 0

    for i in range(len(squares)):
        [y, x] = squares[i]
        matrix[y][x] = -1
        score = alphabeta(matrix, 0, NInfinity, Infinity, False)
        matrix[y][x] = 0

        print(f'{[x, y]} evaluated to {score}')
        if score > bestScore:
            bestScore = score
    # Recast the answer as a readable str. Cast y from int to minuscule letter aswell
    ans = f'{chr(96 + y)} {x}'
    print(f'Returning "{ans}"')
    return ans

# Get only the squares that need to be checked

def getSquaresToCheck(matrix):
    adjacent = []

    for i in range(len(matrix)):
        for j in range(len(matrix[i])):
            if not matrix[i][j] and isTouchingOccupied(matrix, i, j):
                adjacent.append([i, j])
            matrix[i][j] = 0

    return adjacent
 

def isTouchingOccupied(matrix, i, j):
    return (occupied(matrix, i+1, j) or occupied(matrix, i-1, j) or occupied(matrix, i, j+1)
        or occupied(matrix, i, j-1) or occupied(matrix, i+1, j+1) or occupied(matrix, i-1, j+1)
        or occupied(matrix, i-1, j-1) or occupied(matrix, i+1, j-1))

def occupied(matrix, x, y):
    try:
        return matrix[x][y]
    except:
        return False
    

# Alphabeta main function

def alphabeta(matrix, depth, alpha, beta, isAiTurn):

    if depth >= MAX_DEPTH:
        # arrived at max depth, get the alphabeta values from this path and return
        eval = staticEval(matrix)
        return eval

    best = NInfinity if isAiTurn else Infinity
    squares = getSquaresToCheck(matrix)
    for i in range(len(squares)):
        [y, x] = squares[i]
        matrix[y][x] = -1 if isAiTurn else 1

        score = alphabeta(matrix, depth + 1, alpha, beta, not isAiTurn)
        best = max(score, best) if isAiTurn else min(score, best)

        if isAiTurn:
            alpha = max(alpha, best)
        else:
            beta = min(beta, best)

        matrix[y][x] = 0

        if alpha >= beta:
            print(f'alpha beta pruned at {[y, x]}')
            break
    return best

def staticEval(matrix):
    a = horizontalScore(matrix) or 0
    b = verticalScore(matrix) or 0
    c = diagonalScore(matrix) or 0

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

                x = len-1-j
                y = i-j
                res["d2"] = process(matrix[x][y], res["d2"])

                x = j
                y = len-1-i+j
                res["d3"] = process(matrix[x][y], res["d3"])

                x = len-1-i
                y = len - 1 - j
                res["d4"] = process(matrix[x][y], res["d4"])

        score += res["d1"].score + res["d2"].score + res["d3"].score + res["d4"].score
        return -1 * score


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

def process(block, obj):
        return scoreConsecutive(block, obj.current, obj.streak, obj.score)

