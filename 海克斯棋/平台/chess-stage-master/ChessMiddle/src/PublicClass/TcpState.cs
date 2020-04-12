using ChessMiddle.Basics;
using System;
using System.Net.Sockets;

namespace ChessMiddle.PublicClass
{
    /// <summary>
    /// TCP协议的state
    /// </summary>
    internal class TcpState : StateBase
    {
        private bool _connectOk = false;//是否真正与对方相连接;主要用与服务器中的对象;
        private DateTime _lastCallTime = DateTime.Now;
        private bool _thinkInTime = false;

        /// <summary>
        /// 二个作用，客户端真正关闭了引擎；服务器:是否真正与对方相连接;主要用与服务器中的对象;
        /// </summary>
        internal bool ConnectOk
        {
            get { return _connectOk; }
            set { _connectOk = value; }
        }

        /// <summary>
        /// 最后一次轮到下棋的指令发出的时间
        /// </summary>
        public DateTime LastCallTime { get => _lastCallTime; set => _lastCallTime = value; }

        /// <summary>
        /// 是否在限制的事件内返回棋步
        /// </summary>
        public bool ThinkInTime { get => _thinkInTime; set => _thinkInTime = value; }

        /// <summary>
        /// 带参数的构造函数
        /// </summary>
        /// <param name="socket">Socket</param>
        /// <param name="bufferSize">缓冲区</param>
        internal TcpState(Socket socket,int bufferSize)
            : base(socket, bufferSize)
        {
        }
    }
}
