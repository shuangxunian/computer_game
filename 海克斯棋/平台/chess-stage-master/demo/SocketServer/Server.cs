using ChessMiddle;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Net;
using System.Text.RegularExpressions;
using System.Threading;
using System.Windows.Forms;
namespace SocketServer
{
    public partial class Server : Form
    {
        #region TCPServer服务器

        private char[,] chess;
        private ITxServer server = null;
        private PictureBox[] blackItems;
        private PictureBox[] redItems;
        Object locker = new Object();

        public Server()
        {
            InitializeComponent();
            CheckForIllegalCrossThreadCalls = false;
            chessUIInit();
        }

        /// <summary>
        /// 当接收到来之客户端的字节信息的时候
        /// </summary>
        /// <param name="ipEndPoint"></param>
        /// <param name="bytes"></param>
        private void acceptBytes(IPEndPoint ipEndPoint, byte[] bytes)
        {
            ListViewItem item = new ListViewItem(new string[] { DateTime.Now.ToString(),
                ipEndPoint.ToString(), System.Text.Encoding.Default.GetString(bytes) });
            this.listView1.Items.Insert(0, item);
        }

        /// <summary>
        /// 当有客户端连接上来的时候
        /// </summary>
        /// <param name="state"></param>
        private void connect(IPEndPoint ipEndPoint)
        {
            show(ipEndPoint, "上线");
            ResetChessUI();
        }

        /// <summary>
        /// 当对方已收到我方发送数据的时候
        /// </summary>
        /// <param name="state"></param>
        private void datesuccess(IPEndPoint ipendpoint)
        {
            //textbox_msg.text = "已向" + ipendpoint.tostring() + "发送成功";
        }

        /// <summary>
        /// 当有客户端掉线的时候
        /// </summary>
        /// <param name="state"></param>
        /// <param name="str"></param>
        private void disconnection(IPEndPoint ipEndPoint, string str)
        {
            show(ipEndPoint, "下线");
        }

        /// <summary>
        /// 当服务器完全关闭的时候
        /// </summary>
        private void engineClose()
        {
            MessageBox.Show("服务器已关闭");
        }

        /// <summary>
        /// 当服务器非正常原因断开的时候
        /// </summary>
        /// <param name="str"></param>
        private void engineLost(string str)
        {
            MessageBox.Show(str);
        }

        /// <summary>
        /// 下面显示的
        /// </summary>
        /// <param name="ipEndPoint"></param>
        /// <param name="str"></param>
        private void show(IPEndPoint ipEndPoint, string str)
        {
            label_zt.Text = ipEndPoint.ToString() + ":" + str;
            label_all.Text = "当前在线人数:" + this.server.ClientNumber.ToString();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            // server.clientClose(server.StateAll[0]);
            MessageBox.Show(server.ClientAll[0].ToString());
        }

