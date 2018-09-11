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
#ifndef __LOGDATA_H__
#define __LOGDATA_H__

#pragma once

// CLogDataView ���Դϴ�.
#include "SITEDoc.h"
#include "SITEHeader.h"
#include "MssqlCode.h"

class CSITEDoc;
class CLogDataView : public CListView
{
	DECLARE_DYNCREATE(CLogDataView)

public:
	CLogDataView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CLogDataView();

// Ư���Դϴ�.
public:
	CSITEDoc* GetDocument();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	virtual void OnInitialUpdate();
public:
	bool InsertLogDataItem(void);
	int CLogDataView::AddItemToListCtrl(char *pszItem, int nItem, int nSubItem);
};

#endif
