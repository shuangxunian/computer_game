import random
from .agent import Agent


class RandomAgent(Agent):
    def __init__(self, engine):
        self.engine = engine
    
    def compute_next_move(self):
        valid_moves = self.engine.get_valid_moves()
        if len(valid_moves) > 1:
            return valid_moves[random.randint(0, len(valid_moves)-1)]
        elif len(valid_moves) == 1:
            return valid_moves[0]
