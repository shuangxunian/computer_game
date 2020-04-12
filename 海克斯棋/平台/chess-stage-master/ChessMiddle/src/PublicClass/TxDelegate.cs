namespace ChessMiddle
{
    /// <summary>
    /// 不带参数的委托
    /// </summary>
    public delegate void TxDelegate();
    /// <summary>
    /// 带一个参数的委托
    /// </summary>
    /// <typeparam name="T1">T1</typeparam>
    /// <param name="object1"></param>
    public delegate void TxDelegate<T1>(T1 object1);
    /// <summary>
    /// 带二个参数的委托
    /// </summary>
    /// <typeparam name="T1">T1</typeparam>
    /// <typeparam name="T2">T2</typeparam>
    /// <param name="object1">object1</param>
    /// <param name="object2">object2</param>
    public delegate void TxDelegate<T1, T2>(T1 object1, T2 object2);
    /// <summary>
    /// 带四个参数的委托
    /// </summary>
    /// <typeparam name="T1">T1</typeparam>
    /// <typeparam name="T2">T2</typeparam>
    /// <typeparam name="T3">T3</typeparam>
    /// <typeparam name="T4">T4</typeparam>
    /// <param name="object1">object1</param>
    /// <param name="object2">object2</param>
    /// <param name="object3">object3</param>
    /// <param name="object4">object4</param>
    public delegate void TxDelegate<T1, T2, T3, T4>(T1 object1, T2 object2, T3 object3, T4 object4);
}
