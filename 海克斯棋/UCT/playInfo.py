import constants  # 传入棋盘大小的信息
import time # 计算程序运行的时间

#设置棋盘的大小
GRID_SIZE=constants.GRID_SIZE


# 游戏变量-连接的方向
dx = [0, 1, 0, -1, -1, 1]
dy = [-1, -1, 1, 1, 0, 0]

class playInfo():
    # 初始化
    def __init__(self):
        self.board = [[0 for r in range(GRID_SIZE)] for c in range(GRID_SIZE)]  # 创建棋盘

        # -1: 蓝棋先走
        #  1: 红棋先走
        self.mode = -1
        self.winner = 0

    def inRange(self, r,c):
    # 判断是否为合理范围
        if r<0 or r>=GRID_SIZE or c<0 or c>=GRID_SIZE:
            return False
        return True

    # 更新棋盘上的位置 更新下棋的策略 更新下棋的双方
    def update(self, r,c):

        if self.board[r][c] != 0: # 判断此位置已有棋子
            return False
        self.board[r][c] = self.mode # 在此处下棋
        self.checkWinning(self.mode) # 检查此时是否已经胜利
        self.mode = - self.mode # 交换下棋的双方
        return True

    # 检查是否双方有一方赢得比赛
    def checkWinning(self, player):

        start = time.time()

        connection = [[0 for r in range(GRID_SIZE)] for c in range(GRID_SIZE)]
        flag = True
        if player == -1:  # 如果此时下棋的为蓝方棋手
            for j in range(GRID_SIZE):
                if self.board[0][j]==-1:
                    connection[0][j]=-2

            while flag:
                flag=False
                for i in range(GRID_SIZE):
                    for j in range(GRID_SIZE): # 遍历棋盘
                        if connection[i][j]==-2:
                            for k in range(6):
                                if self.inRange(i+dx[k],j+dy[k]) and self.board[i+dx[k]][j+dy[k]]==-1 and connection[i+dx[k]][j+dy[k]]==0:
                                    if i+dx[k]==GRID_SIZE-1:
                                        self.winner= -1
                                        return
                                    connection[i+dx[k]][j+dy[k]]=-2
                                    flag=True
                            connection[i][j]=-1

        elif player==1: # 此时下棋的为红方棋手
            for i in range(GRID_SIZE):
                if self.board[i][0]==1:
                    connection[i][0]=2

            while flag:
                flag=False
                for i in range(GRID_SIZE):
                    for j in range(GRID_SIZE):  # 遍历棋盘
                        if connection[i][j]==2:
                            for k in range(6):
                                if self.inRange(i+dx[k],j+dy[k]) and self.board[i+dx[k]][j+dy[k]]==1 and connection[i+dx[k]][j+dy[k]]==0:
                                    if j+dy[k]==GRID_SIZE-1:
                                        self.winner = 1
                                        return
                                    connection[i+dx[k]][j+dy[k]]=2
                                    flag=True
                            connection[i][j]=1

        end = time.time()
        print(end - start)
        return


