#pragma once

class CFileExplorerView : public CView
{
protected:
    CFileExplorerView() noexcept;
    DECLARE_DYNCREATE(CFileExplorerView)

public:
    CFileExplorerDoc* GetDocument() const;

public:
    virtual void OnDraw(CDC* pDC);
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
    virtual ~CFileExplorerView();

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG
inline CFileExplorerDoc* CFileExplorerView::GetDocument() const
   { return reinterpret_cast<CFileExplorerDoc*>(m_pDocument); }
#endif
