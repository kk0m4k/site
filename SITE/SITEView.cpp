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
// SITEView.cpp : CSITEView Ŭ������ ����
//

#include "stdafx.h"
#include "SITE.h"
#include "SITEView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


char CSITEView::g_szDBname[64] = {0};
char CSITEView::g_szTBname[64] = {0};
char CSITEView::g_szFDname[64] = {0};

// CSITEView
IMPLEMENT_DYNCREATE(CSITEView, CTreeView)

BEGIN_MESSAGE_MAP(CSITEView, CTreeView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTreeView::OnFilePrintPreview)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CSITEView::OnTvnSelchanged)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CSITEView::OnNMDblclk)
//	ON_NOTIFY_REFLECT(NM_CLICK, &CSITEView::OnNMClick)
END_MESSAGE_MAP()

// CSITEView ����/�Ҹ�

CSITEView::CSITEView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CSITEView::~CSITEView()
{
}

BOOL CSITEView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.
	cs.style = cs.style | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_HASLINES;
	
	return CTreeView::PreCreateWindow(cs);
}


// CSITEView ����

#ifdef _DEBUG
void CSITEView::AssertValid() 
{
	CTreeView::AssertValid();
}

void CSITEView::Dump(CDumpContext& dc) 
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG
CSITEDoc* CSITEView::GetDocument()  // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSITEDoc)));
	return (CSITEDoc*)m_pDocument;
}


void CSITEView::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}

void CSITEView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	m_ImageList.Create( 16, 16, ILC_COLOR32, 0, 4);
	CBitmap bm;
	bm.LoadBitmap(IDB_DBMS_TREE_IMAGE_LIST);
	m_ImageList.Add(&bm, RGB(0, 0, 0));

	CTreeCtrl* pCtrl = &GetTreeCtrl();
	ASSERT(pCtrl != NULL);
	
	pCtrl->SetImageList( &m_ImageList, TVSIL_NORMAL);

	TVINSERTSTRUCT tvInsert;
	tvInsert.hParent = NULL;
	tvInsert.hInsertAfter = NULL;
	tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvInsert.item.pszText = "WEB SITE";
	tvInsert.item.iImage = 0;
	tvInsert.item.iSelectedImage = 0;

	hWebsiteCtrl = pCtrl->InsertItem(&tvInsert);
}


bool CSITEView::SetTargetUrlonTree(char * url)
{
	CTreeCtrl* pCtrl = &GetTreeCtrl();
	ASSERT(pCtrl != NULL);
	pCtrl->DeleteAllItems();

	pCtrl->SetImageList( &m_ImageList, TVSIL_NORMAL);

	TVINSERTSTRUCT tvInsert;
	tvInsert.hParent = NULL;
	tvInsert.hInsertAfter = NULL;
	tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvInsert.item.pszText = url;
	tvInsert.item.iImage = 0;
	tvInsert.item.iSelectedImage = 0;
	
	hWebsiteCtrl = pCtrl->InsertItem(&tvInsert);
		
	return false;
}

bool CSITEView::InsertItemonTreeView(char *pszItem, DWORD category_type)
{

	switch ( category_type ) {
		case GET_DBMS:
			InsertDBMSonTreeView(pszItem);
			break;
		case GET_TABLE:
			InsertTableonTreeView(pszItem);
			break;
		case GET_FIELD:
			InsertFieldonTreeView(pszItem);
			break;
		case GET_DATA:
			InsertDataTreeView(pszItem);
			break;
	}

	return false;
}

bool CSITEView::InsertDBMSonTreeView(char *pszItem)
{

	CTreeCtrl* pCtrl = &GetTreeCtrl();
	ASSERT(pCtrl != NULL);

	pCtrl->SetImageList( &m_ImageList, TVSIL_NORMAL);

	TVINSERTSTRUCT tvInsert;
	tvInsert.hParent = hWebsiteCtrl;
	tvInsert.hInsertAfter = NULL;
	tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvInsert.item.pszText = pszItem;
	tvInsert.item.iImage = 1;
	tvInsert.item.iSelectedImage = 1;
	
	hDBMSCtrl = pCtrl->InsertItem(&tvInsert);

	return true;
}



