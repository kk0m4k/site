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
// LogDataView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SITE.h"
#include "LogDataView.h"


// CLogDataView

IMPLEMENT_DYNCREATE(CLogDataView, CListView)

CLogDataView::CLogDataView()
{

}

CLogDataView::~CLogDataView()
{
}

BEGIN_MESSAGE_MAP(CLogDataView, CListView)
END_MESSAGE_MAP()


// CLogDataView 진단입니다.

#ifdef _DEBUG
void CLogDataView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CLogDataView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLogDataView 메시지 처리기입니다.

BOOL CLogDataView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	cs.style|=LVS_REPORT|LVS_SINGLESEL|LVS_SHOWSELALWAYS|LVS_EDITLABELS;
	return CListView::PreCreateWindow(cs);
}

void CLogDataView::OnInitialUpdate()
{
	DWORD dwStyle;
	CListView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CListCtrl &list = GetListCtrl();
	list.InsertColumn(0, _T("번호"), LVCFMT_CENTER,40);
	list.InsertColumn(1, _T("시  간"), LVCFMT_CENTER,120);
	list.InsertColumn(2, _T("로 그 데 이 터"), LVCFMT_LEFT,310);

	dwStyle = list.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	list.SetExtendedStyle(dwStyle);
	
	UpdateData(FALSE);

	return ;
}

bool CLogDataView::InsertLogDataItem(void)
{	
	char szNum[8] = {0};
	CListCtrl &list = GetListCtrl();
	DWORD Itemidx = list.GetItemCount();
	
	wsprintf(szNum, "%d", Itemidx);
	
	AddItemToListCtrl(szNum, Itemidx, 0);
	AddItemToListCtrl(CMssqlCode::g_szLogTime, Itemidx, 1);
	AddItemToListCtrl(CMssqlCode::g_szLogBuffer, Itemidx, 2);

	return true;
}

int CLogDataView::AddItemToListCtrl(char *pszItem, int nItem, int nSubItem)
{
	// 상태창에 아이템 입력
	
	LVITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = nItem;
	lvItem.iSubItem = nSubItem;
	lvItem.pszText = (LPTSTR)pszItem;

	if (nSubItem == 0) {
		return GetListCtrl().InsertItem(&lvItem);
	}
	else {
		return GetListCtrl().SetItem(&lvItem);
	}
}
