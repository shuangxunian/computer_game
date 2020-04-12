import pygame as pg
from math import sqrt, hypot
from collections import deque

class Point:
    def __init__(self, *pos):
        if len(pos) == 1:
            self.x, self.y = pos[0]
            self.X, self.Y = list(map(int, pos[0]))
        else:
            self.x, self.y = pos
            self.X, self.Y = list(map(int, pos))

    def dist(self, other):
        return hypot(self.x - other.x, self.y - other.y)

    def __add__(self, other):
        return Point(self.x + other.x, self.y + other.y)

    def __str__(self):
        return '[x:{x}, y:{y}]'.format(x=self.x, y=self.y)

    def __iter__(self):
        '''for unpacking'''
        return (x for x in (self.x, self.y))


from consts import *

def triangleS(A, B, C):
    '''retrun the surface of a triangle'''
    a = C.dist(B)
    b = A.dist(C)
    c = A.dist(B)
    p = (a+b+c)/2
    return sqrt(p*(p-a)*(p-b)*(p-c))

def inHex(pos, x, y, a):
    '''checks if a point is in a hexagon'''
    P = Point(pos)
    points = [(x+a, y), (x+a/2, y+a*sqrt(3)/2),
              (x-a/2, y+a*sqrt(3)/2), (x-a, y),
              (x-a/2, y-a*sqrt(3)/2), (x+a/2, y-a*sqrt(3)/2)]
    points = list(map(Point, points))
    sum = 0
    for i in range(-1, 5):
        sum += triangleS(points[i], points[i+1], P)
    S = a*a*3*sqrt(3)/2
    return abs(S-sum) < EPS

def inRect(pos, x, y, w, h):
    '''checks if a point is in a rectangle'''
    return (pos.x > x and pos.x < x + w and\
           pos.y > y and pos.y < y + h)

def drawHex(surface, colIn, colOut, pos, a):
    x, y = pos
    points = [(x-a/2, y-a*sqrt(3)/2),
              (x+a/2, y-a*sqrt(3)/2),
              (x+a, y),
              (x+a/2, y+a*sqrt(3)/2),
              (x-a/2, y+a*sqrt(3)/2),
              (x-a, y)]
    pg.draw.polygon(surface, colIn, points)
    pg.draw.polygon(surface, colOut, points, 4)

def inBounds(v, w, h):
    return (v.X >= 0 and v.X < h and\
            v.Y >= 0 and v.Y < w)

def DFS(start, grid, exit, player):
    w = len(grid[0])
    h = len(grid)
    Q = deque()
    Q.append(start)
    used = [[False for _ in range(w)] for __ in range(h)]
    while len(Q):
        cur = Q[-1]
        if exit(cur):
            return True
        used[cur.X][cur.Y] = True
        flag = False
        for m in moves:
            other = cur + m
            if (inBounds(other, w, h) and not used[other.X][other.Y])\
                and grid[other.X][other.Y] == player:
                Q.append(other)
                flag = True
                break
        if not flag:
            Q.pop()
    return False

def textRect(txt, size):
    font = pg.font.SysFont('Verdana', size)
    text = font.render(txt, False, BLACK)
    return text.get_rect()

def textOut(surface, data, size, col, pos):
    txt = str(data)
    font = pg.font.SysFont('Verdana', size)
    text = font.render(txt, False, col)
    rect = text.get_rect(center=pos)
    surface.blit(text, rect)

def textOutMultiline(surface, txt, size, col, pos):
    font = pg.font.SysFont('Verdana', size)
    for y, line in enumerate(txt.split('\n')):
        text = font.render(line, False, col)
        rect = text.get_rect(center=(pos[0], pos[1]+(y+5)*size))
        surface.blit(text, rect)
