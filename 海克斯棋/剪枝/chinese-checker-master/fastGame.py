# generate states
from game import ChineseChecker
from agent import *
import time
import os

_colHash = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1]

		
def fastGame(ccgame, agents):
	state = ccgame.startState() # current state
	max_iter = 200  # deal with some stuck situations
	iter = 0 
	fileName = './data/' +  '_'.join([str(time.time()),agents[1].name,agents[2].name])
	file = File(fileName)
	while (not ccgame.isEnd(state, iter)) and iter < max_iter:
		iter += 1
		player = ccgame.player(state)
		agent = agents[player]
		agent.getAction(state)
		legal_actions = ccgame.actions(state)
		if agent.action not in legal_actions:
			agent.action = random.choice(legal_actions)
		state = ccgame.succ(state, agent.action)
		file.write(state)
	file.close()
	if ccgame.isEnd(state, iter):
		winner = state[1].isEnd(iter)[1]
		file.rename(file.path + '_' + str(winner))
		return winner
	else:  # stuck situation
		print('stuck!')
		return 0
		
if __name__ == '__main__':
	win = 0
	for i in range(1000):
		ccgame = ChineseChecker(10, 4) # the shape of checker game
		agent1 = SimpleGreedyAgent(ccgame)
		agent2 = SimpleGreedyAgent(ccgame)
		agent1._init_()
		agent2._init_()

		result = fastGame(ccgame,{1: agent1, 2: agent2})
		win += (result == 1)
		print(i+1,win/(i+1))