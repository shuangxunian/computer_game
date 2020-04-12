using ChessMiddle.Basics;
using System.Collections.Generic;
using System.Net;

namespace ChessMiddle
{
    /// <summary>
    /// 一个服务器的接口,所有方法和事件都在里面
    /// </summary>
    public interface ITxServer : ITxBase
    {
        /// <summary>
        /// 当有客户连接成功的时候,触发此事件
        /// </summary>
        event TxDelegate<IPEndPoint> Connect;
        /// <summary>
        /// 当有客户突然断开的时候,触发此事件,文本参数是代表断开的原因
        /// </summary>
        event TxDelegate<IPEndPoint, string> Disconnection;
        /// <summary>
        /// 当有客户发送棋步的时候, 触发事件, 参数是棋局改变的序列
        /// </summary>
        event TxDelegate<List<string>, char[,], char, char> PlayChess;
        /// <summary>
        /// 当前客户端数量
        /// </summary>
        int ClientNumber
        {
            get;
        }
        /// <summary>
        /// 每次通知玩家走棋时限制用时.
        /// </summary>
        double LimitThinkSeconds
        {
            get;
            set;
        }
        /// <summary>
        /// 允许最多客户端数
        /// </summary>
        int ClientMax
        {
            get;
            set;
        }
        /// <summary>
        /// 得到所有的客户端
        /// </summary>
        List<IPEndPoint> ClientAll
        {
            get;
        }
        /// <summary>
        /// 获得棋局
        /// </summary>
        /// <returns>返回棋局的二维表示</returns>
        char[,] GetChessLayout();
        /// <summary>
        /// 获得棋局的字符串表示
        /// </summary>
        /// <returns>字符串格式</returns>
        string GetChessLayoutStr();
        /// <summary>
        /// 下棋
        /// </summary>
        /// <param name="data">棋的改变序列</param>
        /// <param name="layout"></param>
        /// <param name="role">玩家身份</param>
        /// <param name="result">现在的棋局情况</param>
        void OnChessPlay(List<string> data, char[,] layout, char role, char result);
        /// <summary>
        /// 服务器向客户端发送字节数据
        /// </summary>
        /// <param name="ipEndPoint">IPEndPoint</param>
        /// <param name="data">未加密的数据</param>
        void sendMessage(IPEndPoint ipEndPoint, byte[] data);
        /// <summary>
        /// 服务器强制关闭一个客户端
        /// </summary>
        /// <param name="ipEndPoint">IPEndPoint</param>
        void clientClose(IPEndPoint ipEndPoint);
        /// <summary>
        /// 检查某个客户端是否在线
        /// </summary>
        /// <param name="ipEndPoint">IPEndPoint</param>
        /// <returns>bool</returns>
        bool clientCheck(IPEndPoint ipEndPoint);
        /// <summary>
        /// 关闭所有客户端连接
        /// </summary>
        void clientAllClose();
    }
}
