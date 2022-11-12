// GoView.cpp : implementation of the CGoView class
//

#include "stdafx.h"
#include "Go.h"

#include "GoDoc.h"
#include "GoView.h"
#include "GoSetup.h"

#include <math.h>
#include "mmsystem.h" //?

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BOARD_MARGIN	10 
extern board Evaluate_Value;
/////////////////////////////////////////////////////////////////////////////
// CGoView

IMPLEMENT_DYNCREATE(CGoView, CView)

BEGIN_MESSAGE_MAP(CGoView, CView)
	//{{AFX_MSG_MAP(CGoView)
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_GO_PASS, OnGamePass)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGoView construction/destruction

CGoView::CGoView()
{
	// TODO: add construction code here

}

CGoView::~CGoView()
{
}

BOOL CGoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGoView drawing

void CGoView::OnDraw(CDC* pDC) //棋盘绘制
{
	CGoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	GetClientRect(&rcWindow);

	boardwidth=(rcWindow.Width()>rcWindow.Height()?rcWindow.Height():rcWindow.Width());
	boardwidth-=BOARD_MARGIN*2;
	if(boardwidth<0) return;

	rcBoard.top=(rcWindow.Height()-boardwidth)/2;
	rcBoard.bottom=rcBoard.top+boardwidth;
	rcBoard.left=(rcWindow.Width()-boardwidth)/2;
	rcBoard.right=rcBoard.left+boardwidth;

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDC,rcWindow.Width(),rcWindow.Height());
	CBitmap* pOldBitmap=memDC.SelectObject(&memBitmap);

	// start to draw board
	memDC.FillRect(&rcWindow,&CBrush(RGB(0,0,0)));
	memDC.FillRect(&rcBoard,&CBrush(boardcolor));

	d=boardwidth/(game.edge+1);	// stone width

	memDC.SetBkMode(TRANSPARENT);//使输出的字符背景透明。
	CFont font;
	CSize size;
	CString str;
	char num[10],*np=num;
	font.CreateFont(d*2/3,0,0,0,FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,"Arial");
	CFont* pOldFont=(CFont*)memDC.SelectObject(&font); 

	int i,x,y;
	if(d>0)
		{
		// draw frame画横竖线
		for(x=0;x<game.edge;x++) {
			i=rcBoard.left+(x+1)*boardwidth/(game.edge+1);
			memDC.MoveTo(i,rcBoard.top+d);
			memDC.LineTo(i,rcBoard.bottom-d);

			str = ((char)('A'+x));//画顶端文字。
			size=memDC.GetTextExtent(str);
			memDC.TextOut(i-size.cx/2+5,rcBoard.top+d-size.cy-10,str);

			//str=CString(_itoa(x+1,np,10));//画底端文字。
			//size=memDC.GetTextExtent(str);
			memDC.TextOut(i-size.cx/2+5,rcBoard.bottom-d+10,str);
		}
		for(y=0;y<game.edge;y++) {
			i=rcBoard.top+(y+1)*boardwidth/(game.edge+1);
			memDC.MoveTo(rcBoard.left+d,i);
			memDC.LineTo(rcBoard.right-d,i);

		//	str=('A'+y);//画左端文字。
		//	size=memDC.GetTextExtent(str);
		//	memDC.TextOut(rcBoard.left+d-size.cx-5,i-size.cy/2,str);

		//	str=CString(_itoa(y+1,np,10));//画右端数字。
			str=CString(_itoa(game.edge-y,np,10));
			size=memDC.GetTextExtent(str);
			memDC.TextOut(rcBoard.left+d-size.cx-20,i-size.cy/2,str);//画左端文字。
			memDC.TextOut(rcBoard.right-d+size.cx/3+18,i-size.cy/2,str);//画右端数字。
		}

		CDC WhiteStoneDC,BlackStoneDC;
		BlackStoneDC.CreateCompatibleDC(pDC);
		WhiteStoneDC.CreateCompatibleDC(pDC);

		CBitmap BlackStoneBmp,WhiteStoneBmp;
		BlackStoneBmp.CreateCompatibleBitmap(pDC,d,d);
		WhiteStoneBmp.CreateCompatibleBitmap(pDC,d,d);

		CBitmap* pOldBlackBmp=BlackStoneDC.SelectObject(&BlackStoneBmp);
		CBitmap* pOldWhiteBmp=WhiteStoneDC.SelectObject(&WhiteStoneBmp);

		DrawStone(d,&WhiteStoneDC,&BlackStoneDC);

//		memDC.BitBlt(10,10,d,d,&BlackStoneDC,0,0,SRCCOPY);
//		memDC.BitBlt(70,70,d,d,&WhiteStoneDC,0,0,SRCCOPY);

		// show stones
		int x1,y1;
		char stone;
		for(x=0;x<game.edge;x++)
			for(y=0;y<game.edge;y++)
			{
				x1=rcBoard.left+(x+1)*boardwidth/(game.edge+1);
				y1=rcBoard.top+(y+1)*boardwidth/(game.edge+1);
				stone=game.theboard[x][y];
				
				if(stone==BLACK)// 黑色棋子
					memDC.BitBlt(x1-d/2,y1-d/2,d,d,&BlackStoneDC,0,0,SRCCOPY);
				else if(stone==WHITE)// 白色棋子
					memDC.BitBlt(x1-d/2,y1-d/2,d,d,&WhiteStoneDC,0,0,SRCCOPY);
			}
			//当前走步显示红十字。
		CPen pen(PS_SOLID,1,RGB(255,0,0));
		CPen *pOldPen=memDC.SelectObject(&pen);	
		x=game.mx;
		y=game.my;
		
		if(x>-1 && y>-1)//如果是有效的点。
		{
			x1=rcBoard.left+(x+0)*boardwidth/(game.edge+1);
			y1=rcBoard.top+(y+0)*boardwidth/(game.edge+1);
			memDC.MoveTo(x1+d,y1+d*3/4);memDC.LineTo(x1+d,y1+d*5/4);
			memDC.MoveTo(x1+d*3/4,y1+d);memDC.LineTo(x1+d*5/4,y1+d);
		}

		memDC.SelectObject(pOldPen);
		
		//ShowPointValue(&memDC,&rcBoard,boardwidth,d,side);//显示点的估值。

		WhiteStoneDC.SelectObject(pOldWhiteBmp);
		BlackStoneDC.SelectObject(pOldBlackBmp);
	}

	pDC->BitBlt(0,0,rcWindow.Width(),rcWindow.Height(),&memDC,0,0,SRCCOPY);
	memDC.SelectObject(pOldBitmap);	
}


