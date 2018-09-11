#ifndef __SITEHEADER_H__
#define __SITEHEADER_H__

#define	HTTP_GET			1000
#define	HTTP_POST			1001
#define	HTTP_NOTYET			1002

#define	DBMS_MSSQL			2000
#define	DBMS_ORACLE			2001
#define	DBMS_MYSQL			2002
#define	DBMS_NOTYET			2003

#define VARTYPE_STRING		3000
#define	VARTYPE_INTEGER		3001

#define	GET_DBMS			4000
#define	GET_TABLE			4001
#define GET_FIELD			4002
#define	GET_DATA			4003

#define	DOMAIN_PARAMETER	5000
#define	DOMAIN_HEADER		5001
#define	DOMAIN_NOTYET		5002

#define	CATEGORY_COUNT		9000
#define	CATEGORY_DATA		9001

#define	DBRANGE_CURRENT		6000
#define	DBRANGE_ALL			6001

#define	NUMOFHTTPHEADER			10
#define	HTTPHEADERAGENT			"User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1;)\r\n"
#define	HTTPHEADERLANGUAGE		"Accept-Language: ko\r\n"
#define	HTTPHEADERCONTENTTYPE	"Content-Type: application/x-www-form-urlencoded\r\n"
#define	HTTPHEADERACCEPT		"Accept: */*\r\n"
#define	HTTPHEADERCOOKIE		"Cookie: \r\n"
#define	HTTPHEADERREFERER		"Referer: \r\n"

#define	TVIEW_DBCLK_TABLE_MSG		"테이블 리스트를 목록화 합니다"
#define	TVIEW_DBCLK_FIELD_MSG		"필드 리스트를 목록화 합니다"
#define	TVIEW_DBCLK_DATAVALUE_MSG	"데이터 값을 목록화 합니다"

#define	DATACOUNT_MSG				"%d 건의 데이터가 저장되어 있습니다."
#define	TOOMUCHDATA_MSG				"저장되어 있는 데이터가 총 %d 건으로, 너무 많습니다. 100건의 데이터만 갖어 옵니다."
#define	LIMITDATACOUNT				100	

#define	HTTPREADBUFFER				1024
#define	MSSQL_ERR_DELIMITER			"'^"

#define	APPSTART_MSG				"SITE 툴은 웹 어플리케이션의 SQL Injection 취약점을 진단하는 목적으로\r\n\
제작된 툴 입니다.\r\n\r\n\
이 툴은 SQL Injection 취약점을 이용하여 DBMS에 존재하는 데이터베이스,\r\n\
테이블,필드, 그리고 데이터 정보를 획득할 수 있는 기능이 구현되어 있습\r\n\
니다.\r\n\r\n\
반드시 허가 받은 사이트에 대한 보안 진단 용으로 사용해야 합니다.\r\n\
악의적인 용도로 사용할 경우 사용자가 법적인 책임을 져야 합니다.\r\n\r\n\
------------------------ 동의 합니까----------------------------\r\n\r\n\
[contact : 김태훈(Kim, Tae Hoon) kimfrancesco@gmail.com]"
									 
									 

struct _tagHTTPHEADER {
	short	idxhttpheader;
	char	szhttpheader[1024];	
};

#endif