import time
import constants
from numpy import random
import math

GRID_SIZE=constants.GRID_SIZE

# Game Variables 游戏的变量，用于判断是否到达最后的胜利局面
dx = [0, 1, 0, -1, -1, 1]
dy = [-1, -1, 1, 1, 0, 0]

# 完全展开节点
# UCT 算法
# 5000 次迭代很慢

class Node(object):
    # 定义节点
    # 节点的初始化操作
    def __init__(self, move_r, move_c, mode, parent=None):
        self.visits=0
        self.reward=0.0
        self.board=[[0 for r in range(GRID_SIZE)] for c in range(GRID_SIZE)]
        self.mode = mode
        self.parent=parent
        if parent != None:
            for i in range(GRID_SIZE):
                for j in range(GRID_SIZE):
                    self.board[i][j] = parent.board[i][j]  # 一开始的时候，每一个节点都是自身的父节点
        self.move=[move_r,move_c]
        self.board[move_r][move_c]=mode # current move player
        self.children=[]

    def inRange(self, r,c):
        # 判断坐标的合理性
        if r<0 or r>=GRID_SIZE or c<0 or c>=GRID_SIZE:
            return False
        return True



    def checkWinning(self, mode, b):
    # 检查双方是否有一方已经胜利
        start = time.time()

        connection = [[0 for r in range(GRID_SIZE)] for c in range(GRID_SIZE)]
        flag = True
        if mode == -1:  # 如果此时下棋的为蓝方棋手
            for j in range(GRID_SIZE):
                if b[0][j]==-1:
                    connection[0][j]=-2

            while flag:
                flag=False
                for i in range(GRID_SIZE):
                    for j in range(GRID_SIZE):
                        if connection[i][j]==-2:  # 检查边界处是否已经有点被下棋
                            for k in range(6):
                                if self.inRange(i+dx[k],j+dy[k]) and b[i+dx[k]][j+dy[k]]==-1 and connection[i+dx[k]][j+dy[k]]==0:
                                    if i+dx[k]==GRID_SIZE-1: # 检查是否达到连接的标准
                                        return -1

                                    connection[i+dx[k]][j+dy[k]]=-2
                                    flag=True
                            connection[i][j]=-1
        elif mode==1:
            for i in range(GRID_SIZE):
                if b[i][0] == 1:
                    connection[i][0] = 2

            while flag:
                flag=False
                for i in range(GRID_SIZE):
                    for j in range(GRID_SIZE):
                        if connection[i][j]==2:
                            for k in range(6):
                                if self.inRange(i+dx[k],j+dy[k]) and b[i+dx[k]][j+dy[k]]==1 and connection[i+dx[k]][j+dy[k]]==0:
                                    if j+dy[k]==GRID_SIZE-1:

                                        return 1
                                    connection[i+dx[k]][j+dy[k]]=2
                                    flag=True
                            connection[i][j]=1

        end = time.time()
        print(end - start)
        return 0

    def expand(self):
        # 仅扩展一个叶子节点
        legal=[]
        for i in range(GRID_SIZE):
            for j in range(GRID_SIZE):
                if self.board[i][j] == 0:
                    legal.append([i,j]) # 扩展节点
        for ch in self.children:
            legal.remove(ch.move)
        if len(legal) == 0:
            # 此时已经为一个叶子节点
            self.simulate(1) # 模拟
        for m in legal:
            child=Node(m[0],m[1],-self.mode,self)
            self.children.append(child)
            child.simulate(1) # 模拟


    def simulate(self,breadth):
        result = 0
        for t in range(breadth):
            m = -self.mode
            board2 = [[0 for r in range(GRID_SIZE)] for c in range(GRID_SIZE)] # 绘制棋盘
            for i in range(GRID_SIZE):
                for j in range(GRID_SIZE):
                    board2[i][j] = self.board[i][j]
            while True:
                legal=[]
                for i in range(GRID_SIZE):
                    for j in range(GRID_SIZE):
                        if board2[i][j] == 0:
                            legal.append([i,j])
                if len(legal)==0:
                    break
                rd = random.randint(0,len(legal))
                board2[legal[rd][0]][legal[rd][1]] = m
                m = -m
            result += self.checkWinning(-1,board2)+self.checkWinning(1,board2)
        self.reward+=result/breadth
        self.visits+=1
        self.backup(result/breadth)

    def backup(self,reward):
        if self.parent == None:
            return
        self.parent.visits+=1
        self.parent.reward+=reward
        self.parent.backup(reward)

    def isleaf(self):  # 判断是否为叶子节点
        legal=[]
        for i in range(GRID_SIZE):
            for j in range(GRID_SIZE):
                if self.board[i][j] == 0:
                    legal.append([i,j])
        if len(legal)==0:
            return True
        else:
            return False

    def select(self):
        # 选择一个叶子节点
        if self.children==[]:
            return self
        best_uct = -1
        best_child = self.children[0]
        for ch in self.children:
            if ch.isleaf():
                continue
            uct=(1-(ch.reward/ch.visits)*self.mode)/2 + 2* math.sqrt(math.log(self.visits)/ch.visits)
            if uct>best_uct:
                best_child = ch
                best_uct=uct
        return best_child.select()

class MCTS:
    def __init__(self, name, move_r, move_c, m, b):
        if name == None:
            name = "UTC Player"
        # 建立根节点
        self.root=Node(move_r,move_c,m,None)
        for i in range(GRID_SIZE):
            for j in range(GRID_SIZE):
                self.root.board[i][j]=b[i][j]

    def move(self):
        for iter in range(200): # 选择501次用来扩展下一个节点
            self.root.select().expand()

            if iter % 10 !=0:
                continue
            best_move=[0,0]
            most_visits=0
            best_reward=0
            for ch in self.root.children:
                if ch.visits > most_visits:
                    best_move = ch.move
                    most_visits=ch.visits
                    best_reward=ch.reward
            print('iter:'+str(iter)+' and best move:'+str(best_move)+' and visits:'+str(most_visits)+' and reward:'+str(best_reward))
        best_move=[0,0]
        most_visits=0
        best_reward=0
        for ch in self.root.children:
            if ch.visits>most_visits:
                best_move = ch.move
                most_visits=ch.visits
                best_reward=ch.reward
        return best_move
