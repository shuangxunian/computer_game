# MCTS_Hex
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

**__Code in the directory should not be used for Vanderbilt University Advanced AI course at any time.__**

### Monte Carlo Tree Search & MiniMax Search

This project is the implementation of Hex game with both MCTS and MiniMax AI. The Monte Carlo Tree Search algorithm, specifically, the UCT algorithm is implemented according to Browne et al.'s [A Survey of Monte Carlo Tree Search Methods](http://mcts.ai/pubs/mcts-survey-master.pdf).

### Special Notes

The function for termination in this game is implemented with a customized union-find data structure, together with the definition **Virtual connections** mentioned in the paper [Search and evaluation in Hex](http://www.cs.cornell.edu/~adith/docs/y_hex.pdf).

For performance, (although Python is not particularly good for this problem) Hex game board is implemented as a dictionary whose key is a tuple (x,y), indicating the coordinate on the board, and whose value is a number, indicating whether a certain position is empty or taken by which player. A list of list implementation in Python would largely increase the run time.

### Prerequisite

```
matplotlib
shapely
descartes
```
