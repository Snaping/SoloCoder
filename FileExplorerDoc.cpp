#include "stdafx.h"
#include "FileExplorer.h"
#include "FileExplorerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CFileExplorerDoc, CDocument)

BEGIN_MESSAGE_MAP(CFileExplorerDoc, CDocument)
END_MESSAGE_MAP()

CFileExplorerDoc::CFileExplorerDoc() noexcept
{
}

CFileExplorerDoc::~CFileExplorerDoc()
{
}

BOOL CFileExplorerDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    return TRUE;
}

void CFileExplorerDoc::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
    }
    else
    {
    }
}

#ifdef _DEBUG
void CFileExplorerDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void CFileExplorerDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif
