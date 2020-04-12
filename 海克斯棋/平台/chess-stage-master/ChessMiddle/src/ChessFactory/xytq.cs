using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using System.Text;


namespace ChessMiddle.ChessFactory
{
    class Xytq : IChess
    {
        public char EMPTY { get => '0'; }
        public char NOT_DONE { get => '~'; }
        public char DRAW { get => 'd'; }
        private char[,] _chessLayout;
        private int _width;
        private int _height;
        private char[] _role;

        private char BLACK;
        private char BLACK_KING;
        private char WHITE;
        private char WHITE_KING;

        public int Width { get => _width; }
        public int Height { get => _height; }
        public char[] Role { get => _role; }
        public char[,] ChessLayout { get => _chessLayout; }
        public string Size => Width + "-" + Height;

        public Xytq()
        {
            _width = 8;
            _height = 8;
            _role = new char[] { 'a', 'b' };
            BLACK = 'b';
            BLACK_KING = 'B';
            WHITE = 'a';
            WHITE_KING = 'A';

            init();
        }

        public Xytq(char[,] layout)
        {
            _width = 8;
            _height = 8;
            _role = new char[] { 'a', 'b' };
            BLACK = 'b';
            BLACK_KING = 'B';
            WHITE = 'a';
            WHITE_KING = 'A';

            _chessLayout = layout;
        }

        private void init()
        {
            _chessLayout = new char[8, 8]{
                {'0','a','0','a','0','a','0','a' },
                {'a','0','a','0','a','0','a','0' },
                {'0','a','0','a','0','a','0','a' },
                {'0','0','0','0','0','0','0','0' },
                {'0','0','0','0','0','0','0','0' },
                {'b','0','b','0','b','0','b','0' },
                {'0','b','0','b','0','b','0','b' },
                {'b','0','b','0','b','0','b','0' }
            };
        }

        public List<string> DefaultDo(char role)
        {
            Dictionary<List<string>, char[,]> Nexts = NextLayout(role, _chessLayout);

            if (Nexts.Count < 1)
            {
                return null;
            }

            //int o = 0;
            //foreach(List<string> l in Nexts.Keys)
            //{
            //    o++;
            //    foreach (string s in l)
            //    {
            //        Console.WriteLine(s);
            //    }
            //}

            int moveableCount = Nexts.Keys.Count;
            List<string>[] forRandom = new List<string>[Nexts.Keys.Count];
            Nexts.Keys.CopyTo(forRandom, 0);

            int randomI = new Random().Next(0, moveableCount);

            _chessLayout = Nexts[forRandom[randomI]];
            //变王
            for (int i = 0; i < _width; i++)
            {
                if (_chessLayout[0, i] == BLACK)
                {
                    _chessLayout[0, i] = BLACK_KING;
                }

                if (_chessLayout[_height - 1, i] == WHITE)
                    _chessLayout[_height - 1, i] = WHITE_KING;
            }

            return forRandom[randomI];
        }

        public bool DoChess(List<string> actionMove, char role)
        {
            Dictionary<List<string>, char[,]> Next = NextLayout(role, _chessLayout);
            foreach (List<string> ableMove in Next.Keys)
            {
                //验证集合如果有重复会导致验证失败,但这里不需考虑这点(不可能出现重复走一条路)
                if (actionMove.Count == ableMove.Count && actionMove.All(ableMove.Contains))
                {
                    _chessLayout = Next[ableMove];

                    //变王
                    for (int i = 0; i < _width; i++)
                    {
                        if (_chessLayout[0, i] == BLACK)
                        {
                            _chessLayout[0, i] = BLACK_KING;
                        }

                        if (_chessLayout[_height - 1, i] == WHITE)
                            _chessLayout[_height - 1, i] = WHITE_KING;
                    }

                    return true;
                }
            }

            return false;
        }

        public char GetResult(char nowPlayer)
        {
            
            char nextPlayer;
            if (nowPlayer == Role[0])
            {
                nextPlayer = Role[1];
            }
            else
            {
                nextPlayer = Role[0];
            }

            Dictionary<List<string>, char[,]> Nexts = NextLayout(nextPlayer, _chessLayout);
            Dictionary<List<string>, char[,]> NextNexts = NextLayout(nowPlayer, _chessLayout);

            if (Nexts.Keys.Count == 0)
            {
                if (NextNexts.Keys.Count == 0)
                {
                    return DRAW;
                }
                return nowPlayer;
            }
            else
            {
                return NOT_DONE;
            }

        }


