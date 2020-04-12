using ChessMiddle.Basics;
using ChessMiddle.ChessFactory;
using ChessMiddle.PublicClass;
using ChessMiddle.PublicTool;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Text;
using System.Threading;
namespace ChessMiddle
{
    //todo 设计一套客户端接口方便其它语言实现

    /// <summary>
    /// 面向客户端的主线程类!
    /// </summary>
    public class SocketClient : TcpFTxBase, ITxClient
    {
        private char _player;
        /// <summary>
        /// 服务器分配的玩家角色
        /// </summary>
        public char Player { get => _player; set => _player = value; }
        private Xytq chess;

        #region 基本属性区块
        private TcpState stateOne = null;
        private int _reconnectMax = 5;//当连接断开时是否重连,0为不重连,默认重连三次;
        private int _outTime = 10;//登录超时时间
        private bool outTimeBool = false;//超时用到的临时变量
        private bool reconnectOn = false;//有没有在重连的临时变量
        private int reconnectCi = 0;//已经重连了几次的临时变量
        /// <summary>
        /// 自动重连开始的时候,触发此事件
        /// </summary>
        public event TxDelegate ReconnectionStart;
        /// <summary>
        /// 客户端登录成功或失败都会触发此事件,登录失败的话会有失败的原因
        /// </summary>
        public event TxDelegate<bool, string> StartResult;

        /// <summary>
        /// 当轮到本方下棋时,显示所有可行解
        /// </summary>
        public event TxDelegate<List<List<string>>> ShowAbleMove;

        /// <summary>
        /// 当连接断开时是否重连,默认重连;
        /// </summary>
        public int ReconnectMax
        {
            get { return _reconnectMax; }
            set { _reconnectMax = value; }
        }
        /// <summary>
        /// 登录超时时间设置，默认10秒
        /// </summary>
        public int OutTime
        {
            get { return _outTime; }
            set { _outTime = value; }
        }

        /// <summary>
        /// 带参数的构造函数
        /// </summary>
        /// <param name="ip">服务器的ip地址</param>
        /// <param name="port">服务器的端口</param>
        internal SocketClient(string ip, int port)
        { Ip = ip; Port = port; }
        #endregion
        
        #region 启动客户端区块
        /// <summary>
        /// 启动客户端,设置超时间在OutTime里设置,无论失败或成功都会触发StartResult事件;
        /// </summary>
        override public void StartEngine()
        {
            if (_engineStart)
                return;
            Thread StartThread = new Thread(start);
            StartThread.IsBackground = true;
            StartThread.Start();
        }
        /// <summary>
        /// 启动客户端基础的一个线程
        /// </summary>
        private void start()
        {
            chess = new Xytq();
            if (reconnectOn)//如果是重连的延迟10秒
                Thread.Sleep(9000);
            try
            {
                Socket socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                socket.SendTimeout = 1000;
                socket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.NoDelay, true);
                socket.BeginConnect(IpEndPoint, new AsyncCallback(AcceptCallback), socket);
                loginTimeout(socket);//超时判断方法
            }
            catch (Exception Ex)
            {
                loginFailure(Ex.Message);//登录失败触发
            }
        }
        /// <summary>
        /// 当连接服务器之后的回调函数
        /// </summary>
        /// <param name="ar">TcpClient</param>
        private void AcceptCallback(IAsyncResult ar)
        {
            Socket socket = (Socket)ar.AsyncState;
            stateOne = new TcpState(socket, BufferSize);
            try
            {
                socket.EndConnect(ar);
                loginSuccess();
                Console.WriteLine("logining...");
                socket.BeginReceive(stateOne.Buffer, 0, stateOne.Buffer.Length, 0, new AsyncCallback(ReadCallback), stateOne);
            }
            catch (Exception Ex)
            {
                if (outTimeBool == true)
                    return; //说明已经超时了，已经触发登录失败了
                loginFailure(Ex.Message);//登录失败触发
            }
        }
        #endregion

        #region  登录篇
        /// <summary>
        /// 重连模块
        /// </summary>
        private void reconnect()
        {
            if (_reconnectMax == 0)
                return;//不重连直接返回
            reconnectCi++;//每重连一次重连的次数加1
            if (stateOne != null)
            {
                stateOne.WorkSocket.Close();
                stateOne = null;
            }
            if (reconnectOn == false)
            {
                reconnectOn = true;
                CommonMethod.eventInvoket(() => { ReconnectionStart(); });
            }
            _engineStart = false;
            StartEngine();
        }
        /// <summary>
        /// 登录之超时判断
        /// </summary>
        private void loginTimeout(Socket socket)
        {
            DateTime time1 = DateTime.Now;
            outTimeBool = false;
            while (true)
            {
                Thread.Sleep(10);
                if (_engineStart == true || outTimeBool == true)
                    break;
                if ((int)(DateTime.Now - time1).TotalSeconds > _outTime)
                {
                    outTimeBool = true; socket.Close();
                    loginFailure("连接超时");//登录失败触发
                    break;
                }
            }
        }
        /// <summary>
        /// 登录失败之后要处理的事情
        /// </summary>
        private void loginFailure(string str)
        {
            outTimeBool = true;//登录有结果了，判断超时的线程跳出
            if (_engineStart == true)//失败的时候引擎都是关闭的
                return;
            if (reconnectOn && reconnectCi < _reconnectMax)
                reconnect();//继续重连
            else
            {
                if (reconnectOn)
                    CommonMethod.eventInvoket(() => { StartResult(false, "重连失败" + str); });
                else
                    CommonMethod.eventInvoket(() => { StartResult(false, str); });
                CloseEngine();
            }
        }
        /// <summary>
        /// 当客户端完全连接上服务器之后要处理的一些事情
        /// </summary>
        private void loginSuccess()
        {
            _engineStart = true;
            if (reconnectOn)
            {
                CommonMethod.eventInvoket(() => { StartResult(true, "重连成功"); });
                reconnectOn = false;
                reconnectCi = 0;
            }
            else
            {
                CommonMethod.eventInvoket(() => { StartResult(true, "启动成功"); });
            }
        }
        #endregion

