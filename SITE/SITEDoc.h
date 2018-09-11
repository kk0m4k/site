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
// SITEDoc.h : CSITEDoc Ŭ������ �������̽�
//
#ifndef __SITEDOC_H__
#define __SITEDOC_H__


#pragma once
#include "SITE.h"
#include "MainFrm.h"
#include "SITEHeader.h"
#include <shlwapi.h>
#include <afxinet.h>

class CMainFrame;
class CSITEDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CSITEDoc();
	DECLARE_DYNCREATE(CSITEDoc)

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);


// �����Դϴ�.
public:
	virtual ~CSITEDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()

public:
	CMainFrame* GetMainFrame(void);
	DWORD m_DBMSApp;
	DWORD m_GetPost;
	DWORD m_VarType;
	DWORD m_Domain;
	DWORD m_dwDBrange;
	DWORD m_InjectionCategory;

	CString m_HttpUrlAddress;
	CString m_HttpParameter;
	CString m_HttpGetParameter;
	CString m_InjectionData;
	CString m_HttpHeader;

	CInternetSession *m_pHTTPSession;
	CHttpConnection * m_pServer;
	CHttpFile * m_pFile;

public:
	void HttpClose(void);
};

#endif