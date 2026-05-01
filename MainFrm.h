#pragma once

class CFileTreeCtrl;
class CFileListCtrl;
class CFilePreview;

class CMainFrame : public CMDIFrameWnd
{
    DECLARE_DYNAMIC(CMainFrame)

public:
    CMainFrame() noexcept;

public:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
    virtual ~CMainFrame();

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    CStatusBar  m_wndStatusBar;
    CToolBar    m_wndToolBar;
    CSplitterWnd m_wndSplitter;

    CFileTreeCtrl* m_pFileTreeCtrl;
    CFileListCtrl* m_pFileListCtrl;
    CFilePreview* m_pFilePreview;

    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnViewToolbar();
    afx_msg void OnUpdateViewToolbar(CCmdUI* pCmdUI);
    afx_msg void OnViewStatusBar();
    afx_msg void OnUpdateViewStatusBar(CCmdUI* pCmdUI);
    afx_msg void OnFileBrowseFolder();
    afx_msg void OnViewPreview();
    afx_msg void OnUpdateViewPreview(CCmdUI* pCmdUI);

    DECLARE_MESSAGE_MAP()

public:
    void OnTreeItemChanged(HTREEITEM hItem);
    void OnListItemChanged(int nItem);
    CFileTreeCtrl* GetFileTreeCtrl() const { return m_pFileTreeCtrl; }
    CFileListCtrl* GetFileListCtrl() const { return m_pFileListCtrl; }
    CFilePreview* GetFilePreview() const { return m_pFilePreview; }
};
