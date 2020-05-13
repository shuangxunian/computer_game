//代码版本		2017年国赛 
//本代码由沈阳航空航天大学计算机博弈协会总负责人王跃霖统一调控
//五子棋组负责人孙公伯所带博弈组开发维护
//本代码适用于中国大学生计算机博弈大赛博弈项目2017版交互协议
//本代码仅提供交互协议的用法示范和简单AI博弈思路，开发者需自行改进完善代码参赛
//如有意见和建议请与我们尽早联系
//王跃霖	QQ：836473734
//孙公伯	QQ：1141685377
#include "stdio.h"
#include "math.h"
#include "windows.h"
#include "time.h"//ʹ�õ�ǰʱ��������
#include "conio.h"
 
#define N 15       //�������̴�С  1��2��
#define M N+8      //�����������̴�С
 
int  chess[N][N];//���̣����ڴ�������λ��
int  count[M][M];//�������̣����ڴ�������λ�ã�����������
char buffer[N*2-1][N*4-3];//�����������ڴ�������λ�ã��������ݣ������̷���
long sum[M][M] = {{0}};//�÷����̣�����÷����
int  p[20][5];//�洢��λ�õ����п��ܵ����������
 
int  player = 1, ai = 2, error = -1;
int  num;//˫���غ�����
int  now;//���������־
int  flag0;//ȷ������������
int  gs;//��Ϸ������־
 
//��������
void RunGame();//���������Ծ�
int  Menu();//���ÿ�ʼ�˵�
void Initialize();//��ʼ�����̣��������̣��ͻ�����
void Print(int x, int y);//������д���������̣��ͻ�����
void Display();//�������������������Ļ
void Buf_Display();//����˫���彫�����������������Ļ
int  Judge_Display();//���ʤ�����
//˫���Ծֺ���
void PGame();//��ҶԾֺ���
void CGame();//���ԶԾֺ���
//���뺯��
void Input(int *x, int *y);//���̷�������ƣ��ո������
void Mark(int x0, int y0, int x, int y);//�������λ��
//�����жϺ���
int  Basic_condition(int x, int y);//�ж�����λ���Ƿ����
void Find_point(int *pp, int *qq);//�ҵ���������ŵ�����λ��
int  JUDGE(int x, int y);//�ж�ʤ��
void Grade();//��������λ�õĵ÷�
void Base(int i, int j);//����Ϊ(i-4,j-4)���������������
long Assessment(int num1, int num2);//���ֱ�׼
 
//������
int main()
{
    system("title ����������");//���ñ���
    //system("mode con cols=58 lines=29");//���ô��ڴ�С
    system("color E0");//������ɫ
 
    Initialize();//��ʼ�����̣��������̣��ͻ�����
    RunGame();//���������Ծ�*/
 
    return 0;
}
 
/***********************************************************************************************************************************************************/
void RunGame()//���������Ծ�
{
    switch(Menu())
    {
        case 1:
            while(1)
            {
                PGame();//��ҶԾֺ���
                if(Judge_Display())
                {
                    break;//�Ծֽ���
                }
                CGame();//���ԶԾֺ���
                if(Judge_Display())
                {
                    break;//�Ծֽ���
                }
            }//while
            break;
        case 2:
            now=ai;
            count[N/2+4][N/2+4] = now; chess[N/2][N/2] = now;
            Print(N/2, N/2);//������д���������̣��ͻ�����
            num++;
            while(1)
            {
                PGame();//��ҶԾֺ���
                if(Judge_Display())//��ҶԾֺ���
                {
                    break;//�Ծֽ���
                }
                CGame();//���ԶԾֺ���
                if(Judge_Display())
                {
                    break;//�Ծֽ���
                }
            }//while
            break;
        case 3:
            now=1;
            count[N/2+4][N/2+4] = now; chess[N/2][N/2] = now;
            Print(N/2, N/2);//������д���������̣��ͻ�����
            num++;
            while(1)
            {
                CGame();//���ԶԾֺ���
                if(Judge_Display())
                {
                    break;//�Ծֽ���
                }
            }
            break;
        default:;
    }
}
/***********************************************************************************************************************************************************/
 
