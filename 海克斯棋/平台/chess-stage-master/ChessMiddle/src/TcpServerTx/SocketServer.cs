using ChessMiddle.Basics;
using ChessMiddle.ChessFactory;
using ChessMiddle.PublicClass;
using ChessMiddle.PublicTool;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Net;
using System.Net.Sockets;
using System.Threading;


namespace ChessMiddle
{
    /// <summary>
    /// 面向服务器的主线程类!
    /// </summary>
    public class SocketServer : TcpFTxBase, ITxServer
    {
        /// <summary>
        /// 带参数的构造函数
        /// </summary>
        /// <param name="port">端口号</param>
        internal SocketServer(int port)
        {
            Port = port;
            if (state == null)
                state = new List<TcpState>();
            Thread.CurrentThread.Name = "main";
        }

        private Xytq _chess;
        //棋局监控属性
        private Dictionary<char, StateBase> roleTable = new Dictionary<char, StateBase>();//角色分配表
        private TcpState currentToken = null;//当前走棋位
        private double limitThinkSeconds = 2;

        #region 基本属性区块
        private List<TcpState> state = null;//所有客户端
        private Socket listener = null;
        private int _clientMax;//允许最多客户端数

        /// <summary>
        /// 当有客户连接成功的时候,触发此事件
        /// </summary>
        public event TxDelegate<IPEndPoint> Connect;
        /// <summary>
        /// 当有客户突然断开的时候,触发此事件,文本参数是代表断开的原因
        /// </summary>
        public event TxDelegate<IPEndPoint, string> Disconnection;
        /// <summary>
        /// 在UI上下棋
        /// </summary>
        public event TxDelegate<List<string>,char[,], char, char> PlayChess;
        /// <summary>
        /// 当前客户端数量
        /// </summary>
        public int ClientNumber
        {
            get { return state.Count; }
        }
        /// <summary>
        /// 允许最多客户端数
        /// </summary>
        public int ClientMax
        {
            get { return _clientMax; }
            set
            {
                if (value > 100)
                    _clientMax = 100;
                else
                    _clientMax = value;
            }
        }
        /// <summary>
        /// 得到所有的客户端
        /// </summary>
        List<IPEndPoint> ITxServer.ClientAll
        {
            get
            {
                if (state == null || state.Count == 0)
                    return null;
                List<IPEndPoint> IpEndPoint = new List<IPEndPoint>();
                foreach (TcpState stateOne in state)
                {
                    IpEndPoint.Add(stateOne.IpEndPoint);
                }
                return IpEndPoint;
            }
        }

        /// <summary>
        /// 设置限时秒数
        /// </summary>
        public double LimitThinkSeconds { get => limitThinkSeconds; set => limitThinkSeconds = value; }

        #endregion

        #region 启动以及接收客户端区块
        /// <summary>
        /// 启动服务器,如果没出现异常,说明启动成功
        /// </summary>
        override public void StartEngine()
        {
            if (EngineStart)
                return;
            //实例化一个西洋跳棋
            char[,] layout = new char[8, 8]{
                    {'0',  'B', '0',  'a',  '0',  'a',  '0',  'a' },
                    {'0',  '0', 'a',  '0',  '0',  '0',  '0',  '0' },
                    {'0',  'a', '0',  '0',  'a',  '0',  'a',  '0' },
                    {'0',  '0', 'a',  '0',  '0',  '0',  '0',  '0' },
                    {'0',  '0', '0',  '0',  'a',  '0',  '0',  '0' },
                    {'0',  '0', '0',  'b',  '0',  '0',  '0',  '0' },
                    {'0',  '0', '0',  'a',  '0',  '0',  '0',  '0' },
                    {'0',  '0', '0',  '0',  'b',  '0',  'b',  '0' }
            };
            _chess = new Xytq();
            _clientMax = _chess.Role.Length;
            try
            {
                listener = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                listener.Bind(IpEndPoint);
                listener.Listen(20);
                listener.BeginAccept(new AsyncCallback(AcceptCallback), listener);
                _engineStart = true;//启动成功
            }
            catch (Exception Ex)
            {
                CloseEngine();
                throw new Exception(Ex.Message);
            }
        }
        /// <summary>
        /// 当连接一个客户端之后的回调函数
        /// </summary>
        /// <param name="ar">TcpClient</param>
        private void AcceptCallback(IAsyncResult ar)
        {
            TcpState stateOne = null;
            try
            {
                Socket Listener = (Socket)ar.AsyncState;
                Socket handler = Listener.EndAccept(ar);
                Console.WriteLine("get one...");
                stateOne = new TcpState(handler, BufferSize);

                /*可假色只有两个玩家*/
                int curRoleCount = roleTable.Count;
                if (curRoleCount < _clientMax)
                {
                    roleTable.Add(_chess.Role[curRoleCount], stateOne);
                    loginSuccess(stateOne);
                    stateOne.WorkSocket.BeginReceive(stateOne.Buffer, 0, stateOne.Buffer.Length
                        , 0, new AsyncCallback(ReadCallback), stateOne);
                    //玩家全部到齐
                    if (curRoleCount == _clientMax-1)
                    {
                        currentToken = (TcpState)roleTable[_chess.Role[0]];
                        //给第一个玩家发送开始游戏的指令
                        sendNextEpisode(roleTable[_chess.Role[0]], _chess.Role[0].ToString());
                    }
                }
                else
                {
                    clientClose(stateOne);
                }
                Listener.BeginAccept(new AsyncCallback(AcceptCallback), Listener);
            }
            catch (Exception Ex)
            {
                OnEngineLost(Ex.Message);//当服务器突然断开触发此事件
                CloseEngine();
            }
        }
        #endregion

