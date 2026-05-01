#pragma once

class CFileTreeCtrl : public CTreeView
{
    DECLARE_DYNCREATE(CFileTreeCtrl)

protected:
    CFileTreeCtrl() noexcept;

protected:
    CImageList m_ImageList;
    CMap<HTREEITEM, HTREEITEM, CString, CString&> m_ItemPathMap;

public:
    virtual ~CFileTreeCtrl();
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

    void InitializeTree();
    void BrowseFolder();
    CString GetItemPath(HTREEITEM hItem);

protected:
    afx_msg void OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnExpanding(NMHDR* pNMHDR, LRESULT* pResult);
    DECLARE_MESSAGE_MAP()

private:
    HTREEITEM AddDriveItem(LPCTSTR lpszDrive);
    HTREEITEM AddFolderItem(HTREEITEM hParent, LPCTSTR lpszFolder);
    void ExpandFolder(HTREEITEM hParent);
    BOOL HasSubfolders(LPCTSTR lpszPath);
    int GetFolderIconIndex(LPCTSTR lpszPath);
    void GetDrives();
};
