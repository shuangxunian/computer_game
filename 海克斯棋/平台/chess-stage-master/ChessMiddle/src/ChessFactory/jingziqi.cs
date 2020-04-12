using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ChessMiddle.ChessFactory
{
    public class Jingziqi : IChess
    {
        /// <summary>
        /// 棋盘中的空位置
        /// </summary>
        public char EMPTY { get => '-'; }
        /// <summary>
        /// 棋局还没结束.
        /// </summary>
        public char NOT_DONE { get => '~'; }
        /// <summary>
        /// 平局
        /// </summary>
        public char DRAW { get => 'd'; }

        private char[] _chess;
        private char[,] _chess2d;
        private int _width;
        private int _height;
        private char[] _role;
        public Jingziqi(int width, int height)
        {
            _role = new char[] { 'x', 'o' };
            _chess = new char[width * height];
            _chess2d = new char[width, height];
            _width = width;
            _height = height;

            init();
        }

        private void init()
        {
            int size = Width * Height;
            for (int i = 0; i < size; i++)
            {
                _chess[i] = EMPTY;
            }

            for (int i = 0; i < _width; i++)
                for (int j = 0; j < _height; j++)
                {
                    _chess2d[i, j] = EMPTY;
                }
        }

        public int Width { get => _width; }
        public int Height { get => _height; }
        public char[] Role { get => _role; }
        public char[,] ChessLayout { get => _chess2d; }
        public string Size { get => "" + Width + "-" + Height; }


        /// <summary>
        /// 井子棋下子,并返回合法性
        /// (序列代表走子的坐标,下限从1开始)
        /// </summary>
        /// <param name="actionMove">包含棋步的序列</param>
        /// <param name="role">哪一方</param>
        /// <returns>是否合法</returns>
        public bool DoChess(List<string> actionMove, char role)
        {
            string position = actionMove[0];
            int x = int.Parse(position.Split(',')[0]);
            int y = int.Parse(position.Split(',')[1]);
            if (x <= 0 || x > Width || y <= 0 || y > Height)
                return false;
            if (role == EMPTY || Array.IndexOf(Role, role) == -1)
                return false;

            int arrayPosition = (x - 1) * Width + y - 1;

            //那个位置为空
            if (_chess[arrayPosition] == EMPTY)
            {
                _chess[arrayPosition] = role;
                _chess2d[x - 1, y - 1] = role;
                return true;
            }

            return false;
        }

        /// <summary>
        /// 获得当前棋局的结果
        /// </summary>
        /// <returns>代表当前结果的字符</returns>
        public char GetResult(char who)
        {
            //检测某列是否有胜者
            bool isVertical;
            for (int i = 0; i < _width; i++)
            {
                isVertical = true;
                for (int j = 0; j < _width - 1; j++)
                {
                    if (_chess2d[j, i] != _chess2d[j + 1, i])
                        isVertical = false;
                }
                if (_chess2d[i, 0] != EMPTY && isVertical == true)
                    return _chess2d[0, i];
            }

            //检测某行是否有胜者
            bool isHorizontal;
            for (int i = 0; i < _height; i++)
            {
                isHorizontal = true;
                for (int j = 0; j < _height - 1; j++)
                {
                    if (_chess2d[i, j] != _chess2d[i, j + 1])
                        isHorizontal = false;
                }
                if (_chess2d[i, 0] != EMPTY && isHorizontal == true)
                    return _chess2d[i, 0];
            }


            bool isSlash = true;
            for (int i = 0; i < _width - 1; i++)
            {

                if (_chess2d[i, i] != _chess2d[i + 1, i + 1])
                    isSlash = false;
            }
            if (_chess2d[0, 0] != EMPTY && isSlash == true)
                return _chess2d[0, 0];

            isSlash = true;
            for (int i = _width - 1; i > 0; i--)
            {
                if (_chess2d[i, Width - i - 1] != _chess2d[i - 1, Width - i])
                    isSlash = false;
            }
            if (_chess2d[0, _width - 1] != EMPTY && isSlash == true)
                return _chess2d[0, _width - 1];


            int size = Width * Height;
            for (int i = 0; i < size; i++)
            {
                if (_chess[i] == EMPTY)
                    return NOT_DONE;
            }

            return DRAW;
        }

        /// <summary>
        /// 执行默认步
        /// </summary>
        /// <param name="role">执行者</param>
        /// <returns>默认步的动作序列</returns>
        public List<string> DefaultDo(char role)
        {
            List<string> action = new List<string>();
            for (int i = 0; i < _width; i++)
            {
                int j;
                for (j = 0; j < _height; j++)
                {
                    if (_chess2d[i, j] == EMPTY)
                    {
                        //example
                        //在(1,2)的位置加一个x
                        //+/1,2/x
                        action.Add("" + (i + 1) + "," + (j + 1));
                        DoChess(action, role);
                        break;
                    }
                }
                //只需找到一个即可
                if (j < _height)
                    break;
            }
            return action;
        }
    }
}