        #region 接收数据区块

        /// <summary>
        /// 当接收到数据之后的回调函数
        /// </summary>
        /// <param name="ar"></param>
        private void ReadCallback(IAsyncResult ar)
        {
            //todo 当客户端断开源码会自动执行这个回调? 如何避免
            TcpState stateOne = (TcpState)ar.AsyncState;
            Socket handler = stateOne.WorkSocket;
            try
            {
                int bytesRead = handler.EndReceive(ar);
                if (bytesRead > 0)
                {
                    byte[] haveDate = ReceiveDateOne.DateOneManage(stateOne, bytesRead);//接收完成之后对数组进行重置
                    //数据处理
                    OnAcceptByte(stateOne.IpEndPoint, haveDate);
                    dataClassify(stateOne, TcpDateOne(stateOne, haveDate));
                }
                handler.BeginReceive(stateOne.Buffer, 0, stateOne.Buffer.Length,
                        0, new AsyncCallback(ReadCallback), stateOne);
            }
            catch (Exception Ex)
            {
                int i = Ex.Message.IndexOf("远程主机强迫关闭了一个现有的连接");
                if (stateOne != null && i != -1)
                { socketRemove(stateOne, Ex.Message); }
            }
        }

        internal override void dataClassify(TcpState stateOne, object dic)
        {
            Dictionary<string, object> data = (Dictionary<string, object>)dic;
            string type = (string)data["type"];
            switch (type)
            {
                case "close":
                    {
                        socketRemove(stateOne, "客户端自己退出！");
                        break;
                    }
                case "action":
                    {
                        //判断令牌对不对
                        if (!stateOne.IpEndPoint.Equals(currentToken.IpEndPoint))
                            return;
                        //可用data里的role替换已再次验证数据来源
                        char role = (char)CommonMethod.getKeyByValue(roleTable, stateOne)[0];
                        List<string> changes = new List<string>();
                        ArrayList changesSource = ((ArrayList)(data["changes"]));
                        foreach (string i in changesSource)
                        {
                            changes.Add(i);
                        }
                        //下棋并检测合法性
                        //合法性失败发送违法api,并允许其继续在限制时间内下棋
                        if (!_chess.DoChess(changes, role))
                        {
                            Send(stateOne, API.getIllegalAPI("棋步违法!"));
                            //先强制中断
                            //socketRemove(stateOne, "违法棋");
                            break;
                        }
                        //多线程可同时读一个内存变量,而写要加锁
                        stateOne.ThinkInTime = true;

                        char result = _chess.GetResult(role);
                        //todo 将行动的棋显示在UI上
                        OnChessPlay(changes, _chess.ChessLayout, role, result);
                        sendResult(result, true);

                        if (result == _chess.NOT_DONE)
                            refreshAndNext(stateOne);

                        break;
                    }
                default:
                    break;

            }
        }

        /// <summary>
        /// 判断局势并发送相应接口通知客户端
        /// </summary>
        /// <param name="result">代表结果的字符</param>
        /// <param name="win">result如果是身份，win代表是赢是输</param>
        internal void sendResult(char result, Boolean win)
        {
            if (result != _chess.NOT_DONE)
            {
                if (CommonMethod.IndexOfKeys(roleTable, result) == -1)
                {
                    //平局(除了角色表外的其它字符均可代表)
                    foreach (StateBase s in roleTable.Values)
                    {
                        Send(s, API.getResultAPI("draw"));
                    }
                }
                else
                {
                    //有人获胜(针对角色表)
                    foreach (char player in roleTable.Keys)
                    {
                        if (player == result)
                        {
                            if (win)
                            {
                                Send(roleTable[player], API.getResultAPI("win"));
                            }
                            else
                            {
                                Send(roleTable[player], API.getResultAPI("fail"));
                            }
                        }
                        else
                        {
                            if (win)
                            {
                                Send(roleTable[player], API.getResultAPI("fail"));
                            }
                            else
                            {
                                Send(roleTable[player], API.getResultAPI("win"));
                            }
                        }
                    }
                }
                MatchFinish();
            }
        }

