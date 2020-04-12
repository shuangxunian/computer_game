from Queues import FIFO_Queue
from copy import deepcopy
from Players import *
import numpy as np
from sys import argv
from playInfo import playInfo
from Minimax import *
from MonteCarloPlayer import *
from Tkinterui import *
from MCTS import *
def main():
    # 定义棋盘的大小
    game = HexGame(11)

    # 初始化游戏的两个玩家
    p1 = AlphaBetaPruningPlayer(game, 2)
    p2 = MonteCarloPlayer(game)
    #p3 = MCTS(a,b,-self.playInfo.mode,self.playInfo.board)

    # Launch the game window with the choice
    launchWindow(game, p1, p2)

class HexGame(object):
    """
    初始化HEX游戏的规则
    """
    def __init__(self, size):
        """
        Constructor for the game.
        @param size -  the size of the board
        """
        self.size = size
        self.reset()

    def reset(self):
        """
        初始化参数值
        """
        self.board = np.zeros([self.size, self.size], int)
        # +1 for black; -1 for red
        self.turn = -1


    def playOneGame(self, gameNumber, player1, player2, gw, window):
        """
        玩一次游戏并返回赢家
        @param gameNumber - the iteration of the run
        """
        # resets the configuration of the game
        self.reset()

        # set the side of the players
        player1.setSide(1)
        player2.setSide(-1)

        # run the iteration
        # 交换谁先手
        while True:
            #
            if self.turn > 0:
                cur_player = player1
            else:
                cur_player = player2
            
            # 对于当前的游戏玩家获得当前游戏玩家的下一步走法
            # AI = MCTS(a, b, -self.playInfo.mode, self.playInfo.board)
            # e = AI.move()

            row, col = cur_player.getMove(self.board)
            # print(row, col)

            # set the board
            self.board[row][col] = self.turn

            # 更新窗口
            gw.gameGrid.mode = self.turn
            gw.gameGrid.toggleColor(gw.gameGrid.label[row][col])
            window.update()    

            # 对于当前的情况返回最后的赢家
            if self.turn > 0 and self.blackWins(self.board):
                gw.gameGrid.display_winner(gameNumber,cur_player.name)
                break
            elif self.turn < 0 and self.redWins(self.board):
                gw.gameGrid.display_winner(gameNumber,cur_player.name)
                break
            
            # reset the player turn
            self.turn *= -1
        return self.turn

    def playNGames(self, n, player1, player2,window):
        """
        Will play out n games between player1 and player2.
        The players alternate going first.
        Prints the total number of games won by each player.
        @param n - the number of iterations to run
        @param player 1 - the first player
        @param player 2 - the second player
        @param window - the launched window
        """

        # resets the game window and update the window
        gw = gameWindow(player1.name, player2.name, window, self.turn, self.size)
        window.update_idletasks()
        first = player1
        second = player2

        # Run all the iterations
        for i in range(n):
            gw.gameGrid.reset("Playing Game " + str(i + 1))
            winner = self.playOneGame(i + 1, first, second, gw, window)

            # update the winner list
            if winner > 0:
                first.won()
                second.lost()
            else:
                first.lost()
                second.won()

        # display the final result and pause execution
        gw.gameGrid.setFinalResult(first, second)
        window.mainloop()

    def getPossibleMoves(self, board):
        """
        Returns a list of all possible moves on the given board.
        @param board - the board configuration
        """
        possible = []
        for row in range(self.size):
            for col in range(self.size):
                if board[row][col] == 0:
                    possible.append((row, col))
        return possible

    def getNextBoard(self, board, move, player):
        """
        Returns a new board showing how the given board would look after the move by player.
        @param board - the board configuration
        @param move - the current move
        @param player - the current player
        """
        row, col = move
        if board[row][col] != 0:
            print("Error invalid move: %s" % (move))
            exit()
        nextBoard = deepcopy(board)
        nextBoard[row][col] = player
        return nextBoard

    def getStronglyConnectedPositions(self, row, col, side):
        """
        Gets the list of strongly connected nodes
        @param row - the current row
        @param col - the current col
        @param side - the current player side
        """
        ls = []       
        if side == 1:# we only care about right and left
            if col > 1:
                ls.append((row, col - 2, 1))
                if row < self.size - 1:
                    ls.append((row + 1, col - 2, 2))
                if row < self.size - 2:
                    ls.append((row + 2, col - 2, 1))
                if row < self.size - 3:
                    ls.append((row + 3, col - 2, 1))
            
            if col < self.size - 2:
                ls.append((row, col + 2, 1))
                if row >= 1:
                    ls.append((row - 1, col + 2, 2))
                if row >= 2:
                    ls.append((row - 2, col + 2, 1))
                if row >= 3:
                    ls.append((row - 3, col + 2, 1))

        else:
            if row > 1:
                ls.append((row - 2, col, 1))
                if col < self.size - 1:
                    ls.append((row - 2, col + 1, 2))
                if col < self.size - 2:
                    ls.append((row - 2, col + 2, 1))
                if col < self.size - 3:
                    ls.append((row - 2, col + 3, 1))
            if row < self.size - 2:
                ls.append((row + 2, col, 1))
                if col >= 1:
                    ls.append((row + 2, col - 1, 2))
                if col >= 2:
                    ls.append((row + 2, col - 2, 1))
                if col >= 3:
                    ls.append((row + 2, col - 3, 1))
                
        return ls


    def getNeighbors(self, row, col):
        """
        Returns a list of neighboring cells to the given row and col.
        @param row - the current row
        @param col - the current col
        """
        ls = []
        if row > 0:
            ls.append((row-1, col))
        if row < self.size-1:
            ls.append((row+1, col))
        if col > 0:
            ls.append((row, col-1))
        if col < self.size-1:
            ls.append((row, col+1))
        if row > 0 and col < self.size-1 :
            ls.append((row-1, col+1))
        if row < self.size-1 and col > 0:
            ls.append((row+1, col - 1))
        return ls
 
    def getNeighborsGap(self, row, col, side):
        """
        Returns a list of neighbors one row/col away from the original pt.
        @param row - the current row
        @param col - the current col
        @param side - the current side
        """
        ls = []
        if side == 1:# we only care about right and left
            if col > 1:
                ls.append((row, col - 2, 1))
                if row < self.size - 1:
                    ls.append((row + 1, col - 2, 2))
                if row < self.size - 2:
                    ls.append((row + 2, col - 2, 1))
            
            if col < self.size - 2:
                ls.append((row, col + 2, 1))
                if row >= 1:
                    ls.append((row - 1, col + 2, 2))
                if row >= 2:
                    ls.append((row - 2, col + 2, 1))

        else:
            if row > 1:
                ls.append((row - 2, col, 1))
                if col < self.size - 1:
                    ls.append((row - 2, col + 1, 2))
                if col < self.size - 2:
                    ls.append((row - 2, col + 2, 1))
            if row < self.size - 2:
                ls.append((row + 2, col, 1))
                if col >= 1:
                    ls.append((row + 2, col - 1, 2))
                if col >= 2:
                    ls.append((row + 2, col - 2, 1))
                
        return ls

 
    def getRelevantNeighbors(self, row, col, side):
        """
        Returns a pared down list of neighbors depending on side
        @param row - the current row
        @param col - the current col
        @param side - the current player side
        """
        ls = []
        if side == 1:#we only care about right and left
            if col >= 1:
                ls.append((row, col - 1))
                if row < self.size - 1:
                    ls.append((row + 1, col - 1))
            
            if col < self.size - 1:
                ls.append((row, col + 1))
                if row >= 1:
                    ls.append((row - 1, col + 1))

        else:#we only care about up and down
            if row >= 1:
                ls.append((row - 1, col))
                if col < self.size - 1:
                    ls.append((row - 1, col + 1))
            if row < self.size - 1:
                ls.append((row + 1, col))
                if col >= 1:
                    ls.append((row + 1, col - 1))
                
        return ls


    def blackWins(self, board):
        """
        Returns True if black player wins, otherwise False.
        @param board the current board
        """
        queue = FIFO_Queue()
        visited = set()
        # Add all locations of black pieces in the leftmost col to queue
        for row in range(self.size):
            if board[row][0] == 1:
                queue.add((row, 0))
        # Try to find a path to the rightmost col
        while len(queue) > 0:
            row, col = queue.get()
            visited.add((row, col))
            for n in self.getNeighbors(row, col):
                r, c = n
                if board[r][c] != 1: continue
                if c == self.size-1: return True
                if n in visited or n in queue:
                    continue
                queue.add(n)
        return False

    def redWins(self, board):
        """
        Returns True if red player wins, otherwise False.
        @param board - the current board
        """
        queue = FIFO_Queue()
        visited = set()
        # Add all locations of white pieces n the top row to queue
        for col in range(self.size):
            if board[0][col] == -1:
                queue.add((0, col))
        # Try to find a path to the bottom row
        while len(queue) > 0:
            row, col = queue.get()
            visited.add((row, col))
            for n in self.getNeighbors(row, col):
                r, c = n
                if board[r][c] != -1: continue
                if r == self.size-1: return True
                if n in visited or n in queue:
                    continue
                queue.add(n)
        return False

    def countConnected(self, board, side):
        """
        Counts how many pieces for the given side touch another piece
        of the same side.
        @param board - the current board
        @param side - the current player side
        """
        queue = FIFO_Queue()
        counted = set()
        acc = 0
        for row in range(self.size):
            for col in range(self.size):
                if board[row][col] == side:
                    queue.add((row, col))
        while len(queue) > 0:
            row, col = queue.get()
            for n in self.getNeighbors(row, col):
                r, c = n
                if board[r][c] == side and n not in counted:
                    counted.add((r, c))
                    acc += 1
                    
        return (acc * side)

    def betterCountConnected(self, board, side):
        """
        Counts how many pieces for the given side touch another piece of the same side
        @param board - the current board
        @param side - the current player side
        """
        queue = FIFO_Queue()
        counted = set()
        acc = 0
        for row in range(self.size):
            for col in range(self.size):
                if board[row][col] == side:
                    queue.add((row, col))
        
        while len(queue) > 0:
            row, col = queue.get()
            for n in self.getRelevantNeighbors(row, col, side):
                r, c = n
                if board[r][c] == side and n not in counted:
                    counted.add((r, c))
                    acc += 1
                    
        return (acc * side)


    def countConnectedGap(self, board, side):
        """
        Counts how many pieces for the given side touch another piece of the same side.
        @param board - the current board
        @param side - the current player side
        """
        queue = FIFO_Queue()
        counted = set()
        acc = 0
        for row in range(self.size):
            for col in range(self.size):
                if board[row][col] == side:
                    queue.add((row, col))

        while len(queue) > 0:
            row, col = queue.get()
            for n in self.getNeighborsGap(row, col, side):
                r, c, s = n
                if board[r][c] == side and (r, c) not in counted:
                    counted.add((r, c))
                    acc += s

        return (acc * side)

    def expansionScore(self, board, side):
        """
        Score for expanding the node.
        @param board - the current board
        @param side - the current player side
        """
        if side == 1:
            colsExpanded = set()
            for row in range(self.size):
                for col in range(self.size):
                    if col in colsExpanded:
                        continue
                    if board[row][col] == side:
                        colsExpanded.add(col)
            return len(colsExpanded)
        
        if side == -1:
            rowsExpanded = set()
            for row in range(self.size):
                if row in rowsExpanded:
                    continue
                for col in range(self.size):
                    if board[row][col] == side:
                        rowsExpanded.add(row)
            return len(rowsExpanded) * -1
               

    def countStronglyConnectedPositions(self, board, side):
        """
        计算牢固连接的位置
        @param board - the current board
        @param side - the current player side
        """
        queue = FIFO_Queue()
        counted = set()
        acc = 0
        for row in range(self.size):
            for col in range(self.size):
                if board[row][col] == side:
                    queue.add((row, col))

        while len(queue) > 0:
            row, col = queue.get()
            for n in self.getStronglyConnectedPositions(row, col, side):
                r, c, s = n
                if board[r][c] == side and (r, c) not in counted:
                    counted.add((r, c))
                    acc += s                    
        return (acc * side)

if __name__ == '__main__':
    """
    程序的入口点
    """
    main()