        #region 接收到信息区块
        /// <summary>
        /// 当接收到数据之后的回调函数
        /// </summary>
        /// <param name="ar"></param>
        private void ReadCallback(IAsyncResult ar)
        {
            if (stateOne == null)
                return;
            Socket handler = stateOne.WorkSocket;
            try
            {
                int bytesRead = handler.EndReceive(ar);
                if (bytesRead > 0)
                {
                    //MessageBox.Show(stateOne.Buffer[0].ToString());
                    byte[] haveDate = ReceiveDateOne.DateOneManage(stateOne, bytesRead); //接收完成之后对数组进行重置
                    handler.BeginReceive(stateOne.Buffer, 0, stateOne.Buffer.Length, 0, new AsyncCallback(ReadCallback), stateOne);
                    dataClassify(TcpDateOne(stateOne, haveDate));
                }
                else { handler.BeginReceive(stateOne.Buffer, 0, stateOne.Buffer.Length, 0, new AsyncCallback(ReadCallback), stateOne); }
            }
            catch (Exception Ex)
            {
                lostClient(Ex.Message);//当突然断开的时候
            }
        }
        #endregion

        private void dataClassify(Dictionary<string, object> data)
        {
            switch((string)data["type"])
            {
                case "do_algorithm":
                {
                    Console.WriteLine("in do");
                    SendList(chess.NextLayout(( (string)data["role"]).ToCharArray()[0], Convertor.arrayListTo2D(data["now"]) ));
                    chess.Print(Convertor.arrayListTo2D(data["now"]));
                    break;
                }
            }
        }

        private void SendList(Dictionary<List<string>, char[,]> dictionary)
        {
            List<List<string>> ableMove = new List<List<string>>();
            foreach(List<string> one in dictionary.Keys)
            {
                ableMove.Add(one);
            }
            CommonMethod.eventInvoket(() => { ShowAbleMove (ableMove); });
        }

        #region 发送信息区块
        /// <summary>
        /// 客户端向服务器发送字节数据
        /// </summary>
        /// <param name="data">字节数据</param>
        public void sendMessage(byte[] data)
        {
            sendMessage(stateOne, data);
        }
        /// <summary>
        /// 向服务器发送数据,最基础和原始的
        /// </summary>
        /// <param name="stateBase">StateBase</param>
        /// <param name="data">发送的数据</param>
        override internal void Send(StateBase stateBase, byte[] data)
        {
            if (stateBase == null)
                return;
            try
            {
                stateBase.SendDate = data;
                stateBase.WorkSocket.BeginSend(data, 0, data.Length, 0, new AsyncCallback(SendCallback), stateBase);
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }
        /// <summary>
        /// 发送完数据之后的回调函数
        /// </summary>
        /// <param name="ar">Clicent</param>
        private void SendCallback(IAsyncResult ar)
        {
            try
            {
                TcpState stateOne = (TcpState)ar.AsyncState;
                int bytesSent = stateOne.WorkSocket.EndSend(ar);
                Console.WriteLine(Encoding.Default.GetString(stateOne.SendDate));
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }
        #endregion

        #region  断开篇
        /// <summary>
        /// 关闭相连的scoket以及关联的StateObject,释放所有的资源
        /// </summary>
        override public void CloseEngine()
        {
            if (stateOne != null)
            {
                stateOne.ConnectOk = true;
                stateOne.WorkSocket.Close();
            }
            reconnectOn = false;
            reconnectCi = 0;//前面三个初始化
            OnEngineClose();//引擎完全释放资源触发此事件
        }
        /// <summary>
        /// 当客户端突然与服务器断开的时候
        /// </summary>
        /// <param name="str"></param>
        private void lostClient(string str)
        {
            if (stateOne.ConnectOk == true)
                return;//说明这个引擎已经触发关闭了;下面的就不用执行了
            if (_engineStart == false)
            { loginFailure(str); return; }//这里说明已经登录了；但由于服务器的原因被拒绝了
            OnEngineLost(str);//当客户端突然断开的时候触发此事件
            if (_reconnectMax > 0)//如果是重连就重连，不重连就关闭客户端释放资源
                reconnect();
            else
                CloseEngine();
        }
        #endregion

    }
}