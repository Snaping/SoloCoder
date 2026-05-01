#include "stdafx.h"
#include "FileExplorer.h"
#include "MainFrm.h"
#include "FileTreeCtrl.h"
#include "FileListCtrl.h"
#include "FilePreview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
    ON_WM_CREATE()
    ON_COMMAND(ID_VIEW_TOOLBAR, &CMainFrame::OnViewToolbar)
    ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR, &CMainFrame::OnUpdateViewToolbar)
    ON_COMMAND(ID_VIEW_STATUS_BAR, &CMainFrame::OnViewStatusBar)
    ON_UPDATE_COMMAND_UI(ID_VIEW_STATUS_BAR, &CMainFrame::OnUpdateViewStatusBar)
    ON_COMMAND(ID_FILE_BROWSE_FOLDER, &CMainFrame::OnFileBrowseFolder)
    ON_COMMAND(ID_VIEW_PREVIEW, &CMainFrame::OnViewPreview)
    ON_UPDATE_COMMAND_UI(ID_VIEW_PREVIEW, &CMainFrame::OnUpdateViewPreview)
END_MESSAGE_MAP()

static UINT indicators[] =
{
    ID_SEPARATOR,
    ID_INDICATOR_CAPS,
    ID_INDICATOR_NUM,
    ID_INDICATOR_SCRL,
};

CMainFrame::CMainFrame() noexcept
    : m_pFileTreeCtrl(nullptr)
    , m_pFileListCtrl(nullptr)
    , m_pFilePreview(nullptr)
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    if (!m_wndToolBar.CreateEx(this, TBSTYLE_TRANSPARENT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
        !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
    {
        TRACE0("未能创建工具栏\n");
        return -1;
    }

    if (!m_wndStatusBar.Create(this))
    {
        TRACE0("未能创建状态栏\n");
        return -1;
    }
    m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

    if (!m_wndSplitter.CreateStatic(this, 1, 3))
    {
        TRACE0("未能创建拆分窗口\n");
        return -1;
    }

    if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CFileTreeCtrl), CSize(200, 0), NULL))
    {
        TRACE0("未能创建文件树控件\n");
        return -1;
    }
    m_pFileTreeCtrl = (CFileTreeCtrl*)m_wndSplitter.GetPane(0, 0);

    if (!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CFileListCtrl), CSize(400, 0), NULL))
    {
        TRACE0("未能创建文件列表控件\n");
        return -1;
    }
    m_pFileListCtrl = (CFileListCtrl*)m_wndSplitter.GetPane(0, 1);

    if (!m_wndSplitter.CreateView(0, 2, RUNTIME_CLASS(CFilePreview), CSize(300, 0), NULL))
    {
        TRACE0("未能创建预览窗口\n");
        return -1;
    }
    m_pFilePreview = (CFilePreview*)m_wndSplitter.GetPane(0, 2);

    m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
    EnableDocking(CBRS_ALIGN_ANY);
    DockControlBar(&m_wndToolBar);

    m_pFileTreeCtrl->InitializeTree();

    return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    if( !CMDIFrameWnd::PreCreateWindow(cs) )
        return FALSE;

    return TRUE;
}

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
    CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
    CMDIFrameWnd::Dump(dc);
}
#endif

void CMainFrame::OnViewToolbar()
{
    ShowControlBar(&m_wndToolBar, !m_wndToolBar.IsWindowVisible(), FALSE);
    RecalcLayout();
}

void CMainFrame::OnUpdateViewToolbar(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_wndToolBar.IsWindowVisible());
}

void CMainFrame::OnViewStatusBar()
{
    ShowControlBar(&m_wndStatusBar, !m_wndStatusBar.IsWindowVisible(), FALSE);
    RecalcLayout();
}

void CMainFrame::OnUpdateViewStatusBar(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_wndStatusBar.IsWindowVisible());
}

void CMainFrame::OnFileBrowseFolder()
{
    if (m_pFileTreeCtrl)
    {
        m_pFileTreeCtrl->BrowseFolder();
    }
}

void CMainFrame::OnViewPreview()
{
    if (m_pFilePreview)
    {
        m_pFilePreview->ShowWindow(m_pFilePreview->IsWindowVisible() ? SW_HIDE : SW_SHOW);
        RecalcLayout();
    }
}

void CMainFrame::OnUpdateViewPreview(CCmdUI* pCmdUI)
{
    if (m_pFilePreview)
    {
        pCmdUI->SetCheck(m_pFilePreview->IsWindowVisible());
    }
}

void CMainFrame::OnTreeItemChanged(HTREEITEM hItem)
{
    if (m_pFileListCtrl && hItem)
    {
        CString strPath = m_pFileTreeCtrl->GetItemPath(hItem);
        if (!strPath.IsEmpty())
        {
            m_pFileListCtrl->DisplayFolder(strPath);
        }
    }
}

void CMainFrame::OnListItemChanged(int nItem)
{
    if (m_pFilePreview && nItem >= 0)
    {
        CString strFilePath = m_pFileListCtrl->GetItemFilePath(nItem);
        if (!strFilePath.IsEmpty())
        {
            m_pFilePreview->PreviewFile(strFilePath);
        }
    }
}
