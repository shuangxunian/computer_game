"""
Python Hex Game with ABS and MCTS.
"""

import copy
import time
import sys
import math
import random
from random import shuffle
from random import randint
from matplotlib import pyplot as plt
from shapely.geometry.polygon import Polygon
from descartes import PolygonPatch
import io
iost = io.StringIO()
# Player mode
HUMAN = 0
ABS = 1
MCTS = 2

# Basic colors
BLUE = '#0000ff'
GRAY = '#999999'
YELLOW = '#ffcc33'
GREEN = '#339933'
RED = '#ff3333'
WHITE = '#ffffff'



##################################################################################################
#
#                                         游戏棋盘类
#
##################################################################################################

class GameBoard:
    def  __init__(self):
        hex_dict = {}
        for x in range(8):
            for y in range(8):
                hex_dict[(x,y)] = 2
        self.hex_record = copy.deepcopy(hex_dict)

    def __eq__(self, other):
        return self.hex_record == other.hex_record

    def print_state_list(self):
        for y in range(8):
            for x in range(8):
                print ("(" + str(x) + "," + str(y) + ") " + str(self.hex_record[(x,y)]) + " ",)
            print (" ")

    def if_taken(self, x, y):
        if self.hex_record[(x,y)] != 2:
            return True
        else:
            return False

    def taken_player(self, x, y):
        return self.hex_record[(x,y)]

    def if_neighbor(self, x1, y1, x2, y2):
        if [x1-x2, y1-y2] in [[-1,0],[-1,1],[0,-1],[0,1],[1,-1],[1,0]]:
            return True
        else:
            return False

    def get_neighbors(self, x, y):
        neighbors = []
        nbr_cords = [[1,-1],[1,0],[0,1],[-1,1],[-1,0],[0,-1]]
        for i in range(6):
            cur_x = nbr_cords[i][0] + x
            cur_y = nbr_cords[i][1] + y
            if (cur_x >= 0) and (cur_x <= 7) and (cur_y >= 0) and (cur_y <= 7):
                neighbors.append((cur_x,cur_y))
        return neighbors

    def play_red(self, x, y):
        self.hex_record[(x,y)] = 0

    def play_blue(self, x, y):
        self.hex_record[(x,y)] = 1

    def display(self):
        fig = plt.figure(1, figsize=(6.8,4), dpi=100)
        ax = fig.add_subplot(111)
        base_poly = [(0, 1), (0, 2), (1, 3), (2, 2), (2, 1), (1, 0), (0, 1)]

        # draw the hex
        for x in range(8):
            for y in range(8):
                cur_poly = []
                for z in range(7):
                    cur_point = (base_poly[z][0]+2*x+y, base_poly[z][1]+2*(8-y-1))
                    cur_poly.append(cur_point)
                ring_mixed = Polygon(cur_poly)
                if self.if_taken(x,y) == True:
                    if self.taken_player(x, y) == 0:
                        ring_patch = PolygonPatch(ring_mixed, fc=RED, alpha=0.8, zorder=3)
                        ax.add_patch(ring_patch)
                    else:
                        ring_patch = PolygonPatch(ring_mixed, fc=BLUE, alpha=0.8, zorder=3)
                        ax.add_patch(ring_patch)
                else:
                    ring_patch = PolygonPatch(ring_mixed, fc=GRAY, alpha=0.3, zorder=3)
                    ax.add_patch(ring_patch)

        # draw the bounds to indicate players
        for x in range(8-1):
            cur_poly = [(x,2*(7-x)+1),(x+1,2*(7-x)),\
                (x+1,2*(7-x)-1),(x,2*(7-x)+1)]
            ring_mixed = Polygon(cur_poly)
            ring_patch = PolygonPatch(ring_mixed, fc=BLUE, alpha=1, zorder=3)
            ax.add_patch(ring_patch)
        for x in range(7):
            cur_poly = [(16+x,2*(8-x)),(17+x,2*(7-x)),\
                (16+x,2*(7-x)+1),(16+x,2*(8-x))]
            ring_mixed = Polygon(cur_poly)
            ring_patch = PolygonPatch(ring_mixed, fc=BLUE, alpha=1, zorder=3)
            ax.add_patch(ring_patch)

        for x in range(7):
            cur_poly = [(x*2+1,17),(x*2+2,16),\
                (x*2+3,17),(x*2+1,17)]
            ring_mixed = Polygon(cur_poly)
            ring_patch = PolygonPatch(ring_mixed, fc=RED, alpha=1, zorder=3)
            ax.add_patch(ring_patch)
        for x in range(7):
            cur_poly = [(8+x*2,0),(8+x*2+1,1),\
                (8+x*2+2,0),(8+x*2,0)]
            ring_mixed = Polygon(cur_poly)
            ring_patch = PolygonPatch(ring_mixed, fc=RED, alpha=1, zorder=3)
            ax.add_patch(ring_patch)

        # add index
        for x in range(8):
            ax.text(-0.8+x, 15.2-2*x, 'y'+str(x), fontsize=10)
            ax.text(16.5+x, 15.2-2*x, 'y'+str(x), fontsize=10)
            ax.text(7.8+x*2, -0.7, 'x'+str(x), fontsize=10)
            ax.text(0.8+x*2, 17.3, 'x'+str(x), fontsize=10)
        ax.set_title('Current Hex Game Board')
        plt.xlabel('x 0 -> 7 (left -> right)')
        plt.ylabel('y 7 -> 0 (bottom -> top)')
        frame = plt.gca()
        frame.axes.get_xaxis().set_ticks([])
        frame.axes.get_yaxis().set_ticks([])
        x_range = [-1, 24]
        y_range = [-1, 18]
        ax.set_xlim(*x_range)
        ax.set_ylim(*y_range)
        ax.set_aspect(0.8)

        plt.show(block=False)