        private void MatchFinish()
        {
            _chess = new Xytq();
            _clientMax = _chess.Role.Length;
            TcpState[] temp =new TcpState[state.Count];
            state.CopyTo(temp);
            //休眠10秒看结果
            Thread.Sleep(10000);
            foreach (TcpState t in temp)
            {
                socketRemove(t, "比赛结束");
            }
        }
        #endregion
        
        #region 通知下一个玩家并检测是否超时

        private void refreshAndNext(StateBase state)
        {
            TcpState stateOne = (TcpState)state;

            //令牌更新
            int oldStatePostion = this.state.IndexOf(stateOne);
            int newStatePostion = oldStatePostion + 1 == this.state.Count
                ? 0 : oldStatePostion + 1;
            currentToken = this.state[newStatePostion];
            //等到UI画完后才通知下一个选手
            Thread.Sleep(2000);
            sendNextEpisode(currentToken,
                (CommonMethod.getKeyByValue(roleTable, currentToken)[0].ToString()));
        }

        /// <summary>
        /// 向下一个选手发送棋局信息
        /// </summary>
        /// <param name="state"></param>
        /// <param name="role"></param>
        private void sendNextEpisode(StateBase state, string role)
        {
            Send(state, API.getNextEpisodeAPI(_chess.ChessLayout, _chess.Size, role, limitThinkSeconds));

            Thread thinkTimeoutThread = new Thread(ThinkTimeout);
            thinkTimeoutThread.IsBackground = true;
            thinkTimeoutThread.Start(state);
        }

        /// <summary>
        /// 思考超时会发送超时信息的线程方法
        /// </summary>
        /// <param name="state">思考者</param>
        private void ThinkTimeout(object state)
        {
            TcpState stateOne = (TcpState)state;
            DateTime startTime = DateTime.Now;
            while (true)
            {
                Thread.Sleep(50);
                if (stateOne.ThinkInTime)
                {
                    stateOne.ThinkInTime = false;
                    return;
                }

                if ((DateTime.Now - startTime).TotalSeconds > limitThinkSeconds)
                {
                    if (!clientCheck(stateOne.IpEndPoint))
                        return;
                    
                    ////直接判输方法
                    char loser = (char)CommonMethod.getKeyByValue(roleTable, stateOne)[0];
                    char winner = getOtherGamer(loser);
                    OnChessPlay(new List<string>(), _chess.ChessLayout, '0', winner);
                    sendResult(loser, false);

                    ////使用默认走棋方法
                    //char role = (char)CommonMethod.getKeyByValue(roleTable, stateOne)[0];
                    //Send(stateOne, API.getTimeoutAPI());
                    //List<string> changes = _chess.DefaultDo(role);
                    //char result = _chess.GetResult();

                    //OnChessPlay(changes, _chess.ChessLayout, role, result);
                    //sendResult(result);
                    //if (result == _chess.NOT_DONE)
                    //{
                    //    refreshAndNext(stateOne);
                    //}

                    return;
                }
            }
        }

        private char getOtherGamer(char loser)
        {
            char winner = '0';
            foreach(char kv in roleTable.Keys)
            {
                if (loser != kv)
                {
                    winner = kv;
                    break;
                } 
            }
            return winner;
        }

        #endregion

        #region 向客户端发送数据的区块
        /// <summary>
        /// 向客户端发送数据,最基础的发送
        /// </summary>
        /// <param name="stateBase">TcpState</param>
        /// <param name="data">发送的数据</param>
        override internal void Send(StateBase stateBase, byte[] data)
        {
            if (stateBase == null)
                return;
            try
            {
                stateBase.WorkSocket.BeginSend(data, 0, data.Length, 0, new AsyncCallback(SendCallback), stateBase);
            }
            catch (Exception Ex)
            {
                int i = Ex.Message.IndexOf("远程主机强迫关闭了一个现有的连接");
                if (i != -1)
                {
                    TcpState stateOne = IPEndPointToState(stateBase.IpEndPoint);
                    socketRemove(stateOne, Ex.Message);
                }
            }
        }
        /// <summary>
        /// 发送完数据之后的回调函数
        /// </summary>
        /// <param name="ar">Clicent</param>
        private void SendCallback(IAsyncResult ar)
        {
            StateBase stateBase = (StateBase)ar.AsyncState;
            if (stateBase == null)
                return;
            Socket handler = stateBase.WorkSocket;
            try
            {
                int bytesSent = handler.EndSend(ar);
            }
            catch
            {
            }
        }