void CSITEView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	char ItemText[128] = {0};
	char tmpItemText[128] = {0};
	int nMSGresult = 0;
	int Image = 0;
	int SelectedImage = 0;	
	int	&nImage = Image;
	int	&nSelectedImage = SelectedImage;
	

	CTreeCtrl* pCtrl = &GetTreeCtrl();
	ASSERT(pCtrl != NULL);
	


	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	HTREEITEM hItem = pCtrl->GetSelectedItem();
	pCtrl->GetItemImage(hItem, nImage, nSelectedImage);
	
	strcpy(ItemText, pCtrl->GetItemText(hItem));

	switch ( nSelectedImage ) {
		case 0:
			break;
		case 1: // �����ͺ��̽� Ʈ���׸��� Ŭ���ϸ� ���̺� ����Ʈ�� ����
			nMSGresult = AfxMessageBox(TVIEW_DBCLK_TABLE_MSG, MB_YESNO|MB_ICONQUESTION);
			if ( nMSGresult != IDYES ) break;

			memset(CSITEView::g_szDBname, '\0', sizeof(CSITEView::g_szDBname));
			strcpy(CSITEView::g_szDBname, ItemText);
			GetTableList(&hItem);
			break;
		case 2: // ���̺� Ʈ�� �׸��� Ŭ���ϸ� �ʵ� ����Ʈ�� ����
			{
				HTREEITEM hDatabaseItem;
				nMSGresult = AfxMessageBox(TVIEW_DBCLK_FIELD_MSG, MB_YESNO|MB_ICONQUESTION);
				if ( nMSGresult != IDYES ) break;

				hDatabaseItem = pCtrl->GetParentItem(hItem);
				memset(CSITEView::g_szDBname, '\0', sizeof(CSITEView::g_szDBname));
				memset(CSITEView::g_szTBname, '\0', sizeof(CSITEView::g_szTBname));
				strcpy(CSITEView::g_szDBname, pCtrl->GetItemText(hDatabaseItem));
				strcpy(CSITEView::g_szTBname, ItemText);
				GetFieldList(&hItem);			
			}
			break;
		case 3: // �ʵ� �׸��� Ŭ���ϸ� ������ ����Ʈ�� ����
			{
				HTREEITEM hTableItem;
				HTREEITEM hDatabaseItem;
		
				nMSGresult = AfxMessageBox(TVIEW_DBCLK_DATAVALUE_MSG, MB_YESNO|MB_ICONQUESTION);
				if ( nMSGresult != IDYES ) break;
		
				hTableItem = pCtrl->GetParentItem(hItem);
				hDatabaseItem = pCtrl->GetParentItem(hTableItem);
				
				memset(CSITEView::g_szDBname, '\0', sizeof(CSITEView::g_szDBname));
				memset(CSITEView::g_szTBname, '\0', sizeof(CSITEView::g_szTBname));

				strcpy(CSITEView::g_szDBname, pCtrl->GetItemText(hDatabaseItem));
				strcpy(CSITEView::g_szTBname, pCtrl->GetItemText(hTableItem));

				memset(CSITEView::g_szFDname, '\0', sizeof(CSITEView::g_szFDname));
				strcpy(CSITEView::g_szFDname, ItemText);
				GetDataList(&hItem);
			}
			break;
	}

	*pResult = 0;
}

