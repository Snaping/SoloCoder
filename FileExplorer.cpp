#include "stdafx.h"
#include "FileExplorer.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "FileExplorerDoc.h"
#include "FileExplorerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CFileExplorerApp, CWinApp)
    ON_COMMAND(ID_APP_ABOUT, &CFileExplorerApp::OnAppAbout)
    ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
    ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
    ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

CFileExplorerApp::CFileExplorerApp() noexcept
{
}

CFileExplorerApp theApp;

BOOL CFileExplorerApp::InitInstance()
{
    CWinApp::InitInstance();

    SetRegistryKey(_T("本地应用程序向导生成的应用程序"));
    LoadStdProfileSettings(4);

    CMultiDocTemplate* pDocTemplate;
    pDocTemplate = new CMultiDocTemplate(IDR_FileExplorerTYPE,
        RUNTIME_CLASS(CFileExplorerDoc),
        RUNTIME_CLASS(CChildFrame),
        RUNTIME_CLASS(CFileExplorerView));
    if (!pDocTemplate)
        return FALSE;
    AddDocTemplate(pDocTemplate);

    CMainFrame* pMainFrame = new CMainFrame;
    if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
    {
        delete pMainFrame;
        return FALSE;
    }
    m_pMainWnd = pMainFrame;

    if (m_lpCmdLine[0] != _T('\0'))
    {
    }

    pMainFrame->ShowWindow(m_nCmdShow);
    pMainFrame->UpdateWindow();

    return TRUE;
}

int CFileExplorerApp::ExitInstance()
{
    return CWinApp::ExitInstance();
}

class CAboutDlg : public CDialog
{
public:
    CAboutDlg() noexcept;

    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialog(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

void CFileExplorerApp::OnAppAbout()
{
    CAboutDlg aboutDlg;
    aboutDlg.DoModal();
}
