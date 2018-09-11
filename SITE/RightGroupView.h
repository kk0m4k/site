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
#ifndef __RIGHTGROUPVIEW_H__
#define __RIGHTGROUPVIEW_H__

#pragma once

#include "LogDataView.h"
#include "SetupView.h"
#include "SplitterWndEx.h"
#include "SITEHeader.h"

// CRightGroupView 뷰입니다.
class CSITEDoc;
class CSetupView;
class CLogDataView;
class CRightGroupView : public CView
{
	DECLARE_DYNCREATE(CRightGroupView)

protected:
	CRightGroupView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CRightGroupView();

public:
	CSplitterWndEx m_wndSplitter;
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
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