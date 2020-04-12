from .coord import Coord


class Engine:
    def __init__(self):
        self.board = [[None for i in range(9)] for j in range(9)]
        self.master_board = [None for i in range(9)]  # contains the win states of all mini boards
        self.prev_move = Coord(None, None)
        self.moves = []
        self.player = 1
        self.game_state = None

    @staticmethod
    def check_board_win(board):
        # checking verticals
        for idx in [0, 3, 6]:
            if board[idx] is not None:
                if board[idx] == board[idx + 1] and board[idx] == board[idx + 2]:
                    return True
        # checking horizontals
        for idx in [0, 1, 2]:
            if board[idx] is not None:
                if board[idx] == board[idx + 3] and board[idx] == board[idx + 6]:
                    return True
        # checking diagonals
        if board[4] is not None:
            if board[0] == board[4] and board[0] == board[8]:
                    return True
            if board[2] == board[4] and board[2] == board[6]:
                    return True
        return False

    def update_master_board(self, move=None):
        if move is None:
            move = self.prev_move
        if self.check_board_win(self.board[move.x]):
            self.master_board[move.x] = self.player
        else:
            self.master_board[move.x] = None

    def update_game_state(self):
        def player_won():
            if self.check_board_win(self.master_board):
                return True
            return False
        
        if player_won():
            self.game_state = 1 if self.player == 1 else -1
        elif len(self.get_valid_moves()) == 0:
            self.game_state = 0
        else:
            self.game_state = None

    def check_valid_move(self, curr_move):
        if curr_move is None:
            return False

        if self.check_board_win(self.master_board):
            return False

        if self.prev_move.x is not None:
            if self.prev_move.y != curr_move.x:
                if self.master_board[self.prev_move.y] is None:
                    return False
            
        try:
            if self.board[curr_move.x][curr_move.y] is not None:
                return False
            if self.master_board[curr_move.x] is not None:
                return False
        except IndexError:
            return False

        return True

    def get_valid_moves(self):
        valid_moves = []
        if self.prev_move.x is None and self.prev_move.y is None:
            for i in range(9):
                for j in range(9):
                    valid_moves.append(Coord(i, j))
        elif self.master_board[self.prev_move.y] is None and None in self.board[self.prev_move.y]:
            for j in range(9):
                move = Coord(self.prev_move.y, j)
                if self.check_valid_move(move):
                    valid_moves.append(move)
        else:
            for i in range(9):
                if self.master_board[i] is None:
                    for j in range(9):
                        move = Coord(i, j)
                        if self.check_valid_move(move):
                            valid_moves.append(move)
        return valid_moves

    def update_player(self):
        if self.player == 1:
            self.player = 2
        else:
            self.player = 1

    def make_move(self, move):
        if self.check_valid_move(move):
            # movement updates
            self.board[move.x][move.y] = self.player
            self.moves.append(move)
            self.prev_move = move
            # game state updates
            self.update_master_board()
            self.update_game_state()
            if self.game_state is None:
                self.update_player()
            return 1
        return None

    def undo_move(self, move=None):
        if move is None:
            move = self.prev_move
        if move == Coord(None, None) or len(self.moves) == 0:
            return
        self.board[move.x][move.y] = None
        self.update_master_board(move)
        if self.game_state is None:
            self.update_player()
        self.update_game_state()
        # hard coded for the last move
        self.moves.pop(-1)
        self.prev_move = self.moves[-1] if len(self.moves) > 0 else Coord(None, None)

    def reset_game(self):
        self.board = [[None for i in range(9)] for j in range(9)]
        self.master_board = [None for i in range(9)]
        self.prev_move = Coord(None, None)
        self.player = 1
        self.game_state = None