        /// <summary>
        /// 返回棋局的下一个可行解.包含解的步骤和棋局
        /// </summary>
        /// <param name="player"></param>
        /// <param name="layout"></param>
        /// <returns></returns>
        public Dictionary<List<string>, char[,]> NextLayout(char player, char[,] layout)
        {
            //add all possible position
            Dictionary<List<string>, char[,]> layoutDic = new Dictionary<List<string>, char[,]>();
            //add all empty position
            Dictionary<List<string>, char[,]> emptyDic = new Dictionary<List<string>, char[,]>();
            //add all eat position
            Dictionary<List<string>, char[,]> eatDic = new Dictionary<List<string>, char[,]>();

            int nextLine;
            char rival, playerKing, rivalKing;
            char playerSoldier, rivalSoldier;
            if (player == WHITE || player == WHITE_KING)
            {
                player = WHITE;
                playerSoldier = WHITE;
                rivalSoldier = BLACK;
                rival = BLACK;
                nextLine = 1;
                playerKing = WHITE_KING;
                rivalKing = BLACK_KING;
            }
            else
            {
                player = BLACK;
                playerSoldier = BLACK;
                rivalSoldier = WHITE;
                rival = WHITE;
                nextLine = -1;
                playerKing = BLACK_KING;
                rivalKing = WHITE_KING;
            }

            int LENGTH = _width;
            for (int i = 0; i < _width; i++)
            {
                for (int j = 0; j < _height; j++)
                {
                    if (layout[i, j] == playerSoldier || layout[i, j] == playerKing)
                    {
                        player = layout[i, j];
                        //left positon
                        if (i + nextLine < _width && i + nextLine >= 0 && j - 1 >= 0)
                        {
                            //deep clone
                            if (layout[i + nextLine, j - 1] == EMPTY)
                            {
                                char[,] t = new char[LENGTH, LENGTH];
                                deepClone(t, layout);

                                t[i + nextLine, j - 1] = player;
                                t[i, j] = EMPTY;

                                List<string> actionMove = new List<string>();
                                actionMove.Add("" + i + "," + j + "-" + (i + nextLine) + "," + (j - 1));
                                //add to list
                                emptyDic.Add(actionMove, t);
                            }
                            if (layout[i + nextLine, j - 1] == rival || layout[i + nextLine, j - 1] == rivalKing)
                            {
                                if (i + 2 * nextLine < LENGTH && i + 2 * nextLine >= 0 && j - 2 >= 0)
                                {
                                    if (layout[i + 2 * nextLine, j - 2] == EMPTY)
                                    {
                                        char[,] t = new char[LENGTH, LENGTH];
                                        deepClone(t, layout);

                                        t[i + 2 * nextLine, j - 2] = player;
                                        t[i + nextLine, j - 1] = EMPTY;
                                        t[i, j] = EMPTY;

                                        List<string> actionMove = new List<string>();
                                        actionMove.Add("" + i + "," + j + "-" + (i + 2 * nextLine) + "," + (j - 2));

                                        continueJump(t, i + 2 * nextLine, j - 2, player, rival, eatDic, actionMove);
                                    }
                                }
                            }
                        }

                        //right position
                        if (i + nextLine < LENGTH && i + nextLine >= 0 && j + 1 < LENGTH)
                        {
                            if (layout[i + nextLine, j + 1] == EMPTY)
                            {
                                //get next layout
                                char[,] t = new char[LENGTH, LENGTH];
                                deepClone(t, layout);

                                t[i + nextLine, j + 1] = player;
                                t[i, j] = EMPTY;
                                //add to list

                                List<string> actionMove = new List<string>();
                                actionMove.Add("" + i + "," + j + "-" + (i + nextLine) + "," + (j + 1));
                                //add to list
                                emptyDic.Add(actionMove, t);

                            }
                            if (layout[i + nextLine, j + 1] == rival || layout[i + nextLine, j + 1] == rivalKing)
                            {
                                if (i + 2 * nextLine < LENGTH && i + 2 * nextLine >= 0 && j + 2 < LENGTH)
                                {
                                    if (layout[i + 2 * nextLine, j + 2] == EMPTY)
                                    {
                                        char[,] t = new char[LENGTH, LENGTH];

                                        deepClone(t, layout);

                                        t[i + 2 * nextLine, j + 2] = player;
                                        t[i + nextLine, j + 1] = EMPTY;
                                        t[i, j] = EMPTY;

                                        List<string> actionMove = new List<string>();
                                        actionMove.Add("" + i + "," + j + "-" + (i + 2 * nextLine) + "," + (j + 2));

                                        continueJump(t, i + 2 * nextLine, j + 2, player, rival, eatDic, actionMove);
                                    }
                                }
                            }
                        }
                    }

                    if (layout[i, j] == playerKing)
                    {
                        player = layout[i, j];
                        //left positon
                        if (i - nextLine < LENGTH && i - nextLine >= 0 && j - 1 >= 0)
                        {
                            //deep clone
                            if (layout[i - nextLine, j - 1] == EMPTY)
                            {
                                char[,] t = new char[LENGTH, LENGTH];
                                deepClone(t, layout);

                                t[i - nextLine, j - 1] = player;
                                t[i, j] = EMPTY;

                                List<string> actionMove = new List<string>();
                                actionMove.Add("" + i + "," + j + "-" + (i - nextLine) + "," + (j - 1));
                                //add to list
                                emptyDic.Add(actionMove, t);
                            }
                            if (layout[i - nextLine, j - 1] == rival || layout[i - nextLine, j - 1] == rivalKing)
                            {
                                if (i - 2 * nextLine < LENGTH && i - 2 * nextLine >= 0 && j - 2 >= 0)
                                {
                                    if (layout[i - 2 * nextLine, j - 2] == EMPTY)
                                    {
                                        char[,] t = new char[LENGTH, LENGTH];

                                        deepClone(t, layout);
                                        t[i - 2 * nextLine, j - 2] = player;
                                        t[i - nextLine, j - 1] = EMPTY;
                                        t[i, j] = EMPTY;

                                        List<string> actionMove = new List<string>();
                                        actionMove.Add("" + i + "," + j + "-" + (i - 2 * nextLine) + "," + (j - 2));

                                        continueJump(t, i - 2 * nextLine, j - 2, player, rival, eatDic, actionMove);
                                    }
                                }
                            }
                        }

                        //right position
                        if (i - nextLine < LENGTH && i - nextLine >= 0 && j + 1 < LENGTH)
                        {
                            if (layout[i - nextLine, j + 1] == EMPTY)
                            {
                                //get next layout
                                char[,] t = new char[LENGTH, LENGTH];
                                deepClone(t, layout);

                                t[i - nextLine, j + 1] = player;
                                t[i, j] = EMPTY;

                                List<string> actionMove = new List<string>();
                                actionMove.Add("" + i + "," + j + "-" + (i - nextLine) + "," + (j + 1));
                                //add to list
                                emptyDic.Add(actionMove, t);

                            }
                            if (layout[i - nextLine, j + 1] == rival || layout[i - nextLine, j + 1] == rivalKing)
                            {
                                if (i - 2 * nextLine < LENGTH && i - 2 * nextLine >= 0 && j + 2 < LENGTH)
                                {
                                    if (layout[i - 2 * nextLine, j + 2] == EMPTY)
                                    {
                                        char[,] t = new char[LENGTH, LENGTH];
                                        deepClone(t, layout);

                                        t[i - 2 * nextLine, j + 2] = player;
                                        t[i - nextLine, j + 1] = EMPTY;
                                        t[i, j] = EMPTY;

                                        List<string> actionMove = new List<string>();
                                        actionMove.Add("" + i + "," + j + "-" + (i - 2 * nextLine) + "," + (j + 2));

                                        continueJump(t, i - 2 * nextLine, j + 2, player, rival, eatDic, actionMove);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            //not exist compulsory layout
            if (eatDic.Count == 0)
            {
                foreach (List<string> am in emptyDic.Keys)
                {
                    layoutDic.Add(am, emptyDic[am]);
                }
            }
            else
            {
                foreach (List<string> am in eatDic.Keys)
                {
                    layoutDic.Add(am, eatDic[am]);
                }
            }

            return layoutDic;
        }

        internal void Print(char[,] v)
        {
            for(int i =0;i<8;i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    Console.Write(v[i, j] + " ");
                }
                Console.WriteLine();
            }
        }

        public void continueJump(char[,] layout, int x, int y, char player, char rival, Dictionary<List<string>, char[,]> a, List<string> actionMove)
        {
            int flag = 0;
            int LENGTH = _width;
            char rival_king = Char.ToUpper(rival);
            if (player == BLACK || player == WHITE_KING || player == BLACK_KING)
            {
                if (x - 2 >= 0 && y - 2 >= 0 && (layout[x - 1, y - 1] == rival || layout[x - 1, y - 1] == rival_king) && layout[x - 2, y - 2] == EMPTY)
                {
                    flag = 1;
                    char[,] t = new char[LENGTH, LENGTH];

                    deepClone(t, layout);
                    t[x - 2, y - 2] = player;
                    t[x - 1, y - 1] = EMPTY;
                    t[x, y] = EMPTY;

                    List<string> newActionMove = Clone<string>(actionMove);
                    newActionMove.Add("" + x + "," + y + "-" + (x - 2) + "," + (y - 2));
                    continueJump(t, x - 2, y - 2, player, rival, a, newActionMove);
                }
                if (x - 2 >= 0 && y + 2 < LENGTH && (layout[x - 1, y + 1] == rival || layout[x - 1, y + 1] == rival_king) && layout[x - 2, y + 2] == EMPTY)
                {
                    flag = 1;
                    char[,] t = new char[LENGTH, LENGTH];

                    deepClone(t, layout);
                    t[x - 2, y + 2] = player;
                    t[x - 1, y + 1] = EMPTY;
                    t[x, y] = EMPTY;

                    List<string> newActionMove = Clone<string>(actionMove);
                    newActionMove.Add("" + x + "," + y + "-" + (x - 2) + "," + (y + 2));
                    continueJump(t, x - 2, y + 2, player, rival, a, newActionMove);
                }
            }
            if (player == WHITE || player == WHITE_KING || player == BLACK_KING)
            {
                if (x + 2 < LENGTH && y - 2 >= 0 && (layout[x + 1, y - 1] == rival||layout[x + 1, y - 1] == rival_king) && layout[x + 2, y - 2] == EMPTY)
                {
                    flag = 1;
                    char[,] t = new char[LENGTH, LENGTH];

                    deepClone(t, layout);
                    t[x + 2, y - 2] = player;
                    t[x + 1, y - 1] = EMPTY;
                    t[x, y] = EMPTY;

                    List<string> newActionMove = Clone<string>(actionMove);
                    newActionMove.Add("" + x + "," + y + "-" + (x + 2) + "," + (y - 2));
                    continueJump(t, x + 2, y - 2, player, rival, a, newActionMove);
                }
                if (x + 2 < LENGTH && y + 2 < LENGTH && (layout[x + 1, y + 1] == rival|| layout[x + 1, y + 1] == rival_king) && layout[x + 2, y + 2] == EMPTY)
                {
                    flag = 1;
                    char[,] t = new char[LENGTH, LENGTH];

                    deepClone(t, layout);
                    t[x + 2, y + 2] = player;
                    t[x + 1, y + 1] = EMPTY;
                    t[x, y] = EMPTY;

                    List<string> newActionMove = Clone<string>(actionMove);
                    newActionMove.Add("" + x + "," + y + "-" + (x + 2) + "," + (y + 2));
                    continueJump(t, x + 2, y + 2, player, rival, a, newActionMove);
                }

            }

            if (0 == flag)
            {
                a.Add(actionMove, layout);
            }
            return;
        }

        public void deepClone(char[,] a, char[,] b)
        {
            int LENGTH = _width;
            for (int i = 0; i < LENGTH; i++)
            {
                for (int j = 0; j < LENGTH; j++)
                {
                    a[i, j] = b[i, j];
                }
            }
        }

        /// <summary>
        /// Clones the specified list.
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="List">The list.</param>
        /// <returns>List{``0}.</returns>
        public static List<T> Clone<T>(object List)
        {
            using (Stream objectStream = new MemoryStream())
            {
                IFormatter formatter = new BinaryFormatter();
                formatter.Serialize(objectStream, List);
                objectStream.Seek(0, SeekOrigin.Begin);
                return formatter.Deserialize(objectStream) as List<T>;
            }
        }

        public void Print(char[,] layout, List<string> am)
        {
            int LENGTH = _width;
            for (int i = 0; i < LENGTH; i++)
            {
                Console.Write("|");
                for (int j = 0; j < LENGTH; j++)
                {
                    if (layout[i, j] == EMPTY)
                    {
                        Console.Write(" " + "|");
                    }
                    if (layout[i, j] == WHITE)
                    {
                        Console.Write("a" + "|");
                    }
                    if (layout[i, j] == WHITE_KING)
                    {
                        Console.Write("A" + "|");
                    }
                    if (layout[i, j] == BLACK)
                    {
                        Console.Write("b" + "|");
                    }
                    if (layout[i, j] == BLACK_KING)
                    {
                        Console.Write("B" + "|");
                    }
                }
                Console.WriteLine();
                Console.WriteLine("-----------------");
            }
            //foreach (string a in am)
            //    Console.WriteLine(a);

            Console.WriteLine();
            Console.WriteLine("----------this is line------------");
            Console.WriteLine();
        }

        /// <summary>
        /// 把布局转换成字符串输出
        /// </summary>
        /// <returns></returns>
        public override string ToString()
        {
            string str = "";
            for (int i = 0; i < Width; i++)
            {
                str += ("|");
                for (int j = 0; j < Height; j++)
                {
                    if (_chessLayout[i, j] == EMPTY)
                    {
                        str += (" " + "|");
                    }
                    if (_chessLayout[i, j] == WHITE)
                    {
                        str += ("a" + "|");
                    }
                    if (_chessLayout[i, j] == WHITE_KING)
                    {
                        str += ("A" + "|");
                    }
                    if (_chessLayout[i, j] == BLACK)
                    {
                        str += ("b" + "|");
                    }
                    if (_chessLayout[i, j] == BLACK_KING)
                    {
                        str += ("B" + "|");
                    }
                }
                str += "\r\n";
                str += ("-----------------\r\n");
            }
            return str;
        }

    }
}
