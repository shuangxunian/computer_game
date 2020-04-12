using ChessMiddle.Basics;
using System;
namespace ChessMiddle.PublicTool
{
    /// <summary>
    /// 对接收到的数据进行第一次处理；找出需要的数据；把空的去掉
    /// </summary>
    public class ReceiveDateOne
    {
       /// <summary>
        /// 把缓冲区的数据拿出来；并且把缓冲区清空；
       /// </summary>
        /// <param name="stateOne">StateBase</param>
       /// <param name="insert">数据实际长度</param>
       /// <returns>需要的数据</returns>
       internal static byte[] DateOneManage(StateBase stateOne,int insert)
       {
           byte[] receiveByte = null;
           if (stateOne.Buffer[0] == 0 && stateOne.BufferBackup != null && stateOne.BufferBackup.Length >= insert)
           { receiveByte = stateOne.BufferBackup; stateOne.BufferBackup = null;}//主要用于缓冲区有扩大缩小
           else { receiveByte = stateOne.Buffer;}
           byte[] haveDate = ByteToByte(receiveByte, insert,0);
           Array.Clear(stateOne.Buffer, 0, stateOne.Buffer.Length);
           return haveDate;
       }

        /// <summary>
        /// 把一个数组取出指定长度
        /// </summary>
        /// <param name="a">数据</param>
        /// <param name="b">长度</param>
        /// <param name="index">起始位置</param>
        /// <returns>返回的数据</returns>
        internal static byte[] ByteToByte(byte[] a, int b, int index)
        {
            byte[] haveDate = new byte[b];
            Array.Copy(a, index, haveDate, 0, b);
            return haveDate;
        }
    }
}
