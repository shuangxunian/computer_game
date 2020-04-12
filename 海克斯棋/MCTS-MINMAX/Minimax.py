# Minimax.py
"""
Minimax with Alpha beta pruning player
"""
from Players import *
from HexGame import *
from random import choice

class Node(object):
    """
    Node used in minimax search
    """
    def __init__(self, board, move, depth, side):
        """
        Constructor to initialize the Node class
        @param board - board to be used
        @param move - move to be made
        @param depth - depth of the player
        @param side - side of the player
        """
        self.board = board
        self.move = move 
        self.depth = depth
        self.side = side

class BoundedMinimaxPlayer(Player):
    """
    Uses depth-bounded minimax to choose a move
    """
    def __init__(self, game, depthLimit, name=None):
        """
        Constructor to initialize the player class
        @param game - initialized game
        @param depthLimit - depth limit
        @param name - name of the player
        """

        #Name of the player
        if name == None:
            name = "BoundedMinimax-Depth"+str(depthLimit)

        #Initialize the base player
        Player.__init__(self, name)

        self.game = game
        self.depthLimit = depthLimit
        self.bestMove = None

    def eval(self, board):
        """
        Evaluates the current position of the players
        @param board - the current board
        @returns - the evaluation result
        """
        #Place at the centre during the initial few moves
        moves_made = self.game.size ** 2 - len(self.game.getPossibleMoves(board))
        if moves_made <= 2:
            reference = self.game.size/2
            if self.game.size % 2 == 0:
                board1 = board[int(reference), int(reference)]
                board2 = board[int(reference - 1), int(reference)]
                board3 = board[int(reference), int(reference - 1)]
                board4 = board[int(reference - 1), int(reference - 1)]
                if self.side == -1 and -1 in [board1, board2, board3, board4]:
                  return -1 * (self.game.size ** 3)
                elif self.side == 1 and 1 in [board1, board2, board3, board4]:
                  return self.game.size ** 3

            if self.game.size % 2 == 1:
                board1 = board[int(reference), int(reference)] 
                board2 = board[int(reference - 1), int(reference)] 
                board3 = board[int(reference + 1), int(reference)] 
                board4 = board[int(reference), int(reference - 1)] 
                board5 = board[int(reference), int(reference + 1)] 
                if self.side == -1 and -1 in [board1, board2, board3, board4, board5]:
                  return -1 * (self.game.size ** 3)
                elif self.side == 1 and 1 in [board1, board2, board3, board4, board5]:
                  return self.game.size ** 3

        #add the adjustment stage
        if self.side == -1:
            adjuststage = 1
        else:
            adjuststage = 0

        #Prioritze the rapid expansion during the initial few moves
        if moves_made > 2 and moves_made <= self.game.size + adjuststage:         
            return (self.game.countConnectedGap(board, self.side) + self.game.expansionScore(board, self.side))

        #check if the player wins and give a higher reward if not set higher values for the connected nodes
        if self.game.blackWins(board):
            return self.game.size ** 3
        elif self.game.redWins(board):
            return (-1 * (self.game.size ** 3))
        else:
            return (self.game.betterCountConnected(board, self.side) + self.game.expansionScore(board, self.side))
       
    def boundedMinimax(self, node):
        """
        Bounded minimax player
        @param node - current node
        """
        #return if we have already evaluated the depth
        if node.depth == self.depthLimit:
            return self.eval(node.board)
        
        #get the list of all the possible moves
        possiblemoves = self.game.getPossibleMoves(node.board)

        #end if there are no possible moves
        if len(possiblemoves) == 0:
            return self.eval(node.board)

        scores = []
        
        #recurse for each possible move
        for move in possiblemoves:
            nextBoard = self.game.getNextBoard(node.board, move, self.side)
            nextNode = Node(nextBoard, move, node.depth + 1, self.side * -1)
            scores.append(self.boundedMinimax(nextNode))
        
        #get the best move for each player
        if self.side == 1:
            if node.depth == 0:
                m = max(scores)
                ilist = [i for i, j in enumerate(scores) if j == m]
                self.bestMove = possiblemoves[choice(ilist)]
            return max(scores)
        else:
            if node.depth == 0:
                m = min(scores)
                ilist = [i for i, j in enumerate(scores) if j == m]
                self.bestMove = possiblemoves[choice(ilist)]
            return min(scores)

    def getMove(self, board):
        """
        Gets the move for the player
        @returns - the position of the best move
        """        
        self.boundedMinimax(Node(board, None, 0, self.side))
        return self.bestMove


class AlphaBetaPruningPlayer(BoundedMinimaxPlayer):
    """
    Alpha-beta prunning player
    """

    def ab_Minimax(self, node, alpha=float("-inf"), beta=float("inf")):
        """
        Alpha-beta prunned player
        @param node - current node
        @param alpha - alpha
        @param beta - beta
        """
        #continue the eval if we have already reached the depth
        if node.depth == self.depthLimit or self.game.redWins(node.board) or self.game.blackWins(node.board):
            return self.eval(node.board)
        
        #set the initial limit 
        if node.side == 1:
            limit = float("-inf")
        else:
            limit = float("inf")
        
        #get the list of all the possible moves
        possiblemoves = self.game.getPossibleMoves(node.board)

        #return if there are no possible moves
        if len(possiblemoves) == 0:
            return self.eval(node.board)

        #return the scores
        if node.depth == 0:
            scores = []
        
        bestmoves = []
        
        #calculate the list of all the best possible moves
        for moves in range(len(possiblemoves)):
            move = possiblemoves[moves]
            nextBoard = self.game.getNextBoard(node.board, move, self.side)
            nextNode = Node(nextBoard, move, node.depth + 1, self.side * -1)
            if node.side == 1:
                result = self.ab_Minimax(nextNode, alpha, beta)
                if result > limit:
                    limit = result
                    bestmoves = [moves]
                elif result == limit:
                    bestmoves.append(moves)
                if node.depth == 0:
                    scores.append(limit)

                if limit > beta:
                    return limit
                alpha = max(limit, alpha)
            else:
                result = self.ab_Minimax(nextNode, alpha, beta)
                if result < limit:
                    limit= result
                    bestmoves = [moves]
                elif result == limit:
                    bestmoves.append(moves)
                if node.depth == 0:
                    scores.append(limit)
                if limit < alpha:
                    return limit
                beta = min(limit, beta)

        if node.depth == 0:
            self.bestMove = possiblemoves[choice(bestmoves)]
                    
        return limit

    
    def getMove(self, board):
        """
        Gets the best move for the player
        @param board - the current board
        @returns - the move for the player
        """
        
        self.ab_Minimax(Node(board, None, 0, self.side), float("-inf"), float("inf"))
        return self.bestMove