def display_plot(board):
        fig = plt.figure(1, figsize=(6.8,4), dpi=100)
        ax = fig.add_subplot(111)
        base_poly = [(0, 1), (0, 2), (1, 3), (2, 2), (2, 1), (1, 0), (0, 1)]

        # draw the hex
        for x in range(8):
            for y in range(8):
                cur_poly = []
                for z in range(7):
                    cur_point = (base_poly[z][0]+2*x+y, base_poly[z][1]+2*(8-y-1))
                    cur_poly.append(cur_point)
                ring_mixed = Polygon(cur_poly)
                if board[x][y] < 2:
                    if board[x][y] == 0:
                        ring_patch = PolygonPatch(ring_mixed, fc=RED, alpha=0.8, zorder=3)
                        ax.add_patch(ring_patch)
                    else:
                        ring_patch = PolygonPatch(ring_mixed, fc=BLUE, alpha=0.8, zorder=3)
                        ax.add_patch(ring_patch)
                else:
                    if board[x][y] == 2:
                        ring_patch = PolygonPatch(ring_mixed, fc=GRAY, alpha=0.3, zorder=3)
                        ax.add_patch(ring_patch)
                    elif board[x][y] == 3:
                        ring_patch = PolygonPatch(ring_mixed, fc=RED, alpha=0.1, zorder=3)
                        ax.add_patch(ring_patch)
                    else:
                        ring_patch = PolygonPatch(ring_mixed, fc=BLUE, alpha=0.1, zorder=3)
                        ax.add_patch(ring_patch)

        # draw the bounds to indicate players
        for x in range(8-1):
            cur_poly = [(x,2*(7-x)+1),(x+1,2*(7-x)),\
                (x+1,2*(7-x)-1),(x,2*(7-x)+1)]
            ring_mixed = Polygon(cur_poly)
            ring_patch = PolygonPatch(ring_mixed, fc=BLUE, alpha=1, zorder=3)
            ax.add_patch(ring_patch)
        for x in range(7):
            cur_poly = [(16+x,2*(8-x)),(17+x,2*(7-x)),\
                (16+x,2*(7-x)+1),(16+x,2*(8-x))]
            ring_mixed = Polygon(cur_poly)
            ring_patch = PolygonPatch(ring_mixed, fc=BLUE, alpha=1, zorder=3)
            ax.add_patch(ring_patch)

        for x in range(7):
            cur_poly = [(x*2+1,17),(x*2+2,16),\
                (x*2+3,17),(x*2+1,17)]
            ring_mixed = Polygon(cur_poly)
            ring_patch = PolygonPatch(ring_mixed, fc=RED, alpha=1, zorder=3)
            ax.add_patch(ring_patch)
        for x in range(7):
            cur_poly = [(8+x*2,0),(8+x*2+1,1),\
                (8+x*2+2,0),(8+x*2,0)]
            ring_mixed = Polygon(cur_poly)
            ring_patch = PolygonPatch(ring_mixed, fc=RED, alpha=1, zorder=3)
            ax.add_patch(ring_patch)

        # add index
        for x in range(8):
            ax.text(-0.8+x, 15.2-2*x, 'y'+str(x), fontsize=10)
            ax.text(16.5+x, 15.2-2*x, 'y'+str(x), fontsize=10)
            ax.text(7.8+x*2, -0.7, 'x'+str(x), fontsize=10)
            ax.text(0.8+x*2, 17.3, 'x'+str(x), fontsize=10)
        ax.set_title('Current Hex Game Board')
        plt.xlabel('x 0 -> 7 (left -> right)')
        plt.ylabel('y 7 -> 0 (bottom -> top)')
        frame = plt.gca()
        frame.axes.get_xaxis().set_ticks([])
        frame.axes.get_yaxis().set_ticks([])
        x_range = [-1, 24]
        y_range = [-1, 18]
        ax.set_xlim(*x_range)
        ax.set_ylim(*y_range)
        ax.set_aspect(0.8)

        plt.show()



##################################################################################################
#
#                                       游戏树节点
#
##################################################################################################

######################################################################
#                             Union Find
######################################################################

class UnionFind:
    """ 联合查找游戏结构 """
    # a disjoint set (a large list) containing small lists of cords
    def __init__(self):
        self.disjoint_set = []

    def makeSet(self, cord):
        self.disjoint_set.append([cord])

    def union(self):
        last_node = self.disjoint_set[-1] # we assume there is a single element
        merge_index = {-1}
        for i in range(len(self.disjoint_set)-1):
            for cords in self.disjoint_set[i]:
                if if_connected(cords, last_node[0]):
                    merge_index.add(i)
                    break
        if len(merge_index) == 1:
            return
        else:
            f_index = -1
            for e in merge_index:
                if e != f_index:
                    f_index = e
                    break
            temp_set = []
            temp_disjoint_set = []
            for index in merge_index:
                temp_set += self.disjoint_set[index]
            temp_disjoint_set.append(temp_set)
            for x in range(len(self.disjoint_set)-1):
                if x not in merge_index:
                    temp_disjoint_set.append(self.disjoint_set[x])
            self.disjoint_set = temp_disjoint_set

    def union_set(self, cord):
        self.makeSet(cord)
        self.union()

    def findSet(self, cord):
        for cords in self.disjoint_set:
            if cord in cords:
                return cords
        return []

    def size(self):
        return len(self.disjoint_set)

    def show(self):
        for sets in self.disjoint_set:
            for cord in sets:
                print (str(cord) + " ",)
            print ("")


######################################################################
#                         虚拟连接
######################################################################

def if_connected(cord1, cord2):
    if [cord1[0]-cord2[0], cord1[1]-cord2[1]] in [[-1,0],[-1,1],[0,-1],[0,1],[1,-1],[1,0]]:
        return True
    else:
        return False

def get_virtual_neighbor_cords(cord, player, game_board):
    opponent = 1 - player
    virtual_neighbors = []
    nbr_cords = [[1,-1],[1,0],[0,1],[-1,1],[-1,0],[0,-1]]
    nbr_cords_n = [[1,0],[0,1],[-1,1],[-1,0],[0,-1],[1,-1]]
    vc_cords = [[2,-1],[1,1],[-1,2],[-2,1],[-1,-1],[1,-2]]
    for x in range(6):
        cur_x = nbr_cords[x][0] + cord[0]
        cur_y = nbr_cords[x][1] + cord[1]
        cur_x_n = nbr_cords_n[x][0] + cord[0]
        cur_y_n = nbr_cords_n[x][1] + cord[1]
        cur_x_v = vc_cords[x][0] + cord[0]
        cur_y_v = vc_cords[x][1] + cord[1]
        if (cur_x >= 0) and (cur_x <= 7) and (cur_y >= 0) and (cur_y <= 7) and \
            (cur_x_n >= 0) and (cur_x_n <= 7) and (cur_y_n >= 0) and (cur_y_n <= 7) and \
            (cur_x_v >= 0) and (cur_x_v <= 7) and (cur_y_v >= 0) and (cur_y_v <= 7) and \
            (game_board.hex_record[(cur_x,cur_y)] == 2) and \
            (game_board.hex_record[(cur_x_n,cur_y_n)] == 2) and \
            (game_board.hex_record[(cur_x_v,cur_y_v)] == player):
            virtual_neighbors.append((cur_x_n,cur_y_n))
            virtual_neighbors.append((cur_x,cur_y))
    return virtual_neighbors

