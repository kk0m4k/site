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
// SITEView.cpp : CSITEView 클래스의 구현
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
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTreeView::OnFilePrintPreview)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CSITEView::OnTvnSelchanged)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CSITEView::OnNMDblclk)
//	ON_NOTIFY_REFLECT(NM_CLICK, &CSITEView::OnNMClick)
END_MESSAGE_MAP()

// CSITEView 생성/소멸

CSITEView::CSITEView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CSITEView::~CSITEView()
{
}

BOOL CSITEView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.
	cs.style = cs.style | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_HASLINES;
	
	return CTreeView::PreCreateWindow(cs);
}


// CSITEView 진단

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
CSITEDoc* CSITEView::GetDocument()  // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSITEDoc)));
	return (CSITEDoc*)m_pDocument;
}


void CSITEView::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void CSITEView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
		case 1: // 데이터베이스 트리항목을 클릭하면 테이블 리스트를 구함
			nMSGresult = AfxMessageBox(TVIEW_DBCLK_TABLE_MSG, MB_YESNO|MB_ICONQUESTION);
			if ( nMSGresult != IDYES ) break;

			memset(CSITEView::g_szDBname, '\0', sizeof(CSITEView::g_szDBname));
			strcpy(CSITEView::g_szDBname, ItemText);
			GetTableList(&hItem);
			break;
		case 2: // 테이블 트리 항목을 클릭하면 필드 리스트를 구함
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
		case 3: // 필드 항목을 클릭하면 데이터 리스트를 구함
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
