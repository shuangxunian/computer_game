import time

from GameMVC import *
from GameAgents import *


class Controller:
    def __init__(self, root):
        self.model = Engine()
        self.view = View(root)
        self.view.pack(fill='both', expand=True)
        self.player1 = GeneticAlphaBetaAgent(engine=self.model, genome=None, player=1, compute_time=1)
        self.player2 = None  # AlphaBetaAgent(engine=self.model, player=2)
        self.write_moves = False
        self.move_list = []
        self.simulate = False
        self.delay = 100

    def check_game_over(self):
        if self.model.game_state is not None:
            if self.model.game_state != 0:
                self.view.popup_msg(f'Player {self.model.player} won', self.restart_game)
            else:
                self.view.popup_msg('The game ended in a draw', self.restart_game)

    def handle_next_move(self):
        if self.model.player == 1:
            if self.player1 is not None:
                self.handle_agent_move(agent=self.player1)
        elif self.model.player == 2:
            if self.player2 is not None:
                self.handle_agent_move(agent=self.player2)

    def handle_click(self, event):
        x = ord(event.widget['text'][0]) - 65
        y = int(event.widget['text'][1])
        move = Coord(x, y)
        if self.model.check_valid_move(move):
            self.handle_move(move)

    def handle_agent_move(self, agent, event=None):
        move_time = time.time()
        move = agent.compute_next_move()
        print(f'Move computation took {round(time.time() - move_time, 4)}s\n')
        self.handle_move(move)

    def handle_move(self, move):
        self.model.make_move(move)
        self.view.render_board(self.model.board)
        self.check_game_over()
        self.view.after(self.delay, self.handle_next_move)

    def output_move(self, move):
        # output and record moves
        move_code = chr(move.x + 97) + str(move.y)
        if self.list_moves:
            print(move_code)
        if self.write_moves:
            self.move_list.append(move_code)
            if self.model.game_state is not None:
                print(self.move_list)

    def handle_enter(self, event):
        x = ord(event.widget['text'][0]) - 65
        y = int(event.widget['text'][1])
        hover_coord = Coord(x, y)
        
        if self.model.check_valid_move(hover_coord):
            colors_to_use = self.view.valid_colors
        else:
            colors_to_use = self.view.invalid_colors
            
        if event.widget['bg'] == self.view.colors[0]:
            event.widget.configure(bg=colors_to_use[0])
        elif event.widget['bg'] == self.view.colors[1]:
            event.widget.configure(bg=colors_to_use[1])

    def handle_leave(self, event):
        if event.widget['bg'] in [self.view.valid_colors[0], self.view.invalid_colors[0]]:
            event.widget.configure(bg=self.view.colors[0])
        elif event.widget['bg'] in [self.view.valid_colors[1], self.view.invalid_colors[1]]:
            event.widget.configure(bg=self.view.colors[1])

    def restart_game(self):
        self.model.reset_game()
        self.view.reset_board()
        self.view.after(self.delay, self.handle_next_move)

    def bind_actions(self):
        for i in range(9):
            for j in range(9):
                self.view.board_spaces[i][j].bind("<Enter>", self.handle_enter)
                self.view.board_spaces[i][j].bind("<Leave>", self.handle_leave)
                self.view.board_spaces[i][j].bind("<Button-1>", self.handle_click)