def fill_in_virtual_connection(player, game_board):
    v_pos = set()
    for x in range(8):
        for y in range(8):
            if game_board.hex_record[(x,y)] == player:
                virtual_neighbors = get_virtual_neighbor_cords((x,y), player, game_board)
                v_pos = v_pos | set(virtual_neighbors)

    # get virtual edge hex
    opponent = 1 - player

    if player == 0:  # red vertical
        y0 = [0,0,0,0,0,0,0,0]
        for x in range(7):
            if (game_board.hex_record[(x,0)] == 2) and \
                (game_board.hex_record[(x+1,0)] == 2) and \
                (game_board.hex_record[(x,1)] == player):
                y0[x] = 1
                y0[x+1] = 1
        for x in range(8):
            if y0[x] == 1:
                v_pos.add((x,0))

        y7 = [0,0,0,0,0,0,0,0]
        for x in range(7):
            if (game_board.hex_record[(x,7)] == 2) and \
                (game_board.hex_record[(x+1,7)] == 2) and \
                (game_board.hex_record[(x+1,6)] == player):
                y7[x] = 1
                y7[x+1] = 1
        for x in range(8):
            if y7[x] == 1:
                v_pos.add((x,7))

    else:  # blue horizontal
        x0 = [0,0,0,0,0,0,0,0]
        for y in range(7):
            if (game_board.hex_record[(0,y)] == 2) and \
                (game_board.hex_record[(0,y+1)] == 2) and \
                (game_board.hex_record[(1,y)] == player):
                x0[y] = 1
                x0[y+1] = 1
        for y in range(8):
            if x0[y] == 1:
                v_pos.add((0,y))

        x7 = [0,0,0,0,0,0,0,0]
        for y in range(7):
            if (game_board.hex_record[(7,y)] == 2) and \
                (game_board.hex_record[(7,y+1)] == 2) and \
                (game_board.hex_record[(6,y+1)] == player):
                x7[y] = 1
                x7[y+1] = 1
        for y in range(8):
            if x7[y] == 1:
                v_pos.add((7,y))

    return list(v_pos)

def update_sets(player, game_board, d_set, o_set):
    opponent = 1 - player
    v_pos = fill_in_virtual_connection(player, game_board)
    v_pos_o = fill_in_virtual_connection(opponent, game_board)
    d_set_s = copy.deepcopy(d_set)
    o_set_s = copy.deepcopy(o_set)
    for pos in v_pos:
        d_set_s.union_set(pos)
    for pos in v_pos_o:
        o_set_s.union_set(pos)
    return d_set_s, o_set_s


######################################################################
#                          游戏树节点
######################################################################

