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
#ifndef __SETUPVIEW_H__
#define __SETUPVIEW_H__

#pragma once

// CSetupView 폼 뷰입니다.
#include "SITEDoc.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "SITEHeader.h"
#include "ProcessControl.h"
class CSITEDoc;
class CHttpHeaderEdit;
class CSetupView : public CFormView
{
	DECLARE_DYNCREATE(CSetupView)

public:
	CSetupView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CSetupView();

// 특성입니다.
public:
	CButton m_CookieChkCtrl;
	CSITEDoc* GetDocument();
	CString m_HttpUrlAddress;
	CString m_HttpHeaderEdit;
	CString m_HttpParameterEdit;
	CString m_HeaderParameterCombo;
	CString m_ParameterType;
	CString m_InjectionData;


public:
	enum { IDD = IDD_SETUPVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	void SetHttpHeaderEdit();

public:
	afx_msg void OnBnClickedRdoMssql();
	afx_msg void OnBnClickedRdoOracle();
	afx_msg void OnBnClickedRdoMysql();

public:
	bool AppStartProcess(void);
	bool ValidateCheck(void);
	CEdit m_HttpUrlAddressCtrl;
	CComboBox m_InjectionDataCtrl;
	CComboBox m_HeaderParameterComboCtrl;
	CComboBox m_ParameterTypeCtrl;
	bool SetDomain(void);
	bool SetVariableType(void);
	afx_msg void OnEnSetfocusParametereditor();
	CEdit m_HttpHeaderEditCtrl;
	CEdit m_HttpParameterEditCtrl;
	afx_msg void OnEnSetfocusHeadereditor();
	bool SetInjectionDataCombo(void);
	afx_msg void OnCbnSelchangeHeaderparmcombo();
	void getHttpServerName(CString url, char * szServerName, DWORD size);
	
public:
	bool SetDBrange(void);
public:
	CString m_szDBRangeType;
public:
	void initialize_DOC_variables(void);
	DWORD m_GetPost;
	DWORD m_DBMSApp;
	DWORD m_Domain;
	DWORD m_dwDBrange;
public:
	DWORD m_VarType;
public:
	CString m_HttpMethodCombo;
public:
	bool SetHttpMethod(void);
public:
	afx_msg void OnCbnSelchangeHttpmethodcombo();
public:
	CComboBox m_HttpMethodComboCtrl;
public:
	bool SetHttpGETParameter(void);
public:
	CString m_HttpGetParameter;
};


#endif