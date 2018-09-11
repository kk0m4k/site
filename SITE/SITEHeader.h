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

#define	TVIEW_DBCLK_TABLE_MSG		"���̺� ����Ʈ�� ���ȭ �մϴ�"
#define	TVIEW_DBCLK_FIELD_MSG		"�ʵ� ����Ʈ�� ���ȭ �մϴ�"
#define	TVIEW_DBCLK_DATAVALUE_MSG	"������ ���� ���ȭ �մϴ�"

#define	DATACOUNT_MSG				"%d ���� �����Ͱ� ����Ǿ� �ֽ��ϴ�."
#define	TOOMUCHDATA_MSG				"����Ǿ� �ִ� �����Ͱ� �� %d ������, �ʹ� �����ϴ�. 100���� �����͸� ���� �ɴϴ�."
#define	LIMITDATACOUNT				100	

#define	HTTPREADBUFFER				1024
#define	MSSQL_ERR_DELIMITER			"'^"

#define	APPSTART_MSG				"SITE ���� �� ���ø����̼��� SQL Injection ������� �����ϴ� ��������\r\n\
���۵� �� �Դϴ�.\r\n\r\n\
�� ���� SQL Injection ������� �̿��Ͽ� DBMS�� �����ϴ� �����ͺ��̽�,\r\n\
���̺�,�ʵ�, �׸��� ������ ������ ȹ���� �� �ִ� ����� �����Ǿ� �ֽ�\r\n\
�ϴ�.\r\n\r\n\
�ݵ�� �㰡 ���� ����Ʈ�� ���� ���� ���� ������ ����ؾ� �մϴ�.\r\n\
�������� �뵵�� ����� ��� ����ڰ� ������ å���� ���� �մϴ�.\r\n\r\n\
------------------------ ���� �մϱ�----------------------------\r\n\r\n\
[contact : ������(Kim, Tae Hoon) kimfrancesco@gmail.com]"
									 
									 

struct _tagHTTPHEADER {
	short	idxhttpheader;
	char	szhttpheader[1024];	
};

#endif