class Game_tree_node:
    """class for the game tree node"""
    def __init__(self, min_max, player, d_set=UnionFind(), o_set=UnionFind(), values=None, children=None, board=None, visit=1, parent=None, reward=0, left_pos=64, if_win=None):
        self.player = player  # 0 for red and 1 for blue
        self.min_max = min_max  # 0 for min and 1 for max
        self.d_set = d_set  # own connecting positions
        self.o_set = o_set  # opponent connecting positions
        self.board = board  # game board
        self.values = values  # utility values of the node
        self.children = children  # current node's children
        self.left_pos = left_pos  # left blank postions
        self.if_win = if_win  # if the current node is win or lose, or None

        # MCTS
        self.visit = visit
        self.parent = parent
        self.reward = reward

    def add_single_child(self, c_board, d_set_s, o_set_s):
        """ add a single successor with its board to the children list """
        player_s = 1 - self.player
        min_max_s = 1 - self.min_max
        n_left_pos = self.left_pos - 1
        successor = Game_tree_node(min_max_s,player_s,d_set=d_set_s,o_set=o_set_s,board=c_board,parent=self,left_pos=n_left_pos)
        if self.children == None:
            self.children = [successor]
        else:
            self.children.append(successor)

    def next_board(self):
        """ for the current player, get the next state randomly from the state space """
        rand_x = random.choice([0,1,2,3,4,5,6,7])
        rand_y = random.choice([0,1,2,3,4,5,6,7])
        while self.board.hex_record[(rand_x,rand_y)] != 2:
            rand_x = random.choice([0,1,2,3,4,5,6,7])
            rand_y = random.choice([0,1,2,3,4,5,6,7])

        board_s = copy.deepcopy(self.board)
        board_s.hex_record[(rand_x,rand_y)] = self.player
        d_set_s = copy.deepcopy(self.o_set)
        o_set_s = copy.deepcopy(self.d_set)
        o_set_s.union_set((rand_x,rand_y))
        return board_s, d_set_s, o_set_s

    def next_board_of(self):
        """ for the current player, get the next best state from the state space """
        best_x, best_y = self.best_attack_cord()  # defensive
        board_s = copy.deepcopy(self.board)
        board_s.hex_record[(best_x,best_y)] = self.player
        d_set_s = copy.deepcopy(self.o_set)
        o_set_s = copy.deepcopy(self.d_set)
        o_set_s.union_set((best_x,best_y))
        return board_s, d_set_s, o_set_s

    def next_board_df(self):
        """ for the current player, get the next best state from the state space """
        best_x, best_y = self.best_defense_cord()  # defensive
        board_s = copy.deepcopy(self.board)
        board_s.hex_record[(best_x,best_y)] = self.player
        d_set_s = copy.deepcopy(self.o_set)
        o_set_s = copy.deepcopy(self.d_set)
        o_set_s.union_set((best_x,best_y))
        return board_s, d_set_s, o_set_s

    def best_attack_cord(self):
        player = self.player
        for cords in self.d_set.disjoint_set:
            for cord in cords:
                bridge_cord = self.get_bridge(cord[0], cord[1], player)
                if bridge_cord != (-1,-1):
                    return bridge_cord
        return random_play(self)

    def best_defense_cord(self):
        player = self.player
        opponent = 1 - player
        for cords in self.o_set.disjoint_set:  # opponent connected pieces
            for cord in cords:
                bridge_cord = self.get_bridge(cord[0], cord[1], opponent)
                if bridge_cord != (-1,-1):
                    return bridge_cord
        return random_play(self)

    def get_successor_offensive(self):
        """ get the next state of the game, player exchanged """
        board_s, d_set_s, o_set_s = self.next_board_of()  # offensive
        player_s = 1 - self.player
        min_max_s = 1 - self.min_max
        n_left_pos = self.left_pos - 1
        successor = Game_tree_node(min_max_s,player_s,d_set=d_set_s,o_set=o_set_s,board=board_s,parent=self,left_pos=n_left_pos)
        return successor

    def get_successor_deffensive(self):
        """ get the next state of the game, player exchanged """
        board_s, d_set_s, o_set_s = self.next_board_df()  # defensive
        player_s = 1 - self.player
        min_max_s = 1 - self.min_max
        n_left_pos = self.left_pos - 1
        successor = Game_tree_node(min_max_s,player_s,d_set=d_set_s,o_set=o_set_s,board=board_s,parent=self,left_pos=n_left_pos)
        return successor

    def get_successor_rand(self):
        """ get the next state of the game, player exchanged """
        board_s, d_set_s, o_set_s = self.next_board()  # random
        player_s = 1 - self.player
        min_max_s = 1 - self.min_max
        n_left_pos = self.left_pos - 1
        successor = Game_tree_node(min_max_s,player_s,d_set=d_set_s,o_set=o_set_s,board=board_s,parent=self,left_pos=n_left_pos)
        return successor

    def get_bridge(self, x, y, c_player):
        b_record = self.board.hex_record
        if c_player == 0:  # red vertical
            if y >= 2:
                if x <= 6:
                    if (b_record[(x,y-1)] != 1) and (b_record[(x+1,y-1)] != 1):
                        if b_record[(x+1,y-2)] == 2:
                            return (x+1,y-2)
            if y <= 5:
                if x >= 1:
                    if (b_record[(x-1,y+1)] != 1) and (b_record[(x,y+1)] != 1):
                        if b_record[(x-1,y+2)] == 2:
                            return (x-1,y+2)
            return (-1,-1)
        else:  # blue horizontal
            # left
            if x >= 2:
                if y <= 6:
                    if (b_record[(x-1,y)] != 0) and (b_record[(x-1,y+1)] != 0):
                        if b_record[(x-2,y+1)] == 2:
                            return (x-2,y+1)
            # right
            if x <= 5:
                if y >= 1:
                    if (b_record[(x+1,y-1)] != 0) and (b_record[(x+1,y)] != 0):
                        if b_record[(x+2,y-1)] == 2:
                            return (x+2,y-1)
            # left
            if x >= 1:
                if y >= 1:
                    if (b_record[(x,y-1)] != 0) and (b_record[(x-1,y)] != 0):
                        if b_record[(x-1,y-1)] == 2:
                            return (x-1,y-1)
            # right
            if x <= 6:
                if y <= 6:
                    if (b_record[(x,y+1)] != 0) and (b_record[(x+1,y)] != 0):
                        if b_record[(x+1,y+1)] == 2:
                            return (x+1,y+1)
            return (-1,-1)

    def update(self, reward):
        """ update the current game tree node for rewards and visit times """
        self.reward += reward
        self.visit += 1

    def fully_expanded(self):
        """ check if a game tree node's successors are all exapnded """
        if self.children == None:
            return False
        left_c = self.left_pos
        return left_c == len(self.children)

    def get_successors(self):
        """ we assume the current player should play the next move """
        successors = []
        player_s = 1 - self.player
        min_max_s = 1 - self.min_max

        for x in range(8):
            for y in range(8):
                if self.board.hex_record[(x,y)] == 2:
                    board_s = copy.deepcopy(self.board)
                    board_s.hex_record[(x,y)] = self.player
                    d_set_s = copy.deepcopy(self.o_set)
                    o_set_s = copy.deepcopy(self.d_set)
                    o_set_s.union_set((x,y))
                    n_left_pos = self.left_pos - 1
                    successor = Game_tree_node(min_max_s,player_s,d_set=d_set_s,o_set=o_set_s,board=board_s,parent=self,left_pos=n_left_pos)
                    successors.append(successor)
        shuffle(successors)
        self.children = successors
        return successors

    def DFS(self, x, y, player):
        """ DFS for as a helper funciton for checking the winning node """
        my_stack = Stack()
        ref = [[0]*8 for i in range(8)]
        my_stack.push((x,y))
        while my_stack.isEmpty() == False:
            cur_x, cur_y = my_stack.pop()
            if ref[cur_x][cur_y] == 0:
                ref[cur_x][cur_y] = 1  # visited
                cur_neighbors = self.board.get_neighbors(cur_x,cur_y)
                for neighbor in cur_neighbors:
                    if self.board.hex_record[(neighbor.cord[0],neighbor.cord[1])] == player:
                        my_stack.push((neighbor.cord[0],neighbor.cord[1]))
                        if player == 0:  # red, vertical:
                            if neighbor.cord[1] == 7:
                                return True
                        else:
                            if neighbor.cord[0] == 7:
                                return True
        return False

    def check_win_DFS(self):
        """ check if the player wins """
        if self.player == 0:  # red, vertical
            for x in range(8):
                if self.DFS(x, 0, self.player):
                    return 1
            for y in range(8):
                if self.DFS(0, y, 1-self.player):
                    return -1
            return 0
        else:  # blue, horizontal
            for y in range(8):
                if self.DFS(0, y, self.player):
                    return 1
            for x in range(8):
                if self.DFS(x, 0, 1-self.player):
                    return -1
            return 0

    def check_win(self):
        if self.if_win != None:
            return self.if_win

        y0 = False
        y1 = False
        x0 = False
        x1 = False
        id1 = -1
        id2 = -1
        c_d_set, c_o_set = update_sets(self.player, self.board, self.d_set, self.o_set)

        # print "d_set: "
        # c_d_set.show()
        # print "o_set: "
        # c_o_set.show()

        if self.player == 0:  # red, vertical
            id1 = 1
            id2 = 0
        else:  # blue, horizontal
            id1 = 0
            id2 = 1

        if c_d_set != None:
            for set_i in c_d_set.disjoint_set:
                if len(set_i) > 6:
                    for i in range(len(set_i)):
                        if set_i[i][id1] == 0:
                            y0 = True
                        elif set_i[i][id1] == 7:
                            y1 = True
                        else:
                            pass
                    if (y0 == True) and (y1 == True):
                        self.if_win = 1
                        return 1
                    y0 = False
                    y1 = False
        if c_o_set != None:
            for set_i in c_o_set.disjoint_set:
                if len(set_i) > 6:
                    for i in range(len(set_i)):
                        if set_i[i][id2] == 0:
                            x0 = True
                        elif set_i[i][id2] == 7:
                            x1 = True
                        else:
                            pass
                    if (x0 == True) and (x1 == True):
                        self.if_win = -1
                        return -1
                    x0 = False
                    x1 = False
        self.if_win = 0
        return 0

    def get_move(self, child):
        """ get the actual move set by comparing the node with its child """
        for x in range(8):
            for y in range(8):
                if child.board.hex_record[(x,y)] != self.board.hex_record[(x,y)]:
                    return (x,y)
        return (-1,-1)


class Stack:
    """ Stack data structure """
    def __init__(self):
        self.items = []

    def isEmpty(self):
        return self.items == []

    def push(self, item):
        self.items.append(item)

    def pop(self):
        return self.items.pop()

    def peek(self):
        return self.items[len(self.items)-1]

    def size(self):
        return len(self.items)



##################################################################################################
#
#                                   游戏玩家类与AI
#
##################################################################################################

