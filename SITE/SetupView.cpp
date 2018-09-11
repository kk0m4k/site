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
// SetupView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SITE.h"
#include "SetupView.h"


// CSetupView
struct _tagHTTPHEADER HTTPHEADER[NUMOFHTTPHEADER] = {
	{1, HTTPHEADERAGENT},
	{2, HTTPHEADERLANGUAGE},
	{3, HTTPHEADERCONTENTTYPE},
	{4, HTTPHEADERACCEPT},
	{5, HTTPHEADERCOOKIE},
	{6, HTTPHEADERREFERER},
	{-1, NULL}
};

IMPLEMENT_DYNCREATE(CSetupView, CFormView)

CSetupView::CSetupView()
	: CFormView(CSetupView::IDD)
	, m_HttpUrlAddress(_T(""))
	, m_HttpHeaderEdit(_T(""))
	, m_HttpParameterEdit(_T(""))
	, m_HeaderParameterCombo(_T(""))
	, m_ParameterType(_T(""))
	, m_InjectionData(_T(""))
	, m_szDBRangeType(_T(""))
	, m_GetPost(0)
	, m_DBMSApp(0)
	, m_Domain(0)
	, m_dwDBrange(0)
	, m_VarType(0)
	, m_HttpMethodCombo(_T(""))
	, m_HttpGetParameter(_T(""))
{

}

CSetupView::~CSetupView()
{
}

void CSetupView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_URL, m_HttpUrlAddress);
	DDX_Text(pDX, IDC_HTTPMETHODCOMBO, m_HttpMethodCombo);
	DDX_Text(pDX, IDC_HEADEREDITOR, m_HttpHeaderEdit);
	DDX_Text(pDX, IDC_PARAMETEREDITOR, m_HttpParameterEdit);
	DDX_CBString(pDX, IDC_HEADERPARMCOMBO, m_HeaderParameterCombo);
	DDX_CBString(pDX, IDC_DATATYPE, m_ParameterType);
	DDX_CBString(pDX, IDC_OUTPUTDBRANGE, m_szDBRangeType);
	DDX_CBString(pDX, IDC_INJECTIONDATA, m_InjectionData);
	DDX_Control(pDX, IDC_URL, m_HttpUrlAddressCtrl);
	DDX_Control(pDX, IDC_INJECTIONDATA, m_InjectionDataCtrl);
	DDX_Control(pDX, IDC_HEADERPARMCOMBO, m_HeaderParameterComboCtrl);
	DDX_Control(pDX, IDC_DATATYPE, m_ParameterTypeCtrl);
	DDX_Control(pDX, IDC_Cookie, m_CookieChkCtrl);
	DDX_Control(pDX, IDC_HEADEREDITOR, m_HttpHeaderEditCtrl);
	DDX_Control(pDX, IDC_PARAMETEREDITOR, m_HttpParameterEditCtrl);
	DDX_Control(pDX, IDC_HTTPMETHODCOMBO, m_HttpMethodComboCtrl);
}

BEGIN_MESSAGE_MAP(CSetupView, CFormView)
	ON_BN_CLICKED(IDC_RDO_MSSQL, &CSetupView::OnBnClickedRdoMssql)
	ON_BN_CLICKED(IDC_RDO_ORACLE, &CSetupView::OnBnClickedRdoOracle)
	ON_BN_CLICKED(IDC_RDO_MYSQL, &CSetupView::OnBnClickedRdoMysql)
	ON_EN_SETFOCUS(IDC_PARAMETEREDITOR, &CSetupView::OnEnSetfocusParametereditor)
	ON_EN_SETFOCUS(IDC_HEADEREDITOR, &CSetupView::OnEnSetfocusHeadereditor)
ON_CBN_SELCHANGE(IDC_HEADERPARMCOMBO, &CSetupView::OnCbnSelchangeHeaderparmcombo)
ON_CBN_SELCHANGE(IDC_HTTPMETHODCOMBO, &CSetupView::OnCbnSelchangeHttpmethodcombo)
END_MESSAGE_MAP()