bool CSITEView::GetTableList(HTREEITEM * hItem)
{
	DWORD idx = 0;
	DWORD loop_count = 0;
	CProcessControl objProcessCtrl;
	DWORD	category_type = GET_TABLE;

	DWORD	category_flag = CATEGORY_COUNT;
	objProcessCtrl.StartSQLInjectionAudit(category_type, category_flag);

	
	category_flag = CATEGORY_DATA;

	loop_count = CMssqlCode::m_dwTBcount;

	for (idx =1 ; idx <= loop_count; idx++) {

		objProcessCtrl.StartSQLInjectionAudit(category_type, category_flag, idx);
	}

	return true;
}

bool CSITEView::GetFieldList(HTREEITEM * hItem)
{
	DWORD idx = 0;
	DWORD loop_count = 0;
	CProcessControl objProcessCtrl;
	DWORD	category_type = GET_FIELD;

	DWORD	category_flag = CATEGORY_COUNT;
	objProcessCtrl.StartSQLInjectionAudit(category_type,  category_flag);

	
	category_flag = CATEGORY_DATA;

	loop_count = CMssqlCode::m_dwFDcount;

	for (idx =1 ; idx <= loop_count; idx++) {
		objProcessCtrl.StartSQLInjectionAudit(category_type, category_flag, idx);
	}

	return true;

}

bool CSITEView::GetDataList(HTREEITEM * hItem)
{
	DWORD idx = 0;
	DWORD loop_count = 0;
	CProcessControl objProcessCtrl;
	DWORD	category_type = GET_DATA;

	DWORD	category_flag = CATEGORY_COUNT;
	objProcessCtrl.StartSQLInjectionAudit(category_type,  category_flag);

	
	category_flag = CATEGORY_DATA;

	loop_count = CMssqlCode::m_dwDTcount;

	for (idx =1 ; idx <= loop_count; idx++) {
		objProcessCtrl.StartSQLInjectionAudit(category_type, category_flag, idx);
	}

	return true;
}


bool CSITEView::InsertTableonTreeView(char *pszItem)
{
	CTreeCtrl* pCtrl = &GetTreeCtrl();
	ASSERT(pCtrl != NULL);

	HTREEITEM hItem =pCtrl->GetSelectedItem();
	pCtrl->SetImageList( &m_ImageList, TVSIL_NORMAL);

	TVINSERTSTRUCT tvInsert;
	tvInsert.hParent = hItem;
	tvInsert.hInsertAfter = NULL;
	tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvInsert.item.pszText = pszItem;
	tvInsert.item.iImage = 2;
	tvInsert.item.iSelectedImage = 2;
	
	pCtrl->InsertItem(&tvInsert);

	return true;
}

bool CSITEView::InsertFieldonTreeView(char * pszItem)
{
	CTreeCtrl* pCtrl = &GetTreeCtrl();
	ASSERT(pCtrl != NULL);

	HTREEITEM hItem =pCtrl->GetSelectedItem();
	pCtrl->SetImageList( &m_ImageList, TVSIL_NORMAL);

	TVINSERTSTRUCT tvInsert;
	tvInsert.hParent = hItem;
	tvInsert.hInsertAfter = NULL;
	tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvInsert.item.pszText = pszItem;
	tvInsert.item.iImage = 3;
	tvInsert.item.iSelectedImage = 3;
	
	pCtrl->InsertItem(&tvInsert);
	return true;
}


bool CSITEView::InsertDataTreeView(char * pszItem)
{
	CTreeCtrl* pCtrl = &GetTreeCtrl();
	ASSERT(pCtrl != NULL);

	HTREEITEM hItem =pCtrl->GetSelectedItem();
	pCtrl->SetImageList( &m_ImageList, TVSIL_NORMAL);

	TVINSERTSTRUCT tvInsert;
	tvInsert.hParent = hItem;
	tvInsert.hInsertAfter = NULL;
	tvInsert.item.mask =  TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvInsert.item.pszText = pszItem;
	tvInsert.item.iImage = 4;
	tvInsert.item.iSelectedImage = 4;
	
	pCtrl->InsertItem(&tvInsert);
	return true;
}
