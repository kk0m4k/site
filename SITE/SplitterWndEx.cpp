/*
  본 프로그램은 MS-SQL서버 연동하는 웹 페이지에 에러 메시지 기반
  SQL 인젝션 취약점이 존재할 경우에 해당 DBMS 정보(DB명,테이블명,
  필드명, 데이터 값)를 뽑아(select)내는 점검 프로그램입니다.

  이 프로그램을 구현하기 위해서 짧은 시간에 MFC를 학습하였기에 코
  드 완성도가 많이 떨어지고 C++ 스타일 답지 않을 수 있습니다.

  해당 소스는 누구나 자유롭게 배포할 수 있으며, 수정도 가능합니다.
  
  단, 악용하였을 경우에는 해킹 프로그램을 오용될 수 있기 때문에 학습
  용도로만 사용해야 합니다.

  악용하였을 경우에는 본인이 책임을 져야 합니다.

  작성자 : 김태훈(franc3sco@paran.com)

*/
// SplitterWndEx.cpp : 구현 파일입니다.
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



// CSplitterWndEx 메시지 처리기입니다.



void CSplitterWndEx::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.
	
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
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	// pDC 가 NULL값이면 Invalidate을 한다.
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