// CSetupView 진단입니다.

#ifdef _DEBUG
void CSetupView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSetupView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


CSITEDoc* CSetupView::GetDocument()  // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSITEDoc)));
	return (CSITEDoc*)m_pDocument;
}
// CSetupView 메시지 처리기입니다.

void CSetupView::SetHttpHeaderEdit()
{
	//CSetupView  *pSetupView = pDocument->GetMainFrame()->GetRightGroupView()->GetRightTopView();
	CString stHttpHeader;

	for ( int idx=0; HTTPHEADER[idx].idxhttpheader != 0; idx ++ ) {
		CString stTmp;
		stTmp.Format("%s",HTTPHEADER[idx].szhttpheader);
		stHttpHeader += stTmp;
	}

	//pSetupView->SetDlgItemTextA(IDC_HEADEREDITOR, stHttpHeader);
	SetDlgItemTextA(IDC_HEADEREDITOR, stHttpHeader);
}


void CSetupView::OnBnClickedRdoMssql()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_DBMSApp = DBMS_MSSQL;
}

void CSetupView::OnBnClickedRdoOracle()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_DBMSApp = DBMS_ORACLE;
}

void CSetupView::OnBnClickedRdoMysql()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_DBMSApp = DBMS_MYSQL;
}

bool CSetupView::AppStartProcess(void)
{
	CSITEDoc * pDocument = GetDocument();
	char szHttpUrlAddress[256] = { 0 };
	CProcessControl objProcessCtrl;
	DWORD	category_type = GET_DBMS;
	
	if ( ValidateCheck() == false ) return false;

	UpdateData();

	initialize_DOC_variables();

	CSITEView * pSITEView =  pDocument->GetMainFrame()->GetSITEView();
	
	
	getHttpServerName(m_HttpUrlAddress,szHttpUrlAddress, sizeof(szHttpUrlAddress));

	if (strlen(szHttpUrlAddress) == 0 ) {
		MessageBox("eee");
		return false;
	}
	pSITEView->SetTargetUrlonTree(&szHttpUrlAddress[0]);
	
	objProcessCtrl.StartSQLInjectionAudit(category_type);

	return false;
}

bool CSetupView::ValidateCheck(void)
{
	CString o_tmpString;
	CSITEDoc * po_Document = GetDocument();
	UpdateData();

	if (m_HttpUrlAddress.IsEmpty()) {
		AfxMessageBox((LPCTSTR)"점검 대상 URL 정보를 입력해 주세요",  MB_ICONERROR);
		m_HttpUrlAddressCtrl.SetFocus();
		m_HttpUrlAddress="http://";
		UpdateData(FALSE);
		return false;
	}

	if ( m_HttpUrlAddress.Find("http://") == -1) {
		o_tmpString.Append("http://");
		o_tmpString.Append(m_HttpUrlAddress);
		m_HttpUrlAddress.Empty();
		m_HttpUrlAddress = o_tmpString;
		UpdateData(FALSE);
	}

	if ( SetHttpMethod() == false ) return false;

	if ( m_GetPost == HTTP_GET) { 
		if ( SetHttpGETParameter() == false ) return false;
	}

	if ( m_HeaderParameterCombo.IsEmpty()) {
		AfxMessageBox((LPCTSTR)"취약점이 존재하는 영역(헤더 또는 파라미터)를 선택해주세요",  MB_ICONERROR);
		m_HeaderParameterComboCtrl.SetFocus();
		return false;
	}


	if ( m_ParameterType.IsEmpty()) {
		AfxMessageBox((LPCTSTR)"취약점이 존재하는 파라미터 또는 헤더의 타입을 선택해주세요",  MB_ICONERROR);
		m_ParameterTypeCtrl.SetFocus();
		return false;
	}

	if ( m_InjectionData.IsEmpty()) {
		AfxMessageBox((LPCTSTR)"취약점이 존재하는 파라미터 또는 헤더를 선택(입력)해주세요",  MB_ICONERROR);
		m_InjectionDataCtrl.SetFocus();
		return false;
	}

	if ( m_GetPost == HTTP_NOTYET) { 
		AfxMessageBox((LPCTSTR)"HTTP Method(GET/POST)를 선택해 주세요",  MB_ICONERROR);
		return false;
	}

	if ( m_DBMSApp == DBMS_NOTYET) { 
		AfxMessageBox((LPCTSTR)"DBMS 타입을 선택해 주세요",  MB_ICONERROR);
		return false;
	}

	if ( SetDomain() == false ) return false;
	if ( SetVariableType() == false ) return false;
	if ( SetDBrange() == false ) return false;


	return true;
}

