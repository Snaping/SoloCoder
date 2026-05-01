#include "stdafx.h"
#include "FileExplorer.h"
#include "FileListCtrl.h"
#include "MainFrm.h"
#include <shellapi.h>

IMPLEMENT_DYNCREATE(CFileListCtrl, CListView)

BEGIN_MESSAGE_MAP(CFileListCtrl, CListView)
    ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, &CFileListCtrl::OnItemChanged)
    ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

CFileListCtrl::CFileListCtrl() noexcept
    : m_bInitialized(FALSE)
{
}

CFileListCtrl::~CFileListCtrl()
{
}

BOOL CFileListCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.style |= LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL;
    return CListView::PreCreateWindow(cs);
}

void CFileListCtrl::InitializeList()
{
    if (m_bInitialized)
        return;

    CListCtrl& listCtrl = GetListCtrl();

    m_ImageList.Create(16, 16, ILC_COLOR32 | ILC_MASK, 10, 10);
    listCtrl.SetImageList(&m_ImageList, LVSIL_SMALL);

    listCtrl.InsertColumn(0, _T("名称"), LVCFMT_LEFT, 200);
    listCtrl.InsertColumn(1, _T("大小"), LVCFMT_RIGHT, 100);
    listCtrl.InsertColumn(2, _T("类型"), LVCFMT_LEFT, 100);
    listCtrl.InsertColumn(3, _T("修改日期"), LVCFMT_LEFT, 150);

    m_bInitialized = TRUE;
}

void CFileListCtrl::DisplayFolder(LPCTSTR lpszFolder)
{
    if (m_strCurrentFolder == lpszFolder)
        return;

    m_strCurrentFolder = lpszFolder;
    CListCtrl& listCtrl = GetListCtrl();

    if (!m_bInitialized)
    {
        InitializeList();
    }

    listCtrl.DeleteAllItems();
    m_ItemPathMap.RemoveAll();

    CFileFind finder;
    CString strFind = CString(lpszFolder) + _T("\\*.*");
    BOOL bWorking = finder.FindFile(strFind);

    int nItem = 0;

    while (bWorking)
    {
        bWorking = finder.FindNextFile();

        if (finder.IsDots())
            continue;

        CString strFileName = finder.GetFileName();
        CString strFullPath = finder.GetFilePath();
        CString strFileType;
        CString strFileSize;
        CString strFileTime;
        int nIconIndex = 0;

        if (finder.IsDirectory())
        {
            strFileType = _T("文件夹");
            strFileSize = _T("");
            nIconIndex = 0;
        }
        else
        {
            strFileType = _T("文件");
            strFileSize = GetFileSizeString(finder.GetLength());
            nIconIndex = GetFileIconIndex(strFullPath);
        }

        CTime time;
        finder.GetLastWriteTime(time);
        strFileTime = GetFileTimeString(time);

        listCtrl.InsertItem(nItem, strFileName, nIconIndex);
        listCtrl.SetItemText(nItem, 1, strFileSize);
        listCtrl.SetItemText(nItem, 2, strFileType);
        listCtrl.SetItemText(nItem, 3, strFileTime);

        m_ItemPathMap.SetAt(nItem, strFullPath);
        nItem++;
    }
    finder.Close();

    listCtrl.SortItems([](LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) -> int {
        CFileListCtrl* pThis = (CFileListCtrl*)lParamSort;
        CListCtrl& listCtrl = pThis->GetListCtrl();

        CString strName1 = listCtrl.GetItemText((int)lParam1, 0);
        CString strName2 = listCtrl.GetItemText((int)lParam2, 0);

        CString strPath1, strPath2;
        pThis->m_ItemPathMap.Lookup((int)lParam1, strPath1);
        pThis->m_ItemPathMap.Lookup((int)lParam2, strPath2);

        BOOL bIsDir1 = (GetFileAttributes(strPath1) & FILE_ATTRIBUTE_DIRECTORY) != 0;
        BOOL bIsDir2 = (GetFileAttributes(strPath2) & FILE_ATTRIBUTE_DIRECTORY) != 0;

        if (bIsDir1 && !bIsDir2)
            return -1;
        else if (!bIsDir1 && bIsDir2)
            return 1;
        else
            return strName1.CompareNoCase(strName2);
    }, (LPARAM)this);
}

int CFileListCtrl::GetFileIconIndex(LPCTSTR lpszFilePath)
{
    SHFILEINFO sfi = { 0 };
    if (SHGetFileInfo(lpszFilePath, 0, &sfi, sizeof(sfi), SHGFI_ICON | SHGFI_SMALLICON))
    {
        int nIndex = m_ImageList.Add(sfi.hIcon);
        DestroyIcon(sfi.hIcon);
        return nIndex;
    }
    return 0;
}

CString CFileListCtrl::GetFileSizeString(ULONGLONG nSize)
{
    CString strSize;
    if (nSize < 1024)
        strSize.Format(_T("%llu 字节"), nSize);
    else if (nSize < 1024 * 1024)
        strSize.Format(_T("%.1f KB"), (double)nSize / 1024.0);
    else if (nSize < 1024 * 1024 * 1024)
        strSize.Format(_T("%.1f MB"), (double)nSize / (1024.0 * 1024.0));
    else
        strSize.Format(_T("%.1f GB"), (double)nSize / (1024.0 * 1024.0 * 1024.0));
    return strSize;
}

CString CFileListCtrl::GetFileTimeString(CTime time)
{
    return time.Format(_T("%Y-%m-%d %H:%M"));
}

void CFileListCtrl::OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    if (pNMListView->uChanged & LVIF_STATE)
    {
        if (pNMListView->uNewState & LVIS_SELECTED)
        {
            CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
            if (pMainFrame)
            {
                pMainFrame->OnListItemChanged(pNMListView->iItem);
            }
        }
    }

    *pResult = 0;
}

void CFileListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    CListCtrl& listCtrl = GetListCtrl();
    LVHITTESTINFO hitinfo = { 0 };
    hitinfo.pt = point;
    listCtrl.HitTest(&hitinfo);

    if (hitinfo.iItem >= 0)
    {
        CString strFilePath;
        if (m_ItemPathMap.Lookup(hitinfo.iItem, strFilePath))
        {
            DWORD dwAttr = GetFileAttributes(strFilePath);
            if (dwAttr != INVALID_FILE_ATTRIBUTES && (dwAttr & FILE_ATTRIBUTE_DIRECTORY))
            {
                DisplayFolder(strFilePath);
            }
            else
            {
                OpenFile(strFilePath);
            }
        }
    }

    CListView::OnLButtonDblClk(nFlags, point);
}

void CFileListCtrl::OpenFile(LPCTSTR lpszFilePath)
{
    ShellExecute(NULL, _T("open"), lpszFilePath, NULL, NULL, SW_SHOWNORMAL);
}

CString CFileListCtrl::GetItemFilePath(int nItem)
{
    CString strPath;
    if (m_ItemPathMap.Lookup(nItem, strPath))
    {
        return strPath;
    }
    return _T("");
}
