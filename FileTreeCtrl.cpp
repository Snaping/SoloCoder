#include "stdafx.h"
#include "FileExplorer.h"
#include "FileTreeCtrl.h"
#include "FileListCtrl.h"
#include "MainFrm.h"
#include <shlobj.h>

IMPLEMENT_DYNCREATE(CFileTreeCtrl, CTreeView)

BEGIN_MESSAGE_MAP(CFileTreeCtrl, CTreeView)
    ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CFileTreeCtrl::OnSelChanged)
    ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, &CFileTreeCtrl::OnExpanding)
END_MESSAGE_MAP()

CFileTreeCtrl::CFileTreeCtrl() noexcept
{
}

CFileTreeCtrl::~CFileTreeCtrl()
{
}

BOOL CFileTreeCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.style |= TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_SHOWSELALWAYS;
    return CTreeView::PreCreateWindow(cs);
}

void CFileTreeCtrl::InitializeTree()
{
    CTreeCtrl& treeCtrl = GetTreeCtrl();

    m_ImageList.Create(16, 16, ILC_COLOR32 | ILC_MASK, 5, 5);
    m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_WINLOGO));
    m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_WINLOGO));
    m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_WINLOGO));
    m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_WINLOGO));
    m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_WINLOGO));

    treeCtrl.SetImageList(&m_ImageList, TVSIL_NORMAL);

    GetDrives();
}

void CFileTreeCtrl::GetDrives()
{
    CTreeCtrl& treeCtrl = GetTreeCtrl();
    treeCtrl.DeleteAllItems();
    m_ItemPathMap.RemoveAll();

    HTREEITEM hDesktop = treeCtrl.InsertItem(_T("我的电脑"), 0, 0);
    CString strEmpty;
    m_ItemPathMap.SetAt(hDesktop, strEmpty);

    DWORD dwDrives = GetLogicalDrives();
    for (int i = 0; i < 26; i++)
    {
        if (dwDrives & (1 << i))
        {
            CString strDrive;
            strDrive.Format(_T("%c:\\"), _T('A') + i);

            UINT uiDriveType = GetDriveType(strDrive);
            if (uiDriveType == DRIVE_FIXED || uiDriveType == DRIVE_REMOVABLE || 
                uiDriveType == DRIVE_CDROM || uiDriveType == DRIVE_RAMDISK)
            {
                CString strDisplay;
                strDisplay.Format(_T("本地磁盘 (%c:)"), _T('A') + i);
                if (uiDriveType == DRIVE_REMOVABLE)
                    strDisplay.Format(_T("可移动磁盘 (%c:)"), _T('A') + i);
                else if (uiDriveType == DRIVE_CDROM)
                    strDisplay.Format(_T("CD驱动器 (%c:)"), _T('A') + i);

                HTREEITEM hDrive = treeCtrl.InsertItem(strDisplay, 1, 1, hDesktop);
                m_ItemPathMap.SetAt(hDrive, strDrive);

                if (HasSubfolders(strDrive))
                {
                    treeCtrl.InsertItem(_T("Temp"), 2, 2, hDrive);
                }
            }
        }
    }

    treeCtrl.Expand(hDesktop, TVE_EXPAND);
}

HTREEITEM CFileTreeCtrl::AddDriveItem(LPCTSTR lpszDrive)
{
    return NULL;
}

HTREEITEM CFileTreeCtrl::AddFolderItem(HTREEITEM hParent, LPCTSTR lpszFolder)
{
    return NULL;
}

void CFileTreeCtrl::ExpandFolder(HTREEITEM hParent)
{
    CTreeCtrl& treeCtrl = GetTreeCtrl();
    CString strParentPath;
    if (!m_ItemPathMap.Lookup(hParent, strParentPath))
        return;

    treeCtrl.DeleteItem(treeCtrl.GetChildItem(hParent));

    CFileFind finder;
    CString strFind = strParentPath + _T("*.*");
    BOOL bWorking = finder.FindFile(strFind);

    while (bWorking)
    {
        bWorking = finder.FindNextFile();

        if (finder.IsDots())
            continue;

        if (finder.IsDirectory())
        {
            CString strFolderName = finder.GetFileName();
            CString strFullPath = finder.GetFilePath();

            HTREEITEM hFolder = treeCtrl.InsertItem(strFolderName, 2, 2, hParent);
            m_ItemPathMap.SetAt(hFolder, strFullPath);

            if (HasSubfolders(strFullPath))
            {
                treeCtrl.InsertItem(_T("Temp"), 3, 3, hFolder);
            }
        }
    }
    finder.Close();
}

BOOL CFileTreeCtrl::HasSubfolders(LPCTSTR lpszPath)
{
    CFileFind finder;
    CString strFind = CString(lpszPath) + _T("*.*");
    BOOL bWorking = finder.FindFile(strFind);

    while (bWorking)
    {
        bWorking = finder.FindNextFile();

        if (finder.IsDots())
            continue;

        if (finder.IsDirectory())
        {
            finder.Close();
            return TRUE;
        }
    }
    finder.Close();
    return FALSE;
}

int CFileTreeCtrl::GetFolderIconIndex(LPCTSTR lpszPath)
{
    return 2;
}

void CFileTreeCtrl::OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
    HTREEITEM hItem = pNMTreeView->itemNew.hItem;

    CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
    if (pMainFrame && hItem)
    {
        pMainFrame->OnTreeItemChanged(hItem);
    }

    *pResult = 0;
}

void CFileTreeCtrl::OnExpanding(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
    HTREEITEM hItem = pNMTreeView->itemNew.hItem;

    if (pNMTreeView->action & TVE_EXPAND)
    {
        CTreeCtrl& treeCtrl = GetTreeCtrl();
        HTREEITEM hChild = treeCtrl.GetChildItem(hItem);
        if (hChild)
        {
            CString strText = treeCtrl.GetItemText(hChild);
            if (strText == _T("Temp"))
            {
                ExpandFolder(hItem);
            }
        }
    }

    *pResult = 0;
}

CString CFileTreeCtrl::GetItemPath(HTREEITEM hItem)
{
    CString strPath;
    if (m_ItemPathMap.Lookup(hItem, strPath))
    {
        return strPath;
    }
    return _T("");
}

void CFileTreeCtrl::BrowseFolder()
{
    BROWSEINFO bi = { 0 };
    bi.lpszTitle = _T("选择文件夹");
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
    if (pidl != NULL)
    {
        TCHAR szPath[MAX_PATH];
        if (SHGetPathFromIDList(pidl, szPath))
        {
            CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
            if (pMainFrame && pMainFrame->GetFileListCtrl())
            {
                pMainFrame->GetFileListCtrl()->DisplayFolder(CString(szPath));
            }
        }
        CoTaskMemFree(pidl);
    }
}
