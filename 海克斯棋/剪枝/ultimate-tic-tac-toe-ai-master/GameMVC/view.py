import tkinter as tk
from GameMVC import Coord


class View(tk.Frame):
    def __init__(self, parent):
               
        tk.Frame.__init__(self, parent)

        self.colors = ('gainsboro', 'gray58')
        self.valid_colors = ('SeaGreen2', 'SpringGreen3')
        self.p1_colors = ('brown1', 'firebrick3')
        self.p2_colors = ('RoyalBlue1', 'SlateBlue3')
        self.invalid_colors = ('gray15', 'gray10')

        # creating game board interface
        self.board_spaces = [[tk.Label(self, text=chr(65+i)+str(j), font='Courier', bg=(self.colors[0] if i%2==0 else self.colors[1]), width=4, height=2) for j in range(9)] for i in range(9)]
        self.create_board_ui()
        # self.after(10000, self.create_settings_ui)

    def create_board_ui(self):
        for i in range(9):
            for j in range(9):
                k, l = self.convert_board_index(i, j)
                self.board_spaces[i][j].grid(row=k, column=l)

    def clear_board_ui(self):
        for i in range(9):
            for j in range(9):
                self.board_spaces[i][j].grid_forget()

    def create_settings_ui(self):
        self.clear_board_ui()

    def convert_board_index(self, i, j):
        # calculation of converted row idx
        k = (6 if i > 5 else (3 if i > 2 else 0)) + (2 if j > 5 else (1 if j > 2 else 0))
        # calculation of converted col idx
        l = 3 * (i % 3) + (j % 3)
        return k, l

    def reset_board(self):
        for i in range(9):
            for j in range(9):
                color_idx = 1
                if i % 2 == 0:
                    color_idx = 0
                self.board_spaces[i][j].configure(bg=self.colors[color_idx])

    def update_visuals(self, coord, player):
        active_tile = self.board_spaces[coord.x][coord.y]
        if active_tile['bg'] not in self.p1_colors and active_tile['bg'] not in self.p2_colors:
            color_idx = 1
            if active_tile['bg'] == self.valid_colors[0] or active_tile['bg'] == self.colors[0]:
                color_idx = 0
            if player == 1:
                active_tile.configure(bg=self.p1_colors[color_idx])
            else:
                active_tile.configure(bg=self.p2_colors[color_idx])

    def render_board(self, board):
        for i in range(len(board)):
            for j in range(len(board[i])):
                if board[i][j] is not None:
                    coord = Coord(i, j)
                    player = board[i][j]
                    self.update_visuals(coord, player)

    def popup_msg(self, msg, func=None):
        def popup_func():
            popup.destroy()
            if func is not None:
                func()
        popup = tk.Tk()
        popup.wm_title()
        label = tk.Label(popup, text=msg)
        label.pack(side='top', fill='x', pady=10)
        btn = tk.Button(popup, text='Okay', command=popup_func)
        btn.pack()
        popup.mainloop()