int Menu()//���ÿ�ʼ�˵�
{
    int x;
	printf("������ƶ����ո��ȷ��\n");
    printf("1���������\n");
    printf("2����Һ���\n");
    printf("3�������Զ���\n");

    scanf("%d", &x);
    return x;
}
 
void Initialize()//��ʼ�����̣��������̣��ͻ�����
{
    int i, j;
 
    //��ʼ����������
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
    //��ʼ��������
    for(i = 0; i < N*2-1; i++)
    {
        for(j = 0; j < N*4-3; j++)
        {
            if(i == 0 && j == 0)//����Ϊ��Ե������ʽ
                     buffer[i][j]='a';//"��";
            else if(i == 0 && j == (N*4-3)-1)
                     buffer[i][j] = 'b';//"��";
            else if(i == (N*2-1)-1 && j == (N*4-3)-1)
                     buffer[i][j] = 'c';//"��";
            else if(i == (N*2-1)-1&&j == 0)
                     buffer[i][j]='d';//"��";
            else if(i == 0 && j % 4 == 0)
                     buffer[i][j]='e';//"��";
            else if(i == (N*2-1)-1 && j % 4 == 0)
                     buffer[i][j] = 'f';//"��";
            else if(i % 2 == 0 && j == 0)
                     buffer[i][j] = 'g';//"��";
            else if(i % 2 == 0 && j == (N*4-3)-1)
                     buffer[i][j] = 'h';//"��";
            else if((i == 0 || i == (N*2-1)-1) && (j/2) % 2 != 0)
                     buffer[i][j] = 'i';//"��";
            else if(i != 0 && i != (N*2-1)-1 && i % 2 == 0 && (j/2) % 2 != 0)
                     buffer[i][j] = 'j';//"��";
            else if(i % 2 != 0 && (j == 0 || j == (N*4-3)-1))
                     buffer[i][j] = 'k';//"��";
            else if(i % 2 != 0 && j != 0 && j != (N*4-3)-1 && j % 4 == 0)
                     buffer[i][j] = 'l';//"��";
            else if(i != 0 && j != 0 && i != (N*2-1)-1 && j != (N*4-3)-1 && i % 2 == 0 && j % 4 == 0)
                     buffer[i][j] = 'm';//�м�Ŀ�λ"��"
        }
    }
}
 
void Print(int x, int y)//������д���������̣��ͻ�����
{
    count[x+4][y+4] = chess[x][y];
    buffer[x*2][y*4] = chess[x][y] + 48;//����������ת��Ϊ�ַ�������
}
 
void Display()//�������������������Ļ
{
    int i, j;
    system("cls");//������Ļ��׼��д��
    for(i = 0; i < N*2-1; i++)
    {
        for(j = 0; j < N*4-3; j++)
        {
//            printf("%c", buffer[i][j]);
            switch(buffer[i][j])
            {
                case '1' : printf("��"); j++; break;//j++����ʡ
                case '2' : printf("��"); j++; break;//j++����ʡ
                case 'a' : printf("��"); break;
                case 'b' : printf("��"); break;
                case 'c' : printf("��"); break;
                case 'd' : printf("��"); break;
                case 'e' : printf("��"); break;
                case 'f' : printf("��"); break;
                case 'g' : printf("��"); break;
                case 'h' : printf("��"); break;
                case 'i' : printf("��"); break;//���µĺ��߸���
                case 'j' : printf("��"); j++; break;//�м�ĺ��߸�ϸ//j++;�������"��"ʱռ�����ַ���j++���Աܿ������һ���Ŀո�
                case 'k' : printf("��"); break;//���ҵ����߸���
                case 'l' : printf("��"); break;//�м�����߸���
                case 'm' : printf("��"); break;
                case ' ' : printf(" "); break;
                default  : printf("%c", buffer[i][j]);
 
            }//switch
        }//for
        printf("\n");
    }//for
	Sleep(100);
/*
    //����
    printf("%d\n", num);
    for(i = 4; i < M-4; i++)
    {
        for( j = 4; j < M-4; j++)
        printf("%8d", sum[i][j]);
        printf("\n");
    }
*/
}
 
