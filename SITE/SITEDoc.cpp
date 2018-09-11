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
// SITEDoc.cpp : CSITEDoc 클래스의 구현
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


// CSITEDoc 생성/소멸

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
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
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

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CSITEDoc serialization

void CSITEDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}


// CSITEDoc 진단

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
