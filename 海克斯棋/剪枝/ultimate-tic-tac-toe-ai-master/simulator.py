import math
import os
import pickle
import random
import sys
import time
from copy import deepcopy
from multiprocessing import Process, Lock, Queue, Manager, cpu_count, Value

from tqdm import tqdm

from GameMVC import Engine
from GameAgents import GeneticAlphaBetaAgent
from main import clear_screen


class Simulator:
    def __init__(self, num_gens=10, pop_size=30, max_matchups=30, num_parents=4, epsilon=0.1, parallel=False):
        self.engine = Engine()
        self.num_generations = num_gens
        self.population_size = pop_size
        self.max_matchups = max_matchups
        self.num_parents = num_parents
        self.epsilon = epsilon
        self.population = []
        self.matchups = []
        self.parents = []
        self.breed_method = 'randrange'
        self.neural = True
        self.genome_len = 488 if self.neural else 11
        self.gen_num = 0
        self.parallel = parallel
        self.num_matchups = 0
        self.elo_K = 32
        if self.parallel:
            self.manager = Manager()
            self.matchups = Queue()
            self.population = self.manager.list()

    def init_sample(self, genome):
        if self.parallel:
            return self.manager.dict({
                'genome': genome,
                'wins': 0,
                'games': 0,
                'record': 0,
                'elo': 1200
            })
        return {
            'genome': genome,
            'wins': 0,
            'games': 0,
            'record': 0,
            'elo': 1200
        }

    def handle_next_move(self, engine, agent1, agent2):
        move = None
        if engine.player == 1:
            move = agent1.compute_next_move()
        elif engine.player == 2:
            move = agent2.compute_next_move()
        engine.make_move(move)

    def build_random_sample(self):
        genome = []
        for i in range(self.genome_len):
            genome.append((random.randrange(0, 20000) - 10000) / 10000)
        # if self.parallel:
            # return self.manager.Sample(Sample(genome))
        return self.init_sample(genome)

    def build_breeded_sample(self):
        genome = [0 for i in range(self.genome_len)]
        p1 = self.parents.pop(random.randrange(0, len(self.parents)))
        p2 = self.parents[random.randrange(0, len(self.parents))]
        self.parents.append(p1)
        for i in range(self.genome_len):
            mutate = (random.randrange(0, 100) / 100) < self.epsilon
            p1_gene = p1['genome'][i]
            p2_gene = p2['genome'][i]
            if mutate:
                genome[i] = (random.randrange(0, 20000) - 10000) / 10000
            elif self.breed_method == 'randrange':
                try:
                    if p1_gene > p2_gene:
                        gene = (random.randrange(int(p2_gene * 10000 + 10000), int(p1_gene * 10000 + 10000)) - 10000) / 10000
                        genome[i] = gene
                    elif p1_gene < p2_gene:
                        gene = (random.randrange(int(p1_gene * 10000 + 10000), int(p2_gene * 10000 + 10000)) - 10000) / 10000
                        genome[i] = gene
                    elif p1_gene == p2_gene:
                        genome[i] = p1_gene
                except ValueError:
                    genome[i] = p1_gene
                # else:
                #     min_gene = p1_gene - 0.1 * p1_gene
                #     max_gene = p1_gene + 0.1 * p1_gene
                #     gene = random.randrange(int(min_gene * 1000), int(max_gene * 1000)) / 1000
                #     genome.append(gene)
            elif self.breed_method == 'weighted_sum':
                p1_weight = p1.record / (p1.record + p2.record)
                p2_weight = p2.record / (p1.record + p2.record)
                gene = p1_gene * p1_weight + p2_gene * p2_weight
                genome.append(gene)
            elif self.breed_method == 'interpolate':
                if random.randrange(0, 100) / 100 > 0.5:
                    genome.append(p1_gene)
                else:
                    genome.append(p2_gene)
        return self.init_sample(genome)

    def generate_population(self):
        self.population = []
        if self.parallel:
            self.population = Manager().list()
        if len(self.parents) == 0:
            for i in range(self.population_size):
                sample = self.build_random_sample()
                self.population.append(sample)
        else:
            for i in range(self.population_size):
                sample = self.build_breeded_sample()
                self.population.append(sample)

    def select_parents(self):
        self.population = sorted(self.population, key=lambda k: k['elo'], reverse=True)
        self.parents = self.population[:self.num_parents]

    def generate_matchups(self):
        def rotate(n):
            return n[1:] + n[:1]
        pop_size = len(self.population)
        matchups_per_sample = self.max_matchups if self.max_matchups < pop_size else pop_size-1
        matchups = []
        if pop_size % 2 == 0:
            for i in range(matchups_per_sample):
                for j in range(pop_size // 2):
                    if random.randrange(0, 100) / 100 > 0.5:
                        matchups.append((self.population[j], self.population[pop_size-1-j]))
                    else:
                        matchups.append((self.population[pop_size-1-j], self.population[j]))
                constant = self.population[0]
                self.population.pop(0)
                self.population = rotate(self.population)
                self.population = [constant, *self.population]

        else:
            for i in range(matchups_per_sample):
                for j in range(pop_size // 2):
                    matchups.append((self.population[j], self.population[pop_size-2-j]))
                self.population = rotate(self.population)
        self.matchups = matchups

    def generate_matchups_vs_parents(self):
        if len(self.parents) == 0:
            self.generate_matchups()
        else:
            matchups = []
            for i in range(self.population_size):
                for j in range(len(self.parents)):
                    matchups.append((self.parents[j], self.population[i]))
            self.matchups = matchups

    def generate_tournament_mactchups(self):
        matchups = []
        pop_size = len(self.population)
        if len(self.population) % 2 != 0:
            raise Exception(f'Invalid population size {pop_size}, must be 2^x for tournament matchups')
        for i in range(len(self.population)//2):
            if random.randrange(0, 100) / 100 > 0.5:
                matchups.append((self.population[i], self.population[pop_size-1-i]))
            else:
                matchups.append((self.population[pop_size-1-i], self.population[i]))
        self.matchups = matchups

    def play_match(self, matchup, lock=None):
        engine_copy = deepcopy(self.engine)
        if lock is not None:
            lock.acquire()
        p1 = matchup[0]
        p2 = matchup[1]
        if p1['genome'] != self.genome_len or p2['genome'] != self.genome_len:
            pass
        p1_agent = GeneticAlphaBetaAgent(engine_copy, 1, p1['genome'], allowed_depth=4, simulation=True)
        p2_agent = GeneticAlphaBetaAgent(engine_copy, 2, p2['genome'], allowed_depth=4, simulation=True)
        if lock is not None:
            lock.release()
        while engine_copy.game_state is None:
            self.handle_next_move(engine_copy, p1_agent, p2_agent)
        if lock is not None:
            lock.acquire()
        self.update_game_stats(p1, p2, engine_copy.game_state)
        self.update_elo(p1, p2, engine_copy.game_state)
        if lock is not None:
            lock.release()

    def update_game_stats(self, p1, p2, game_state):
        p1['games'] += 1
        p2['games'] += 1
        if game_state == 0:
            p1['wins'] += 0.5
            p2['wins'] += 0.5
        elif game_state == 1:
            p1['wins'] += 1
        else:
            p2['wins'] += 1
        p1['record'] = p1['wins'] / p1['games']
        p2['record'] = p2['wins'] / p2['games']

    def update_elo(self, p1, p2, game_state):
        p1_rating = 10**(p1['elo']/400)
        p2_rating = 10**(p2['elo']/400)
        p1_expected = p1_rating / (p1_rating + p2_rating)
        p2_expected = p2_rating / (p1_rating + p2_rating)
        p1_score = 1 if game_state == 1 else 0.5 if game_state == 0 else 0
        p2_score = 0 if game_state == 1 else 0.5 if game_state == 0 else 1
        p1['elo'] = int(p1['elo'] + self.elo_K * (p1_score - p1_expected))
        p2['elo'] = int(p2['elo'] + self.elo_K * (p2_score - p2_expected))

    def output_parent_genomes(self):
        if len(self.parents) == 0:
            return
        count = 1
        for parent in self.parents:
            print(f'Parent {count:02d}', end=': [ ')
            for gene in parent['genome']:
                print(f'{gene:06.3f}', end=' ]\t' if gene == parent['genome'][-1] else ', ')
            # print(f'Fitness (ELO): {parent["elo"]:04d}')
            print(f'Fitness (Wins): {parent["wins"]:04.1f}')
            count += 1

    def run(self):
        if self.parallel:
            self.run_parallel()
        else:
            for i in range(self.num_generations):
                print(f'\nBEGINNING GENERATION {i}')
                self.generate_population()
                self.generate_matchups()
                while len(self.matchups) != 0:
                    print(f'Simulations left: {len(self.matchups)} of gen. {i}')
                    matchup = self.matchups.pop()
                    self.play_match(matchup)
                self.select_parents()
                self.output_parent_genomes(i)

    def do_work(self, matchup_count, matchup_lock, update_lock):
        while True:
            if self.matchups.qsize() == 0:
                break
            matchup_lock.acquire()
            matchup_count.value += 1
            sys.stdout.write(f'\rSimulation {matchup_count.value:04d}/{self.num_matchups:04d} of Generation {self.gen_num:03d}')
            sys.stdout.flush()
            matchup = self.matchups.get()
            matchup_lock.release()
            self.play_match(matchup, update_lock)

    def save_parents(self):
        if len(self.parents) != 0:
            # print('Saving Genomes of parents')
            genomes = list(map(lambda p: p['genome'], self.parents))
            dir_path = os.path.dirname(os.path.realpath(__file__))
            if not os.path.exists(os.path.join(dir_path, 'scratch')):
                os.mkdir(os.path.join(dir_path, 'scratch'))
            pickle_path = os.path.join(dir_path, 'scratch/genome.p')
            with open(pickle_path, 'wb') as genome_file:
                pickle.dump(genomes, genome_file)

    def init_parents(self):
        if self.gen_num == 1:
            try:
                dir_path = os.path.dirname(os.path.realpath(__file__))
                pickle_path = os.path.join(dir_path, 'scratch/genome.p')
                with open(pickle_path, 'rb') as genome_file:
                    genomes = pickle.load(genome_file)
                for genome in genomes:
                    self.parents.append(self.init_sample(genome))
            except FileNotFoundError:
                self.parents = []

    def run_parallel(self):
        for gen_num in tqdm(range(1, self.num_generations + 1)):
            self.gen_num = gen_num
            time.sleep(1)
            print(f'\nBEGINNING GENERATION {gen_num:03d}')
            self.init_parents()
            self.output_parent_genomes()
            self.generate_population()
            self.generate_matchups()
            self.num_matchups = len(self.matchups)
            matchup_lock = Lock()
            update_lock = Lock()
            temp = self.matchups
            self.matchups = Queue()
            for m in temp:
                self.matchups.put(m)
            processes = []
            for i in range(cpu_count()):
                processes.append(Process(target=self.do_work, args=(matchup_lock, update_lock)))
                processes[i].start()
            for i in range(cpu_count()):
                processes[i].join()
            print('\n')
            self.select_parents()
            self.save_parents()
            clear_screen()

    def run_tournament(self):
        for gen_num in tqdm(range(1, self.num_generations + 1)):
            self.gen_num = gen_num
            time.sleep(1)
            print(f'\nBEGINNING GENERATION {gen_num:03d}')
            self.init_parents()
            # self.output_parent_genomes()
            self.generate_population()
            matchup_count = Value('i', 0)
            while len(self.population) > self.num_parents:
                self.generate_tournament_mactchups()
                self.num_matchups = sum([2**i for i in range(int(math.log2(self.num_parents)), int(math.log2(self.population_size)))])
                matchup_lock = Lock()
                update_lock = Lock()
                temp = self.matchups
                self.matchups = Queue()
                for m in temp:
                    self.matchups.put(m)
                processes = []
                for i in range(cpu_count()):
                    processes.append(Process(target=self.do_work, args=(matchup_count, matchup_lock, update_lock)))
                    processes[i].start()
                for i in range(cpu_count()):
                    processes[i].join()
                self.population = sorted(self.population, key=lambda k: k['wins'], reverse=True)
                self.population = self.population[:len(self.population)//2]
            self.parents = self.population[:self.num_parents]
            self.save_parents()
            clear_screen()


if __name__ == '__main__':
    sim = Simulator(num_gens=256, pop_size=256, max_matchups=32, num_parents=8, epsilon=0.1, parallel=True)
    sim.run_tournament()