void PGame()//��ҶԾֺ���
{
    static int Px0 = N/2, Py0 = N/2, Px = N/2, Py = N/2;
    now = 1;
    do{
        Mark(Px0, Py0, Px, Py);//�������λ��
        Buf_Display();//�������������������Ļ
        Px0 = Px; Py0 = Py;
        Input(&Px, &Py);
    }while(!(chess[Px][Py] && flag0 == 1));
    Print(Px, Py);//������д���������̣��ͻ�����
    num++;
    gs = JUDGE(Px + 4,Py + 4);
}
void CGame()//���ԶԾֺ���
{
    static int Cp = N/2, Cq = N/2, Cp0, Cq0;
    Buf_Display();//�������������������Ļ
    Grade();//��������λ�õĵ÷�
    Cp0 = Cp; Cq0 = Cq;//��¼��һ�ε�������λ��
    Find_point(&Cp, &Cq);//�ҵ���������ŵ�����λ��
    now = 3 - now;
    chess[Cp - 4][Cq - 4] = now;
    Mark(Cp0 - 4, Cq0 - 4, Cp - 4, Cq - 4);//�������λ��
    Print(Cp - 4, Cq - 4);//������д���������̣��ͻ�����
    num++;
    gs = JUDGE(Cp, Cq);
}
void Input(int *x, int *y)//���̷�������ƣ��ո������
{
    int input;
    flag0 = 0;
    input = getch();//��õ�һ��������Ϣ
    if(input == 0x20 && !chess[*x][*y])//�ж��ƶ���������Ƿ�����
    {
        chess[*x][*y] = player;
        flag0 = 1;
    }
    else if(input == 0xE0)//������µ��Ƿ������������������룬��һ��Ϊ0xE0��ʾ���µ��ǿ��Ƽ�
         {
             input = getch();//��õڶ���������Ϣ
             switch(input)//�жϷ���������ƶ����λ��
             {
                 case 0x48 : (*x)--; break;
                 case 0x4B : (*y)--; break;
                 case 0x50 : (*x)++; break;
                 case 0x4D : (*y)++; break;
             }//switch
             if(*x<0)   (*x) = N - 1;//������λ��Խ�����ƶ����Բ�
             if(*y<0)   (*y) = N - 1;
             if(*x>N-1) (*x) = 0;
             if(*y>N-1) (*y) = 0;
        }
}
 
void Mark(int x0,int y0,int x,int y)//��Ǽ�������λ��
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
 
    if(y == (N*4-3)-1)//�����������ƶ������Ҳ�ʱ����귢����λ��
    {
        buffer[x - 1][y + 2] = ' ';
        buffer[x + 1][y + 2] = ' ';
    }
}
 
int Basic_condition(int x,int y)//�ж�����λ���Ƿ����
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
 
void Find_point(int *pp, int *qq)//�ҵ���������ŵ�����λ��
{
    int i, j, flag = 0;//flagΪ������־
    long t = 0;
    int r, c=0, sumed[N*N][2];
    //Ѱ�ҵ�һ��δ����λ�ö�Ӧ��sumֵ
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
    //Ѱ������sumֵ
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
    //������ô��
    //�洢���е����sumֵ
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
    srand((unsigned)time(NULL)); //��ʼ�������
    //���ѡ�����sumֵ�е�һ������
    r = rand()%c;
    *pp = sumed[r][0];
    *qq = sumed[r][1];
}
 
int JUDGE(int x, int y)//�ж�ʤ��������1Ӯ������0��
{
    int a = 1, b = 1, c = 1, d = 1, i;//�ۼƺ�����б��б�ĸ������������ͬ������Ŀ
 
    for(i=1;i<5;i++)if(y+i<M-4&&count[x][y+i]==now)a++;else break;//���¼��
    for(i=1;i<5;i++)if(y-i>=4&&count[x][y-i]==now)a++;else break;//���ϼ��
    if(a>=5)return now;//���ﵽ5�����жϵ�ǰ�������ΪӮ��
 
    for(i=1;i<5;i++)if(x+i<M-4&&count[x+i][y]==now)b++;else break;//���Ҽ��
    for(i=1;i<5;i++)if(x-i>=4&&count[x-i][y]==now)b++;else break;//������
    if(b>=5)return now;//���ﵽ5�����жϵ�ǰ�������ΪӮ��
 
    for(i=1;i<5;i++)if(x+i<M-4&&y+i<M-4&&count[x+i][y+i]==now)c++;else break;//�����¼��
    for(i=1;i<5;i++)if(x-i>=4&&y-i>=4&&count[x-i][y-i]==now)c++;else break;//�����ϼ��
    if(c>=5)return now;//���ﵽ5�����жϵ�ǰ�������ΪӮ��
 
    for(i=1;i<5;i++)if(x+i<M-4&&y-i>=4&&count[x+i][y-i]==now)d++;else break;//�����ϼ��
    for(i=1;i<5;i++)if(x-i>=4&&y+i<M-4&&count[x-i][y+i]==now)d++;else break;//�����¼��
    if(d>=5)return now;//���ﵽ5�����жϵ�ǰ�������ΪӮ��
 
    return 0;//��û�м�鵽�����飬�򷵻�0��ʾ��û����Ҵ��ʤ��
}
 
