#pragma once


// CDBMSView ���Դϴ�.
class CDBMSDoc;
class CDBMSView : public CView
{
	DECLARE_DYNCREATE(CDBMSView)

protected:
	CDBMSView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CDBMSView();

public:
	CDBMSDoc * GetDocument();
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


