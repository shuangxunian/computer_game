//代码版本		2017年国赛 
//本代码由沈阳航空航天大学计算机博弈协会总负责人王跃霖统一调控
//五子棋组负责人孙公伯所带博弈组开发维护
//代码来自哈工大开源代码,由以上成员维护 
//本代码适用于中国大学生计算机博弈大赛博弈项目2017版交互协议
//本代码仅提供交互协议的用法示范和简单AI博弈思路，开发者需自行改进完善代码参赛
//如有意见和建议请与我们尽早联系
//王跃霖	QQ：836473734
//孙公伯	QQ：1141685377
#include "stdio.h"
#include "math.h"
#include "windows.h"
#include "time.h"//使用当前时钟做种子
#include "conio.h"
 
#define N 15       //定义棋盘大小  1黑2白
#define M N+8      //定义数据棋盘大小
 
int  chess[N][N];//棋盘，用于储存下子位置
int  count[M][M];//数据棋盘，用于储存下子位置，和盘外数据
char buffer[N*2-1][N*4-3];//缓冲器，用于储存下子位置，盘外数据，和棋盘符号
long sum[M][M] = {{0}};//得分棋盘，储存得分情况
int  p[20][5];//存储该位置的所有可能的五连珠情况
 
int  player = 1, ai = 2, error = -1;
int  num;//双方回合总数
int  now;//设置下棋标志
int  flag0;//确定己方已下子
int  gs;//游戏结束标志
 
//基础函数
void RunGame();//进行整个对局
int  Menu();//设置开始菜单
void Initialize();//初始化棋盘，数据棋盘，和缓冲器
void Print(int x, int y);//将数据写入数据棋盘，和缓冲器
void Display();//将缓冲器数据输出到屏幕
void Buf_Display();//利用双缓冲将缓冲器数据输出到屏幕
int  Judge_Display();//输出胜负情况
//双方对局函数
void PGame();//玩家对局函数
void CGame();//电脑对局函数
//输入函数
void Input(int *x, int *y);//键盘方向键控制，空格键输入
void Mark(int x0, int y0, int x, int y);//标记下子位置
//电脑判断函数
int  Basic_condition(int x, int y);//判断下子位置是否出界
void Find_point(int *pp, int *qq);//找到最佳性最优的下子位置
int  JUDGE(int x, int y);//判断胜负
void Grade();//计算所有位置的得分
void Base(int i, int j);//坐标为(i-4,j-4)的所有五连珠情况
long Assessment(int num1, int num2);//评分标准
 
//主程序
int main()
{
    system("title 简易五子棋");//设置标题
    //system("mode con cols=58 lines=29");//设置窗口大小
    system("color E0");//设置颜色
 
    Initialize();//初始化棋盘，数据棋盘，和缓冲器
    RunGame();//进行整个对局*/
 
    return 0;
}
 
/***********************************************************************************************************************************************************/
void RunGame()//进行整个对局
{
    switch(Menu())
    {
        case 1:
            while(1)
            {
                PGame();//玩家对局函数
                if(Judge_Display())
                {
                    break;//对局结束
                }
                CGame();//电脑对局函数
                if(Judge_Display())
                {
                    break;//对局结束
                }
            }//while
            break;
        case 2:
            now=ai;
            count[N/2+4][N/2+4] = now; chess[N/2][N/2] = now;
            Print(N/2, N/2);//将数据写入数据棋盘，和缓冲器
            num++;
            while(1)
            {
                PGame();//玩家对局函数
                if(Judge_Display())//玩家对局函数
                {
                    break;//对局结束
                }
                CGame();//电脑对局函数
                if(Judge_Display())
                {
                    break;//对局结束
                }
            }//while
            break;
        case 3:
            now=1;
            count[N/2+4][N/2+4] = now; chess[N/2][N/2] = now;
            Print(N/2, N/2);//将数据写入数据棋盘，和缓冲器
            num++;
            while(1)
            {
                CGame();//电脑对局函数
                if(Judge_Display())
                {
                    break;//对局结束
                }
            }
            break;
        default:;
    }
}
/***********************************************************************************************************************************************************/
 
