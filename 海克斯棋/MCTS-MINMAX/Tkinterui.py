# Tkinterui.py
"""
该模块具有显示UI的功能，以允许用户选择棋盘配置和游戏数。
"""

from tkinter import *
from tkinter.ttk import *
import array
import numpy as np
from collections import defaultdict
from math import *
import os

# Size of the image used to display the position of 2 agents
IMG_SIZE = 35
# Padding for the top and bottom postions in the game frame
# 填充游戏框架中的顶部和底部位置
XPAD = 40
YPAD = 50

# Initialize the Globals for the board size, we would be getting the information from the user
# 初始化游戏尺寸的全局变量，我们将从用户那里获取信息
GRID_SIZE = 0
WIN_HEIGHT = 0
WIN_WIDTH = 0

class gameGrid():


    def __init__(self, name1, name2, keyFrame, gameFrame, resultFrame,mode, gridSize):
        """
        构造函数初始化游戏网格。
        @:param name1 - 指出第一个玩家的名字
        @:param name2 - 表示第二名玩家的名字
        @:param keyFrame - 表示保持按键的框架
        @:param gameFrame - 指示用于显示游戏网格的框架
        @:param resultFrame - 表示保存单个结果的框架
        @:param mode - 当前开始游戏的玩家
        @:param gridSize - t网格的大小
        """
        self.gameFrame = gameFrame
        self.mode = mode 

        # 初始化所有3帧
        gameFrame.pack(side="left", expand=True, fill="both")
        gameFrame.pack_propagate(False)
        self.keyFrame = keyFrame
        keyFrame.pack(side="bottom", expand=True, fill="both")
        keyFrame.pack_propagate(False) 
        self.resultFrame = resultFrame
        resultFrame.pack(side="right", expand=True, fill="both")
        resultFrame.pack_propagate(False)

        #Get the path of the 3 nodes - white (empty), black (player 1) and red (player 2s)
        base_folder = os.path.dirname(__file__) 
        image_path = os.path.join(base_folder, 'hex_white.png')
        self.white = PhotoImage(file=image_path)
        image_path = os.path.join(base_folder, 'hex_red.png')
        self.red = PhotoImage(file=image_path)
        image_path = os.path.join(base_folder, 'hex_black.png')
        self.black = PhotoImage(file=image_path)

        # initialize all the 3 grids
        self.drawGrid(None)
        self.drawKey(name1, name2)
        self.drawResult()      

    def drawGrid(self, name):
        """
        Draws the grid.
        @:param name - the game tag
        """
        #Add the game label if available
        if name != None:
            gameLabel = Label(self.gameFrame, text=name)
            gameLabel.pack(side=TOP)
        
        #Add the border for the hex board
        redTopLabel = Label(self.gameFrame, image=self.red)
        redTopLabel.pack(side=TOP)
        redBottomLabel = Label(self.gameFrame, image=self.red)
        redBottomLabel.pack(side=BOTTOM)
        blackLeftLabel = Label(self.gameFrame, image=self.black)
        blackLeftLabel.pack(side=LEFT)
        blackRightLabel = Label(self.gameFrame, image=self.black)
        blackRightLabel.pack(side=RIGHT)

        #Add the grid with all the empty white nodes
        self.label = np.zeros([GRID_SIZE, GRID_SIZE], object)
        for yi in range(0, GRID_SIZE):
            xi = XPAD + yi * IMG_SIZE
            for i in range(0, GRID_SIZE):
                l = Label(self.gameFrame, image=self.white)
                l.pack()
                l.image = self.white
                l.place(anchor=NW, x=xi, y=YPAD + yi * IMG_SIZE)
                xi += 2 * IMG_SIZE
                self.label[yi][i] = l

    def drawKey(self,name1, name2):
        """
        Draws the Key frame.
        @:param name1 - Player1 name
        @:param name2 - Player2 name
        """
        titleLabel = Label(self.keyFrame, text = "KEY")
        titleLabel.pack()
        player1Label = Label(self.keyFrame, image=self.black, text=name1, compound=LEFT)
        player1Label.pack()
        player2Label = Label(self.keyFrame, image=self.red, text=name2, compound=LEFT)
        player2Label.pack()
      
    
    def drawResult(self):
        """
        Draws the result frame.
        """
        titleLabel = Label(self.resultFrame, text = "RESULTS")
        titleLabel.pack()
    
    def toggleColor(self, widget):
        """
        Toggle the node
        @:param widget - associated widget
        """
        if self.mode == 1:
            widget.config(image=self.black)
            widget.image = self.black
        else:
            widget.config(image=self.red)
            widget.image = self.red

    def display_winner(self, gameNumber, winner):
        """
        Display the winner for the single game
        @:param gameNumber - game number associated
        @:param winner - winner of the corresponding game
        """
        label = Label(self.resultFrame,text = "Winner of " + str(gameNumber) + ": " + winner )
        label.pack()
    
    def reset(self, name):
        """
        Resets the game grid
        @:param name - the name of the play
        """
        for widget in self.gameFrame.winfo_children():
            widget.destroy()
        self.drawGrid(name)

    def setFinalResult(self,player1,player2):
        """
        Resets the game grid
        @:param player1 - Player 1
        @:param player2 - Player 2
        """
        finalResultFrame = Frame(self.resultFrame, borderwidth=2, relief="solid")
        finalResultFrame.pack(side="top", expand=True, fill="both")
        finalResultFrame.pack_propagate(False)
        titleLabel = Label(finalResultFrame, text = "FINAL RESULT")
        titleLabel.pack()
        player1Label = Label(finalResultFrame, text = player1.name + " - " + "Wins: " + str(player1.wins) + ", Percent: " + str((player1.wins/(player1.wins + player1.losses))*100) + "%")
        player1Label.pack()
        player2Label = Label(finalResultFrame, text = player2.name + " - " + "Wins: " + str(player2.wins) + ", Percent: " + str((player2.wins/(player2.wins + player2.losses))*100) + "%")
        player2Label.pack()

