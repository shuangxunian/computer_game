package util;

public class DeepClone {
    public static void deepClone(char[][] a, char[][] b)
    {
        int LENGTH = a.length;
        for (int i = 0; i < LENGTH; i++)
        {
            for (int j = 0; j < LENGTH; j++)
            {
                a[i][j] = b[i][j];
            }
        }
    }
}
