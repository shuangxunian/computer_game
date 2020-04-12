using ChessMiddle.PublicClass;
using ChessMiddle.PublicTool;
using System;
using System.Collections.Generic;
using System.Text;
namespace ChessMiddle.Basics
{
    /// <summary>
    /// 主要是继承FTxBase;还有一些TCP协议需要用到的一些方法；
    /// </summary>
    public class TcpFTxBase : FTxBase
    {
        /// <summary>
        /// 当Tcp收到数据全部在这里处理;也是数据的第一次处理
        /// </summary>
        /// <param name="stateOne">TcpState</param>
        /// <param name="reciverByte">数据</param>
        internal Dictionary<string,object> TcpDateOne(TcpState stateOne, byte[] reciverByte)
        {
            string json =Encoding.Default.GetString(reciverByte);
            Console.WriteLine(json);
            return jsonAndDictionary.JsonToDictionary(json);
        }

        virtual internal void dataClassify(TcpState stateOne, object dic)
        {

        }
    }
}