void Grade()//��������λ�õĵ÷�
{
    int i, j, m, n;
    int num1, num2;
    for(i = 4; i < M - 4; i++)
    {
        for(j = 4; j < M - 4; j++)
        {
            if(!chess[i - 4][j - 4])
            {
                sum[i][j] = 0;//ѭ����ʼ��sum[i][j]
                Base(i, j);//����Ϊ(i-4,j-4)���������������
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
 
void Base(int i,int j)//����Ϊ(i-4,j-4)���������������
{
    //����->����
    p[0][0]=count[i-4][j-4];p[0][1]=count[i-3][j-3];p[0][2]=count[i-2][j-2];p[0][3]=count[i-1][j-1];p[0][4]=count[i][j];
    p[1][0]=count[i-3][j-3];p[1][1]=count[i-2][j-2];p[1][2]=count[i-1][j-1];p[1][3]=count[i][j];p[1][4]=count[i+1][j+1];
    p[2][0]=count[i-2][j-2];p[2][1]=count[i-1][j-1];p[2][2]=count[i][j];p[2][3]=count[i+1][j+1];p[2][4]=count[i+2][j+2];
    p[3][0]=count[i-1][j-1];p[3][1]=count[i][j];p[3][2]=count[i+1][j+1];p[3][3]=count[i+2][j+2];p[3][4]=count[i+3][j+3];
    p[4][0]=count[i][j];p[4][1]=count[i+1][j+1];p[4][2]=count[i+2][j+2];p[4][3]=count[i+3][j+3];p[4][4]=count[i+4][j+4];
    //��->��
    p[5][0]=count[i-4][j];p[5][1]=count[i-3][j];p[5][2]=count[i-2][j];p[5][3]=count[i-1][j];p[5][4]=count[i][j];
    p[6][0]=count[i-3][j];p[6][1]=count[i-2][j];p[6][2]=count[i-1][j];p[6][3]=count[i][j];p[6][4]=count[i+1][j];
    p[7][0]=count[i-2][j];p[7][1]=count[i-1][j];p[7][2]=count[i][j];p[7][3]=count[i+1][j];p[7][4]=count[i+2][j];
    p[8][0]=count[i-1][j];p[8][1]=count[i][j];p[8][2]=count[i+1][j];p[8][3]=count[i+2][j];p[8][4]=count[i+3][j];
    p[9][0]=count[i][j];p[9][1]=count[i+1][j];p[9][2]=count[i+2][j];p[9][3]=count[i+3][j];p[9][4]=count[i+4][j];
    //����->����
    p[10][0]=count[i-4][j+4];p[10][1]=count[i-3][j+3];p[10][2]=count[i-2][j+2];p[10][3]=count[i-1][j+1];p[10][4]=count[i][j];
    p[11][0]=count[i-3][j+3];p[11][1]=count[i-2][j+2];p[11][2]=count[i-1][j+1];p[11][3]=count[i][j];p[11][4]=count[i+1][j-1];
    p[12][0]=count[i-2][j+2];p[12][1]=count[i-1][j+1];p[12][2]=count[i][j];p[12][3]=count[i+1][j-1];p[112][4]=count[i+2][j-2];
    p[13][0]=count[i-1][j+1];p[13][1]=count[i][j];p[13][2]=count[i+1][j-1];p[13][3]=count[i+2][j-2];p[13][4]=count[i+3][j-3];
    p[14][0]=count[i][j];p[14][1]=count[i+1][j-1];p[14][2]=count[i+2][j-2];p[14][3]=count[i+3][j-3];p[14][4]=count[i+4][j-4];
    //��->��
    p[15][0]=count[i][j-4];p[15][1]=count[i][j-3];p[15][2]=count[i][j-2];p[15][3]=count[i][j-1];p[15][4]=count[i][j];
    p[16][0]=count[i][j-3];p[16][1]=count[i][j-2];p[16][2]=count[i][j-1];p[16][3]=count[i][j];p[16][4]=count[i][j+1];
    p[17][0]=count[i][j-2];p[17][1]=count[i][j-1];p[17][2]=count[i][j];p[17][3]=count[i][j+1];p[17][4]=count[i][j+2];
    p[18][0]=count[i][j-1];p[18][1]=count[i][j];p[18][2]=count[i][j+1];p[18][3]=count[i][j+2];p[18][4]=count[i][j+3];
    p[19][0]=count[i][j];p[19][1]=count[i][j+1];p[19][2]=count[i][j+2];p[19][3]=count[i][j+3];p[19][4]=count[i][j+4];
}
 
long Assessment(int num1,int num2)//���ֱ�׼
{
    if(num2 == 0)//�жϵ�����Ԫ��÷�
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
    if(num2 != 0 && num1 == 0)//�ж������Ԫ��÷�
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
 
void Buf_Display()//����˫���彫�����������������Ļ
{
    int i, j;
    //������Ļ������
    HANDLE hNewConsole = CreateConsoleScreenBuffer(GENERIC_WRITE | GENERIC_READ,
    0,
    NULL,
    CONSOLE_TEXTMODE_BUFFER,
    NULL);
 
    //���ع��
    CONSOLE_CURSOR_INFO cci = {1, 0};
    SetConsoleCursorInfo(hNewConsole, &cci);
 
    //���ô��ڵĻ�������С
    COORD cdBufferSize = {58, 29};
    SetConsoleScreenBufferSize(hNewConsole, cdBufferSize);
    //Ȼ���������������鿴��
    for(i = 0; i < N*2-1; i++)
    {
        for(j = 0; j < N*4-3; j++)
        {
            COORD cdCursorPos = {j, i};
            char *p = NULL;
//          printf("%c", buffer[i][j]);
            switch(buffer[i][j])
            {
                case '1' : p = "��"; j++; break;//j++��ʡ
                case '2' : p = "��"; j++; break;//j++��ʡ
                case 'a' : p = "��"; break;
                case 'b' : p = "��"; break;
                case 'c' : p = "��"; break;
                case 'd' : p = "��"; break;
                case 'e' : p = "��"; break;
                case 'f' : p = "��"; break;
                case 'g' : p = "��"; break;
                case 'h' : p = "��"; break;
                case 'i' : p = "��"; break;//���µĺ��߸���
                case 'j' : p = "��"; j++; break;//�м�ĺ��߸�ϸ//j++;�������"��"ʱռ�����ַ���j++���Աܿ������һ���Ŀո�
                case 'k' : p = "��"; break;//���ҵ����߸���
                case 'l' : p = "��"; break;//�м�����߸���
                case 'm' : p = "��"; break;
                case ' ' : p = " "; break;
                default  : p = &buffer[i][j];
            }//switch,for
            if( ! p) continue;
            SetConsoleCursorPosition(hNewConsole, cdCursorPos);
            WriteConsole(hNewConsole, p, strlen(p), NULL, NULL);
        }
//      printf("\n");
    }//for
    //�ѻ��ƺõĶ�������ʾ����
    SetConsoleActiveScreenBuffer(hNewConsole);
    //�����Ϸ��������������ģʽ
/*    if(gs)
    {
        SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
    }*/
}
 
int  Judge_Display()//���ʤ�����
{
    if(num == N*N)
    {
        Buf_Display();//�������������������Ļ
        printf("ƽ�֣�\n");
        return 1;//�Ծֽ���
    }
    if(gs == 1)
    {
        Buf_Display();//�������������������Ļ
        printf("����Ӯ��\n");
        return 1;//�Ծֽ���
    }
    if(gs == 2)
    {
        Buf_Display();//�������������������Ļ
        printf("����Ӯ��\n");
        return 1;//�Ծֽ���
    }
 
    return 0;

}
