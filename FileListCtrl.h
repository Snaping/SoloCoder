#pragma once

class CFileListCtrl : public CListView
{
    DECLARE_DYNCREATE(CFileListCtrl)

protected:
    CFileListCtrl() noexcept;

protected:
    CImageList m_ImageList;
    CMap<int, int, CString, CString&> m_ItemPathMap;
    CString m_strCurrentFolder;
    BOOL m_bInitialized;

public:
    virtual ~CFileListCtrl();
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

    void DisplayFolder(LPCTSTR lpszFolder);
    CString GetItemFilePath(int nItem);
    CString GetCurrentFolder() const { return m_strCurrentFolder; }

protected:
    afx_msg void OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    DECLARE_MESSAGE_MAP()

private:
    void InitializeList();
    int GetFileIconIndex(LPCTSTR lpszFilePath);
    CString GetFileSizeString(ULONGLONG nSize);
    CString GetFileTimeString(CTime time);
    void OpenFile(LPCTSTR lpszFilePath);
};