        /// <summary>
        /// 服务器向客户端发送字节数据
        /// </summary>
        /// <param name="ipEndPoint">IPEndPoint</param>
        /// <param name="data">字节的数据</param>
        public void sendMessage(IPEndPoint ipEndPoint, byte[] data)
        {
            TcpState stateOne = IPEndPointToState(ipEndPoint);
            Send(stateOne, data);
        }
        #endregion

        #region 针对表现层需要操作的一些方法

        /// <summary>
        /// 获得棋局
        /// </summary>
        /// <returns></returns>
        public char[,] GetChessLayout()
        {
            return _chess.ChessLayout;
        }

        /// <summary>
        /// 获得棋局的字符串表示
        /// </summary>
        /// <returns></returns>
        public string GetChessLayoutStr()
        {
            return _chess.ToString();
        }

        /// <summary>
        /// 下棋
        /// </summary>
        /// <param name="data">关键数据</param>
        /// <param name="layout">得到棋局</param>
        /// <param name="role">玩家身份</param>
        /// <param name="result">棋步下完后的结果</param>
        public void OnChessPlay(List<string> data, char[,] layout, char role, char result)
        {
            CommonMethod.eventInvoket(() => { PlayChess(data, layout, role, result); });
        }

        /// <summary>
        /// 客户端完全登录成功之后要处理的事情
        /// </summary>
        /// <param name="stateOne">TcpState</param>
        private void loginSuccess(TcpState stateOne)
        {
            stateOne.ConnectOk = true;
            state.Add(stateOne);
            CommonMethod.eventInvoket(() => { Connect(stateOne.IpEndPoint); });
        }

        /// <summary>
        /// 关闭服务器,释放所有资源
        /// </summary>
        override public void CloseEngine()
        {
            try
            {
                clientAllClose();
                state = null;
                if (listener != null)
                    listener.Close();
                listener = null;
                OnEngineClose();
            }
            catch { }
        }

        /// <summary>
        /// 关闭所有客户端连接
        /// </summary>
        public void clientAllClose()
        {
            foreach (TcpState stateo in state)
            {
                socketRemove(stateo, "服务器关闭所有的客户端");
            }
        }

        /// <summary>
        /// 发送代码的形式服务器强制关闭一个客户端
        /// </summary>
        /// <param name="stateOne">TcpState</param>
        private void clientClose(TcpState stateOne)
        {
            if (stateOne == null || ClientNumber == 0)
                return;
            state.Remove(stateOne);
            roleTable.Remove((char)CommonMethod.getKeyByValue(roleTable, stateOne)[0]);
            Send(stateOne, API.getCloseAPI(""));//发送一个强制关闭的代码过去
        }
        /// <summary>
        /// 服务器强制关闭一个客户端
        /// </summary>
        /// <param name="ipEndPoint">IPEndPoint</param>
        public void clientClose(IPEndPoint ipEndPoint)
        {
            TcpState stateOne = IPEndPointToState(ipEndPoint);
            clientClose(stateOne);
        }

        /// <summary>
        /// 关闭相连的scoket以及关联的TcpState,释放所有的资源
        /// </summary>
        /// <param name="stateOne">TcpState</param>
        /// <param name="str">原因</param>
        private void socketRemove(TcpState stateOne, string str)
        {
            if (stateOne == null)
                return;
            stateOne.WorkSocket.Close();

            List<object> findGamer = CommonMethod.getKeyByValue(roleTable, stateOne);
            if (findGamer.Count != 0) 
                roleTable.Remove((char)findGamer[0]);

            if (state.Remove(stateOne))//当没有登录的时候断掉，不触发下面的事件
            {
                CommonMethod.eventInvoket(() => { Disconnection(stateOne.IpEndPoint, str); }); //当客户端断掉的时候触发此事件
            }
            stateOne = null;
        }

        /// <summary>
        /// 检查某个客户端是否在线
        /// </summary>
        /// <param name="ipEndPoint">IPEndPoint</param>
        /// <returns>bool</returns>
        public bool clientCheck(IPEndPoint ipEndPoint)
        {
            TcpState stateOne = IPEndPointToState(ipEndPoint);
            if (stateOne == null)
                return false;
            return true;
        }

        /// <summary>
        /// 用ip地址从state里找到TcpState
        /// </summary>
        /// <param name="ipEndPoint">IPEndPoint</param>
        /// <returns>TcpState</returns>
        private TcpState IPEndPointToState(IPEndPoint ipEndPoint)
        {
            try
            {
                return state.Find(delegate (TcpState state1) { return state1.IpEndPoint == ipEndPoint; });
            }
            catch { return null; }
        }

        #endregion
    }
}