class Player:
    def  __init__(self, mode=HUMAN, game_tree_node=None, board=None, d_set=None, o_set=None):
        self.mode = mode
        self.game_tree_node = game_tree_node
        self.board = board
        self.d_set = d_set
        self.o_set = o_set

    def play(self, board):
        if self.mode == HUMAN:
            input_x = '0'
            while True:
                input_x = input("Players'x cord : ")
                try:
                    temp_x = int(input_x)
                    if (temp_x < 0) or (temp_x >= 8):
                        int('1.1')
                    break
                except ValueError:
                    print ("Not a valid integer or not in board's range, please type the x cord again: ")
            input_y = '0'
            while True:
                input_y = input("Players'y cord : ")
                try:
                    temp_y = int(input_y)
                    if (temp_y < 0) or (temp_y >= 8):
                        int('1.1')
                    break
                except ValueError:
                    print ("Not a valid integer or not in board's range, please type the y cord again: ")
            self.d_set.union_set((int(input_x), int(input_y)))
            return (input_x, input_y)

        elif self.mode == ABS:
            cur_node = self.game_tree_node
            temp_block = input("Type 'ENTER' to start this player ")
            print ("Current node tries to: " + str(cur_node.min_max))
            num_evl, val = minimax_ab(cur_node, 3, -sys.maxsize, sys.maxsize, heuristic3)  # could change to different heuristics
            game_tree_size = len(cur_node.children) * (len(cur_node.children)-1) * (len(cur_node.children)-2)
            # game_tree_size = len(cur_node.children) * (len(cur_node.children)-1)

            print ("Game tree size: " + str(game_tree_size))
            print ("Number of nodes evaluated: " + str(num_evl))
            print ("Evaluation saved by pruning: " + str((game_tree_size-num_evl)*1.0/game_tree_size * 100) + "%")
            val_list = cur_node.values
            max_val_index = [i for i, x in enumerate(val_list) if x == max(val_list)]
            random_child_index = randint(0,len(max_val_index)-1)
            index_max = max_val_index[random_child_index]
            print ("node vals: " + str(val_list))
            next_node = cur_node.children[index_max]
            temp_block = input("Type 'ENTER' to continue the AI process... ")
            print ("AI's (MiniMax) best move: " + str(cur_node.get_move(next_node)))
            self.d_set.union_set(cur_node.get_move(next_node))
            return cur_node.get_move(next_node)
            
        else:
            cur_node = self.game_tree_node
            cur_node.children = None
            temp_block = input("Type 'ENTER' to start this player ")
            best_successor = MCTS(cur_node, 60000)
            temp_block = input("Type 'ENTER' to continue the AI process... ")
            print ("best move possible: " + str(cur_node.get_move(best_successor)))
            self.d_set.union_set(cur_node.get_move(best_successor))
            return cur_node.get_move(best_successor)


######################################################################
#                           极大极小搜索
######################################################################

def minimax_ab(node, depth, min_val, max_val, evaluate):
    """ general minimax search algorithm that is limited to a max depth 
        with Alpha-Beta pruning
        we assume the minimax player is trying to maximize its own utility
    """
    num_evl = 0
    if node.check_win() == 1:
        num_evl += 1
        if node.min_max == 1:
            return num_evl, 100
        else:
            return num_evl, -100
    if depth == 0:
        num_evl += 1
        return num_evl, evaluate(node,node.min_max)
    node.values = []
    node.children = node.get_successors()

    if node.min_max == 1:  # max node
        val = min_val
        for child in node.children:
            evl, val_p = minimax_ab(child, depth-1, val, max_val, evaluate)
            num_evl += evl
            node.values.append(val_p)
            if val_p > val:
                val = val_p
            if val > max_val:
                return num_evl, max_val
        return num_evl, val
    if node.min_max == 0:  # min node
        val = max_val
        for child in node.children:
            evl, val_p = minimax_ab(child, depth-1, min_val, val, evaluate)
            num_evl += evl
            node.values.append(val_p)
            if val_p < val:
                val = val_p
            if val < min_val:
               return num_evl, min_val
        return num_evl, val


def minimax(node, depth, min_val, max_val, evaluate):
    """ general minimax search algorithm that is limited to a max depth 
        we suppose the minimax player is trying to maximize its own utility
    """
    num_evl = 0
    if node.check_win() == 1:
        num_evl += 1
        if node.min_max == 1:
            return num_evl, 100
        else:
            return num_evl, -100
    if depth == 0:
        num_evl += 1
        return num_evl, evaluate(node,node.min_max)
    node.values = []
    node.children = node.get_successors()

    if node.min_max == 1:  # max node
        val = min_val
        for child in node.children:
            evl, val_p = minimax(child, depth-1, min_val, max_val, evaluate)
            num_evl += evl
            node.values.append(val_p)
            if val_p > val:
                val = val_p
        return num_evl, val
    if node.min_max == 0:  # min node
        val = max_val
        for child in node.children:
            evl, val_p = minimax(child, depth-1, min_val, max_val, evaluate)
            num_evl += evl
            node.values.append(val_p)
            if val_p < val:
                val = val_p
        return num_evl, val


def minimax_ab_quiet(node, depth, min_val, max_val, evaluate):
    """ general minimax search algorithm that is limited to a max depth 
        with Alpha-Beta pruning
        we assume the minimax player is trying to maximize its own utility
    """
    num_evl = 0
    if node.check_win() == 1:
        num_evl += 1
        if node.min_max == 1:
            return num_evl, 100
        else:
            return num_evl, -100
    if depth == 0:
        num_evl += 1
        if quiet(node,node.min_max):
            return num_evl, evaluate(node,node.min_max)
        else:
            return minimax_ab(node, 2, min_val, max_val, evaluate)
    node.values = []
    node.children = node.get_successors()

    if node.min_max == 1:  # max node
        val = min_val
        for child in node.children:
            evl, val_p = minimax_ab_quiet(child, depth-1, val, max_val, evaluate)
            num_evl += evl
            node.values.append(val_p)
            if val_p > val:
                val = val_p
            if val > max_val:
                return num_evl, max_val
        return num_evl, val
    if node.min_max == 0:  # min node
        val = max_val
        for child in node.children:
            evl, val_p = minimax_ab_quiet(child, depth-1, min_val, val, evaluate)
            num_evl += evl
            node.values.append(val_p)
            if val_p < val:
                val = val_p
            if val < min_val:
               return num_evl, min_val
        return num_evl, val


