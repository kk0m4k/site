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
// ProcessControl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SITE.h"
#include "ProcessControl.h"


// CProcessControl

IMPLEMENT_DYNAMIC(CProcessControl, CWnd)

CProcessControl::CProcessControl()
: m_szServerName(_T(""))
, m_szObject(_T(""))
, m_dstPort(0)
, m_dwServiceType(0)
, m_dwHttpRequestFlags(0)
{
	m_dwHttpRequestFlags = INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_NO_AUTO_REDIRECT;
}

CProcessControl::~CProcessControl()
{
}


BEGIN_MESSAGE_MAP(CProcessControl, CWnd)
END_MESSAGE_MAP()

CSITEDoc* CProcessControl::GetDocument()
{
	CSITEDoc * po_Document =  (CSITEDoc *)((CMainFrame *)AfxGetMainWnd())->GetActiveDocument();
	return po_Document;
}

// CProcessControl 메시지 처리기입니다.

bool CProcessControl::StartSQLInjectionAudit(DWORD category_type, DWORD category_flag, DWORD count_idx)
{
	CSITEDoc * po_Document;
	
	po_Document = GetDocument();
	InitialReferenceData();

	if ( HttpParameterReconstruction() == false) return false;
	if ( HttpConnectionInitial() == false ) {
		HttpConnectionClose();
		return false;
	}

	po_Document->m_InjectionCategory = category_type;


	switch ( po_Document->m_DBMSApp ) {
		case DBMS_MSSQL:
			{
				CMssqlCode objMSSQL;			
				objMSSQL.SQLInjectionAttack(this, category_flag, count_idx);
			}
			break;
		case DBMS_ORACLE:
			break;
		case DBMS_MYSQL:
			break;
	}

	return true;
}

void CProcessControl::InitialReferenceData(void)
{
	CSITEDoc * po_Document = GetDocument();
	CSetupView  *pSetupView = ((CMainFrame *)AfxGetMainWnd())->GetRightGroupView()->GetRightTopView();

}


bool CProcessControl::HttpConnectionInitial(void)
{
	char szHttpUrlAddress[256] = {0};
	CSITEDoc * po_Document = GetDocument();
	
	po_Document->m_pHTTPSession = new CInternetSession;
	po_Document->m_pServer = NULL;
	po_Document->m_pFile = NULL;

	strncpy(szHttpUrlAddress, po_Document->m_HttpUrlAddress, sizeof(szHttpUrlAddress) - 1);

	if (!AfxParseURL((LPCTSTR)szHttpUrlAddress, m_dwServiceType, m_szServerName, m_szObject, m_dstPort) ||
		m_dwServiceType != INTERNET_SERVICE_HTTP) {
			AfxMessageBox("URL를 잘못 입력하셨습니다. 올바르게 입력해 주세요",MB_ICONERROR);
			return false;
	}
		
	try {

		po_Document->m_pServer = po_Document->m_pHTTPSession->GetHttpConnection(m_szServerName, m_dstPort);	
		  	
	}
	catch (CInternetException* pEx) {
		
		pEx->ReportError(); 
        pEx->Delete(); 
		return false;
	}

	return true;
}


/*
	URL 파라미터를 재구성하는 작업으로 SQL Injection 취약점이 존재하는
	파라미터를 파라미터 맨 뒤에 위치시킴
*/
bool CProcessControl::HttpParameterReconstruction(void)
{
	DWORD	idx = 0;
	BOOL	bFlag = true;
	CString szSubText;
	CString	szFoundParameter;
	CString OriginalHttpParameter;
	CString InjectionData;
	CString o_tmpHttpParameter;

	CSITEDoc * po_Document = GetDocument();
	
	InjectionData = po_Document->m_InjectionData;
	InjectionData.Append("=");

	if ( po_Document->m_GetPost == HTTP_GET ) OriginalHttpParameter = po_Document->m_HttpGetParameter;
	if ( po_Document->m_GetPost == HTTP_POST ) OriginalHttpParameter = po_Document->m_HttpParameter;
	
	po_Document->m_HttpParameter.Empty();
	po_Document->m_HttpGetParameter.Empty();

	if ( OriginalHttpParameter.Find(InjectionData) == -1 ) {
		MessageBox("취약한 파라미터를 정확히 입력해주세요");
		return false;
	}

	while ( bFlag ) {

		bFlag = AfxExtractSubString(szSubText, OriginalHttpParameter, idx, '&');
		if ( bFlag == false ) break;
		if ( szSubText.Find(InjectionData) == -1 ) {
			o_tmpHttpParameter.Append(szSubText);
			o_tmpHttpParameter.Append("&");
		} else {
			szFoundParameter.Append(szSubText);
		}
		idx++;
	}

	o_tmpHttpParameter.Append(szFoundParameter);

	if ( po_Document->m_GetPost == HTTP_GET ) po_Document->m_HttpGetParameter = o_tmpHttpParameter;
	if ( po_Document->m_GetPost == HTTP_POST ) po_Document->m_HttpParameter = o_tmpHttpParameter;

	return true;
}


void CProcessControl::HttpConnectionClose(void)
{
	CSITEDoc * po_Document = GetDocument();
	po_Document->HttpClose();
}