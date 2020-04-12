import os
import tkinter as tk

from GameMVC import Controller


def clear_screen():
    os.system('cls' if os.name == 'nt' else 'clear')


def main():
    root = tk.Tk()
    app = Controller(root)
    app.bind_actions()
    root.after(1000, app.handle_next_move)
    root.geometry('396x378')
    root.title('Ultimate Tic Tac Toe')
    # root.config(menu=app.view.menubar)
    root.mainloop()


if __name__ == '__main__':
    main()
