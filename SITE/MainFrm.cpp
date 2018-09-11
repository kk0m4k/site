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
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_APP_START, &CMainFrame::OnAppStart)
	ON_COMMAND(ID_APP_STOP, &CMainFrame::OnAppStop)
	ON_COMMAND(ID_EDIT_PASTE, &CMainFrame::OnEditPaste)
	ON_COMMAND(ID_EDIT_COPY, &CMainFrame::OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, &CMainFrame::OnEditCut)
	ON_COMMAND(ID_EDIT_UNDO, &CMainFrame::OnEditUndo)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

CSITEDoc* CMainFrame::GetDocument()
{
	CSITEDoc * po_Document =  (CSITEDoc *)((CMainFrame *)AfxGetMainWnd())->GetActiveDocument();
	return po_Document;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("도구 모음을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	// TODO: 도구 모음을 도킹할 수 없게 하려면 이 세 줄을 삭제하십시오.

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.
	
	cs.style &= ~FWS_ADDTOTITLE;
	return TRUE;
}


// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 메시지 처리기




BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if( !m_wndSplitter.CreateStatic(this, 1, 2) )
	{	
		TRACE0("Failed to create splitter window\n");
		return FALSE;
	}

	if( !m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CSITEView), CSize(250, 0), pContext) )
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}


	if( !m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CRightGroupView), CSize(0, 0), pContext) )
	{	
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}

	return TRUE;

}


CSITEView * CMainFrame::GetSITEView(void)
{
	CSITEView* pView = DYNAMIC_DOWNCAST(CSITEView, m_wndSplitter.GetPane(0, 0));
	ASSERT_KINDOF(CSITEView, pView);
	return pView;
}

CRightGroupView * CMainFrame::GetRightGroupView(void)
{
	CRightGroupView* pView = DYNAMIC_DOWNCAST(CRightGroupView, m_wndSplitter.GetPane(0, 1));
	ASSERT_KINDOF(CRightGroupView, pView);

	return pView;
}


void CSetupView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	SetHttpHeaderEdit();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
}

void CMainFrame::OnAppStart()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CRightGroupView * pRightGroupView = GetRightGroupView();	
	
	CSetupView * pRightTopView = pRightGroupView->GetRightTopView();
	
	pRightTopView->AppStartProcess();

}

void CMainFrame::OnAppStop()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSITEDoc * po_Document = GetDocument();
	po_Document->HttpClose();

}

void CMainFrame::OnEditPaste()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWnd* pWnd = GetFocus();
    
	if(pWnd && (pWnd->IsKindOf(RUNTIME_CLASS(CEdit))) ) {
		((CEdit*)pWnd)->Paste();
	}
}

void CMainFrame::OnEditCopy()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWnd* pWnd = GetFocus();
    
	if(pWnd && (pWnd->IsKindOf(RUNTIME_CLASS(CEdit))) ) {
        ((CEdit*)pWnd)->Copy();
	}
}

void CMainFrame::OnEditCut()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWnd* pWnd = GetFocus();
    
	if(pWnd && (pWnd->IsKindOf(RUNTIME_CLASS(CEdit))) ) {
        ((CEdit*)pWnd)->Cut();
	}
}



void CMainFrame::OnEditUndo()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWnd* pWnd = GetFocus();
    
	if(pWnd && (pWnd->IsKindOf(RUNTIME_CLASS(CEdit))) ) {
        ((CEdit*)pWnd)->Undo();
	}
}