import math
import numpy as np
from hex.HexGame import display

EPS = 1e-8

class MCTS():
    """
    This class handles the MCTS tree.
    """

    def __init__(self, game, nnet, args):
        self.game = game
        self.nnet = nnet
        self.args = args
        self.Qsa = {}       # stores Q values for s,a (as defined in the paper) 节点的价值
        self.Nsa = {}       # stores #times edge s,a was visited 此节点被访问的次数
        self.Ns = {}        # stores #times board s was visited 整个板上的节点被访问的次数
        self.Ps = {}        # stores initial policy (returned by neural net) 初始的策略

        self.Es = {}        # stores game.getGameEnded ended for board s
        self.Vs = {}        # stores game.getValidMoves for board s
        self.sim_count = 0

    def getActionProb(self, board, player, temp=1): # 动作预测
        """
        This function performs numMCTSSims simulations of MCTS starting from
        canonicalBoard.

        Returns:
            probs: a policy vector where the probability of the ith action is
                   proportional to Nsa[(s,a)]**(1./temp)
        """

        canonicalBoard = self.game.getCanonicalForm(board, player)

        for i in range(self.args.numMCTSSims):
            # print('================= start search player {} ================='.format(player))
            self.search(board, player)

        num = range(self.game.getActionSize())

        s = self.game.stringRepresentation(canonicalBoard)
        counts = [self.Nsa[(s,a)] if (s,a) in self.Nsa else 0 for a in num]
        # print(self.Nsa)
        # print(counts)
        """print(self.game.getActionSize())"""




        if temp==0:
            bestA = np.argmax(counts)
            probs = [0]*len(counts)
            probs[bestA]=1
            return probs

        counts = [x**(1./temp) for x in counts]
        probs = [x/float(sum(counts)) for x in counts]
        return probs


    def search(self, board, player):
        """
        This function performs one iteration of MCTS. It is recursively called
        till a leaf node is found. The action chosen at each node is one that
        has the maximum upper confidence bound as in the paper.
        此函数执行一次MCTS迭代。 递归调用直到找到叶节点。
        每个节点选择的动作是具有本文所述的最大置信上限。

        Once a leaf node is found, the neural network is called to return an
        initial policy P and a value v for the state. This value is propogated
        up the search path. In case the leaf node is a terminal state, the
        outcome is propogated up the search path. The values of Ns, Nsa, Qsa are
        updated.
        找到叶节点后，将调用神经网络返回一个初始策略P和状态的值v。
        此值已传播搜索路径。 如果叶节点为终端状态，则结果沿搜索路径传播。
        Ns，Nsa，Qsa的值将更新。

        NOTE: the return values are the negative of the value of the current
        state. This is done since v is in [-1,1] and if v is the value of a
        state for the current player, then its value is -v for the other player.
        注意：返回值是当前值的负数。 这样做是因为v在[-1,1]中，并且v是a的状态为当前播放器，则其他播放器的值为-v。

        Returns:
            v: the negative of the value of the current canonicalBoard
        """

        self.sim_count += 1

        canonicalBoard = self.game.getCanonicalForm(board, player)

        # print('MCTS board player', player)
        # display(board)
        # print('MCTS conn board')
        # display(canonicalBoard) 



        s = self.game.stringRepresentation(canonicalBoard)

        if s not in self.Es:
            self.Es[s] = self.game.getGameEnded(canonicalBoard, 1)
        if self.Es[s]!=0:
            # terminal node
            return -self.Es[s]

        if s not in self.Ps:
            # leaf node
            self.Ps[s], v = self.nnet.predict(canonicalBoard)
            valids = self.game.getValidMoves(canonicalBoard, 1)
            self.Ps[s] = self.Ps[s]*valids      # masking invalid moves
            sum_Ps_s = np.sum(self.Ps[s])
            if sum_Ps_s > 0:
                self.Ps[s] /= sum_Ps_s    # renormalize
            else:
                # if all valid moves were masked make all valid moves equally probable
                
                # NB! All valid moves may be masked if either your NNet architecture is insufficient or you've get overfitting or something else.
                # If you have got dozens or hundreds of these messages you should pay attention to your NNet and/or training process.   
                print("All valid moves were masked, do workaround.")
                self.Ps[s] = self.Ps[s] + valids
                self.Ps[s] /= np.sum(self.Ps[s])

            self.Vs[s] = valids
            self.Ns[s] = 0
            return -v

        valids = self.Vs[s]
        cur_best = -float('inf')
        best_act = -1

        num = range(self.game.getActionSize())
        """
        for a in range(self.game.getActionSize()):
            if (self.Nsa[(s,a)]<(self.Ns(s,a))):
                num = num -1
        """
        # pick the action with the highest upper confidence bound
        for a in num:
            if valids[a]:
                """if (s,a) in self.Qsa && :
                    u = self.Qsa[(s,a)] + self.args.cpuct*self.Ps[s][a]*math.sqrt(self.Ns[s])/(1+self.Nsa[(s,a)])"""

                if (s,a) in self.Qsa:
                    u = self.Qsa[(s,a)] + self.args.cpuct*self.Ps[s][a]*math.sqrt(self.Ns[s])/(1+self.Nsa[(s,a)])
                else:
                    u = self.args.cpuct*self.Ps[s][a]*math.sqrt(self.Ns[s] + EPS)     # Q = 0 ?

                if u > cur_best:
                    cur_best = u
                    best_act = a

        a = best_act
        """print(int(a/self.game.n), a%self.game.n)"""


        if valids[a]==0:
            print('invalid action in MCTS', a)
            assert valids[a] >0         

        next_s, _ = self.game.getNextState(canonicalBoard, 1, a)
        # print('conn after apply')
        # display(next_s)       
        # print('====================')       
        next_s = self.game.getOriginalForm(next_s, player)
        next_player = -player        

        # next_s = self.game.getCanonicalForm(next_s, next_player)

        v = self.search(next_s, next_player)

        if (s,a) in self.Qsa:
            self.Qsa[(s,a)] = (self.Nsa[(s,a)]*self.Qsa[(s,a)] + v)/(self.Nsa[(s,a)]+1)
            self.Nsa[(s,a)] += 1

        else:
            self.Qsa[(s,a)] = v
            self.Nsa[(s,a)] = 1

        self.Ns[s] += 1
        return -v
