//代码版本		2017年国赛 
//本代码由沈阳航空航天大学计算机博弈协会总负责人王跃霖统一调控
//爱因斯坦组负责人蒋嘉辰所带博弈组开发维护
//本代码适用于中国大学生计算机博弈大赛博弈项目2017版交互协议
//本代码仅提供交互协议的用法示范和简单AI博弈思路，开发者需自行改进完善代码参赛
//如有意见和建议请与我们尽早联系
//王跃霖	QQ：836473734
//蒋嘉辰	QQ：843795534 
// MonteCarlo.cpp : �������̨Ӧ�ó������ڵ㡣
#include "stdafx.h"
#include "CBoard.h"
#include "CMove.h"
#include "Competition.h"

using namespace std;

/*int _tmain(int argc, _TCHAR* argv[])
{
	srand((unsigned)time(NULL));   //���������
	int LayoutMode;				   //����ģʽ��Ĭ�ϲ��֣��ֶ����֣�
	int BattleMode;				   //��սģʽ�����˶�ս���˻���ս��
	int FirstHand;				   //���ַ����췽��������
	int AI;						   //AI��
	int Dice;                      //����
	MovePiece move;                //�ƶ�����
	int win =0;                    //ʤ����1���췽��-1��������
	while (1)
	{
		system("cls");
		cout << "Welcome Einstein Chess!\n" << endl;
		cout << "�� 0 ��Ĭ�ϲ���     �� 1 ���ֶ����� " << endl;
		cin >> LayoutMode;
		{}
		while (LayoutMode != 0 && LayoutMode != 1)
		{
			cout << "������ѡ�񲼾�ģʽ��" << endl;
			cin >> LayoutMode;
		}
		//int BOARD[5][5] = { { 0,0,0,0,0 },{ 1,-1,0,2,0 },{ 0,0,0,-6,0 },{ 0,0,-2,0,0 },{ 0,0,0,6,-0 } };  //Ĭ����
		int BOARD[5][5] = { { 1,2,3,0,0 },{ 4,5,0,0,0 },{ 6,0,0,0,-6 },{ 0,0,0,-5,-4 },{ 0,0,-3,-2,-1 } };  //Ĭ�����
		if (LayoutMode == 1)
		{
			print();
			cout << "����췽���Ӱڲ�:(1,1) (1,2) (1,3) (2,1) (2,2) (3,1)" << endl;
			cin >> BOARD[0][0] >> BOARD[0][1] >> BOARD[0][2] >> BOARD[1][0] >> BOARD[1][1] >> BOARD[2][0];  //����췽
			cout << "�����������Ӱڲ�:(5,5) (5,4) (5,3) (4,5) (4,4) (3,5)" << endl;
			int a[6];
			for (int i = 0; i < 6; i++)
				cin >> a[i];
			BOARD[4][4] = -a[0];
			BOARD[4][3] = -a[1];
			BOARD[4][2] = -a[2];
			BOARD[3][4] = -a[3];
			BOARD[3][3] = -a[4];
			BOARD[2][4] = -a[5]; //��������
		}
		Board CB(BOARD);           //�������̲���ʼ��
		CB.show();
		cout << "�� 0 ���˻���ս     �� 1 �����˶�ս " << endl;
		cin >> BattleMode;
		while (BattleMode != 0 && BattleMode != 1)
		{
			cout << "������ѡ���սģʽ��" << endl;
			cin >> BattleMode;
		}
		cout << "�� 0 ���췽����     �� 1 ���������� " << endl;         
		cin >> FirstHand;
		while (FirstHand != 0 && FirstHand != 1)
		{
			cout << "������ѡ�����ַ���" << endl;
			cin >> FirstHand;
		}
		int faction = -(FirstHand * 2 - 1);                  //1Ϊ�죬-1Ϊ��
		switch (BattleMode)
		{
		case 0:
			cout << "�� 0 ��AI�췽       �� 1 ��AI���� " << endl;
			cin >> AI;
			while (AI != 0 && AI != 1)
			{
				cout << "������ѡ��AI����" << endl;
				cin >> AI;
			}
			AI = -(AI * 2 - 1);                       //1Ϊ�죬-1Ϊ��
			while (1)
			{
				CB.show();
				//CB.GameState();
				if (win == 1)
				{
					cout << "\n�췽��ʤ��";
					win = 0;
					system("pause");
					break;
				}
				if (win == -1)
				{
					cout << "\n������ʤ��";
					win = 0;
					system("pause");
					break;
				}
				//Dice = RandDice();
				
				//cout << "������Ϊ:" << Dice << endl;
				if (faction == AI)
				{
					cout << "������Ϊ:" << endl;
					cin >> Dice;
					cout <<"����س�ȷ��AI����"<<endl;
					if (AI == Red)
						cout << "�췽";
					else
						cout << "����";
					cout << "AI����˼����..." << endl;
					MonteCarloMove(CB, AI*Dice, 1);
					system("pause");
				}
				else
				{
					if (faction == Red)
						cout << "�췽����" << endl;
					else
						cout << "��������" << endl;
					cout << "�����ʽ:Ҫ�ƶ������� �ƶ����X���� �ƶ����Y����" << endl;
					cin >> move.piece >> move.loc.x >> move.loc.y;
					move.loc.x--;
					move.loc.y--;
					CB.Move(move.loc, faction*move.piece);
				}
				win = CB.Winner();
				faction *= -1;
			}
			break;
		case 1:
			while (1)
			{
				CB.show();
				if (win == 1)
				{
					cout << "\n�췽��ʤ��";
					win = 0;
					system("pause");
					break;
				}
				if (win == -1)
				{
					cout << "\n������ʤ��";
					win = 0;
					system("pause");
					break;
				}
				Dice = RandDice();
				cout << "������Ϊ:" << Dice << endl;
				if (faction == Red)
					cout << "�췽����" << endl;
				else
					cout << "��������" << endl;
				cout << "�����ʽ:Ҫ�ƶ������� �ƶ����X���� �ƶ����Y����" << endl;
				cin >> move.piece >> move.loc.x >> move.loc.y;
				move.loc.x--;
				move.loc.y--;
				CB.Move(move.loc, faction*move.piece);
				win = CB.Winner();
				faction *= -1;
			}
		}
	}
	return 0;
}*/

//����
/*int main()
{
	srand((unsigned)time(NULL));
	double red = 0, blue = 0;
	int win, player = -1, Dice, times = 1;
	int BOARD[5][5] = { { 1,2,3,0,0 },{ 4,5,0,0,0 },{ 6,0,0,0,-6 },{ 0,0,0,-5,-4 },{ 0,0,-3,-2,-1 } };
	for (int i = 0; i < times; i++)
	{
		Board CB(BOARD);
		player = -1 * player;
		int _player = player;
		while (1)
		{
			win = CB.Winner();
			_player *= -1;
			system("cls");
			CB.show();
			if (win == Red)
			{
				red = red + 1;
				break;
			}
			else if (win == Blue)
			{
				blue = blue + 1;
				break;
			}
			else
			{
				
				Dice = RandDice();
				cout << CB.GameState() << endl;
				cout << "������Ϊ:" << _player*Dice << endl;
				system("pause");
				if (_player == 1)
					MonteCarloMove(CB, _player*Dice, 0);
				else
					MonteCarloMove(CB, _player*Dice, 1);
			}
		}
	}
	cout << red/times  << endl << blue/times ;
	return 0;
}*/

int main()
{
	Match();
}