        /// <summary>
        /// 启动按钮Tcp服务器
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                server = TxStart.startServer(int.Parse(textBox_port.Text));
                server.AcceptByte += new TxDelegate<IPEndPoint, byte[]>(acceptBytes);
                server.Connect += new TxDelegate<IPEndPoint>(connect);
                //server.dateSuccess += new TxDelegate<IPEndPoint>(dateSuccess);
                server.Disconnection += new TxDelegate<IPEndPoint, string>(disconnection);
                server.EngineClose += new TxDelegate(engineClose);
                server.EngineLost += new TxDelegate<string>(engineLost);
                server.PlayChess += new TxDelegate<List<string>, char[,], char, char>(playChess);
                server.LimitThinkSeconds = 300;
                server.StartEngine();
                this.button1.Enabled = false;
                chessInit(server.GetChessLayout());
            }
            catch (Exception Ex)
            {
                MessageBox.Show(Ex.Message);
            }

        }

        private void chessUIInit()
        {
            this.layerImageBox.Load(System.IO.Path.Combine(Application.StartupPath + @"\..\..\res\dribble.jpg"));
            blackItems = new PictureBox[12];
            redItems = new PictureBox[12];
            Random x = new Random();
            for (int i = 0; i < 12; i++)
            {
                blackItems[i] = new PictureBox();
                layerImageBox.Controls.Add(blackItems[i]);
                blackItems[i].Name = "black_" + i;
                blackItems[i].BackColor = Color.Transparent;
                blackItems[i].Load(System.IO.Path.Combine(Application.StartupPath + @"\..\..\res\black.png"));
                blackItems[i].Size = new System.Drawing.Size(53, 53);
                blackItems[i].Location = GetAbsoluteLocation(i / 4, ((i / 4) % 2 == 0 ? 1 : 0) + 2 * (i % 4));
                blackItems[i].BringToFront();

                redItems[i] = new PictureBox();
                layerImageBox.Controls.Add(redItems[i]);
                redItems[i].Name = "red_" + i;
                redItems[i].BackColor = Color.Transparent;
                redItems[i].Load(System.IO.Path.Combine(Application.StartupPath + @"\..\..\res\red.png"));
                redItems[i].Size = new System.Drawing.Size(53, 53);
                redItems[i].Location = GetAbsoluteLocation(i / 4 + 5, ((i / 4) % 2 == 0 ? 0 : 1) + 2 * (i % 4));
                redItems[i].BringToFront();
            }
        }

        private void ResetChessUI()
        {
            for (int i = 0; i < 12; i++)
            {
                blackItems[i].Load(System.IO.Path.Combine(Application.StartupPath + @"\..\..\res\black.png"));
                blackItems[i].Location = GetAbsoluteLocation(i / 4, ((i / 4) % 2 == 0 ? 1 : 0) + 2 * (i % 4));
                redItems[i].Load(System.IO.Path.Combine(Application.StartupPath + @"\..\..\res\red.png"));
                redItems[i].Location = GetAbsoluteLocation(i / 4 + 5, ((i / 4) % 2 == 0 ? 0 : 1) + 2 * (i % 4));
            }
            resultLabel.Text = ":)";
        }

        Point GetAbsoluteLocation(int x, int y)
        {
            Point p = new Point();
            p.Y = 125 + 70 * x;
            p.X = 28 + 70 * y;
            return p;
        }

        private void movePieceAsyn(List<string> actionMove, char[,] layout, char role, char result)
        {
            LockMoving lockv = new LockMoving(false);
            new Thread(new ThreadStart(() =>
            {
                foreach (string s in actionMove)
                {
                    Console.WriteLine(s);
                }
                foreach (string s in actionMove)
                {
                    Thread.Sleep(10);
                    string[] posFormat = Regex.Split(s, "-");
                    int startPointX = Convert.ToInt32(posFormat[0].Split(',')[0]);
                    int startPointY = Convert.ToInt32(posFormat[0].Split(',')[1]);
                    int endPointX = Convert.ToInt32(posFormat[1].Split(',')[0]);
                    int endPointY = Convert.ToInt32(posFormat[1].Split(',')[1]);
                    Point startPos = GetAbsoluteLocation(startPointX, startPointY);
                    Point endPos = GetAbsoluteLocation(endPointX, endPointY);
                    Point victimPos = new Point(0, 0);
                    if (Math.Abs(startPointX - endPointX) > 1)
                    {
                        victimPos = GetAbsoluteLocation((startPointX + endPointX) / 2, (endPointY + startPointY) / 2);
                    }
                    lockv.LockV = true;
                    if (role == 'a')
                    {
                        foreach (PictureBox pb in blackItems)
                        {
                            if (pb.Location.Equals(startPos))
                            {
                                new Thread(new ThreadStart(() =>
                                {
                                    moveFun(pb, startPos, endPos, lockv);
                                    if (endPointX == 7)
                                    {
                                        pb.ImageLocation = System.IO.Path.Combine(Application.StartupPath + @"\..\..\res\blackKing.png");
                                    }
                                    foreach (PictureBox pbv in redItems)
                                    {
                                        if (pbv.Location.Equals(victimPos))
                                        {
                                            pbv.Location = new Point(50, 20);
                                        }
                                    }
                                })).Start();

                                //pb.Location = endPos;
                                //if (endPointX == 7)
                                //{
                                //    pb.ImageLocation = System.IO.Path.Combine(Application.StartupPath + @"\..\..\res\blackKing.png");
                                //}
                                //foreach (PictureBox pbv in redItems)
                                //{
                                //    if (pbv.Location.Equals(victimPos))
                                //    {
                                //        pbv.Location = new Point(50, 20);
                                //    }
                                //}
                            }
                        }
                    }
                    else
                    {
                        foreach (PictureBox pb in redItems)
                        {
                            if (pb.Location.Equals(startPos))
                            {
                                new Thread(new ThreadStart(() =>
                                {
                                    moveFun(pb, startPos, endPos, lockv);
                                    if (endPointX == 0)
                                    {
                                        pb.ImageLocation = System.IO.Path.Combine(Application.StartupPath + @"\..\..\res\redKing.png");
                                    }
                                    foreach (PictureBox pbv in blackItems)
                                    {
                                        if (pbv.Location.Equals(victimPos))
                                        {
                                            pbv.Location = new Point(50, 740);
                                        }
                                    }
                                })).Start();

                                //pb.Location = endPos;
                                //if (endPointX == 0)
                                //{
                                //    pb.ImageLocation = System.IO.Path.Combine(Application.StartupPath + @"\..\..\res\redKing.png");
                                //}
                                //foreach (PictureBox pbv in blackItems)
                                //{
                                //    if (pbv.Location.Equals(victimPos))
                                //    {
                                //        pbv.Location = new Point(50, 740);
                                //    }
                                //}
                            }
                        }
                    }

                    while (lockv.LockV == true)
                    {
                        Thread.Sleep(1);
                    }

                }
            })).Start();
            
        }

        private void playChess(List<string> actionMove, char[,] layout, char role, char result)
        {
            //线程锁,某个线程发现这段代码正被执行就会等待
            lock(locker)
            {
                //画面的UI处理函数
                convertToUI(layout, result);
                movePieceAsyn(actionMove, layout, role, result);
                if (result == 'a' || result == 'b')
                {
                    resultLabel.Text = "winner：" + result;
                }
            }
        }

        private void moveFun(PictureBox piece, Point startPoint, Point endPoint, LockMoving lockMoving)
        {
            int speed = 3;
            while (1 == 1)
            {
                int xVector = (endPoint.X - startPoint.X) > 0 ? 1 : -1;
                int yVector = (endPoint.Y - startPoint.Y) > 0 ? 1 : -1;
                piece.Location = new Point(piece.Location.X + xVector, piece.Location.Y + yVector);
                if (piece.Location.Equals(endPoint))
                {
                    lockMoving.LockV = false;
                    break;
                }
                Thread.Sleep(speed);
            }

        }

        private void chessInit(char[,] chessLayout)
        {
            chess = chessLayout;
            convertToUI(chess, '~');
        }

        private void convertToUI(char[,] layout, char result)
        {
            string UI = server.GetChessLayoutStr();
            this.textBox1.Text = UI + "\r\n 局势:" + result;
        }
        #endregion

        //private delegate void myDelegate(int index, Point b);
        //private void SetLocation(int index, Point b)
        //{
        //    if (this.blackItems[index].InvokeRequired)
        //    {
        //        myDelegate md = new myDelegate(this.SetLocation);
        //        this.Invoke(md, new object[] { index ,b });
        //    }
        //    else
        //        this.blackItems[index].Location = b;
        //}
    }
}
