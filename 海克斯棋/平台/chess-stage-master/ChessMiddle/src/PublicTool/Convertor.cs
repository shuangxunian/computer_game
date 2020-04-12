using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ChessMiddle.PublicTool
{
    class Convertor
    {
        public static char[,] arrayListTo2D(Object chessLayout)
        {
            ArrayList layoutArray = chessLayout as ArrayList;
            char[,] chess2d = new char[8,8];
            int counter = 0;
            foreach (Object charS in layoutArray)
            {
                int x = counter / 8;
                int y = counter % 8;
                chess2d[x,y] = (charS as string).ToCharArray()[0];
                counter++;
            }
            return chess2d;
        }

    }
}
