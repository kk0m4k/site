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
// RightGroupView.cpp : ���� �����Դϴ�.
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


// CRightGroupView �׸����Դϴ�.

void CRightGroupView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
}


// CRightGroupView �����Դϴ�.

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


// CRightGroupView �޽��� ó�����Դϴ�.

int CRightGroupView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	// Framework�� ���� Context ������ ��´�.

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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
		// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

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
