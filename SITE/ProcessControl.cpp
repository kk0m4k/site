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
// ProcessControl.cpp : ���� �����Դϴ�.
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

// CProcessControl �޽��� ó�����Դϴ�.

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
			AfxMessageBox("URL�� �߸� �Է��ϼ̽��ϴ�. �ùٸ��� �Է��� �ּ���",MB_ICONERROR);
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
	URL �Ķ���͸� �籸���ϴ� �۾����� SQL Injection ������� �����ϴ�
	�Ķ���͸� �Ķ���� �� �ڿ� ��ġ��Ŵ
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
		MessageBox("����� �Ķ���͸� ��Ȯ�� �Է����ּ���");
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