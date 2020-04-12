from GameAgents import Agent
from math import sqrt, log
from random import randrange
from copy import deepcopy


class Node:
    def __init__(self, parent=None, player=None):
        self.move = None
        self.player = player
        self.parent = parent
        self.children = []
        self.plays = 0
        self.value = 0
        self.depth = 0


class MonteCarloAgent(Agent):
    def __init__(self, engine, max_sim=500, confidence=sqrt(2)):
        self.tree_root = Node()
        self.cur_node = self.tree_root
        self.total_sims = 0
        self.c = confidence
        self.phase = None
        self.engine = engine
        self.max_sim = max_sim

    # Upper Confidence Bound 1 applied to Trees (UCT)
    def uct(self, node):
        return node.value / node.plays + self.c * sqrt(log(node.parent.plays) / node.plays)

    def children_moves(self, node):
        return list(map(lambda child: (child.move.x, child.move.y), node.children))

    def select(self, valid_moves):
        if len(self.cur_node.children) != len(valid_moves):
            return self.expand(valid_moves)

        max_child = self.cur_node.children[0]
        max_uct = self.uct(max_child)
        for child in self.cur_node.children:
            if self.uct(child) > max_uct:
                max_uct = self.uct(child)
                max_child = child
        self.cur_node = max_child
        return self.cur_node.move

    def expand(self, valid_moves):
        move = None
        if len(self.cur_node.children) == 0:
            move = valid_moves[randrange(0, len(valid_moves))]
        else:
            while move is None or (move.x, move.y) in self.children_moves(self.cur_node):
                move = valid_moves[randrange(0, len(valid_moves))]

        child = Node(parent=self.cur_node)
        child.move = move
        child.depth = self.cur_node.depth + 1
        child.player = 1 if child.parent.player == 2 else 2
        self.cur_node.children.append(child)
        self.cur_node = child
        self.phase = 'Simulation'
        return self.cur_node.move

    def simulate(self, valid_moves):
        return valid_moves[randrange(0, len(valid_moves))]

    # -1 -> p2 win, 0 -> tie, 1 -> p1 win
    def update(self, game_result):
        self.total_sims += 1
        while self.cur_node is not None:
            self.cur_node.plays += 1
            if game_result == 0:
                self.cur_node.value += 0.5
            elif game_result == 1 and self.cur_node.player == 1:
                self.cur_node.value += 1
            elif game_result == -1 and self.cur_node.player == 2:
                self.cur_node.value += 1
            self.cur_node = self.cur_node.parent

    def play(self):
        max_child = None
        max_avg_val = 0
        for child in self.cur_node.children:
            child_avg_val = (child.value / child.plays)
            if child_avg_val > max_avg_val:
                max_avg_val = child_avg_val
                max_child = child
        print(f'MonteCarlo Move: {chr(max_child.move.x + 97)}{max_child.move.y}, Val/Plays: {round(max_avg_val, 4)}')
        self.tree_root = max_child
        self.tree_root.parent = None
        return self.tree_root.move

    def reset_agent(self, phase=None):
        self.phase = phase
        self.cur_node = self.tree_root

    def reset_tree(self):
        player = 1 if self.engine.player != 1 else 2
        self.tree_root = Node(player=player)
        self.cur_node = self.tree_root

    def compute_next_move(self):
        self.reset_tree()
        self.run_simulations()
        return self.play()

    def get_simulation_move(self, valid_moves):
        if self.phase == 'Selection':
            return self.select(valid_moves)
        elif self.phase == 'Simulation':
            return self.simulate(valid_moves)

    def run_simulations(self):
        sim_count = 0
        while sim_count < self.max_sim:
            sim_count += 1
            engine_copy = deepcopy(self.engine)
            self.reset_agent('Selection')
            while engine_copy.game_state is None:
                move = self.get_simulation_move(engine_copy.get_valid_moves())
                engine_copy.make_move(move)
            self.update(engine_copy.game_state)
        self.reset_agent()
