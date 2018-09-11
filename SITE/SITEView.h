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
// SITEView.h : CSITEView Ŭ������ �������̽�
//
#ifndef __SITEVIEW_H__
#define __SITEVIEW_H__

#pragma once
#include "SITEHeader.h"
#include "ProcessControl.h"
#include "MssqlCode.h"

class CSITEDoc;
//class CProcessControl;
class CSITEView : public CTreeView
{
protected: // serialization������ ��������ϴ�.
	CSITEView();
	DECLARE_DYNCREATE(CSITEView)

// Ư���Դϴ�.
public:
	CSITEDoc	*GetDocument();
	HTREEITEM hWebsiteCtrl;
	HTREEITEM hDBMSCtrl;
	//HTREEITEM hTableCtrl;
	//HTREEITEM hFieldCtrl;


private:
	CImageList m_ImageList;

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	static	char	g_szDBname[64];
	static	char	g_szTBname[64];
	static	char	g_szFDname[64];

// �����Դϴ�.
public:
	virtual ~CSITEView();
#ifdef _DEBUG
	virtual void AssertValid();
	virtual void Dump(CDumpContext& dc) ;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnInitialUpdate();
	bool SetTargetUrlonTree(char * url);
	bool InsertItemonTreeView(char *pszItem, DWORD category_type);
	bool InsertDBMSonTreeView(char *pszItem);
	bool GetTableList(HTREEITEM * hItem);
	bool InsertTableonTreeView(char *pszItem);
	bool GetFieldList(HTREEITEM * hItem);
public:
	bool InsertFieldonTreeView(char * pszItem);
public:
	bool GetDataList(HTREEITEM * hItem);
public:
	bool InsertDataTreeView(char * pszItem);
};


#endif