////////////////////////////////////////////////////////////////////////////
//
//   DrawStone() is based on the source code of Jago 棋子
//
////////////////////////////////////////////////////////////////////////////

void CGoView::DrawStone(int d, CDC *pWhiteStoneDC, CDC *pBlackStoneDC) 
{
	
	double pixel=0.8,shadow=0.7;
	BOOL Alias=true;

	int red=255-GetRValue(boardcolor);
	int green=255-GetGValue(boardcolor);
	int blue=255-GetBValue(boardcolor);

	COLORREF pb,pw;
	double di,dj,d2=(double)d/2.0-5e-1,r=d2-2e-1,f=sqrt(3.0);
	double x,y,z,xr,xg,hh;
	int g;

	//if (GF.getParameter("smallerstones",false)) r-=1;
	for (int i=0; i<d; i++) {
		for (int j=0; j<d; j++)
		{	di=i-d2; dj=j-d2;
			hh=r-sqrt(di*di+dj*dj);
			if (hh>=0)
			{	z=r*r-di*di-dj*dj;
			    if (z>0) z=sqrt(z)*f;
			    else z=0;
				x=di; y=dj;
				xr=sqrt(6*(x*x+y*y+z*z));
				xr=(2*z-x+y)/xr;
				if (xr>0.9) xg=(xr-0.9)*10;
				else xg=0;
				if (hh>pixel || !Alias) {
					g=(int)(10+10*xr+xg*140);
					//pb=(255<<24)|(g<<16)|(g<<8)|g;
					pb=(g<<16)|(g<<8)|g;
					g=(int)(200+10*xr+xg*45);
					//pw=(255<<24)|(g<<16)|(g<<8)|g;
					pw=(g<<16)|(g<<8)|g;
				}
				else
				{	hh=(pixel-hh)/pixel;
					g=(int)(10+10*xr+xg*140);
					double shade;
					if (di-dj<r/3) shade=1;
					else shade=shadow;

					pb=(//(255<<24)|
					    (((int)((1-hh)*g+hh*shade*red))<<16)
						|(((int)((1-hh)*g+hh*shade*green))<<8)
						|((int)((1-hh)*g+hh*shade*blue)));

					g=(int)(200+10*xr+xg*45);

					pw=(//(255<<24)|
					    (((int)((1-hh)*g+hh*shade*red))<<16)
						|(((int)((1-hh)*g+hh*shade*green))<<8)
						|((int)((1-hh)*g+hh*shade*blue)));

				}
			}
			else pb=pw=boardcolor;
			
			pBlackStoneDC->SetPixel(j,i,pb);
			pWhiteStoneDC->SetPixel(j,i,pw);
	}
	}
}



