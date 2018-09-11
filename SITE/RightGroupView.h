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
#ifndef __RIGHTGROUPVIEW_H__
#define __RIGHTGROUPVIEW_H__

#pragma once

#include "LogDataView.h"
#include "SetupView.h"
#include "SplitterWndEx.h"
#include "SITEHeader.h"

// CRightGroupView ���Դϴ�.
class CSITEDoc;
class CSetupView;
class CLogDataView;
class CRightGroupView : public CView
{
	DECLARE_DYNCREATE(CRightGroupView)

protected:
	CRightGroupView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CRightGroupView();

public:
	CSplitterWndEx m_wndSplitter;
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	CSetupView* GetRightTopView(void);
public:
	CLogDataView * GetRightBottomView(void);
};
#endif