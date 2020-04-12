package publicClass;

import java.util.Collection;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.ListIterator;

import com.google.gson.Gson;

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
    /// <param name="why">关闭原因@param
    /// <returns></returns>
    public static byte[] getCloseAPI(String why)
    {
        Hashtable<String, Object> obj = new Hashtable<String, Object>() ;
        obj.put("type","close");
        obj.put("why", why);
        String apiJson = new Gson().toJson(obj);
        System.out.println("call api:  " + apiJson);
        return apiJson.getBytes();
    }
    
    /// <summary>
    /// 违法的走棋接口 （S）
    /// </summary>
    /// <returns>why</returns>
    public static byte[] getIllegalAPI(String why)
    {
        Hashtable<String, Object> obj = new Hashtable<String, Object>();
        obj.put("type", "illegal");
        obj.put("why", why);
        String apiJson = new Gson().toJson(obj);
        System.out.println("call api:  " + apiJson);
        return apiJson.getBytes();
    }

    /// <summary>
    /// 生成走棋超时接口 (S)
    /// </summary>
    /// <returns>why</returns>
    public static byte[] getTimeoutAPI()
    {
        Hashtable<String, Object> obj = new Hashtable<String, Object>();
        obj.put("type", "timeout");
        String apiJson = new Gson().toJson(obj);
        System.out.println("call api:  " + apiJson);
        return apiJson.getBytes();
    }

    /// <summary>
    /// 走棋接口 (C)
    /// </summary>
    /// <param name="changes">改变序列(字符串表示),"5,3-6,4"...@param
    /// <param name="role">身份证(连接后会分配一个省份证)@param
    /// <returns></returns>
    public static byte[] getActionAPI(List<String> changes, String role)
    {
        Hashtable<String, Object> obj = new Hashtable<String, Object>();
        obj.put("type", "action");
        obj.put("changes", changes);
        obj.put("role", role);
        String apiJson = new Gson().toJson(obj);
        System.out.println("call api:  " + apiJson);
        return apiJson.getBytes();
    }
    
  /// <summary>
    /// 走棋接口 (C)
    /// </summary>
    /// <param name="changes">结果棋局@param
    /// <param name="role">身份证(连接后会分配一个省份证)@param
    /// <returns></returns>
    public static byte[] getActionAPI(char[][] now, String role)
    {
        Hashtable<String, Object> obj = new Hashtable<String, Object>();
        obj.put("type", "now");
        obj.put("now", now);
        obj.put("role", role);
        String apiJson = new Gson().toJson(obj);
        System.out.println("call api:  " + apiJson);
        return apiJson.getBytes();
    }

    /// <summary>
    /// 生成发给下一个选手的走棋接口 (S)
    /// </summary>
    /// <param name="now">当前棋局@param
    /// <param name="size">棋盘规格(用于表示棋盘大小,还原棋局)@param
    /// <param name="role">下一个选手的身份@param
    /// <param name="limitTimeSecond">运算等待时间@param
    /// <returns></returns>
    public static byte[] getNextEpisodeAPI(char[][] now, String size, String role, double limitTimeSecond)
    {
        Hashtable<String, Object> obj = new Hashtable<String, Object>();
        obj.put("type", "do_algorithm");
        obj.put("now", now);
        obj.put("size", size);
        obj.put("role", role);
        obj.put("limitTimeSecond", limitTimeSecond);
        String apiJson = new Gson().toJson(obj);
        System.out.println("call call api:  " + apiJson);
        return apiJson.getBytes();
    }

    /// <summary>
    /// 生成结果接口 (S)
    /// </summary>
    /// <param name="situation">需要的结论(win|fail|draw)@param
    /// <returns></returns>
    public static byte[] getResultAPI(String situation)
    {
        Hashtable<String, Object> obj = new Hashtable<String, Object>();
        obj.put("type", "result");
        obj.put("result", situation);
        String apiJson = new Gson().toJson(obj);
        System.out.println("call api:  " + apiJson);
        return apiJson.getBytes();
    }


    public static void main(String[] args)
    {
        char[][] a ={
            {'g','s','d','s','a' },
            {'g','s','d','s','a' },
            {'g','s','d','s','a' }
        };
        getNextEpisodeAPI(a, "8,8", "a", 5f);
        
        List<String> actionList = new LinkedList<String>();
        actionList.add("5,6-7,8");
        actionList.add("7,8-8,9");
        getActionAPI(actionList, "2");
    }
}

