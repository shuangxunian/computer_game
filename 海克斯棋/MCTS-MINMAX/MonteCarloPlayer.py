# MonteCarloPlayer.py
"""
Monte Carlo player 蒙特卡洛树搜索玩家
"""
from __future__ import division
import datetime
from Players import *
from HexGame import *
from math import ceil
from random import choice
from math import log, sqrt
import time
import random


class Node(object):
    """
    Node object with the board, move and side.
    包含板、移动和边信息的节点
    """
    def __init__(self, board, move, side):
        """
        Constructor to initialize the Node class
        @param board - board to be used 板的定义
        @param move - move to be made 移动的定义
        @param side - side of the player 游戏玩家的边的定义
        """
        self.board = board
        self.move = move
        self.side = side

class MonteCarloPlayer(Player):
    """
    Monte carlo player class
    """
    def __init__(self, game, name=None):
        """
        Constructor to initialize the Node class
        @param game - Game
        @param name - name of the player
        """
        #set the name
        if name == None:
            name = "MonteCarlo Player"

        # initialize the base class
        # 初始化基本类
        Player.__init__(self, name)

        self.game = game
        self.plays = {}
        self.win_object = {}
        self.max_moves = 100
        self.run_time = datetime.timedelta(seconds = 2) # 设置时间的限制

    def getMove(self, board):
        """
        Gets the current move for the player
        @param board - the current board
        @returns - the new position for the player
        """
        #get the list of all the possible moves
        possiblemoves = self.game.getPossibleMoves(board)

        # Bail out early if there is no real choice to be made.
        if not possiblemoves:
            return
        if len(possiblemoves) == 1:
            return possiblemoves[0]

        games = 0

        # Run the simulations for the given number of seconds
        # 在给定的秒数内运行仿真
        begin = datetime.datetime.utcnow()
        while ((datetime.datetime.utcnow() - begin) <= self.run_time):
            self.run_simulation(Node(board, None, self.side))
            games += 1

        # Get all the move states from the possible moves
        moves_states = [(self.side, move) for move in possiblemoves]

        # identify the selected move from the move with the most possible wins.
        max_percent_wins = 0
        selected_move = None
        for side,move in moves_states:
            percent_wins = self.win_object.get((self.side, move), 0) / self.plays.get((self.side, move), 1)
            if max_percent_wins < percent_wins:
                max_percent_wins = percent_wins
                selected_move = move

        return selected_move
        # 返回最优的动作选择

    def run_simulation(self, node):
        """
        Runs the simulation fro the passed in node
        @param node - the current node 当前的节点
        """
        # set up the initial variables
        # 设置初始变量
        plays, wins = self.plays, self.win_object
        visited_states = set()
        board = node.board
        expand = True
        winner = False

        # for each possible move determine the move with the most win percentage
        # 对于每个可能的举动，确定获胜百分比最高的举动
        for move in range(self.max_moves):
            possiblemoves = self.game.getPossibleMoves(board)
            moves_states = [(node.side, move) for move in possiblemoves]
            if all(plays.get((node.side, move)) for side, move in moves_states):
                # If we have stats on all of the legal moves here, use them.
                log_total = log(
                    sum(plays[(node.side, move)] for side, move in moves_states))
                value, side, selected_move = max(
                    ((wins[(node.side, move)] / plays[(node.side, move)]) +
                     1.4 * sqrt(log_total / plays[(node.side, move)]), side, move)
                    for side, move in moves_states
                )
            else:
                # Otherwise, just make an arbitrary decision.
                side, selected_move = choice(moves_states)

            # get the board with the new position
            board = self.game.getNextBoard(board, selected_move, node.side)

            # check if the player can win from the current position
            if expand and (node.side, selected_move) not in self.plays:
                expand = False
                self.plays[(node.side, selected_move)] = 0
                self.win_object[(node.side, selected_move)] = 0

            # add to the list of visited states
            visited_states.add((node.side, selected_move))

            # check if the player has won
            if node.side == 1 and self.game.blackWins(board):
                winner = selected_move
                break
            elif node.side == -1 and self.game.redWins(board):
                winner = selected_move
                break

        # update the collection for the winning moves
        for side, move in visited_states:
            if(side, move) not in self.plays:
                continue
            self.plays[(side, move)] += 1
            if move == winner:
                self.win_object[(side, move)] += 1
