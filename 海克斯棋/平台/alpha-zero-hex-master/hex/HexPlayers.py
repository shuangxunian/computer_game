import numpy as np
from hex.HexGame import display

"""
the play method of Player give original board ,player and return action in canonical board
this is correct for play from mcts too
"""

class RandomPlayer():
    def __init__(self, game):
        self.game = game

    def play(self, board, player):
        canonicalBoard = self.game.getCanonicalForm(board, player)
        a = np.random.randint(self.game.getActionSize())
        valids = self.game.getValidMoves(canonicalBoard, 1)
        while valids[a]!=1:
            a = np.random.randint(self.game.getActionSize())
        return a


class HumanHexPlayer():
    def __init__(self, game):
        self.game = game

    def getCanonicalPosition(self, pos, player):
        if player == 1:
            return pos

        n = self.game.n
        x, y = pos
        board = np.zeros(shape=(n, n))
        board[x][y] = 1
        board = self.game.getCanonicalForm(board, -1)
        loc = np.where(board == -1)
        x, y = loc[0][0], loc[1][0]
        return (x, y)

    def play(self, board, player):
        # display(board)
        canonicalBoard = self.game.getCanonicalForm(board, player)
        valid = self.game.getValidMoves(canonicalBoard, 1)
        # for i in range(len(valid)):
        #     if not valid[i]:
        #         print(int(i/self.game.n), int(i%self.game.n))        
        while True:
            a = input()

            x, y = [int(x) for x in a.split(' ')]
            x, y = self.getCanonicalPosition((x, y), player)
            a = self.game.n * x + y
            if valid[a]:
                break
            else:
                print('Invalid')

        return a


class AlphaBetaPlayer():
    def __init__(self, game, maxDepth):
        self.game = game
        self.maxDepth = maxDepth
        self.sim_count = 0

    def alphaBeta(self, board, depth, alpha, beta, maximizingPlayer, player):
        """ 
        board here is in canonical foarm for the root player who call play of AlphaBeta
        """

        # print('depth {} alpha {} beta {} maximizingPlayer {} player {}'.format(depth, alpha, beta, maximizingPlayer, player))
        # display(board)

        self.sim_count += 1

        if depth == 0 or self.game.getGameEnded(board, 1) != 0:
            return self.game.getScore(board), None

        valids = self.game.getValidMoves(board, player)
            
        if maximizingPlayer:
            value = (float('-inf'), None)
            for action in range(self.game.getActionSize()):
                if valids[action]==0:
                    continue
                nextBoard, _ = self.game.getNextState(board, player, action)
                childValue, childAction = self.alphaBeta(nextBoard, depth - 1, alpha, beta, False, -player)
                value = max(value, (childValue, action), key=lambda t: t[0])
                alpha = max(alpha, value[0])
                if alpha >= beta:
                    break
            return value                                           
        else:
            value = (float('inf'), None)
            for action in range(self.game.getActionSize()):
                if valids[action]==0:
                    continue
                nextBoard, _ = self.game.getNextState(board, player, action)
                childValue, childAction = self.alphaBeta(nextBoard, depth - 1, alpha, beta, True, -player)
                value = min(value, (childValue, action), key=lambda t: t[0])
                beta = min(beta, value[0])
                if alpha >= beta:
                    break
            return value

    def play(self, board, player):
        canonicalBoard = self.game.getCanonicalForm(board, player)
        score, action = self.alphaBeta(canonicalBoard, self.maxDepth, float('-inf'), float('inf'), True, 1)
        return action
        