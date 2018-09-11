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
// SITEView.h : CSITEView 클래스의 인터페이스
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
protected: // serialization에서만 만들어집니다.
	CSITEView();
	DECLARE_DYNCREATE(CSITEView)

// 특성입니다.
public:
	CSITEDoc	*GetDocument();
	HTREEITEM hWebsiteCtrl;
	HTREEITEM hDBMSCtrl;
	//HTREEITEM hTableCtrl;
	//HTREEITEM hFieldCtrl;


private:
	CImageList m_ImageList;

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	static	char	g_szDBname[64];
	static	char	g_szTBname[64];
	static	char	g_szFDname[64];

// 구현입니다.
public:
	virtual ~CSITEView();
#ifdef _DEBUG
	virtual void AssertValid();
	virtual void Dump(CDumpContext& dc) ;
#endif

protected:

// 생성된 메시지 맵 함수
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