int Menu()//设置开始菜单
{
    int x;
	printf("方向键移动，空格键确定\n");
    printf("1、玩家先手\n");
    printf("2、玩家后手\n");
    printf("3、电脑自对弈\n");

    scanf("%d", &x);
    return x;
}
 
void Initialize()//初始化棋盘，数据棋盘，和缓冲器
{
    int i, j;
 
    //初始化数据棋盘
    for(i = 0; i < M; i++)
    {
        for(j = 0; j < M; j++)
        {
            if((i < 4 || i >= M - 4)&&(j < 4 || j>= M - 4))
            {
                count[i][j] = error;
            }
        }
    }
    //初始化缓冲器
    for(i = 0; i < N*2-1; i++)
    {
        for(j = 0; j < N*4-3; j++)
        {
            if(i == 0 && j == 0)//以下为边缘棋盘样式
                     buffer[i][j]='a';//"┏";
            else if(i == 0 && j == (N*4-3)-1)
                     buffer[i][j] = 'b';//"┓";
            else if(i == (N*2-1)-1 && j == (N*4-3)-1)
                     buffer[i][j] = 'c';//"┛";
            else if(i == (N*2-1)-1&&j == 0)
                     buffer[i][j]='d';//"┗";
            else if(i == 0 && j % 4 == 0)
                     buffer[i][j]='e';//"┯";
            else if(i == (N*2-1)-1 && j % 4 == 0)
                     buffer[i][j] = 'f';//"┷";
            else if(i % 2 == 0 && j == 0)
                     buffer[i][j] = 'g';//"┠";
            else if(i % 2 == 0 && j == (N*4-3)-1)
                     buffer[i][j] = 'h';//"┨";
            else if((i == 0 || i == (N*2-1)-1) && (j/2) % 2 != 0)
                     buffer[i][j] = 'i';//"━";
            else if(i != 0 && i != (N*2-1)-1 && i % 2 == 0 && (j/2) % 2 != 0)
                     buffer[i][j] = 'j';//"—";
            else if(i % 2 != 0 && (j == 0 || j == (N*4-3)-1))
                     buffer[i][j] = 'k';//"┃";
            else if(i % 2 != 0 && j != 0 && j != (N*4-3)-1 && j % 4 == 0)
                     buffer[i][j] = 'l';//"│";
            else if(i != 0 && j != 0 && i != (N*2-1)-1 && j != (N*4-3)-1 && i % 2 == 0 && j % 4 == 0)
                     buffer[i][j] = 'm';//中间的空位"┼"
        }
    }
}
 
void Print(int x, int y)//将数据写入数据棋盘，和缓冲器
{
    count[x+4][y+4] = chess[x][y];
    buffer[x*2][y*4] = chess[x][y] + 48;//将整型数据转化为字符型数据
}
 
void Display()//将缓冲器数据输出到屏幕
{
    int i, j;
    system("cls");//清理屏幕，准备写入
    for(i = 0; i < N*2-1; i++)
    {
        for(j = 0; j < N*4-3; j++)
        {
//            printf("%c", buffer[i][j]);
            switch(buffer[i][j])
            {
                case '1' : printf("●"); j++; break;//j++不可省
                case '2' : printf("○"); j++; break;//j++不可省
                case 'a' : printf("┏"); break;
                case 'b' : printf("┓"); break;
                case 'c' : printf("┛"); break;
                case 'd' : printf("┗"); break;
                case 'e' : printf("┯"); break;
                case 'f' : printf("┷"); break;
                case 'g' : printf("┠"); break;
                case 'h' : printf("┨"); break;
                case 'i' : printf("━"); break;//上下的横线更粗
                case 'j' : printf("—"); j++; break;//中间的横线更细//j++;由于输出"—"时占两个字符，j++可以避开输出下一个的空格
                case 'k' : printf("┃"); break;//左右的竖线更粗
                case 'l' : printf("│"); break;//中间的竖线更粗
                case 'm' : printf("┼"); break;
                case ' ' : printf(" "); break;
                default  : printf("%c", buffer[i][j]);
 
            }//switch
        }//for
        printf("\n");
    }//for
	Sleep(100);
/*
    //调试
    printf("%d\n", num);
    for(i = 4; i < M-4; i++)
    {
        for( j = 4; j < M-4; j++)
        printf("%8d", sum[i][j]);
        printf("\n");
    }
*/
}
 
