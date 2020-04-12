# Queues.py
"""
处理先进先出节点和优先级的队列
"""
from collections import deque
from heapq import heappush, heappop

class _Queue(object):
    """
    FIFO_Queue，LIFO_QUEUE和Priority_Queue的抽象基类
    """
    def __init__(self):
        """
        构造函数初始化基本队列。
        """
        self.contents = deque()

    def add(self, item):
        """
       将项目存储在队列中
        @:param item - the item to be added
        """
        self.contents.append(item)

    def get(self):
        """
        从队列中删除一些项目并返回它。
        @:returns - the item
        """
        raise NotImplementedError("Child classes must implement get.")

    def __len__(self):
        """ 
        返回队列的长度。
        @:returns - the length of the queue
        """
        return len(self.contents)

    def __repr__(self):
        """ 
        网格的字符串表示形式。
        @:returns - the string representation of the string
        """
        return repr(self.contents)

    def __contains__(self, item):
        """ 
        检查队列中是否已存在元素。
        @:returns - boolean to indicate that the item in the queue
        """
        return item in self.contents

class FIFO_Queue(_Queue):
    """
    返回添加到队列的第一个元素
    """
    def get(self):
        """
        从队列中删除最旧的项目并返回它。
        @:returns - the item
        """        
        return self.contents.popleft()

class LIFO_Queue(_Queue):
    """
    返回队列中的最后一个元素。
    """
    def get(self):
        """
        从队列中删除最新项并返回它。
        @:returns - the item
        """
        return self.contents.pop()

class Priority_Queue(_Queue):
    """
    根据优先级返回元素
    """
    def __init__(self):
        """
        初始化队列
        """
        self.contents = []

    def add(self, item, priority):
        """
        将元素添加到堆中。
        """
        heappush(self.contents, (priority, item))

    def get(self):
        """
        从队列中删除优先级最低的项目并返回它。
         @:returns - the item
        """
        return heappop(self.contents)[1]