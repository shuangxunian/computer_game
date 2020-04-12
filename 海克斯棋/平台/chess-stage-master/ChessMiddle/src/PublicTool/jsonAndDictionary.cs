using System;
using System.Collections.Generic;
using System.Web.Script.Serialization;

namespace ChessMiddle.PublicTool
{
    public class jsonAndDictionary
    {
        /// <summary>  
        /// 将json数据反序列化为Dictionary  
        /// </summary>  
        /// <param name="jsonData">json数据</param>  
        /// <returns></returns>  
        public static Dictionary<string, object> JsonToDictionary(string jsonData)
        {
            //实例化JavaScriptSerializer类的新实例  
            JavaScriptSerializer jss = new JavaScriptSerializer();
            try
            {
                //将指定的 JSON 字符串转换为 Dictionary<string, object> 类型的对象  
                return jss.Deserialize<Dictionary<string, object>>(jsonData);
            }
            catch (Exception ex)
            {
                throw new Exception(ex.Message);
            }
        }
        /// <summary>  
        /// 将Dictionary序列化为json数据  
        /// </summary>  
        /// <param name="dic">字典对象</param>  
        /// <returns></returns>  
        public static string DictionaryToJson(Dictionary<string, object> dic)
        {
            //实例化JavaScriptSerializer类的新实例  
            JavaScriptSerializer jss = new JavaScriptSerializer();
            try
            {
                //将指定的 JSON 字符串转换为 Dictionary<string, object> 类型的对象  
                return jss.Serialize(dic);
            }
            catch (Exception ex)
            {
                throw new Exception(ex.Message);
            }
        }
    }
}
