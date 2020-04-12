# Agent BEGIN
class MEADOWLARKAgent():
	def __init__(self,game):
		import pickle
		import time
		self.game = game
		self.name = "BMEAgent"	
		self.startTime = 0
		self.timeLimit = 1.96
		# loads endGame
		self._colHash = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1]
		
		# BG
		self.BGHash1 = self._hashBGState(self.game.startState(),player = 1)
		self.BGHash2 = self._hashBGState(self.game.startState(),player = 2)
		self.iteration = 0
		self.intree = True
		with open('./model/MGValue.mct', 'br') as f:
			self.MGValue = pickle.load(f)
		with open('./model/MGVisitedTimes.mct', 'br') as f:
			self.MGVisitedTimes = pickle.load(f)		
			
		# MG
		# loads model 
		from keras.models import load_model
		self.model = load_model("./model/model_exp.h5")
		self.game = game
		
		
		# EG
		with open('./model/EGPA.meadowlark', 'br') as f:
			self.endgameParentAction = pickle.load(f)
		with open('./model/EGV.meadowlark', 'br') as f:
			self.endgameValue = pickle.load(f)
		print("EG states load: ",len(self.endgameValue))
	def _hashBGState(self,state,player):
		status = state[1].board_status
		hashVal = ""
		if player == 1:
			for row in range(15,20):
				for col in range(1,self._colHash[row] + 1):
					hashVal+=str(int(status[(row,col)]))
		elif player == 2:
			for row in range(1,6):
				for col in range(1,self._colHash[row] + 1):
					hashVal+=str(int(status[(row,col)]))			
		else:
			print("Something Wrong")
		return hashVal		
	def _hashEGState(self,status,player = 1,gaming = False):
		if player == 1 and not gaming or player == 2 and gaming:
			nMarbles = 0
			nRow = 1
			for row in range(19,0,-1):
				for col in range(1,self._colHash[row] + 1):
					if status[(row,col)] == player:
						nMarbles+=1
					elif status[(row,col)] == (3 - player):
						return False
				if nMarbles == 10:
					nRow = row
					break
			hashVal = ""
			for row in range(19,nRow-1,-1):
				for col in range(1,self._colHash[row] + 1):
					hashVal+=str(int(status[(row,col)] == player))
			return hashVal
		else:
			nMarbles = 0
			nRow = 19
			for row in range(1,20):
				for col in range(1,self._colHash[row] + 1):
					if status[(row,col)] == player:
						nMarbles+=1
					elif status[(row,col)] == (3 - player):
						return False
				if nMarbles == 10:
					nRow = row
					break
			hashVal = ""
			for row in range(1,nRow+1):
				for col in range(1,self._colHash[row] + 1):
					hashVal+=str(int(status[(row,col)] == player) + 2)
			return hashVal		
	def _hashMGState(self,state):
		hashList = []
		status = state[1].board_status
		for row in range(1, 20):
			for col in range(1,self._colHash[row] + 1):
				hashList.append(str(status[(row,col)]))
		return ','.join(hashList)
	def model_predict(self,x):
		import numpy as np
		x1_t = []
		x2_t = []
		for t in range(len(x)):
			if x[t] == '1':
				x1_t.append(1)
				x2_t.append(0)
			elif x[t] == '2':
				x1_t.append(0)
				x2_t.append(1)
			else:
				x1_t.append(0)
				x2_t.append(0)
		x1_t = np.array(x1_t).astype('float32').reshape((-1,100))
		x2_t = np.array(x2_t).astype('float32').reshape((-1,100))
		val = self.model.predict({'input_x1': x1_t, 'input_x2': x2_t})
		return val[0][0]			
	def evaluate(self,state):
		status = state[1].board_status
		result =0
		
		val = 0
		# Simple Val
		# Assume the player is the bottom one
		for row in range(1, 20):
			for col in range(1,self._colHash[row] + 1):				
				if status[(row,col)] == 1: # friend
					val += (20 - row)
				elif status[(row,col)] == 2: # enemy
					val -= row
		if self.player_ID == 1:
			result += val
		else:
			result += (-val)
		
		# DNN Val
		stateList = []
		for row in range(1, 20):
			for col in range(1,self._colHash[row] + 1):
				stateList.append(str(status[(row,col)]))
		val = self.model_predict(stateList)
		
		if self.player_ID == 1:
			result += val*2
		else:
			result += (1 - val)*2
		
		return result		
	def dnnEval(self,state):
		status = state[1].board_status
		stateList = []
		for row in range(1, 20):
			for col in range(1,self._colHash[row] + 1):
				stateList.append(str(status[(row,col)]))
		val = self.model_predict(stateList)
		
		if self.player_ID == 1:
			return val
		else:
			return 1 - val
	def expand(self,state):
		import random
		player = state[0]
		legal_actions = self.game.actions(state)
		expand_actions = []
		for action in legal_actions:
			if player == 1 and action[0][0] - action[1][0] > 0:
				expand_actions.append(action)
			elif player == 2 and action[1][0] - action[0][0] > 0:
				expand_actions.append(action)	
		
		random.shuffle(expand_actions)
		
		return expand_actions		
	def maxLayer(self, alpha, beta, depth, state):
		import time
		if time.time() - self.startTime >= self.timeLimit:
			return 0,"TIMEOUT"
		if depth == 0:
			result = self.evaluate(state)
			return result, None
			
		if state[1].isEnd(1)[0]:
			return float('-inf'), None
			
		bestAction = None
		v = float('-inf')
		
		expand_actions = self.expand(state)
		
		for action in expand_actions:
			next_state = self.game.succ(state, action)
			min_result, _timeoutToken_ = self.minLayer(alpha, beta, depth - 1, next_state)
			if _timeoutToken_ == "TIMEOUT":
				return 0,"TIMEOUT"
			if v < min_result:
				v = min_result
				bestAction = action
				alpha = max(alpha, v)
			if beta <= alpha:
				break
		return v, bestAction
		
	def minLayer(self, alpha, beta, depth, state):
		import time
		if time.time() - self.startTime >= self.timeLimit:
			return 0,"TIMEOUT"
		if depth == 0:
			result = self.evaluate(state)
			return result, None
			
		if state[1].isEnd(1)[0]:
			return float('inf'), None
		
		bestAction = None
		expand_actions = self.expand(state)
		v = float('inf')
		
		for action in expand_actions:
			next_state = self.game.succ(state, action)
			max_result, _timeoutToken_ = self.maxLayer(alpha, beta, depth - 1, next_state)
			if _timeoutToken_ == "TIMEOUT":
				return 0,"TIMEOUT"
			if v > max_result:
				v = max_result
				bestAction = action
				beta = min(beta, v)
			if beta <= alpha:
				break
		return v, bestAction	
		
	def getAction(self,state):
		import random
		import time
		self.startTime = time.time()
		self.player_ID = state[0]
		
		# Decide whether it is the beginning of a new game
		hashBG = self._hashBGState(state,player = self.player_ID)
		if self.player_ID == 1 and hashBG == self.BGHash1:
			print("Start Game Detected. ")
			self.intree = True
			self.iteration = 1
		elif self.player_ID == 2 and hashBG == self.BGHash2:
			print("Start Game Detected. ")
			self.intree = True
			self.iteration = 2
		else:
			self.iteration += 2
		print("iter,",self.iteration)	
		
		# EndGame
		hashEG = self._hashEGState(state[1].board_status,player = state[0],gaming = True)
		if hashEG != False and self.endgameParentAction.get(hashEG,None) != None:
			new_pos, old_pos = self.endgameParentAction[hashEG]
			print("endgameValue",self.endgameValue[hashEG])
			self.action = (old_pos,new_pos)
			return
			
		# MiddleGame
		legal_actions = self.game.actions(state)
		self.action = random.choice(legal_actions)
		expand_actions = []
		for action in legal_actions:
			if self.player_ID == 1 and action[0][0] - action[1][0] >= 0:
				expand_actions.append(action)
			elif self.player_ID == 2 and action[1][0] - action[0][0] >= 0:
				expand_actions.append(action)
		self.action = random.choice(expand_actions)			
		
		if self.intree:
			print("Still in MCTS")
			hashMG = self._hashMGState(state)
			
			flagFound = False
			bestVal = float("-inf")
			
			for action in expand_actions:
				next_state = self.game.succ(state,action)
				hashNextState = self._hashMGState(next_state)
				
				if self.MGVisitedTimes[self.player_ID].get(hashNextState,None) != None and	self.MGVisitedTimes[self.player_ID][hashNextState] >= 10 and self.MGValue[self.player_ID][hashNextState] > 0.5:
					if bestVal < self.MGValue[self.player_ID][hashNextState]:
						bestVal = self.MGValue[self.player_ID][hashNextState]
						flagFound = True
						self.action = action
					
			if flagFound:
				print("mctVal", bestVal)
			else:
				self.intree = False
				
		if not self.intree:
			print("RMA")
			# Minimax
			for depth in [1,2,3,4,5]:
				_, bestAction = self.maxLayer(float('-inf'), float('inf'), depth , state)
				if bestAction == "TIMEOUT":
					print(depth - 1, time.time() - self.startTime)
					print("RMA Depth",depth - 1)
					break
				else:
					self.action = bestAction
					
		
		next_state = self.game.succ(state,self.action)
		dnnVal = self.dnnEval(next_state)
		dnnVal = dnnVal - int(dnnVal)
		if self.player_ID == 1:
			print("dnnVal",dnnVal)
		else:
			print("dnnVal",1 - dnnVal)
			
# Agent End