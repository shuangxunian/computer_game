import time
from math import inf

from GameAgents import MinimaxAgent


class CacheObj:
    def __init__(self, val, depth, node=None):
        self.val = val
        self.depth = depth
        self.best = node 


class AlphaBetaAgent(MinimaxAgent):
    def __init__(self, engine, player, compute_time=1, allowed_depth=None):
        super().__init__(engine=engine, player=player)
        self.agent_type = 'AlphaBeta'
        self.compute_time = compute_time
        self.allowed_depth = allowed_depth
        self.cache = {}

    def minimax(self, node, depth, maximizing_player):
        start_time = time.time()
        max_depth = 1
        while True:
            child, value = self.alpha_beta(node, max_depth, -inf, inf, maximizing_player)
            if time.time() - start_time > self.compute_time:
                break
            if value == 10000 or value == -10000:
                break
            if self.allowed_depth is not None and max_depth == self.allowed_depth:
                break
            max_depth += 1
        return child, value

    def alpha_beta(self, node, depth, alpha, beta, maximizing_player):
        if depth == 0 or node.engine.game_state is not None:
            return node, node.value
        cache_keys = self.cache.keys()
        node_key = self.to_key(node.engine.board, node.engine.prev_move)
        moves = node.engine.get_valid_moves()
        if node_key in cache_keys and self.cache[node_key].depth >= depth:
            return self.cache[node_key].best, self.cache[node_key].val
        if maximizing_player:
            max_child = None
            max_value = -inf
            if node_key in self.cache.keys() and self.cache[node_key].depth == (depth-1):
                best_move = self.cache[node_key].best.move
                moves.remove(best_move)
                moves = [best_move, *moves]
            for move in moves:
                child = self.build_child(parent=node, move=move)
                _, child_max = self.alpha_beta(child, depth-1, alpha, beta, False)
                child.engine.undo_move(child.move)
                if child_max > max_value:
                    max_child = child
                    max_value = child_max
                    node.value = max_value
                if max_value >= beta:
                    break
                if max_value > alpha:
                    alpha = max_value
            if node_key not in cache_keys or self.cache[node_key].depth < depth:
                self.cache[node_key] = CacheObj(max_value, depth, max_child)
            return max_child, max_value
        else:
            min_child = None
            min_value = inf
            moves = node.engine.get_valid_moves()
            if node_key in self.cache.keys() and self.cache[node_key].depth == (depth-1):
                best_move = self.cache[node_key].best.move
                moves.remove(best_move)
                moves = [best_move, *moves]
            for move in moves:
                child = self.build_child(parent=node, move=move)
                _, child_min = self.alpha_beta(child, depth - 1, alpha, beta, True)
                child.engine.undo_move(child.move)
                if child_min < min_value:
                    min_child = child
                    min_value = child_min
                    node.value = min_value
                if min_value <= alpha:
                    break
                if min_value < beta:
                    beta = min_value
            if node_key not in cache_keys or self.cache[node_key].depth < depth:
                self.cache[node_key] = CacheObj(min_value, depth, min_child)
            return min_child, min_value

    def to_key(self, board, prev_move):
        return tuple(tuple(board[i]) for i in range(9)), prev_move.to_tuple()
