using ChessMiddle.PublicTool;
using System;
using System.Net;
namespace ChessMiddle.Basics
{
    /// <summary>
    /// 一个有基本属性的父类(FatherTxBase);
    /// </summary>
    public class FTxBase : ITxBase
    {
        #region ITxBase 成员
        /// <summary>
        /// 当引擎非正常原因自动断开的时候触发此事件
        /// </summary>
        public event TxDelegate<string> EngineLost;
        /// <summary>
        /// 当引擎完全关闭释放资源的时候
        /// </summary>
        public event TxDelegate EngineClose;
        /// <summary>
        /// 当接收到字节数据的时候,触发此事件
        /// </summary>
        public event TxDelegate<IPEndPoint, byte[]> AcceptByte;
        /// <summary>
        /// 当将数据发送成功且对方已经收到的时候,触发此事件
        /// </summary>
        public event TxDelegate<IPEndPoint> dateSuccess;

        /// <summary>
        /// 引擎是否启动
        /// </summary>
        protected bool _engineStart = false;
        private string _ip = "";//服务器的IP地址
        private int _bufferSize = 1024;//缓冲区大小
        private int _port = 0;
        private IPEndPoint _ipEndPoint = null;//终结点地址项目里面用
        /// <summary>
        /// 本地的终结点地址封装；
        /// </summary>
        internal IPEndPoint IpEndPoint
        {
            get
            {
                try
                {
                    IPAddress ipAddress = null;
                    if (Ip == "")
                        ipAddress = IPAddress.Any;
                    else
                        ipAddress = IPAddress.Parse(Ip);
                    _ipEndPoint = new IPEndPoint(ipAddress, Port);
                    _port = _ipEndPoint.Port;
                }
                catch { throw; }
                return _ipEndPoint;
            }
        }
        /// <summary>
        /// ip 的getter和setter
        /// </summary>
        public string Ip { get => _ip; set => _ip = value; }
        /// <summary>
        /// port 的getter和setter
        /// </summary>
        public int Port { get => _port; set => _port = value; }
        /// <summary>
        /// 缓冲区大小
        /// </summary>
        public int BufferSize
        {
            get
            {
                return _bufferSize;
            }
            set
            {
                _bufferSize = value;
            }
        }
        /// <summary>
        /// 引擎是否已经启动;
        /// </summary>
        public bool EngineStart
        {
            get { return _engineStart; }
        }

        /// <summary>
        /// 启动引擎
        /// </summary>
        virtual public void StartEngine()
        {
            throw new NotImplementedException();
        }
        /// <summary>
        /// 关闭引擎释放资源
        /// </summary>
        virtual public void CloseEngine()
        {
            throw new NotImplementedException();
        }
        #endregion

        #region 事件注册成方法

        /// <summary>
        /// 当引擎非正常原因自动断开的时候触发此事件
        /// </summary>
        /// <param name="str">断开原因</param>
        internal void OnEngineLost(string str)
        {
            if (this.EngineLost != null)
            {
                CommonMethod.eventInvoket(() => { this.EngineLost(str); });
            }
        }

        /// <summary>
        /// 当引擎完全关闭释放资源的时候触发此事件
        /// </summary>
        internal void OnEngineClose()
        {
            if (this.EngineClose != null && _engineStart == true)
            {
                _engineStart = false;
                CommonMethod.eventInvoket(() => { this.EngineClose(); });
            }
        }
        /// <summary>
        /// 当接收到字节数据的时候,触发此事件
        /// </summary>
        /// <param name="iPEndPoint">对方终结点</param>
        /// <param name="bytes">字节数据</param>
        internal void OnAcceptByte(IPEndPoint iPEndPoint, byte[] bytes)
        {
            if (this.AcceptByte != null)
            {
                CommonMethod.eventInvoket(() => { this.AcceptByte(iPEndPoint, bytes); });
            }
        }

        /// <summary>
        /// 当将数据发送成功且对方已经收到的时候,触发此事件
        /// </summary>
        /// <param name="iPEndPoint">对方终结点</param>
        internal void OndateSuccess(IPEndPoint iPEndPoint)
        {
            if (this.dateSuccess != null)
            {
                CommonMethod.eventInvoket(() => { this.dateSuccess(iPEndPoint); });
            }
        }

        #endregion

        /// <summary>
        /// 向终端发送字节数据
        /// </summary>
        /// <param name="stateOne">连接对象</param>
        /// <param name="data">数据</param>
        internal void sendMessage(StateBase stateOne, byte[] data)
        {
            if (stateOne == null)
                return;
            Send(stateOne, data);
        }
        /// <summary>
        /// 向终端发送数据,最基础的发送
        /// </summary>
        /// <param name="stateOne">StateBase</param>
        /// <param name="data">发送的数据</param>
        virtual internal void Send(StateBase stateOne, byte[] data)
        { }

    }
}
