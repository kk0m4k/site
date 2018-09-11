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
// LogDataView.cpp : ���� �����Դϴ�.
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


// CLogDataView �����Դϴ�.

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


// CLogDataView �޽��� ó�����Դϴ�.

BOOL CLogDataView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	cs.style|=LVS_REPORT|LVS_SINGLESEL|LVS_SHOWSELALWAYS|LVS_EDITLABELS;
	return CListView::PreCreateWindow(cs);
}

void CLogDataView::OnInitialUpdate()
{
	DWORD dwStyle;
	CListView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	CListCtrl &list = GetListCtrl();
	list.InsertColumn(0, _T("��ȣ"), LVCFMT_CENTER,40);
	list.InsertColumn(1, _T("��  ��"), LVCFMT_CENTER,120);
	list.InsertColumn(2, _T("�� �� �� �� ��"), LVCFMT_LEFT,310);

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
	// ����â�� ������ �Է�
	
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
