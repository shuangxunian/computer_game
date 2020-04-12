using ChessMiddle.Basics;
using System.Collections.Generic;

namespace ChessMiddle
{
    /// <summary>
    /// 一个客户端的接口所有方法和事件都在里面
    /// </summary>
    public interface ITxClient : ITxBase
    {
        /// <summary>
        /// 引擎登录成功或失败都会触发此事件,登录失败的话会有失败的原因
        /// </summary>
        event TxDelegate<bool, string> StartResult;
        /// <summary>
        /// 自动重连开始的时候,触发此事件
        /// </summary>
        event TxDelegate ReconnectionStart;
        /// <summary>
        /// 当轮到本方下棋时,显示所有可行解
        /// </summary>
        event TxDelegate<List<List<string>>> ShowAbleMove;
       /// <summary>
        /// 当连接断开时是否重连,0为不重连,默认重连三次;
       /// </summary>
        int ReconnectMax
        {
            get;
            set;
        }
        /// <summary>
        /// 登录超时时间，默认为10秒
        /// </summary>
        int OutTime
        {
            get;
            set;
        }
        /// <summary>
        /// 客户端向服务器发送字节数据
        /// </summary>
        /// <param name="data">字节数据</param>
        void sendMessage(byte[] data);
    }
}
