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
#ifndef __SETUPVIEW_H__
#define __SETUPVIEW_H__

#pragma once

// CSetupView �� ���Դϴ�.
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
	CSetupView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CSetupView();

// Ư���Դϴ�.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
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