from Coach import Coach
from hex.HexGame import HexGame as Game
from hex.pytorch.NNet import NNetWrapper as nn
from utils import *

args = dotdict({
    'numIters': 1000,
    'numEps': 100,
    'tempThreshold': 15,
    'updateThreshold': 0.6,
    'maxlenOfQueue': 200000,
    'numMCTSSims': 25,
    'arenaCompare': 40,
    'cpuct': 1,

    'checkpoint': './pretrained_models/hex/pytorch/temp/',
    'load_model': True,
    'load_folder_file': ('./pretrained_models/hex/pytorch/dev/6x100','checkpoint_26.pth.tar'),
    'numItersForTrainExamplesHistory': 20,

})

if __name__=="__main__":
    g = Game(6)
    nnet = nn(g)

    if args.load_model:
        nnet.load_checkpoint(args.load_folder_file[0], args.load_folder_file[1])

    c = Coach(g, nnet, args)
    if args.load_model:
        print("Load trainExamples from file")
        c.loadTrainExamples()
    c.learn()