void PGame()//玩家对局函数
{
    static int Px0 = N/2, Py0 = N/2, Px = N/2, Py = N/2;
    now = 1;
    do{
        Mark(Px0, Py0, Px, Py);//标记下子位置
        Buf_Display();//将缓冲器数据输出到屏幕
        Px0 = Px; Py0 = Py;
        Input(&Px, &Py);
    }while(!(chess[Px][Py] && flag0 == 1));
    Print(Px, Py);//将数据写入数据棋盘，和缓冲器
    num++;
    gs = JUDGE(Px + 4,Py + 4);
}
void CGame()//电脑对局函数
{
    static int Cp = N/2, Cq = N/2, Cp0, Cq0;
    Buf_Display();//将缓冲器数据输出到屏幕
    Grade();//计算所有位置的得分
    Cp0 = Cp; Cq0 = Cq;//记录上一次电脑下子位置
    Find_point(&Cp, &Cq);//找到最佳性最优的下子位置
    now = 3 - now;
    chess[Cp - 4][Cq - 4] = now;
    Mark(Cp0 - 4, Cq0 - 4, Cp - 4, Cq - 4);//标记下子位置
    Print(Cp - 4, Cq - 4);//将数据写入数据棋盘，和缓冲器
    num++;
    gs = JUDGE(Cp, Cq);
}
void Input(int *x, int *y)//键盘方向键控制，空格键输入
{
    int input;
    flag0 = 0;
    input = getch();//获得第一次输入信息
    if(input == 0x20 && !chess[*x][*y])//判断移动光标后，玩家是否下子
    {
        chess[*x][*y] = player;
        flag0 = 1;
    }
    else if(input == 0xE0)//如果按下的是方向键，会填充两次输入，第一次为0xE0表示按下的是控制键
         {
             input = getch();//获得第二次输入信息
             switch(input)//判断方向键方向并移动光标位置
             {
                 case 0x48 : (*x)--; break;
                 case 0x4B : (*y)--; break;
                 case 0x50 : (*x)++; break;
                 case 0x4D : (*y)++; break;
             }//switch
             if(*x<0)   (*x) = N - 1;//如果光标位置越界则移动到对侧
             if(*y<0)   (*y) = N - 1;
             if(*x>N-1) (*x) = 0;
             if(*y>N-1) (*y) = 0;
        }
}
 
void Mark(int x0,int y0,int x,int y)//标记己方下子位置
{
    x0 = x0*2; y0 = y0*4;
    buffer[x0 - 1][y0 - 2] = ' ';
    buffer[x0 - 1][y0 + 2] = ' ';
    buffer[x0 + 1][y0 + 2] = ' ';
    buffer[x0 + 1][y0 - 2] = ' ';
 
    x = x*2; y = y*4;
    buffer[x - 1][y - 2] = 'a';
    buffer[x - 1][y + 2] = 'b';
    buffer[x + 1][y + 2] = 'c';
    buffer[x + 1][y - 2] = 'd';
 
    if(y == (N*4-3)-1)//解决“当光标移动到最右侧时，光标发生错位”
    {
        buffer[x - 1][y + 2] = ' ';
        buffer[x + 1][y + 2] = ' ';
    }
}
 