class gameWindow:
    """
    Creates the game window
    """

    def __init__(self, name1, name2, window, mode, gridSize):
        """
        Initializes the game window
        @:param name1 - Player 1
        @:param name2 - Player 2
        @:param window - tkinter window
        @:param mode - the current player
        @:param gridSize - grid size
        """
        #recalculate the globals
        global GRID_SIZE, WIN_HEIGHT, WIN_WIDTH
        GRID_SIZE = gridSize
        WIN_HEIGHT = 2 * YPAD + GRID_SIZE * IMG_SIZE
        WIN_WIDTH = 2 * XPAD + (3 * GRID_SIZE - 1) * IMG_SIZE

        #Create 3 frames from the window for  Game, Keys and results
        keyFrame = Frame(window, width=75, height=100,borderwidth=2, relief="solid")
        resultFrame = Frame(window, width=WIN_WIDTH, height=WIN_HEIGHT, borderwidth=2, relief="solid")
        gameFrame = Frame(window, width=WIN_WIDTH, height=WIN_HEIGHT, borderwidth=2, relief="solid")

        #create the game grid
        self.gameGrid = gameGrid(name1, name2,keyFrame,gameFrame,resultFrame,mode,gridSize)

class launchWindow:
    """
    Creates the landing window
    """
    def __init__(self, game, player1, player2):
        """
        Initializes the landing window
        @:param game - the initialized hex game
        @:param player1 - Player 1
        @:param player2 - Player 2
        """
        self.game = game
        self.player1 = player1
        self.player2 = player2

        #set the default size and number of games
        self.boardSize = 11
        self.numberOfGames = 1

        #create the window
        window = Tk()
        window.wm_title("Hex Game")

        #create the frame
        frame = Frame(window, width=200, height=150,borderwidth=2, relief="solid")
        frame.pack()
        frame.pack_propagate(False)

        #board size label and option list
        label = Label(frame, text = "Choose a board size: ")
        label.pack()
        OPTIONS = [
        "11 x 11",
        "13 x 13"
        ]
        variable = StringVar(frame)
        variable.set(OPTIONS[0]) # default value
        w = OptionMenu(frame, variable, *OPTIONS, command=self.setBoardSize)
        w.pack()

        #Number of games label and options list
        label = Label(frame, text = "Choose the number of games: ")
        label.pack()
        OPTIONS = [
        "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
        "11", "12", "13", "14", "15", "16", "17", "18", "19", "20",
        "21", "22", "23", "24", "25", "26", "27", "28", "29", "30",
        ]
        variable = StringVar(frame)
        variable.set(OPTIONS[0]) # default value
        w = OptionMenu(frame, variable, *OPTIONS, command=self.setNumberOfGames)
        w.pack()

        #button to start the simulation
        b = Button(frame, text="Go", command=self.buttonClick)
        b.pack()
        self.window = window
        mainloop()

    def setBoardSize(self, value):
        """
        Sets the Board size command
        @:param value - board size value
        """
        if(value == "11 x 11"):
            self.boardSize = 11
        else:
            self.boardSize = 13                
    
    def setNumberOfGames(self, value):
        """
        Sets the number of games command
        @:param value - number of games value
        """
        self.numberOfGames = value
    
    def buttonClick(self):
        """
        Launch the simulation
        """
        #Kill the existing widgets
        for widget in self.window.winfo_children():
            widget.destroy()

        #Launch the simulation
        self.game.size = self.boardSize
        self.game.playNGames(int(self.numberOfGames), self.player1, self.player2,self.window)