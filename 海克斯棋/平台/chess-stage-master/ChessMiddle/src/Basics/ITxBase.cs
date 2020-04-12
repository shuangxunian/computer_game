using System.Net;
namespace ChessMiddle.Basics
{
    /// <summary>
    /// 一个有基本通信属性的接口主要是被ITxClient和ITxServer继承;
    /// </summary>
    public interface ITxBase
    {    
        /// <summary>
        /// 当引擎非正常原因自动断开的时候触发此事件
        /// </summary>
        event TxDelegate<string> EngineLost;
        /// <summary>
        /// 当引擎完全关闭释放资源的时候
        /// </summary>
        event TxDelegate EngineClose;
        /// <summary>
        /// 当接收到图片数据的时候,触发此事件
        /// </summary>
        event TxDelegate<IPEndPoint, byte[]> AcceptByte;
        /// <summary>
        /// 当将数据发送成功且对方已经收到的时候,触发此事件
        /// </summary>
        event TxDelegate<IPEndPoint> dateSuccess;
        /// <summary>
        /// 缓冲区大小；默认为1024字节；不影响最大发送量，如果内存够大或经常发送大数据可以适当加大缓冲区
        /// 大小；从而可以提高发送速度；否则会自动分包发送，到达对方自动组包；UDP这里不超过65507;
        /// </summary>
        int BufferSize
        {
            get;
            set;
        }
        /// <summary>
        /// 引擎是否已经启动;
        /// </summary>
        bool EngineStart
        { get; }
        /// <summary>
        /// 启动ip地址;
        /// </summary>
        string Ip
        {
            get;
            set;
        }
        /// <summary>
        /// 启动端口号
        /// </summary>
        int Port
        {
            get;
            set;
        }
        /// <summary>
        /// 启动引擎
        /// </summary>
        void StartEngine();
        /// <summary>
        /// 关闭引擎
        /// </summary>
        void CloseEngine();
    }
}