bool CSetupView::SetHttpMethod(void)
{
	CSITEDoc * po_Document = GetDocument();
	if (m_HttpMethodCombo.Compare("GET") == 0) {
			m_GetPost = HTTP_GET;
			m_HttpParameterEditCtrl.EnableWindow(FALSE);		

	} 
	else if (m_HttpMethodCombo.Compare("POST") == 0 ) {
			m_GetPost = HTTP_POST;
			m_HttpParameterEditCtrl.EnableWindow(TRUE);
	}
	else {
		AfxMessageBox((LPCTSTR)"HTTP Method 값을 선택 해주세요",  MB_ICONERROR);
		return false;
	}

	return true;
}


bool CSetupView::SetDomain(void)
{

	CSITEDoc * po_Document = GetDocument();

	if (m_HeaderParameterCombo.Compare("GET/POST Parameter") == 0) {
			m_Domain = DOMAIN_PARAMETER;
			m_CookieChkCtrl.EnableWindow(FALSE);

	} 
	else if (m_HeaderParameterCombo.Compare("HTTP Header") == 0 ) {
			m_Domain = DOMAIN_HEADER;
			m_CookieChkCtrl.EnableWindow(TRUE);
	}
	else {
		AfxMessageBox((LPCTSTR)"콤보박스에 있는 값을 선택 해주세요",  MB_ICONERROR);
		return false;
	}

	return true;
}

bool CSetupView::SetVariableType(void)
{
	CSITEDoc * po_Document = GetDocument();

	if (m_ParameterType.Compare("String Type") == 0) {
		m_VarType = VARTYPE_STRING;	
	} 
	else if (m_ParameterType.Compare("Integer Type") == 0 ) {
		m_VarType = VARTYPE_INTEGER;	
	} 
	else {
		AfxMessageBox((LPCTSTR)"파라미터 데이터 타입을 선택해 주세요",  MB_ICONERROR);
		return false;
	}

	return true;
}


bool CSetupView::SetDBrange(void)
{
	CSITEDoc * po_Document = GetDocument();

	if (m_szDBRangeType.Compare("현재 데이터베이스만 조회") == 0) {
		m_dwDBrange = DBRANGE_CURRENT;	
	} 
	else if (m_szDBRangeType.Compare("전체 데이터베이스 조회") == 0 ) {
		m_dwDBrange  = DBRANGE_ALL;	
	} 
	else {
		AfxMessageBox((LPCTSTR)"데이터베이스 죄회 범위를 선택 해주세요",  MB_ICONERROR);
		return false;
	}
	return true;
}



void CSetupView::OnEnSetfocusParametereditor()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_HttpParameterEditCtrl.SetActiveWindow();
}

void CSetupView::OnEnSetfocusHeadereditor()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_HttpHeaderEditCtrl.SetActiveWindow();

}


