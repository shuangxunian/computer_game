package AI;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Random;

import publicClass.XYTQ;

public class GameTree {
	private Node root;
	private XYTQ chess;
	private Boolean thinking;
	public int nodeCounter=0;

	public Boolean getThinking() {
		return thinking;
	}

	public GameTree(char[][] layout, char role) {
		root = new Node(layout, role);
		chess = new XYTQ();
		thinking = true;
		Runnable build = () -> {
			Node TopNodeParent = new Node(null, role);
			TopNodeParent.score = root.isPlayerA()?Integer.MAX_VALUE:Integer.MIN_VALUE;
			build(Config.AI_DEPTH, root, TopNodeParent);
			System.out.println("Game Tree built");
			//printChildCount(root,1);
			thinking = false;
		};
		new Thread(build).start();
	}
	

	private void printChildCount(Node root2, int deep) {
		// TODO Auto-generated method stub
		if(root2.nodeList.size()==0){
			System.out.println("end node at deep:"+deep);
		}
		//System.out.println("deep: "+deep+", count: " + root2.nodeList.size());
		for(Node child : root2.nodeList)
		{
			printChildCount(child, deep+1);
		}
	}

	/**
	 * @return the move the computer should make
	 */
	public List<String> bestMove() {
		if (root.nodeList.isEmpty()) {
			return null;
		}
		List<Node> someNodeList = new LinkedList<Node>();
		for (Node node : root.nodeList) {
			if (node.score == root.score) {
				someNodeList.add(node);
			}
		}
//		if (root.isPlayerA()) {
//			for (Node node : root.nodeList) {
//				if (maxmin.score == node.score) {
//					maxmin = node;
//				}
//			}
//		} else {
//			for (Node node : root.nodeList) {
//				if (maxmin.score == node.score) {
//					maxmin = node;
//				}
//			}
//		}
		Node bestNode = someNodeList.get(new Random().nextInt(someNodeList.size()));
		if (Config.DEBUG) {
			System.out.println("Move to use: " + bestNode.actionMove);
		}
		return bestNode.actionMove;
	}

	/**
	 * @param layersDeep
	 *            number of layers to check (-1 will fully build tree)
	 * @param node
	 *            node being looked at right now
	 */
	public final void build(int layersDeep, Node node, Node parentNode) {
		if (node.checkWin() == 0 && layersDeep != 0) {
			if (node.nodeList.isEmpty()) {
				HashMap<List<String>, char[][]> x = chess.NextLayout(node.layout, node.role);
				for (List<String> am : x.keySet()) {
					Node newNode = new Node(x.get(am), am, node.army);
					node.nodeList.add(newNode);
					//递归构建树
					if ((node.isPlayerA() && parentNode.score > node.score)
							|| (!node.isPlayerA() && parentNode.score < node.score)) {
						build(layersDeep - 1, newNode, node);
						nodeCounter++;
					} else {
						if (Config.DEBUG) {
							System.out.println("剪枝: " + node);
						}
						node.nodeList.remove(newNode);
						break;
					}

					if (node.isPlayerA()) {
						if (node.score < newNode.score) {
							node.score = newNode.score;
						}
					} else {
						if (node.score > newNode.score) {
							node.score = newNode.score;
						}
					}
				}
			}
			if (node.isPlayerA()) {
				if (node.score < parentNode.score) {
					parentNode.score = node.score;
				}
				
			} else {
				if (node.score > parentNode.score) {
					parentNode.score = node.score;
				}
			}
		} else {
			node.score = gujiaFun(node);
		}
		if (Config.DEBUG) {
			System.out.println("结束于 " + node + " 还有 " + layersDeep + " 层");
		}
	}

	public final int gujiaA(Node node) {
		int score = 0;
		int winner= node.checkWin();// 1:'a' -1:'b'
		if (winner != 0) {
			score = Config.GAME_OVER * winner;
		} else {
			int[] PiceCount = chess.getPieceCount(node.layout);
			score += PiceCount[0] * Config.SOlDIER_WORTH + PiceCount[1] * Config.KING_WORTH;
			score -= PiceCount[2] * Config.SOlDIER_WORTH + PiceCount[3] * Config.KING_WORTH;
			//score += (node.isPlayerA()?-1:1)*Config.MutilJump;
		}
		return score;
	}

	public int gujiaFun(Node node) {
		return gujiaA(node) + chess.NextLayout(node.layout, 'a').keySet().size()
				- chess.NextLayout(node.layout, 'b').keySet().size();
	}

	public boolean isThinking() {
		return thinking;
	}

	private class Node {
		final ArrayList<Node> nodeList;
		final char[][] layout;
		final List<String> actionMove;
		final char role;
		final char army;
		final XYTQ chessStatic;
		int score;

		Node(char[][] layout, char role) {
			this(layout, null, role);
		}

		public int checkWin() {
			if (chessStatic.NextLayout(layout, 'b').keySet().size() == 0) {
				return 1;
			} else if (chessStatic.NextLayout(layout, 'a').keySet().size() == 0) {
				return -1;
			} else
				return 0;
		}

		Node(char[][] layout, List<String> actionMove, char role) {
			this.layout = layout;
			this.actionMove = actionMove;
			this.role = role;
			army = (role == 'a' ? 'b' : 'a');
			chessStatic = new XYTQ();
			nodeList = new ArrayList<>();
			score = isPlayerA() ? Integer.MIN_VALUE : Integer.MAX_VALUE;
		}

		public Boolean isPlayerA() {
			return this.role == 'a';
		}

		public final String toString() {
			return actionMove == null ? "no move" : actionMove.toString() + ", score " + score;
		}
	}
	public static void main(String[] args) throws InterruptedException {
		// TODO Auto-generated method stub
		char[][] now = new char[][]{
			 {'0','a','0','a','0','a','0','a' },
             {'0','0','b','0','0','0','a','0' },
             {'0','0','0','0','0','0','0','0' },
             {'0','0','a','0','0','0','0','0' },
             {'0','0','0','b','0','0','0','b' },
             {'b','0','0','0','0','0','0','0' },
             {'0','b','0','b','0','b','0','b' },
             {'b','0','b','0','b','0','b','0' }
		};
		char[][] now1 = new char[][]{
			{'0','a','0','0','0','0','0','a' },
            {'0','0','b','0','0','0','b','0' },
            {'0','0','0','0','0','0','0','0' },
            {'0','0','0','0','b','0','0','0' },
            {'0','0','0','0','0','0','0','0' },
            {'0','0','B','0','0','0','0','0' },
            {'0','0','0','0','0','0','0','0' },
            {'0','0','0','0','0','0','0','0' }
		};
		HashMap<List<String>, char[][]> k = new XYTQ().NextLayout(now1, 'a');
		for (List<String> am : k.keySet()) {
			System.out.println(am);
		}
		GameTree x = new GameTree(now1, 'a');
		while(x.isThinking()){
			Thread.sleep(10);
		}
		System.out.println(x.nodeCounter);
		System.out.println(x.bestMove());
		System.out.println(x.root.score);
		for(Node child : x.root.nodeList)
		{
			System.out.println(child);
			
		}
	}
}
