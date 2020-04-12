using System.Net;
using System.Net.Sockets;

namespace ChessMiddle.Basics
{
    /// <summary>
    /// 一个State父类
    /// </summary>
    internal class StateBase
    {
       private Socket _workSocket = null;//工作的socket
       private IPEndPoint ipEndPoint = null;
       private int _bufferSize = 1024;//缓冲区大小
       private byte[] _buffer = null;//缓冲区
       private byte[] _sendDate = null;//已发送的数据
       private byte[] _bufferBackup = null;//备份缓冲区;主要是缓冲区有时候需要增大或缩小的时候用到；
        /// <summary>
       /// 备份缓冲区;主要是缓冲区有时候需要增大或缩小的时候用到；
        /// </summary>
       internal byte[] BufferBackup
       {
           get { return _bufferBackup; }
           set { _bufferBackup = value; }
       }
        /// <summary>
        /// 缓冲区大小
        /// </summary>
       internal int BufferSize
       {
           get { return _bufferSize; }
       }
       /// <summary>
       /// 带参数的构造函数
       /// </summary>
       /// <param name="socket">Socket</param>
       /// <param name="bufferSize">缓冲区大小</param>
       internal StateBase(Socket socket, int bufferSize)
       {
           _bufferSize = bufferSize;
           _buffer = new byte[bufferSize];
           _workSocket = socket;
           try
           {
               ipEndPoint = (IPEndPoint)socket.RemoteEndPoint;
           }
           catch { }
       }
       /// <summary>
       /// 工作的Socket
       /// </summary>
       internal Socket WorkSocket
       {
           get { return _workSocket; }
           set { _workSocket = value; }
       }
       /// <summary>
       /// 缓冲区
       /// </summary>
       internal byte[] Buffer
       {
           get { return _buffer; }
           set { _buffer = value; }
       }
       /// <summary>
       /// 已发送的数据,主要用于对方没有收到信息可以重发用
       /// </summary>
       internal byte[] SendDate
       {
           get { return _sendDate; }
           set { _sendDate = value; }
       }
       /// <summary>
       /// IPEndPoint得到客户端地址,端口号；
       /// </summary>
       internal IPEndPoint IpEndPoint
       {
           get { return ipEndPoint; }
           set { ipEndPoint = value; }
       }
    }
}