def quiet(node, min_max):
    game_board = node.board
    player = node.player
    y0 = 8
    y1 = -1
    x0 = 8
    x1 = -1
    id1 = -1
    id2 = -1
    d_span = 0
    o_span = 0

    c_d_set, c_o_set = update_sets(node.player, node.board, node.d_set, node.o_set)

    if node.player == 0:  # red, vertical
        id1 = 1
        id2 = 0
    else:  # blue, horizontal
        id1 = 0
        id2 = 1

    if c_o_set != None:
        for set_i in c_o_set.disjoint_set:
            for i in range(len(set_i)):
                if set_i[i][id2] > x1:
                    x1 = set_i[i][id2]
                if set_i[i][id2] < x0:
                    x0 = set_i[i][id2]
            if x1-x0 > o_span:
                o_span = x1-x0
                if o_span >= 5:
                    if min_max == 1:
                        return False
            x0 = 8
            x1 = -1

    if c_d_set != None:
        for set_i in c_d_set.disjoint_set:
            for i in range(len(set_i)):
                if set_i[i][id1] > y1:
                    y1 = set_i[i][id1]
                if set_i[i][id1] < y0:
                    y0 = set_i[i][id1]
            if y1-y0 > d_span:
                d_span = y1-y0
                if d_span >= 5:
                    if min_max != 1:
                        return False
            y0 = 8
            y1 = -1

    return True


######################################################################
#                             MCTS
######################################################################

CP = 2 / math.sqrt(2.0) # higher CP rewards more on exploration

def MCTS(root, bound):
    print ("Do simulation for " + str(bound) + " times...")
    root_player = root.player
    start = time.time()
    # d_depth = -1
    num_nodes = 0
    for x in range(bound):
        indict, front = tree_policy(root, root_player)  # find child to expand
        if indict == 1:
            t_depth = back_up(front, -1.8)
            # if t_depth > d_depth:
            #     d_depth = t_depth
        elif indict == 2:
            t_depth = back_up(front, 1.6)
            # if t_depth > d_depth:
            #     d_depth = t_depth
        else:
            num_nodes += 1
            reward = default_policy(front, root_player)  # do simulation on expansion
            t_depth = back_up(front, reward)  # backpropagate the rewards
            # if t_depth > d_depth:
            #     d_depth = t_depth
        
    end = time.time()
    print (">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>")
    print ("Finished in " + str((end - start)) + " sec")
    # print "Max expanding depth: " + str(d_depth)
    # print "Number of nodes expanded: " + str(num_nodes)
    print ("Time each child being visited: ")
    for child in root.children:
        print (str(child.visit) + " ",)
    print ("")
    # print_children_reward(root)
    # print len(root.children)
    return best_child(root, 0)


def tree_policy(node, root_player):
    """ find a child to expand """
    danger = 0
    while node.check_win() == 0:
        if node.fully_expanded() == False:  # not fully expanded, then randomly choose one
            return 0, expand(node)
        else:
            danger += 1
            node = best_child(node, CP)  # otherwise, choose one with the best reward
    # must play pos
    if node.check_win() == 1:
        if node.player != root_player:
            return 1, node
    else:
        if node.player == root_player:
            return 1, node
    return 2, node


def expand(node):
    """ randomly choose a child to expand """
    tried_board = [child.board for child in node.children] if node.children != None else []
    # randomly pick a successor node
    new_board, d_set_s, o_set_s = node.next_board()
    while new_board in tried_board:
        new_board, d_set_s, o_set_s = node.next_board()
    node.add_single_child(new_board,d_set_s,o_set_s)
    return node.children[-1]


def best_child(node, scalar):
    """ get the best child availble for a given node, UCT """
    best_score = -sys.maxsize
    best_children = []

    for child in node.children:
        exploit = child.reward * 1.0 / child.visit
        explore = math.sqrt(2.0 * math.log(node.visit) / float(child.visit))
        score = exploit + scalar * explore

        if score == best_score:
            best_children.append(child)
        if score > best_score:
            best_children = [child]
            best_score = score

    if len(best_children)==0:
        print ("Failed, no children available...")

    # DEBUG
    # print "Reward: " + str(best_score) + ": [ ",
    # for b_child in best_children:
    #     print str(node.get_move(b_child)) + " ", 
    # print "]"

    return random.choice(best_children)


def default_policy(node, root_player):
    """ do simulation and return a reward value based on the single result """
    cur_player = node.player
    result = random_simulation(node, root_player)
    return result


def back_up(node, reward):
    """ update the time visited and reward """
    depth = 0
    while node != None:
        depth += 1
        node.visit += 1
        node.reward += reward
        node = node.parent
    return depth


def print_children_reward(node):
    for child in node.children:
        print (str(node.get_move(child)) + ":" + str(child.reward) + " ",)
    print ("")


def random_simulation(node, root_player):
    player = node.player
    c_board = {}
    left_cords = []
    for x in range(8):
        for y in range(8):
            plyr = node.board.hex_record[(x,y)]
            c_board[(x,y)] = plyr
            if plyr == 2:
                left_cords.append((x,y))
    for x in range(node.left_pos):
        rand_x, rand_y = random.choice(left_cords)
        del left_cords[left_cords.index((rand_x, rand_y))]
        c_board[(rand_x,rand_y)] = player
        player = 1 - player
    return check_win_all_DFS(c_board, root_player)


def DFS_board(board, x, y, player):
    """ DFS for as a helper funciton for checking the winning node """
    my_stack = Stack()
    ref = [[0]*8 for i in range(8)]
    my_stack.push((x,y))
    while my_stack.isEmpty() == False:
        cur_x, cur_y = my_stack.pop()
        if ref[cur_x][cur_y] == 0:
            ref[cur_x][cur_y] = 1  # visited
            cur_neighbors = get_board_neighbors(board, cur_x,cur_y)
            for (nx,ny) in cur_neighbors:
                if board[(nx,ny)] == player:
                    my_stack.push((nx,ny))
                    if player == 0:  # red, vertical:
                        if ny == 7:
                            return True
                    else:
                        if nx == 7:
                            return True
    return False


def check_win_all_DFS(board, player):
    """ check if the player wins """
    if player == 0:  # red, vertical
        for x in range(8):
            if DFS_board(board, x, 0, player):
                return 1
        return 0
    else:  # blue, horizontal
        for y in range(8):
            if DFS_board(board, 0, y, player):
                return 1
        return 0


def get_board_neighbors(board, x, y):
        neighbors = []
        nhbr_vctr = [[-1,0],[-1,1],[0,-1],[0,1],[1,-1],[1,0]]
        for i in range(6):
            cur_x = x + nhbr_vctr[i][0]
            cur_y = y + nhbr_vctr[i][1]
            if cur_x >= 0 and cur_x < 8 and\
                cur_y >= 0 and cur_y < 8:
                neighbors.append((cur_x,cur_y))
        return neighbors



######################################################################
#                              随机
######################################################################

