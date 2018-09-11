/*
  �� ���α׷��� MS-SQL���� �����ϴ� �� �������� ���� �޽��� ���
  SQL ������ ������� ������ ��쿡 �ش� DBMS ����(DB��,���̺��,
  �ʵ��, ������ ��)�� �̾�(select)���� ���� ���α׷��Դϴ�.

  �� ���α׷��� �����ϱ� ���ؼ� ª�� �ð��� MFC�� �н��Ͽ��⿡ ��
  �� �ϼ����� ���� �������� C++ ��Ÿ�� ���� ���� �� �ֽ��ϴ�.

  �ش� �ҽ��� ������ �����Ӱ� ������ �� ������, ������ �����մϴ�.
  
  ��, �ǿ��Ͽ��� ��쿡�� ��ŷ ���α׷��� ����� �� �ֱ� ������ �н�
  �뵵�θ� ����ؾ� �մϴ�.

  �ǿ��Ͽ��� ��쿡�� ������ å���� ���� �մϴ�.

  �ۼ��� : ������(franc3sco@paran.com)

*/
// SplitterWndEx.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SITE.h"
#include "SplitterWndEx.h"

#define X_BORDER 1
#define Y_BORDER 1
// CSplitterWndEx

IMPLEMENT_DYNAMIC(CSplitterWndEx, CSplitterWnd)

CSplitterWndEx::CSplitterWndEx()
{

}

CSplitterWndEx::~CSplitterWndEx()
{
}


BEGIN_MESSAGE_MAP(CSplitterWndEx, CSplitterWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CSplitterWndEx �޽��� ó�����Դϴ�.



void CSplitterWndEx::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CWnd::OnPaint()��(��) ȣ������ ���ʽÿ�.
	
//	CRect rcClient;
//	GetClientRect(&rcClient);
	
	CRect rcInside;
	GetInsideRect(rcInside);
//	rcInside.InflateRect(4, 4);
	
//	OnDrawSplitter(&dc, splitBox, CRect(rcInside.right, rcClient.top,
//			rcClient.right, rcClient.top + m_cySplitter));
		
	// extend split bars to window border (past margins)
	DrawAllSplitBars(&dc, rcInside.right, rcInside.bottom);
//	DrawAllSplitBars(&dc, rcClient.right, rcClient.bottom);


}

void CSplitterWndEx::OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rectArg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	// pDC �� NULL���̸� Invalidate�� �Ѵ�.
	if( !pDC )
	{
		RedrawWindow(rectArg, NULL, RDW_INVALIDATE|RDW_NOCHILDREN);
		return;
	}
	ASSERT_VALID(pDC);
	
	CRect rect = rectArg;

	switch( nType )
	{
	case splitBorder:
		{
			pDC->Draw3dRect(rect, GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
			rect.InflateRect(-X_BORDER, -Y_BORDER);
			pDC->Draw3dRect(rect, GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNHIGHLIGHT));
			return;
		}

	case splitIntersection:
		break;

	case splitBox:
		{
			pDC->Draw3dRect(rect, GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_WINDOWFRAME));
			rect.InflateRect(-X_BORDER, -Y_BORDER);
			pDC->Draw3dRect(rect, GetSysColor(COLOR_BTNHIGHLIGHT), GetSysColor(COLOR_BTNSHADOW));
			rect.InflateRect(-X_BORDER, -Y_BORDER);
		}
		break;
		
	case splitBar:
		break;

	default:
		ASSERT(FALSE);
	}	
	
	pDC->FillSolidRect(rect, GetSysColor(COLOR_BTNFACE));

	//CSplitterWnd::OnDrawSplitter(pDC, nType, rect);
}
