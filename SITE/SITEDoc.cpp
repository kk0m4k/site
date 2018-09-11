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
// SITEDoc.cpp : CSITEDoc Ŭ������ ����
//

#include "stdafx.h"

#include "SITEDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSITEDoc

IMPLEMENT_DYNCREATE(CSITEDoc, CDocument)

BEGIN_MESSAGE_MAP(CSITEDoc, CDocument)
END_MESSAGE_MAP()


// CSITEDoc ����/�Ҹ�

CSITEDoc::CSITEDoc()
: m_DBMSApp(0)
, m_GetPost(0)
, m_VarType(0)
, m_Domain(0)
, m_dwDBrange(0)
, m_InjectionCategory(0)
, m_HttpUrlAddress(_T(""))
, m_HttpGetParameter(_T(""))
, m_HttpParameter(_T(""))
, m_InjectionData(_T(""))
, m_HttpHeader(_T(""))
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.
	m_DBMSApp = DBMS_NOTYET;
	m_GetPost = HTTP_NOTYET;
	m_Domain  = DOMAIN_NOTYET;
	m_dwDBrange = DBRANGE_CURRENT;

	m_pHTTPSession	= NULL;
	m_pServer		= NULL;
	m_pFile			= NULL;

}

CSITEDoc::~CSITEDoc()
{
}

BOOL CSITEDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CSITEDoc serialization

void CSITEDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}


// CSITEDoc ����

#ifdef _DEBUG
void CSITEDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSITEDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG



CMainFrame* CSITEDoc::GetMainFrame(void)
{
	CMainFrame* pFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	ASSERT_KINDOF(CMainFrame, pFrame);

	return pFrame;
}

void CSITEDoc::HttpClose(void)
{	
	if (m_pFile != NULL) { 
		m_pFile->Close(); 
		delete m_pFile;
		m_pFile = NULL;

	}
	if (m_pServer != NULL ) {
		m_pServer->Close();
		delete m_pServer;
		m_pServer = NULL;
	}

	if (m_pHTTPSession != NULL ) {
		m_pHTTPSession->Close(); 
		delete m_pHTTPSession;
		m_pHTTPSession = NULL; 
	}
}
