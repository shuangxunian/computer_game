using ChessMiddle.PublicTool;
using System;
using System.Collections.Generic;
using System.Text;

namespace ChessMiddle.PublicClass
{
    /// <summary>
    /// 生成字节形式通用传输对象
    /// </summary>
    public class API
    {

        //S:服务端发，客户端收
        //C:客户端发，服务端收
        //S/C：双向的


        /// <summary>
        /// 关闭对象(C/S)
        /// </summary>
        /// <param name="why">关闭原因</param>
        /// <returns></returns>
        public static byte[] getCloseAPI(string why)
        {
            Dictionary<string, object> obj = new Dictionary<string, object>();
            obj.Add("type", "close");
            obj.Add("why", why);
            Console.WriteLine("api:  "+jsonAndDictionary.DictionaryToJson(obj));
            return Encoding.Default.GetBytes(jsonAndDictionary.DictionaryToJson(obj));
        }

        /// <summary>
        /// 违法的走棋接口 （S）
        /// </summary>
        /// <returns>why</returns>
        public static byte[] getIllegalAPI(string why)
        {
            Dictionary<string, object> obj = new Dictionary<string, object>();
            obj.Add("type", "illegal");
            obj.Add("why", why);
            Console.WriteLine("api:  " + jsonAndDictionary.DictionaryToJson(obj));
            return Encoding.Default.GetBytes(jsonAndDictionary.DictionaryToJson(obj));
        }

        /// <summary>
        /// 生成走棋超时接口 (S)
        /// </summary>
        /// <returns>why</returns>
        public static byte[] getTimeoutAPI()
        {
            Dictionary<string, object> obj = new Dictionary<string, object>();
            obj.Add("type", "timeout");
            Console.WriteLine("api:  " + jsonAndDictionary.DictionaryToJson(obj));
            return Encoding.Default.GetBytes(jsonAndDictionary.DictionaryToJson(obj));
        }

        /// <summary>
        /// 走棋接口 (C)
        /// </summary>
        /// <param name="changes">改变序列(字符串表示),"5,3x6,4"...</param>
        /// <param name="role">身份证(连接后会分配一个省份证)</param>
        /// <returns></returns>
        public static byte[] getActionAPI(List<string> changes, string role)
        {
            Dictionary<string, object> obj = new Dictionary<string, object>();
            obj.Add("type", "action");
            obj.Add("changes", changes);
            obj.Add("role", role);
            Console.WriteLine("api:  " + jsonAndDictionary.DictionaryToJson(obj));
            return Encoding.Default.GetBytes(jsonAndDictionary.DictionaryToJson(obj));
        }

        /// <summary>
        /// 生成发给下一个选手的走棋接口 (S)
        /// </summary>
        /// <param name="now">当前棋局</param>
        /// <param name="size">棋盘规格(用于表示棋盘大小,还原棋局)</param>
        /// <param name="role">下一个选手的身份</param>
        /// <param name="limitTimeSecond">运算等待时间</param>
        /// <returns></returns>
        public static byte[] getNextEpisodeAPI(char[,] now, string size, string role, double limitTimeSecond)
        {
            Dictionary<string, object> obj = new Dictionary<string, object>();
            obj.Add("type", "do_algorithm");
            obj.Add("now", now);
            obj.Add("size", size);
            obj.Add("role", role);
            obj.Add("limitTimeSecond", limitTimeSecond);
            Console.WriteLine("api:  " + jsonAndDictionary.DictionaryToJson(obj));
            return Encoding.Default.GetBytes(jsonAndDictionary.DictionaryToJson(obj));
        }

        /// <summary>
        /// 生成结果接口 (S)
        /// </summary>
        /// <param name="situation">需要的结论(win|fail|draw)</param>
        /// <returns></returns>
        public static byte[] getResultAPI(string situation)
        {
            Dictionary<string, object> obj = new Dictionary<string, object>();
            obj.Add("type", "result");
            obj.Add("result", situation);
            Console.WriteLine("api:  " + jsonAndDictionary.DictionaryToJson(obj));
            return Encoding.Default.GetBytes(jsonAndDictionary.DictionaryToJson(obj));
        }


        public static void UnitTest()
        {
            char[,] a = new char[3, 5] {
                {'g','s','d','s','a' },
                {'g','s','d','s','a' },
                {'g','s','d','s','a' }
            };
            getNextEpisodeAPI(a, "s+5", "a", 5f);
        }
    }
}