def random_play(node):
    cur_board = node.board
    rand_x = random.choice([0,1,2,3,4,5,6,7])
    rand_y = random.choice([0,1,2,3,4,5,6,7])
    while cur_board.hex_record[(rand_x,rand_y)] != 2:
        rand_x = random.choice([0,1,2,3,4,5,6,7])
        rand_y = random.choice([0,1,2,3,4,5,6,7])
    return (rand_x, rand_y)



##################################################################################################
#
#                                          海克斯棋
#
##################################################################################################

def heuristic1(node, min_max=1):
    """ number of connected pieces of the player minus the number of connected pieces of the opponent """
    # red: 0
    # blue: 1
    if node.check_win() == 1:
        return 65
    connected_pieces = 0
    pieces_list = []
    o_pieces_list = []
    game_board = node.board
    player = node.player
    opponent = 1 - player
    for x in range(8):
        for y in range(8):
            if game_board.hex_record[(x,y)] == player:
                pieces_list.append((x,y))
            elif game_board.hex_record[(x,y)] == opponent:
                o_pieces_list.append((x,y))
            else:
                pass

    connected_pieces = len(pieces_list)
    o_connected_pieces = len(o_pieces_list)

    for x in range(len(pieces_list)):
        cur_x,xur_y = pieces_list[x]
        neighbors = game_board.get_neighbors(cur_x,xur_y)
        if_connected = False
        for neighbor in neighbors:
            if (neighbor.cord[0],neighbor.cord[1]) in pieces_list:
                if_connected = True
                break
        if if_connected == False:
            connected_pieces -= 1

    for x in range(len(o_pieces_list)):
        cur_x,xur_y = o_pieces_list[x]
        neighbors = game_board.get_neighbors(cur_x,xur_y)
        if_connected = False
        for neighbor in neighbors:
            if (neighbor.cord[0],neighbor.cord[1]) in o_pieces_list:
                if_connected = True
                break
        if if_connected == False:
            o_connected_pieces -= 1

    if min_max == 1:
        return connected_pieces-o_connected_pieces
    else:
        return o_connected_pieces-connected_pieces


def heuristic2(node, min_max=1):
    """ calculate scores based on bridge patterns in the board """
    # red: 0
    # blue: 1
    game_board = node.board
    player = node.player
    opponent = 1 - player
    p_patterns = 0
    o_patterns = 0

    for x in range(8):
        for y in range(8):
            if game_board.hex_record[(x,y)] == player:
                p_patterns += check_bridge(game_board, player, x, y)
            elif game_board.hex_record[(x,y)] == opponent:
                o_patterns += check_bridge(game_board, opponent, x, y)
            else:
                pass

    if min_max == 1:
        return p_patterns-o_patterns
    else:
        return o_patterns-p_patterns


def check_bridge(board, player, x, y):
    """ check if there is a bridge pattern"""
    if board.hex_record[(x,y)] != player:
        return 0

    bridge_pattern = 0
    opponent = 1 - player
    v_score = 1.0
    h_score = 1.0
    if player == 0:  # red
        v_score = 2.0
        h_score = 1.4
    else:  #blue
        h_score = 2.0

    # check vertical patterns
    if y <= 5:
        if x >= 1:
            bridge_pattern += (board.hex_record[(x-1,y+2)] == player)*v_score
            bridge_pattern -= (board.hex_record[(x-1,y+1)] == opponent)*v_score*0.5
            bridge_pattern -= (board.hex_record[(x,y+1)] == opponent)*v_score*0.5
    if y == 6:
        if x >= 1:
            bridge_pattern += (board.hex_record[(x-1,y+1)] == player)*v_score*0.5
            bridge_pattern += (board.hex_record[(x-1,y+1)] != opponent)*v_score*0.5
        if x <= 7:
            bridge_pattern += (board.hex_record[(x,y+1)] == player)*v_score*0.5
            bridge_pattern += (board.hex_record[(x,y+1)] != opponent)*v_score*0.5
    else:
        bridge_pattern += v_score*0.5

    # check horizontal patterns
    if x <= 5:
        if y >= 1:
            bridge_pattern += (board.hex_record[(x+2,y-1)] == player)*h_score
            bridge_pattern -= (board.hex_record[(x+1,y-1)] == opponent)*h_score*0.5
            bridge_pattern -= (board.hex_record[(x+1,y)] == opponent)*h_score*0.5
        if y <= 6:
            bridge_pattern += (board.hex_record[(x+1,y+1)] == player)*h_score
            bridge_pattern -= (board.hex_record[(x+1,y)] == opponent)*h_score*0.5
            bridge_pattern -= (board.hex_record[(x,y+1)] == opponent)*h_score*0.5
    elif x == 6:
        if y >= 1:
            bridge_pattern += (board.hex_record[(x+1,y-1)] == player)*h_score*0.5
            bridge_pattern += (board.hex_record[(x+1,y-1)] != opponent)*h_score*0.5
        if y <= 6:
            bridge_pattern += (board.hex_record[(x+1,y)] == player)*h_score*0.5
            bridge_pattern += (board.hex_record[(x+1,y)] != opponent)*h_score*0.5
    else:
        bridge_pattern += h_score*0.5

    return bridge_pattern


def heuristic3(node, min_max=1):
    """ calculate scores based on bridge patterns using union-find in the board """
    # red: 0
    # blue: 1
    game_board = node.board
    player = node.player
    y0 = 8
    y1 = -1
    x0 = 8
    x1 = -1
    id1 = -1
    id2 = -1
    d_span = 0
    o_span = 0

    c_d_set, c_o_set = update_sets(node.player, node.board, node.d_set, node.o_set)

    # print "d_set: "
    # c_d_set.show()
    # print "o_set: "
    # c_o_set.show()

    if node.player == 0:  # red, vertical
        id1 = 1
        id2 = 0
    else:  # blue, horizontal
        id1 = 0
        id2 = 1

    if c_o_set != None:
        for set_i in c_o_set.disjoint_set:
            for i in range(len(set_i)):
                if set_i[i][id2] > x1:
                    x1 = set_i[i][id2]
                if set_i[i][id2] < x0:
                    x0 = set_i[i][id2]
            if x1-x0 > o_span:
                o_span = x1-x0
                if o_span >= 6:
                    if min_max == 1:
                        return -24
                    else:
                        return 12
                elif o_span >= 5:
                    if min_max == 1:
                        return -12
                    else:
                        return 6
                elif o_span >= 4:
                    if min_max == 1:
                        return -6
                    else:
                        return 3
                else:
                    pass
            x0 = 8
            x1 = -1

    if c_d_set != None:
        for set_i in c_d_set.disjoint_set:
            for i in range(len(set_i)):
                if set_i[i][id1] > y1:
                    y1 = set_i[i][id1]
                if set_i[i][id1] < y0:
                    y0 = set_i[i][id1]
            if y1-y0 > d_span:
                d_span = y1-y0
                if d_span >= 6:
                    if min_max == 1:
                        return 12
                    else:
                        return -24
                elif d_span >= 5:
                    if min_max == 1:
                        return 6
                    else:
                        return -12
                elif d_span >= 4:
                    if min_max == 1:
                        return 3
                    else:
                        return -6
                else:
                    pass
            y0 = 8
            y1 = -1

    if min_max == 1:
        return d_span-o_span
    else:
        return o_span-d_span


