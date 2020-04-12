namespace ChessMiddle
{
    /// <summary>
    /// 一个最基础的类,服务器和客户端共同要用到的一些方法和事件
    /// </summary>
    public class TxStart
    {
       /// <summary>
       /// 注册服务器,返回一个ITxServer类,再从ITxServer的startServer一个方法启动服务器
       /// </summary>
       /// <param name="port">端口</param>
       /// <returns>ITxServer</returns>
       public static ITxServer startServer(int port)
       {
           ITxServer server = new SocketServer(port);
           return (server);
       }
       /// <summary>
       /// 注册客户端,返回一个ITxServer类,再从ITxClient的startClient一个方法启动客户端;
       /// </summary>
       /// <param name="ip">ip地址</param>
       /// <param name="port">端口</param>
       /// <returns>ITxClient</returns>
       public static ITxClient startClient(string ip, int port)
       {
           ITxClient client = new SocketClient(ip, port);
           return (client);
       }
    }
}
