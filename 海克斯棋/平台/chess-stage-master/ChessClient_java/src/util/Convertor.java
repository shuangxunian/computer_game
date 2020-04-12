package util;

import java.util.ArrayList;

public class Convertor {
	public static char[][] arrayListTo2D(ArrayList<String> chessLayout)
	{
		char[][] chess2d= new char[8][8];
		int counter=0;
		for (String charS : chessLayout) {
			int x= counter/8;
			int y= counter%8;
			chess2d[x][y] = charS.charAt(0);
			counter++;
		}
		return chess2d;
	}
}
