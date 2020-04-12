package publicClass;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Random;

import util.DeepClone;

public class XYTQ {
	public char[][] layout;
	private int _width;
	private int _height;
	private char role;
	private char BLACK;
	private char BLACK_KING;
	private char WHITE;
	private char WHITE_KING;
	private char EMPTY;

	private Random random;

	public XYTQ() {
		_width = 8;
		_height = 8;
		BLACK = 'b';
		BLACK_KING = 'B';
		WHITE = 'a';
		WHITE_KING = 'A';
		EMPTY = '0';
		layout = null;

		random = new Random();
	}

	public List<String> getRandomNextAction(char role) {
		if (this.layout == null)
			layoutInit();
		HashMap<List<String>, char[][]> x = NextLayout(this.layout, role);
		int setSize = x.keySet().size();
		int ri = 0;
		int i = 0;
		if(setSize!=0)
			ri = random.nextInt(setSize);
		int index = 0;
		for (List<String> am : x.keySet()) {
			System.out.println(index);
			index++;
			for(String s : am)
			{
				System.out.print(s+"|");
			}
			System.out.println();
		}
		for (List<String> am : x.keySet()) {
			if (i == ri)
				return am;
			i++;
		}
		return null;
	}

	public int[] getPieceCount(char[][] layout) {
		int a=0,b=0,A=0,B=0;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (layout[i][j] == 'a')
					a++;
				if (layout[i][j] == 'A')
					A++;
				if (layout[i][j] == 'b')
					b++;
				if (layout[i][j] == 'B')
					B++;
			}
		}
		return new int[]{a,A,b,B};

	}
	
	public char[][] getRandomNextLayout(char role) {
		if (this.layout == null)
			layoutInit();
		HashMap<List<String>, char[][]> x = NextLayout(this.layout, role);
		int setSize = x.keySet().size();
		int ri = 0;
		int i = 0;
		if(setSize!=0)
			ri = random.nextInt(setSize);
		for (List<String> am : x.keySet()) {
			if (i == ri)
				return x.get(am);
			i++;
		}
		return null;
	}
	

	private void layoutInit() {
		// TODO Auto-generated method stub
		this.layout = new char[][] {
			{ '0', 'a', '0', 'a', '0', 'a', '0', 'a' },
			{ 'a', '0', 'a', '0', 'a', '0', 'a', '0' },
			{ '0', 'a', '0', 'a', '0', 'a', '0', 'a' },
			{ '0', '0', '0', '0', '0', '0', '0', '0' },
			{ '0', '0', '0', '0', '0', '0', '0', '0' },
			{ 'b', '0', 'b', '0', 'b', '0', 'b', '0' },
			{ '0', 'b', '0', 'b', '0', 'b', '0', 'b' },
			{ 'b', '0', 'b', '0', 'b', '0', 'b', '0' }
		};
	}

	public HashMap<List<String>, char[][]> NextLayout(char[][] layout, char player) {
		// add all possible position
		HashMap<List<String>, char[][]> layoutDic = new HashMap<List<String>, char[][]>();
		// add all empty position
		HashMap<List<String>, char[][]> emptyDic = new HashMap<List<String>, char[][]>();
		// add all eat position
		HashMap<List<String>, char[][]> eatDic = new HashMap<List<String>, char[][]>();

		int nextLine;
		char rival, playerKing, rivalKing;
		char playerSoldier;
		if (player == WHITE || player == WHITE_KING) {
			player = WHITE;
			playerSoldier = WHITE;
			rival = BLACK;
			nextLine = 1;
			playerKing = WHITE_KING;
			rivalKing = BLACK_KING;
		} else {
			player = BLACK;
			playerSoldier = BLACK;
			rival = WHITE;
			nextLine = -1;
			playerKing = BLACK_KING;
			rivalKing = WHITE_KING;
		}

		int LENGTH = _width;
		for (int i = 0; i < _width; i++) {
			for (int j = 0; j < _height; j++) {
				if (layout[i][j] == playerSoldier || layout[i][j] == playerKing) {
					player = layout[i][j];
					// left positon
					if (i + nextLine < _width && i + nextLine >= 0 && j - 1 >= 0) {
						// deep clone
						if (layout[i + nextLine][j - 1] == EMPTY) {
							char[][] t = new char[LENGTH][LENGTH];
							DeepClone.deepClone(t, layout);

							t[i + nextLine][j - 1] = player;
							t[i][j] = EMPTY;

							List<String> actionMove = new LinkedList<String>();
							actionMove.add("" + i + "," + j + "-" + (i + nextLine) + "," + (j - 1));
							// add to list
							emptyDic.put(actionMove, t);
						}
						if (layout[i + nextLine][j - 1] == rival || layout[i + nextLine][j - 1] == rivalKing) {
							if (i + 2 * nextLine < LENGTH && i + 2 * nextLine >= 0 && j - 2 >= 0) {
								if (layout[i + 2 * nextLine][j - 2] == EMPTY) {
									char[][] t = new char[LENGTH][LENGTH];
									DeepClone.deepClone(t, layout);

									t[i + 2 * nextLine][j - 2] = player;
									t[i + nextLine][j - 1] = EMPTY;
									t[i][j] = EMPTY;

									List<String> actionMove = new LinkedList<String>();
									actionMove.add("" + i + "," + j + "-" + (i + 2 * nextLine) + "," + (j - 2));

									continueJump(t, i + 2 * nextLine, j - 2, player, rival, eatDic, actionMove);
								}
							}
						}
					}

					// right position
					if (i + nextLine < LENGTH && i + nextLine >= 0 && j + 1 < LENGTH) {
						if (layout[i + nextLine][j + 1] == EMPTY) {
							// get next layout
							char[][] t = new char[LENGTH][LENGTH];
							DeepClone.deepClone(t, layout);

							t[i + nextLine][j + 1] = player;
							t[i][j] = EMPTY;
							// add to list

							List<String> actionMove = new LinkedList<String>();
							actionMove.add("" + i + "," + j + "-" + (i + nextLine) + "," + (j + 1));
							// add to list
							emptyDic.put(actionMove, t);

						}
						if (layout[i + nextLine][j + 1] == rival || layout[i + nextLine][j + 1] == rivalKing) {
							if (i + 2 * nextLine < LENGTH && i + 2 * nextLine >= 0 && j + 2 < LENGTH) {
								if (layout[i + 2 * nextLine][j + 2] == EMPTY) {
									char[][] t = new char[LENGTH][LENGTH];

									DeepClone.deepClone(t, layout);

									t[i + 2 * nextLine][j + 2] = player;
									t[i + nextLine][j + 1] = EMPTY;
									t[i][j] = EMPTY;

									List<String> actionMove = new LinkedList<String>();
									actionMove.add("" + i + "," + j + "-" + (i + 2 * nextLine) + "," + (j + 2));

									continueJump(t, i + 2 * nextLine, j + 2, player, rival, eatDic, actionMove);
								}
							}
						}
					}
				}

				if (layout[i][j] == playerKing) {
					player = layout[i][j];
					// left positon
					if (i - nextLine < LENGTH && i - nextLine >= 0 && j - 1 >= 0) {
						// deep clone
						if (layout[i - nextLine][j - 1] == EMPTY) {
							char[][] t = new char[LENGTH][LENGTH];
							DeepClone.deepClone(t, layout);

							t[i - nextLine][j - 1] = player;
							t[i][j] = EMPTY;

							List<String> actionMove = new LinkedList<String>();
							actionMove.add("" + i + "," + j + "-" + (i - nextLine) + "," + (j - 1));
							// add to list
							emptyDic.put(actionMove, t);
						}
						if (layout[i - nextLine][j - 1] == rival || layout[i - nextLine][j - 1] == rivalKing) {
							if (i - 2 * nextLine < LENGTH && i - 2 * nextLine >= 0 && j - 2 >= 0) {
								if (layout[i - 2 * nextLine][j - 2] == EMPTY) {
									char[][] t = new char[LENGTH][LENGTH];

									DeepClone.deepClone(t, layout);
									t[i - 2 * nextLine][j - 2] = player;
									t[i - nextLine][j - 1] = EMPTY;
									t[i][j] = EMPTY;

									List<String> actionMove = new LinkedList<String>();
									actionMove.add("" + i + "," + j + "-" + (i - 2 * nextLine) + "," + (j - 2));

									continueJump(t, i - 2 * nextLine, j - 2, player, rival, eatDic, actionMove);
								}
							}
						}
					}

					// right position
					if (i - nextLine < LENGTH && i - nextLine >= 0 && j + 1 < LENGTH) {
						if (layout[i - nextLine][j + 1] == EMPTY) {
							// get next layout
							char[][] t = new char[LENGTH][LENGTH];
							DeepClone.deepClone(t, layout);

							t[i - nextLine][j + 1] = player;
							t[i][j] = EMPTY;

							List<String> actionMove = new LinkedList<String>();
							actionMove.add("" + i + "," + j + "-" + (i - nextLine) + "," + (j + 1));
							// add to list
							(emptyDic).put(actionMove, t);

						}
						if (layout[i - nextLine][j + 1] == rival || layout[i - nextLine][j + 1] == rivalKing) {
							if (i - 2 * nextLine < LENGTH && i - 2 * nextLine >= 0 && j + 2 < LENGTH) {
								if (layout[i - 2 * nextLine][j + 2] == EMPTY) {
									char[][] t = new char[LENGTH][LENGTH];
									DeepClone.deepClone(t, layout);

									t[i - 2 * nextLine][j + 2] = player;
									t[i - nextLine][j + 1] = EMPTY;
									t[i][j] = EMPTY;

									List<String> actionMove = new LinkedList<String>();
									actionMove.add("" + i + "," + j + "-" + (i - 2 * nextLine) + "," + (j + 2));

									continueJump(t, i - 2 * nextLine, j + 2, player, rival, eatDic, actionMove);
								}
							}
						}
					}
				}
			}
		}

		// not exist compulsory layout
		if (eatDic.isEmpty()) {
			for (List<String> am : emptyDic.keySet()) {
				layoutDic.put(am, emptyDic.get(am));
			}
		} else {
			for (List<String> am : eatDic.keySet()) {
				layoutDic.put(am, eatDic.get(am));
			}
		}

		return layoutDic;
	}

	public void continueJump(char[][] layout, int x, int y, char player, char rival, HashMap<List<String>, char[][]> a,
			List<String> actionMove) {
		int flag = 0;
		int LENGTH = _width;
		char rival_king = Character.toUpperCase(rival);
		if (player == BLACK || player == WHITE_KING || player == BLACK_KING) {
			if (x - 2 >= 0 && y - 2 >= 0 && (layout[x - 1][y - 1] == rival||layout[x - 1][y - 1] == rival_king) && layout[x - 2][y - 2] == EMPTY) {
				flag = 1;
				char[][] t = new char[LENGTH][LENGTH];

				DeepClone.deepClone(t, layout);
				t[x - 2][y - 2] = player;
				t[x - 1][y - 1] = EMPTY;
				t[x][y] = EMPTY;

				List<String> newActionMove = new LinkedList<String>();
				newActionMove.addAll(actionMove);
				newActionMove.add("" + x + "," + y + "-" + (x - 2) + "," + (y - 2));
				continueJump(t, x - 2, y - 2, player, rival, a, newActionMove);
			}
			if (x - 2 >= 0 && y + 2 < LENGTH && (layout[x - 1][y + 1] == rival||layout[x - 1][y + 1] == rival_king) && layout[x - 2][y + 2] == EMPTY) {
				flag = 1;
				char[][] t = new char[LENGTH][LENGTH];

				DeepClone.deepClone(t, layout);
				t[x - 2][y + 2] = player;
				t[x - 1][y + 1] = EMPTY;
				t[x][y] = EMPTY;

				List<String> newActionMove = new LinkedList<String>();
				newActionMove.addAll(actionMove);
				newActionMove.add("" + x + "," + y + "-" + (x - 2) + "," + (y + 2));
				continueJump(t, x - 2, y + 2, player, rival, a, newActionMove);
			}
		}
		if (player == WHITE || player == WHITE_KING || player == BLACK_KING) {
			if (x + 2 < LENGTH && y - 2 >= 0 && (layout[x + 1][y - 1] == rival||layout[x + 1][y - 1] == rival_king) && layout[x + 2][y - 2] == EMPTY) {
				flag = 1;
				char[][] t = new char[LENGTH][LENGTH];

				DeepClone.deepClone(t, layout);
				t[x + 2][y - 2] = player;
				t[x + 1][y - 1] = EMPTY;
				t[x][y] = EMPTY;

				List<String> newActionMove = new LinkedList<String>();
				newActionMove.addAll(actionMove);
				newActionMove.add("" + x + "," + y + "-" + (x + 2) + "," + (y - 2));
				continueJump(t, x + 2, y - 2, player, rival, a, newActionMove);
			}
			if (x + 2 < LENGTH && y + 2 < LENGTH && (layout[x + 1][y + 1] == rival||layout[x + 1][y + 1] == rival_king) && layout[x + 2][y + 2] == EMPTY) {
				flag = 1;
				char[][] t = new char[LENGTH][LENGTH];

				DeepClone.deepClone(t, layout);
				t[x + 2][y + 2] = player;
				t[x + 1][y + 1] = EMPTY;
				t[x][y] = EMPTY;

				List<String> newActionMove = new LinkedList<String>();
				newActionMove.addAll(actionMove);
				newActionMove.add("" + x + "," + y + "-" + (x + 2) + "," + (y + 2));
				continueJump(t, x + 2, y + 2, player, rival, a, newActionMove);
			}
		}

		if (0 == flag) {
			a.put(actionMove, layout);
		}
		return;
	}

	public char[][] getLayout() {
		return layout;
	}

	public void setLayout(char[][] layout) {
		this.layout = layout;
	}

	public char getRole() {
		return role;
	}

	public void setRole(char role) {
		this.role = role;
	}

	public static void main(String[] args) {
		// TODO Auto-generated method stub

	}

}