int Basic_condition(int x,int y)//判断下子位置是否出界
{
    if(x >= 4 && x < M - 4 && y >= 4 && y < M - 4)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
 
void Find_point(int *pp, int *qq)//找到最佳性最优的下子位置
{
    int i, j, flag = 0;//flag为结束标志
    long t = 0;
    int r, c=0, sumed[N*N][2];
    //寻找第一个未下子位置对应的sum值
    for(i = 4; i < M - 4 && flag == 0; i++)
    {
        for(j = 4; j < M - 4 && flag == 0; j++)
        {
            if(!chess[i - 4][j - 4])
            {
                t = sum[i][j];
                flag = 1;
            }
        }
    }//for
    //寻找最大的sum值
    for(i = 4; i < M - 4; i++)
    {
        for(j = 4; j < M - 4 ; j++)
        {
            if(!chess[i - 4][j - 4] && sum[i][j] > t)
            {
                t = sum[i][j];
            }
        }
    }//for
    //等于怎么办
    //存储所有的最大sum值
    for(i = 4; i < M - 4; i++)
    {
        for(j = 4; j < M - 4 ; j++)
        {
            if(!chess[i - 4][j - 4] && sum[i][j] == t)
            {
                sumed[c][0] = i;
                sumed[c][1] = j;
                c++;
            }
        }
    }//for
    srand((unsigned)time(NULL)); //初始化随机数
    //随机选用最大sum值中的一组数据
    r = rand()%c;
    *pp = sumed[r][0];
    *qq = sumed[r][1];
}
 
int JUDGE(int x, int y)//判断胜负，返回1赢，返回0无
{
    int a = 1, b = 1, c = 1, d = 1, i;//累计横竖正斜反斜四个方向的连续相同棋子数目
 
    for(i=1;i<5;i++)if(y+i<M-4&&count[x][y+i]==now)a++;else break;//向下检查
    for(i=1;i<5;i++)if(y-i>=4&&count[x][y-i]==now)a++;else break;//向上检查
    if(a>=5)return now;//若达到5个则判断当前走子玩家为赢家
 
    for(i=1;i<5;i++)if(x+i<M-4&&count[x+i][y]==now)b++;else break;//向右检查
    for(i=1;i<5;i++)if(x-i>=4&&count[x-i][y]==now)b++;else break;//向左检查
    if(b>=5)return now;//若达到5个则判断当前走子玩家为赢家
 
    for(i=1;i<5;i++)if(x+i<M-4&&y+i<M-4&&count[x+i][y+i]==now)c++;else break;//向右下检查
    for(i=1;i<5;i++)if(x-i>=4&&y-i>=4&&count[x-i][y-i]==now)c++;else break;//向左上检查
    if(c>=5)return now;//若达到5个则判断当前走子玩家为赢家
 
    for(i=1;i<5;i++)if(x+i<M-4&&y-i>=4&&count[x+i][y-i]==now)d++;else break;//向右上检查
    for(i=1;i<5;i++)if(x-i>=4&&y+i<M-4&&count[x-i][y+i]==now)d++;else break;//向左下检查
    if(d>=5)return now;//若达到5个则判断当前走子玩家为赢家
 
    return 0;//若没有检查到五连珠，则返回0表示还没有玩家达成胜利
}
 
void Grade()//计算所有位置的得分
{
    int i, j, m, n;
    int num1, num2;
    for(i = 4; i < M - 4; i++)
    {
        for(j = 4; j < M - 4; j++)
        {
            if(!chess[i - 4][j - 4])
            {
                sum[i][j] = 0;//循环初始化sum[i][j]
                Base(i, j);//坐标为(i-4,j-4)的所有五连珠情况
                for(m = 0; m < 20; m++)
                {
                    num1 = 0; num2 = 0;
                    for(n = 0; n < 5; n++)
                    {
                        if(p[m][n] == now)
                            num1++;
                        else if(p[m][n] == 3 - now)
                                 num2++;
                        else if(p[m][n] == error)
                        {
                            num1 = error; num2 = error;
                            break;
                        }
                    }
                    sum[i][j] += Assessment(num1, num2);
                }
            }
        }
    }
}
 
void Base(int i,int j)//坐标为(i-4,j-4)的所有五连珠情况
{
    //左上->右下
    p[0][0]=count[i-4][j-4];p[0][1]=count[i-3][j-3];p[0][2]=count[i-2][j-2];p[0][3]=count[i-1][j-1];p[0][4]=count[i][j];
    p[1][0]=count[i-3][j-3];p[1][1]=count[i-2][j-2];p[1][2]=count[i-1][j-1];p[1][3]=count[i][j];p[1][4]=count[i+1][j+1];
    p[2][0]=count[i-2][j-2];p[2][1]=count[i-1][j-1];p[2][2]=count[i][j];p[2][3]=count[i+1][j+1];p[2][4]=count[i+2][j+2];
    p[3][0]=count[i-1][j-1];p[3][1]=count[i][j];p[3][2]=count[i+1][j+1];p[3][3]=count[i+2][j+2];p[3][4]=count[i+3][j+3];
    p[4][0]=count[i][j];p[4][1]=count[i+1][j+1];p[4][2]=count[i+2][j+2];p[4][3]=count[i+3][j+3];p[4][4]=count[i+4][j+4];
    //上->下
    p[5][0]=count[i-4][j];p[5][1]=count[i-3][j];p[5][2]=count[i-2][j];p[5][3]=count[i-1][j];p[5][4]=count[i][j];
    p[6][0]=count[i-3][j];p[6][1]=count[i-2][j];p[6][2]=count[i-1][j];p[6][3]=count[i][j];p[6][4]=count[i+1][j];
    p[7][0]=count[i-2][j];p[7][1]=count[i-1][j];p[7][2]=count[i][j];p[7][3]=count[i+1][j];p[7][4]=count[i+2][j];
    p[8][0]=count[i-1][j];p[8][1]=count[i][j];p[8][2]=count[i+1][j];p[8][3]=count[i+2][j];p[8][4]=count[i+3][j];
    p[9][0]=count[i][j];p[9][1]=count[i+1][j];p[9][2]=count[i+2][j];p[9][3]=count[i+3][j];p[9][4]=count[i+4][j];
    //右上->左下
    p[10][0]=count[i-4][j+4];p[10][1]=count[i-3][j+3];p[10][2]=count[i-2][j+2];p[10][3]=count[i-1][j+1];p[10][4]=count[i][j];
    p[11][0]=count[i-3][j+3];p[11][1]=count[i-2][j+2];p[11][2]=count[i-1][j+1];p[11][3]=count[i][j];p[11][4]=count[i+1][j-1];
    p[12][0]=count[i-2][j+2];p[12][1]=count[i-1][j+1];p[12][2]=count[i][j];p[12][3]=count[i+1][j-1];p[112][4]=count[i+2][j-2];
    p[13][0]=count[i-1][j+1];p[13][1]=count[i][j];p[13][2]=count[i+1][j-1];p[13][3]=count[i+2][j-2];p[13][4]=count[i+3][j-3];
    p[14][0]=count[i][j];p[14][1]=count[i+1][j-1];p[14][2]=count[i+2][j-2];p[14][3]=count[i+3][j-3];p[14][4]=count[i+4][j-4];
    //左->右
    p[15][0]=count[i][j-4];p[15][1]=count[i][j-3];p[15][2]=count[i][j-2];p[15][3]=count[i][j-1];p[15][4]=count[i][j];
    p[16][0]=count[i][j-3];p[16][1]=count[i][j-2];p[16][2]=count[i][j-1];p[16][3]=count[i][j];p[16][4]=count[i][j+1];
    p[17][0]=count[i][j-2];p[17][1]=count[i][j-1];p[17][2]=count[i][j];p[17][3]=count[i][j+1];p[17][4]=count[i][j+2];
    p[18][0]=count[i][j-1];p[18][1]=count[i][j];p[18][2]=count[i][j+1];p[18][3]=count[i][j+2];p[18][4]=count[i][j+3];
    p[19][0]=count[i][j];p[19][1]=count[i][j+1];p[19][2]=count[i][j+2];p[19][3]=count[i][j+3];p[19][4]=count[i][j+4];
}
 
long Assessment(int num1,int num2)//评分标准
{
    if(num2 == 0)//判断电脑五元组得分
    {
        switch(num1)
        {
            case 0 : return 7;
            case 1 : return 35;
            case 2 : return 800;
            case 3 : return 15000;
            case 4 : return 800000;
        }
    }
    if(num2 != 0 && num1 == 0)//判断玩家五元组得分
    {
        switch(num2)
        {
            case 1 : return 15;
            case 2 : return 400;
            case 3 : return 1800;
            case 4 : return 100000;
        }
    }
    return 0;
}
 
void Buf_Display()//利用双缓冲将缓冲器数据输出到屏幕
{
    int i, j;
    //创建屏幕缓冲区
    HANDLE hNewConsole = CreateConsoleScreenBuffer(GENERIC_WRITE | GENERIC_READ,
    0,
    NULL,
    CONSOLE_TEXTMODE_BUFFER,
    NULL);
 
    //隐藏光标
    CONSOLE_CURSOR_INFO cci = {1, 0};
    SetConsoleCursorInfo(hNewConsole, &cci);
 
    //设置窗口的缓冲区大小
    COORD cdBufferSize = {58, 29};
    SetConsoleScreenBufferSize(hNewConsole, cdBufferSize);
    //然后我们输出这个数组看看
    for(i = 0; i < N*2-1; i++)
    {
        for(j = 0; j < N*4-3; j++)
        {
            COORD cdCursorPos = {j, i};
            char *p = NULL;
//          printf("%c", buffer[i][j]);
            switch(buffer[i][j])
            {
                case '1' : p = "●"; j++; break;//j++可省
                case '2' : p = "○"; j++; break;//j++可省
                case 'a' : p = "┏"; break;
                case 'b' : p = "┓"; break;
                case 'c' : p = "┛"; break;
                case 'd' : p = "┗"; break;
                case 'e' : p = "┯"; break;
                case 'f' : p = "┷"; break;
                case 'g' : p = "┠"; break;
                case 'h' : p = "┨"; break;
                case 'i' : p = "━"; break;//上下的横线更粗
                case 'j' : p = "—"; j++; break;//中间的横线更细//j++;由于输出"—"时占两个字符，j++可以避开输出下一个的空格
                case 'k' : p = "┃"; break;//左右的竖线更粗
                case 'l' : p = "│"; break;//中间的竖线更粗
                case 'm' : p = "┼"; break;
                case ' ' : p = " "; break;
                default  : p = &buffer[i][j];
            }//switch,for
            if( ! p) continue;
            SetConsoleCursorPosition(hNewConsole, cdCursorPos);
            WriteConsole(hNewConsole, p, strlen(p), NULL, NULL);
        }
//      printf("\n");
    }//for
    //把绘制好的东西都显示出来
    SetConsoleActiveScreenBuffer(hNewConsole);
    //如果游戏结束，返回正常模式
/*    if(gs)
    {
        SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
    }*/
}
 
int  Judge_Display()//输出胜负情况
{
    if(num == N*N)
    {
        Buf_Display();//将缓冲器数据输出到屏幕
        printf("平局！\n");
        return 1;//对局结束
    }
    if(gs == 1)
    {
        Buf_Display();//将缓冲器数据输出到屏幕
        printf("黑棋赢！\n");
        return 1;//对局结束
    }
    if(gs == 2)
    {
        Buf_Display();//将缓冲器数据输出到屏幕
        printf("白棋赢！\n");
        return 1;//对局结束
    }
 
    return 0;

}
