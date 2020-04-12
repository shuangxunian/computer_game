from abc import ABC, abstractmethod


class Agent(ABC):

    def __init__(self):
        pass

    @abstractmethod
    def compute_next_move(self, board, valid_moves):
        raise NotImplementedError