def heuristic4(node, min_max=1):
    """ number of connected pieces of the player minus the number of connected pieces of the opponent """
    # red: 0
    # blue: 1
    if node.check_win() == 1:
        return 65
    connected_pieces = 0
    pieces_list = []
    o_pieces_list = []
    game_board = node.board
    player = node.player
    opponent = 1 - player
    for x in range(8):
        for y in range(8):
            if game_board.hex_record[(x,y)] == player:
                pieces_list.append((x,y))
            elif game_board.hex_record[(x,y)] == opponent:
                o_pieces_list.append((x,y))
            else:
                pass

    connected_pieces = len(pieces_list)
    o_connected_pieces = len(o_pieces_list)

    if min_max == 1:
        for x in range(len(pieces_list)):
            cur_x,xur_y = pieces_list[x]
            neighbors = game_board.get_neighbors(cur_x,xur_y)
            if_connected = False
            for neighbor in neighbors:
                if (neighbor.cord[0],neighbor.cord[1]) in pieces_list:
                    if_connected = True
                    break
            if if_connected == False:
                connected_pieces -= 1

        return connected_pieces
    else:
        for x in range(len(o_pieces_list)):
            cur_x,xur_y = o_pieces_list[x]
            neighbors = game_board.get_neighbors(cur_x,xur_y)
            if_connected = False
            for neighbor in neighbors:
                if (neighbor.cord[0],neighbor.cord[1]) in o_pieces_list:
                    if_connected = True
                    break
            if if_connected == False:
                o_connected_pieces -= 1

        return o_connected_pieces


def heuristic5(node, min_max=1):
    """ calculate scores based on bridge patterns in the board """
    # red: 0
    # blue: 1
    game_board = node.board
    player = node.player
    opponent = 1 - player
    p_patterns = 0
    o_patterns = 0

    if min_max == 1:
        for x in range(8):
            for y in range(8):
                if game_board.hex_record[(x,y)] == player:
                    p_patterns += check_bridge(game_board, player, x, y)
        return p_patterns
    else:
        for x in range(8):
            for y in range(8):
                if game_board.hex_record[(x,y)] == opponent:
                    o_patterns += check_bridge(game_board, opponent, x, y)
        return o_patterns



##################################################################################################
#
#                                          游戏的交互界面
#
##################################################################################################

print ("######################################################### ")
print ("#  Hex Game (8*8) ")
print ("#")
print ("#  Author: Xiaotian Wang")
print ("######################################################### ")
print ("")

print ("Please choose the player modes for both players:")
print ("Type '0' for human mode; ")
print ("Type '1' for AI mode with MiniMax Search;")
print ("Type '2' for AI mode with Monte Carlo Tree Search.")
print ("")

mode1 = int(input("Player 1's mode : "))
mode2 = int(input("Player 2's mode : "))
print ("")

game_board = GameBoard()
cur_step = 0
game_left_pos = 64

d_set1 = UnionFind()
d_set2 = UnionFind()
player1 = Player(mode=mode1, game_tree_node=Game_tree_node(1, 0, board=game_board, left_pos=game_left_pos), \
        d_set=d_set1, o_set=d_set2)
player2 = Player(mode=mode2, game_tree_node=Game_tree_node(1, 1, board=game_board, left_pos=game_left_pos), \
        d_set=d_set2, o_set=d_set1)

# main game loop
while True:
    cur_player = None
    opponent = None
    if cur_step%2 == 0:
        print ("Player 1 (Red)")
        cur_player = player1
        opponent = player2
    else:
        print ("Player 2 (Blue)")
        cur_player = player2
        opponent = player1

    game_board.display()
    input_x, input_y = cur_player.play(game_board)

    if game_board.if_taken(int(input_x), int(input_y)):
        print ("The place has been taken, please input the valid x,y cord: ")
        plt.close('all')
    else:
        plt.close('all')
        time.sleep(0.5)

        # update disjoint set for the opponent
        opponent.o_set = copy.deepcopy(cur_player.d_set)

        # DEBUG
        # display the disjoint set
        # print "d_set: "
        # cur_player.d_set.show()
        # print "o_set: "
        # cur_player.o_set.show()

        # update the game board for both players
        player1.game_tree_node.board = game_board
        player2.game_tree_node.board = game_board

        # update left position number
        player1.game_tree_node.left_pos -= 1
        player2.game_tree_node.left_pos -= 1

        # update win record
        player1.game_tree_node.if_win = None
        player2.game_tree_node.if_win = None

        if cur_step%2 == 0:
            player1.game_tree_node.d_set = copy.deepcopy(cur_player.d_set)
            player1.game_tree_node.o_set = copy.deepcopy(cur_player.o_set)
            player2.game_tree_node.d_set = copy.deepcopy(opponent.d_set)
            player2.game_tree_node.o_set = copy.deepcopy(opponent.o_set)
        else:
            player1.game_tree_node.d_set = copy.deepcopy(opponent.d_set)
            player1.game_tree_node.o_set = copy.deepcopy(opponent.o_set)
            player2.game_tree_node.d_set = copy.deepcopy(cur_player.d_set)
            player2.game_tree_node.o_set = copy.deepcopy(cur_player.o_set)

        if cur_step%2 == 0:
            game_board.play_red(int(input_x),int(input_y))
            cur_step += 1
        else:
            game_board.play_blue(int(input_x),int(input_y))
            cur_step += 1

        tree_node = cur_player.game_tree_node

        # check if the play wins
        if_win = tree_node.check_win()
        if if_win == 1:
            game_board.display()
            print ("")
            print ("!!==!!==!!==!!==!!==!!==!!")
            print ("!!")
            print ("!!     Player " + str((cur_step+1)%2 + 1) + " wins!")
            print ("!!")
            print ("!!==!!==!!==!!==!!==!!==!!")
            print ("")
            time.sleep(0.5)
            temp_block = input("End Game, Exit...")
            time.sleep(2)
            break
        elif if_win == -1:
            game_board.display()
            print ("")
            print ("!!==!!==!!==!!==!!==!!==!!")
            print ("!!")
            print ("!!     Player " + str((cur_step)%2 + 1) + " wins!")
            print ("!!")
            print ("!!==!!==!!==!!==!!==!!==!!")
            print ("")
            time.sleep(0.5)
            temp_block = input("End Game, Exit...")
            time.sleep(2)
            break
        print ("")

