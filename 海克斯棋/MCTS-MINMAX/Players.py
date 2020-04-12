# Players.py
"""
The base player class
"""
from random import choice, shuffle

class Player(object):
    """
    基本的游戏玩家类
    """
    def __init__(self, name=None):
        """
        构造函数初始化基类。
        @param name - 游戏玩家的名称
        """
        self.name = name
        self.side = None
        self.game = None
        self.wins = 0
        self.losses = 0

    def setSide(self, side):
        """
        为游戏玩家设置角色
        @param side - side to be set.
        """
        self.side = side

    def otherSide(self, side):
        """
        改变游戏玩家的角色
        @param side - side to be set.
        """
        return -1*side

    def won(self):
        """
        增加玩家的胜利的计数器
        """
        self.wins += 1

    def lost(self):
        """
        增加玩家的失败的计数器
        """
        self.losses += 1


    def getMove(self, board):
        """
        返回当前局面下的游戏的下一步走法
        @param board - the board to be considered
        """
        raise NotImplementedError()