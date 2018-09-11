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
// RightGroupView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SITE.h"
#include "RightGroupView.h"


// CRightGroupView

IMPLEMENT_DYNCREATE(CRightGroupView, CView)

CRightGroupView::CRightGroupView()
{

}

CRightGroupView::~CRightGroupView()
{
}

BEGIN_MESSAGE_MAP(CRightGroupView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CRightGroupView 그리기입니다.

void CRightGroupView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CRightGroupView 진단입니다.

#ifdef _DEBUG
void CRightGroupView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CRightGroupView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CRightGroupView 메시지 처리기입니다.

int CRightGroupView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	// Framework로 부터 Context 정보를 얻는다.

	CCreateContext *pContext = (CCreateContext*)lpCreateStruct->lpCreateParams;
	
	if( !m_wndSplitter.CreateStatic(this, 2, 1) )
	{
		return FALSE;
	}


	if( !m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CSetupView), CSize(0, 360), pContext) )
	{
		m_wndSplitter.DestroyWindow();
		return -1;
	}

	
	if( !m_wndSplitter.CreateView(1, 0, RUNTIME_CLASS(CLogDataView), CSize(0, 0), pContext) )
	{
		m_wndSplitter.DestroyWindow();
		return -1;
	}

	return 0;
}

void CRightGroupView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
		// TODO: 여기에 메시지 처리기 코드를 추가합니다.

		if( m_wndSplitter.GetSafeHwnd())
	{
		int nCX = ::GetSystemMetrics(SM_CXEDGE);
		int nCY = ::GetSystemMetrics(SM_CYEDGE);
				
		m_wndSplitter.MoveWindow(-nCX, -nCY, cx+(nCX*2), cy+(nCY*2));
	}
}

CSetupView* CRightGroupView::GetRightTopView()
{
	CSetupView* pView = DYNAMIC_DOWNCAST(CSetupView, m_wndSplitter.GetPane(0, 0));
	ASSERT_KINDOF(CSetupView, pView);

	return pView;
}

CLogDataView * CRightGroupView::GetRightBottomView(void)
{
	CLogDataView* pView = DYNAMIC_DOWNCAST(CLogDataView, m_wndSplitter.GetPane(1, 0));
	ASSERT_KINDOF(CLogDataView, pView);

	return pView;
}
