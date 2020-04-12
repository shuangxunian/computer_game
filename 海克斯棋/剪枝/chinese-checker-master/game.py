# uncompyle6 version 3.2.3
# Python bytecode 3.5 (3350)
# Decompiled from: Python 3.5.4 |Anaconda, Inc.| (default, Feb 19 2018, 10:59:04) 
# [GCC 7.2.0]
# Embedded file name: /Users/yuegao/Downloads/code_new-1/code_new/game.py
# Compiled at: 2018-09-29 06:37:24
# Size of source mod 2**32: 1766 bytes
from board import Board
import copy

class ChineseChecker(object):

    def __init__(self, size, piece_rows):
        self.size = size
        self.piece_rows = piece_rows
        self.board = Board(self.size, self.piece_rows)

    def startState(self):
        self.board = Board(self.size, self.piece_rows)
        return (
         1, self.board)

    def isEnd(self, state, iter):
        return state[1].isEnd(iter)[0]

    def actions(self, state):
        action_list = []
        player = state[0]
        board = state[1]
        player_piece_pos_list = board.getPlayerPiecePositions(player)
        for pos in player_piece_pos_list:
            for adj_pos in board.adjacentPositions(pos):
                if board.isEmptyPosition(adj_pos):
                    action_list.append((pos, adj_pos))

        for pos in player_piece_pos_list:
            boardCopy = copy.deepcopy(board)
            boardCopy.board_status[pos] = 0
            for new_pos in boardCopy.getAllHopPositions(pos):
                if (
                 pos, new_pos) not in action_list:
                    action_list.append((pos, new_pos))

        return action_list

    def player(self, state):
        return state[0]

    def succ(self, state, action):
        player = state[0]
        board = copy.deepcopy(state[1])
        if not board.board_status[action[0]] == player:
            raise AssertionError
        if not board.board_status[action[1]] == 0:
            raise AssertionError
        board.board_status[action[1]] = player
        board.board_status[action[0]] = 0
        return (
         3 - player, board)
# okay decompiling game.pyc