bool CSetupView::SetInjectionDataCombo(void)
{

	DWORD	fidx = 0;
	DWORD	sidx = 0;
	BOOL	bFirstFlag = true;
	BOOL	bSecondFlag = true;
	CString szfirstSubText;
	CString szSecondSubText;
	CString	o_HttpParameter;

	if ( m_GetPost == HTTP_GET ) o_HttpParameter = m_HttpGetParameter;
	if ( m_GetPost == HTTP_POST ) o_HttpParameter = m_HttpParameterEdit;


	CSITEDoc * po_Document = GetDocument();

	if ( m_Domain == DOMAIN_PARAMETER ) {

		while ( bFirstFlag ) {

			bFirstFlag = AfxExtractSubString(szfirstSubText, o_HttpParameter, fidx, '&');
			if ( bFirstFlag == false ) break;
			
			while ( bSecondFlag ) {
				bSecondFlag = AfxExtractSubString(szSecondSubText, szfirstSubText, sidx, '=');
				if ( bSecondFlag == false ) break;
				m_InjectionDataCtrl.AddString(szSecondSubText);
				break;
			}
			fidx++;
		}
		
	}
	else if ( m_Domain == DOMAIN_HEADER ) {
			
		while ( bFirstFlag ) {

			bFirstFlag = AfxExtractSubString(szfirstSubText, m_HttpHeaderEdit, fidx, '\n');
			if ( bFirstFlag == false ) break;
			
			while ( bSecondFlag ) {
				bSecondFlag = AfxExtractSubString(szSecondSubText, szfirstSubText, sidx, ':');
				if ( bSecondFlag == false ) break;
				m_InjectionDataCtrl.AddString(szSecondSubText);
				break;
			}
			fidx++;
		}

	}

	return true;
}

void CSetupView::OnCbnSelchangeHeaderparmcombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	SetDomain();
	m_InjectionDataCtrl.ResetContent();
	if ( m_GetPost == HTTP_GET) SetHttpGETParameter();
	SetInjectionDataCombo();

}


void CSetupView::initialize_DOC_variables(void)
{
	UpdateData(FALSE);
	CSITEDoc * po_Document			= GetDocument();

	po_Document->m_GetPost			= m_GetPost;
	po_Document->m_DBMSApp			= m_DBMSApp;
	po_Document->m_Domain			= m_Domain;
	po_Document->m_dwDBrange		= m_dwDBrange;
	po_Document->m_VarType			= m_VarType;
	po_Document->m_HttpHeader		= m_HttpHeaderEdit;
	if (m_GetPost == HTTP_GET) po_Document->m_HttpGetParameter	= m_HttpGetParameter;
	if (m_GetPost == HTTP_POST) po_Document->m_HttpParameter	= m_HttpParameterEdit;
	po_Document->m_HttpUrlAddress	= m_HttpUrlAddress;
	po_Document->m_InjectionData	= m_InjectionData;

}

void CSetupView::getHttpServerName(CString url, char * szServerName, DWORD size)
{
	CString o_ResultString;
	int idx = 0;
	int loop_idx = 0;	
	CString tmp;

	o_ResultString = url.Tokenize(":// /", idx);

	while ( o_ResultString != "" && loop_idx < 1 ) {
		loop_idx++;
		o_ResultString = url.Tokenize(":// /", idx);
	}
	
	strncpy(szServerName,o_ResultString, size); 

	return;
}


void CSetupView::OnCbnSelchangeHttpmethodcombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	SetHttpMethod();
	return ;
}

bool CSetupView::SetHttpGETParameter(void)
{
	DWORD	fidx = 0;
	DWORD	sidx = 0;
	BOOL	bFlag = true;
	CString o_SubText;
	CString o_HttpURL;

	o_HttpURL = m_HttpUrlAddress;

	CSITEDoc * po_Document = GetDocument();

	while ( fidx < 2 ) {

		bFlag = AfxExtractSubString(o_SubText, m_HttpUrlAddress, fidx, '?');

		if ( bFlag == false && fidx == 1) {
			AfxMessageBox((LPCTSTR)"URL 경로에 ? 구분자가 없습니다.",  MB_ICONERROR);
			return false;
		} else if ( bFlag == false)  break;

		fidx++;
	}

	m_HttpGetParameter = o_SubText;

	return true;
}
