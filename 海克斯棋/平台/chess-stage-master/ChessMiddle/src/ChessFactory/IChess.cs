using System.Collections.Generic;

namespace ChessMiddle.ChessFactory
{
    interface IChess
    {

        /// <summary>
        /// 棋盘宽度
        /// </summary>
        int Width { get; }
        /// <summary>
        /// 棋盘高度
        /// </summary>
        int Height { get; }
        /// <summary>
        /// 角色集
        /// </summary>
        char[] Role { get; }
        /// <summary>
        /// 棋的实例
        /// </summary>
        char[,] ChessLayout { get; }
        /// <summary>
        /// 棋盘型号
        /// </summary>
        string Size { get; }

        /// <summary>
        /// 棋盘中的空位置
        /// </summary>
        char EMPTY { get; }
        /// <summary>
        /// 棋局还没结束.
        /// </summary>
        char NOT_DONE { get; }
        /// <summary>
        /// 平局
        /// </summary>
        char DRAW { get; }

        /// <summary>
        /// 走棋
        /// </summary>
        /// <param name="actionMove">走棋序列</param>
        /// <param name="role">角色</param>
        /// <returns>返回走棋合法性</returns>
        bool DoChess(List<string> actionMove, char role);

        /// <summary>
        /// 判断当前结果
        /// 有三种可能:
        ///     1.某个玩家赢了,返回赢家的代码(Role[?])
        ///     2.平局,返回平局码('d')
        /// </summary>
        /// <returns>返回一个代表结果的字符</returns>
        char GetResult(char who);

        /// <summary>
        /// 执行角色role的默认步
        /// </summary>
        /// <param name="role">角色</param>
        /// <returns>返回修改集</returns>
        List<string> DefaultDo(char role);
    }
}
