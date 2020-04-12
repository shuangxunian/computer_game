from __future__ import print_function
from tkinter import *
import array
from sys import stdout
from collections import namedtuple
from math import *
import constants
from playInfo import playInfo
from MCTS import MCTS

# 游戏的棋盘大小
GRID_SIZE=constants.GRID_SIZE
# GUI界面的大小
IMG_SIZE = 35
# X的初始化坐标
XPAD = 40
# Y的初始化坐标
YPAD = 40
# 图形的高度
WIN_HEIGHT = 2 * YPAD + GRID_SIZE * IMG_SIZE + 100
# 图形的宽度
WIN_WIDTH = 2 * XPAD + (3 * GRID_SIZE - 1) * IMG_SIZE

class gameGrid():
    def __init__(self, frame): # 初始化
        self.frame = frame
        self.white = PhotoImage(file="./media/white35.gif")
        self.red = PhotoImage(file="./media/red35.gif")
        self.blue = PhotoImage(file="./media/blue35.gif")
        self.drawGrid()
        self.playInfo = playInfo()

    def drawGrid(self): # 绘制GUI界面
        for yi in range(0, GRID_SIZE):
            xi = XPAD + yi * IMG_SIZE
            for i in range(0, GRID_SIZE):
                l = Label(self.frame, image=self.white)
                l.pack()
                l.image = self.white
                l.place(anchor=NW, x=xi, y=YPAD + yi * IMG_SIZE)
                l.bind('<Button-1>', lambda e: self.on_click(e))
                xi += 2 * IMG_SIZE

    def getCoordinates(self, widget): # 获取点的坐标
        row = (widget.winfo_y() - YPAD) // IMG_SIZE
        col = (widget.winfo_x() - XPAD - row * IMG_SIZE) // (2 * IMG_SIZE)
        return row , col

    def toggleColor(self, widget): # 根据进行的双方不同显示不同的棋子
        if self.playInfo.mode == 1:
            widget.config(image=self.red)
            widget.image = self.red
        else:
            widget.config(image=self.blue)
            widget.image = self.blue

    def display_winner(self, winner_label):  # 根据最终的赢家，进行显示
        winner_window = Tk()
        winner_window.wm_title("Winner")
        frame = Frame(winner_window, width=220, height=80)
        frame.pack()
        label = Label(frame,text = "Winner is Player : " + winner_label)
        label.pack()
        label.place(anchor=NW, x = 20, y = 20)
#？
    def on_click(self, event): # 点击时的GUI界面显示
        if event.widget.image != self.white:
            return
        self.toggleColor(event.widget)  # 根据进行的双方不同显示不同的棋子
        a, b = self.getCoordinates(event.widget)  # 获取点的坐标
        self.playInfo.update(a,b)  # 更新点的坐标
        if self.playInfo.winner!=0:
            winner_label = ""
            if self.playInfo.winner == -1:
                winner_label = " -1 ( Blue ) "
            else:
                winner_label += " 1 ( Red ) "
            print(winner_label)

        else:

            AI = MCTS(a,b,-self.playInfo.mode,self.playInfo.board)
            e = AI.move()
            l = Label(self.frame, image=self.red)
            l.pack()
            l.image = self.red
            l.place(anchor=NW, x=XPAD + e[0] * IMG_SIZE + 2* e[1]*IMG_SIZE, y=YPAD + e[0] * IMG_SIZE)
            print(e)
            print("SUCCESS")
            self.playInfo.update(e[0],e[1])

            if self.playInfo.winner!=0:
                winner_label = ""
                if self.playInfo.winner == -1:
                    winner_label = " -1 ( Blue ) "
                else:
                    winner_label += " 1 ( Red ) "
                self.display_winner(winner_label)
                print(winner_label)
#？
class gameWindow:
    def __init__(self, window):
        self.frame = Frame(window, width=WIN_WIDTH, height=WIN_HEIGHT)
        self.frame.pack()
        self.gameGrid = gameGrid(self.frame)


def main():
    window = Tk()
    window.wm_title("Hex Game")
    gameWindow(window)
    window.mainloop()
main()
