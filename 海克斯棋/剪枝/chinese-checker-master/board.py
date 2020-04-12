# uncompyle6 version 3.2.3
# Python bytecode 3.5 (3350)
# Decompiled from: Python 3.5.4 |Anaconda, Inc.| (default, Feb 19 2018, 10:59:04) 
# [GCC 7.2.0]
# Embedded file name: /Users/yuegao/Downloads/code_new-1/code_new/board.py
# Compiled at: 2018-09-29 23:56:17
# Size of source mod 2**32: 8833 bytes


class Board(object):
    def __init__(self, size, piece_rows):
        if not piece_rows < size:
            raise AssertionError
        self.size = size
        self.piece_rows = piece_rows
        self.board_status = {}
        for row in range(1, size + 1):
            for col in range(1, self.getColNum(row) + 1):
                if row <= piece_rows:
                    self.board_status[(row, col)] = 2
                else:
                    self.board_status[(row, col)] = 0

        for row in range(size + 1, size * 2):
            for col in range(1, self.getColNum(row) + 1):
                if row < size * 2 - piece_rows:
                    self.board_status[(row, col)] = 0
                else:
                    self.board_status[(row, col)] = 1

    def getColNum(self, row):
        if row in range(1, self.size + 1):
            return row
        else:
            return self.size * 2 - row

    def isEmptyPosition(self, pos):
        return self.board_status[pos] == 0

    def leftPosition(self, pos):
        row = pos[0]
        col = pos[1]
        if (row, col - 1) in list(self.board_status.keys()):
            return (row, col - 1)

    def rightPosition(self, pos):
        row = pos[0]
        col = pos[1]
        if (row, col + 1) in list(self.board_status.keys()):
            return (row, col + 1)

    def upLeftPosition(self, pos):
        row = pos[0]
        col = pos[1]
        if row <= self.size and (row - 1, col - 1) in list(self.board_status.keys()):
            return (row - 1, col - 1)
        if row > self.size and (row - 1, col) in list(self.board_status.keys()):
            return (row - 1, col)

    def upRightPosition(self, pos):
        row = pos[0]
        col = pos[1]
        if row <= self.size and (row - 1, col) in list(self.board_status.keys()):
            return (row - 1, col)
        if row > self.size and (row - 1, col + 1) in list(self.board_status.keys()):
            return (row - 1, col + 1)

    def downLeftPosition(self, pos):
        row = pos[0]
        col = pos[1]
        if row < self.size and (row + 1, col) in list(self.board_status.keys()):
            return (row + 1, col)
        if row >= self.size and (row + 1, col - 1) in list(self.board_status.keys()):
            return (row + 1, col - 1)

    def downRightPosition(self, pos):
        row = pos[0]
        col = pos[1]
        if row < self.size and (row + 1, col + 1) in list(self.board_status.keys()):
            return (row + 1, col + 1)
        if row >= self.size and (row + 1, col) in list(self.board_status.keys()):
            return (row + 1, col)

    def adjacentPositions(self, pos):
        result = []
        result.append(self.leftPosition(pos))
        result.append(self.rightPosition(pos))
        result.append(self.upLeftPosition(pos))
        result.append(self.upRightPosition(pos))
        result.append(self.downLeftPosition(pos))
        result.append(self.downRightPosition(pos))
        return [x for x in result if x is not None]

    def getPlayerPiecePositions(self, player):
        result1 = [(row, col) for row in range(1, self.size + 1) if self.board_status[(row, col)] == player]
        result2 = [(row, col) for row in range(self.size + 1, self.size * 2) if self.board_status[(row, col)] == player]
        return result1 + result2

    def getOneDirectionHopPosition(self, pos, dir_func):
        hop_over_pos = dir_func(pos)
        count = 0
        while hop_over_pos is not None:
            if self.board_status[hop_over_pos] != 0:
                break
            hop_over_pos = dir_func(hop_over_pos)
            count += 1

        if hop_over_pos is not None:
            target_position = dir_func(hop_over_pos)
            while count > 0:
                if target_position is None or self.board_status[target_position] != 0:
                    break
                target_position = dir_func(target_position)
                count -= 1

            if count == 0 and target_position is not None and self.board_status[target_position] == 0:
                return target_position

    def getOneHopPositions(self, pos):
        result = []
        result.append(self.getOneDirectionHopPosition(pos, self.leftPosition))
        result.append(self.getOneDirectionHopPosition(pos, self.rightPosition))
        result.append(self.getOneDirectionHopPosition(pos, self.upLeftPosition))
        result.append(self.getOneDirectionHopPosition(pos, self.upRightPosition))
        result.append(self.getOneDirectionHopPosition(pos, self.downLeftPosition))
        result.append(self.getOneDirectionHopPosition(pos, self.downRightPosition))
        return [x for x in result if x is not None]

    def getAllHopPositions(self, pos):
        result = self.getOneHopPositions(pos)
        start_index = 0
        while start_index < len(result):
            cur_size = len(result)
            for i in range(start_index, cur_size):
                for new_pos in self.getOneHopPositions(result[i]):
                    if new_pos not in result:
                        result.append(new_pos)

            start_index = cur_size

        if pos in result:
            result.remove(pos)
        return result

    def ifPlayerWin(self, player, iter):
        if player == 1:
            for row in range(1, self.piece_rows + 1):
                for col in range(1, self.getColNum(row) + 1):
                    if self.board_status[(row, col)] == 1:
                        continue
                    else:
                        if iter > 100 and self.board_status[(row, col)] == 2:
                            return True
                        return False

        else:
            for row in range(self.size * 2 - self.piece_rows, self.size * 2):
                for col in range(1, self.getColNum(row) + 1):
                    if self.board_status[(row, col)] == 2:
                        continue
                    else:
                        if iter > 100 and self.board_status[(row, col)] == 1:
                            return True
                        return False

        return True

    def isEnd(self, iter):
        player_1_reached = self.ifPlayerWin(1, iter)
        player_2_reached = self.ifPlayerWin(2, iter)
        if player_1_reached:
            return (True, 1)
        if player_2_reached:
            return (True, 2)
        return (False, None)

    def printBoard(self):
        for row in range(1, self.size + 1):
            print(' ' * (self.size - row), end=' ')
            for col in range(1, self.getColNum(row) + 1):
                print(str(self.board_status[(row, col)]), end=' ')

            print('\n', end=' ')

        for row in range(self.size + 1, self.size * 2):
            print(' ' * (row - self.size), end=' ')
            for col in range(1, self.getColNum(row) + 1):
                print(str(self.board_status[(row, col)]), end=' ')

            print('\n', end=' ')

    def printBoardOriginal(self):
        for row in range(1, self.size + 1):
            for col in range(1, self.getColNum(row) + 1):
                print(str(self.board_status[(row, col)]), end=' ')

            print('\n', end=' ')

        for row in range(self.size + 1, self.size * 2):
            for col in range(1, self.getColNum(row) + 1):
                print(str(self.board_status[(row, col)]), end=' ')

            print('\n', end=' ')
# okay decompiling board.pyc
