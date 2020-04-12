import os
import pickle
import random
from copy import deepcopy

from GameAgents import AlphaBetaAgent
from GameAgents.evaluator import Evaluator, NeuralNetworkEvaluator
from GameAgents.minimax_agent import Node


class GeneticAlphaBetaAgent(AlphaBetaAgent):
    def __init__(self, engine, player, genome, compute_time=1, allowed_depth=None, simulation=False):
        super().__init__(engine, player, compute_time, allowed_depth)
        self.agent_type = 'Genetic AlphaBeta'
        if not simulation and genome is None:
            dir_path = os.path.dirname(os.path.dirname(os.path.realpath(__file__)))
            pickle_path = os.path.join(dir_path, 'scratch/genome.p')
            with open(pickle_path, 'rb') as genome_file:
                genomes = pickle.load(genome_file)
            genome = genomes[random.randrange(0, len(genomes))]
        self.evaluator = NeuralNetworkEvaluator(genome)
        self.simulation = simulation

    def compute_position_value(self, engine):
        return self.evaluator.eval(engine)

    def compute_next_move(self):
        self.root = Node(engine=deepcopy(self.engine))
        node, _ = self.minimax(self.root, self.depth, self.player == 1)
        if not self.simulation:
            print(f'{self.agent_type}: {chr(node.move.x + 97)}{node.move.y}, Board Eval: {round(node.value, 4)}')
        return node.move