/////////////////////////////////////////////////////////////////////////////
// CGoView printing

BOOL CGoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CGoView diagnostics

#ifdef _DEBUG
void CGoView::AssertValid() const
{
	CView::AssertValid();
}

void CGoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGoDoc* CGoView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGoDoc)));
	return (CGoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGoView message handlers

void CGoView::OnInitialUpdate() //起始设置
{
	CView::OnInitialUpdate();
	boardcolor=RGB(193,187,97);	// brown 棋盘颜色
	GameSetup=true;
    game.initgame();//初始化游戏

}

void CGoView::OnLButtonDown(UINT nFlags, CPoint point) //落子规则
{
	// TODO: Add your message handler code here and/or call default	

		if(GameSetup)//如果还没有进行初始设置，则不处理点击事件。
		return;

	
	
	char msg[50];
	
	if(*(game.player)!=game.ComputerColor)
	{
		int result_h;
		if(boardwidth<=0) return;
		int x=(point.x+d/2-rcBoard.left)*(game.edge+1)/boardwidth-1;
		int y=(point.y+d/2-rcBoard.top)*(game.edge+1)/boardwidth-1;
		if(x<0 || y<0 || x>=game.edge || y>=game.edge) return;	// out of bound
		if(game.theboard[x][y]!=EMPTY) return;
		result_h=game.nextmove(x,y);
		Invalidate();
		PlaySound("IDR_ALERT", NULL, SND_RESOURCE | SND_ASYNC);
		
		 if(result_h==COMPUTER_WIN)
		{
			Invalidate();
			if(game.ComputerColor==BLACK)
				strcpy(msg,"黑棋计算机赢！");
			else
				strcpy(msg,"白棋计算机赢！");
			GameSetup=true;
			AfxMessageBox(msg);
			
			return;
		}
	}

 //如果是计算机走。
	{ 
		int result=game.nextmove();
		Invalidate();
		PlaySound("IDR_ALERT", NULL, SND_RESOURCE | SND_ASYNC);
		if(result==0) //计算机已经落子了。
		{
		
			Invalidate();
		}
		else if(result==COMPUTER_PASS)
		{
			AfxMessageBox("计算机不走了!");
		}
		else if(result==COMPUTER_LOS)
		{
			Invalidate();
			if(game.ComputerColor==BLACK)
				strcpy(msg,"白棋人赢！");
			else
				strcpy(msg,"黑棋人赢！");
			GameSetup=true;
			AfxMessageBox(msg);
		}//
		else 
			AfxMessageBox("ComputerMove的返回值意义不明确");
		//game.OutputBoard();
	}
}

BOOL CGoView::OnEraseBkgnd(CDC* pDC) 
{
	return true;	
//	return CView::OnEraseBkgnd(pDC);
}



void CGoView::OnGamePass() //过棋
{
	if(GameSetup) return;

	game.nextmove(-1,0);	// pass

	// now computer's turn
	int result=game.nextmove();
	if(result==0) {
		PlaySound("IDR_ALERT", NULL, SND_RESOURCE | SND_ASYNC);
		Invalidate();
	}
	else if(result==PASS) AfxMessageBox("Computer Pass !");
	else AfxMessageBox("Game Over !");

}

void CGoView::OnFileNew() 
{
	CGoSetup Dialog;
	if(Dialog.DoModal()==IDOK)
	{
		GameSetup=false;
		game.edge=EDGE;//19-Dialog.m_boardsize;
		game.ComputerColor=(Dialog.m_black==0?BLACK:WHITE);
		*game.player=BLACK;
	
		game.initgame();
		if(game.ComputerColor==BLACK)
		{
			CPoint cp;
			cp.x=1;
			cp.y=1;
			OnLButtonDown(1, cp);
		}
		Invalidate();
		
		return ;
	}
	
}

void CGoView::OnFileOpen() 
{
	// TODO: Add your command handler code here
	//SaveScratch
game.SaveScratch(Evaluate_Value,"scratch_patterns.txt");
}
