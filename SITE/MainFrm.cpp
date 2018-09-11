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
// MainFrm.cpp : CMainFrame Ŭ������ ����
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
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
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
		TRACE0("���� ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("���� ǥ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}

	// TODO: ���� ������ ��ŷ�� �� ���� �Ϸ��� �� �� ���� �����Ͻʽÿ�.

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.
	
	cs.style &= ~FWS_ADDTOTITLE;
	return TRUE;
}


// CMainFrame ����

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


// CMainFrame �޽��� ó����




BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

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

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	
	SetHttpHeaderEdit();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
}

void CMainFrame::OnAppStart()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CRightGroupView * pRightGroupView = GetRightGroupView();	
	
	CSetupView * pRightTopView = pRightGroupView->GetRightTopView();
	
	pRightTopView->AppStartProcess();

}

void CMainFrame::OnAppStop()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CSITEDoc * po_Document = GetDocument();
	po_Document->HttpClose();

}

void CMainFrame::OnEditPaste()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CWnd* pWnd = GetFocus();
    
	if(pWnd && (pWnd->IsKindOf(RUNTIME_CLASS(CEdit))) ) {
		((CEdit*)pWnd)->Paste();
	}
}

void CMainFrame::OnEditCopy()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CWnd* pWnd = GetFocus();
    
	if(pWnd && (pWnd->IsKindOf(RUNTIME_CLASS(CEdit))) ) {
        ((CEdit*)pWnd)->Copy();
	}
}

void CMainFrame::OnEditCut()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CWnd* pWnd = GetFocus();
    
	if(pWnd && (pWnd->IsKindOf(RUNTIME_CLASS(CEdit))) ) {
        ((CEdit*)pWnd)->Cut();
	}
}



void CMainFrame::OnEditUndo()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CWnd* pWnd = GetFocus();
    
	if(pWnd && (pWnd->IsKindOf(RUNTIME_CLASS(CEdit))) ) {
        ((CEdit*)pWnd)->Undo();
	}
}