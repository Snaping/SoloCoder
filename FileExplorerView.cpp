#include "stdafx.h"
#include "FileExplorer.h"
#include "FileExplorerDoc.h"
#include "FileExplorerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CFileExplorerView, CView)

BEGIN_MESSAGE_MAP(CFileExplorerView, CView)
    ON_WM_LBUTTONDOWN()
    ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

CFileExplorerView::CFileExplorerView() noexcept
{
}

CFileExplorerView::~CFileExplorerView()
{
}

BOOL CFileExplorerView::PreCreateWindow(CREATESTRUCT& cs)
{
    return CView::PreCreateWindow(cs);
}

void CFileExplorerView::OnDraw(CDC* pDC)
{
    CFileExplorerDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    CRect rect;
    GetClientRect(&rect);
    pDC->FillSolidRect(&rect, RGB(240, 240, 240));

    CString strText = _T("文件资源管理器");
    pDC->SetTextColor(RGB(128, 128, 128));
    pDC->SetBkMode(TRANSPARENT);

    CRect textRect = rect;
    pDC->DrawText(strText, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

#ifdef _DEBUG
void CFileExplorerView::AssertValid() const
{
    CView::AssertValid();
}

void CFileExplorerView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

CFileExplorerDoc* CFileExplorerView::GetDocument() const
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFileExplorerDoc)));
    return (CFileExplorerDoc*)m_pDocument;
}
#endif

void CFileExplorerView::OnLButtonDown(UINT nFlags, CPoint point)
{
    CView::OnLButtonDown(nFlags, point);
}
