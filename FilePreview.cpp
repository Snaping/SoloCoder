#include "stdafx.h"
#include "FileExplorer.h"
#include "FilePreview.h"

IMPLEMENT_DYNAMIC(CFilePreview, CWnd)

BEGIN_MESSAGE_MAP(CFilePreview, CWnd)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

CFilePreview::CFilePreview() noexcept
    : m_ePreviewType(PT_NONE)
    , m_gdiplusToken(0)
{
    GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);
}

CFilePreview::~CFilePreview()
{
    if (m_gdiplusToken != 0)
    {
        GdiplusShutdown(m_gdiplusToken);
    }
}

void CFilePreview::PreviewFile(LPCTSTR lpszFilePath)
{
    if (m_strCurrentFile == lpszFilePath)
        return;

    ClearPreview();
    m_strCurrentFile = lpszFilePath;
    m_ePreviewType = DeterminePreviewType(lpszFilePath);

    switch (m_ePreviewType)
    {
    case PT_TEXT:
        LoadTextFile(lpszFilePath);
        break;
    case PT_IMAGE:
        LoadImageFile(lpszFilePath);
        break;
    default:
        break;
    }

    Invalidate();
}

void CFilePreview::ClearPreview()
{
    m_ePreviewType = PT_NONE;
    m_strCurrentFile.Empty();
    m_strTextContent.Empty();
    if (!m_Image.IsNull())
    {
        m_Image.Destroy();
    }
}

CFilePreview::PreviewType CFilePreview::DeterminePreviewType(LPCTSTR lpszFilePath)
{
    CString strExt = GetFileExtension(lpszFilePath);
    if (strExt.IsEmpty())
        return PT_UNSUPPORTED;

    if (IsTextFile(strExt))
        return PT_TEXT;
    if (IsImageFile(strExt))
        return PT_IMAGE;

    return PT_UNSUPPORTED;
}

CString CFilePreview::GetFileExtension(LPCTSTR lpszFilePath)
{
    CString strPath(lpszFilePath);
    int nDot = strPath.ReverseFind(_T('.'));
    if (nDot == -1)
        return _T("");

    CString strExt = strPath.Mid(nDot + 1);
    strExt.MakeLower();
    return strExt;
}

BOOL CFilePreview::IsTextFile(LPCTSTR lpszExt)
{
    CString strExt(lpszExt);
    return (strExt == _T("txt") || strExt == _T("cpp") || strExt == _T("h") || 
            strExt == _T("c") || strExt == _T("hpp") || strExt == _T("java") ||
            strExt == _T("py") || strExt == _T("cs") || strExt == _T("js") ||
            strExt == _T("html") || strExt == _T("htm") || strExt == _T("css") ||
            strExt == _T("xml") || strExt == _T("json") || strExt == _T("ini") ||
            strExt == _T("log") || strExt == _T("bat") || strExt == _T("cmd"));
}

BOOL CFilePreview::IsImageFile(LPCTSTR lpszExt)
{
    CString strExt(lpszExt);
    return (strExt == _T("bmp") || strExt == _T("jpg") || strExt == _T("jpeg") || 
            strExt == _T("png") || strExt == _T("gif") || strExt == _T("tiff") ||
            strExt == _T("ico"));
}

BOOL CFilePreview::LoadTextFile(LPCTSTR lpszFilePath)
{
    m_strTextContent.Empty();

    CStdioFile file;
    if (!file.Open(lpszFilePath, CFile::modeRead | CFile::typeText))
        return FALSE;

    CString strLine;
    int nLines = 0;
    const int MAX_LINES = 100;

    while (file.ReadString(strLine) && nLines < MAX_LINES)
    {
        m_strTextContent += strLine + _T("\n");
        nLines++;
    }

    if (nLines >= MAX_LINES)
    {
        m_strTextContent += _T("... (文件内容过多，仅显示前100行)");
    }

    file.Close();
    return TRUE;
}

BOOL CFilePreview::LoadImageFile(LPCTSTR lpszFilePath)
{
    if (!m_Image.IsNull())
    {
        m_Image.Destroy();
    }

    HRESULT hr = m_Image.Load(lpszFilePath);
    return SUCCEEDED(hr);
}

void CFilePreview::OnPaint()
{
    CPaintDC dc(this);
    CRect rect;
    GetClientRect(&rect);

    dc.FillSolidRect(&rect, RGB(255, 255, 255));

    switch (m_ePreviewType)
    {
    case PT_TEXT:
        DrawTextPreview(&dc);
        break;
    case PT_IMAGE:
        DrawImagePreview(&dc);
        break;
    case PT_UNSUPPORTED:
        DrawUnsupportedPreview(&dc);
        break;
    case PT_NONE:
    default:
        break;
    }
}

void CFilePreview::DrawTextPreview(CDC* pDC)
{
    CRect rect;
    GetClientRect(&rect);

    if (!m_font.GetSafeHandle())
    {
        m_font.CreatePointFont(90, _T("Consolas"));
    }

    CFont* pOldFont = pDC->SelectObject(&m_font);
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(RGB(0, 0, 0));

    CRect textRect = rect;
    textRect.DeflateRect(10, 10);

    pDC->DrawText(m_strTextContent, &textRect, DT_LEFT | DT_TOP | DT_EXPANDTABS | DT_NOCLIP);

    pDC->SelectObject(pOldFont);
}

void CFilePreview::DrawImagePreview(CDC* pDC)
{
    if (m_Image.IsNull())
        return;

    CRect rect;
    GetClientRect(&rect);

    int nImgWidth = m_Image.GetWidth();
    int nImgHeight = m_Image.GetHeight();

    if (nImgWidth <= 0 || nImgHeight <= 0)
        return;

    double dRatioImg = (double)nImgWidth / (double)nImgHeight;
    double dRatioRect = (double)rect.Width() / (double)rect.Height();

    int nDrawWidth, nDrawHeight;
    if (dRatioImg > dRatioRect)
    {
        nDrawWidth = rect.Width() - 20;
        nDrawHeight = (int)(nDrawWidth / dRatioImg);
    }
    else
    {
        nDrawHeight = rect.Height() - 20;
        nDrawWidth = (int)(nDrawHeight * dRatioImg);
    }

    int nX = (rect.Width() - nDrawWidth) / 2;
    int nY = (rect.Height() - nDrawHeight) / 2;

    m_Image.Draw(pDC->m_hDC, nX, nY, nDrawWidth, nDrawHeight);
}

void CFilePreview::DrawUnsupportedPreview(CDC* pDC)
{
    CRect rect;
    GetClientRect(&rect);

    CString strMsg;
    strMsg.Format(_T("不支持预览此类型的文件\n\n文件: %s"), m_strCurrentFile);

    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(RGB(128, 128, 128));

    CRect textRect = rect;
    textRect.DeflateRect(20, 20);

    pDC->DrawText(strMsg, &textRect, DT_CENTER | DT_VCENTER | DT_WORDBREAK);
}

BOOL CFilePreview::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}
