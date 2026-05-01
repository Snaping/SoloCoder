#pragma once

class CFilePreview : public CWnd
{
    DECLARE_DYNAMIC(CFilePreview)

protected:
    CFilePreview() noexcept;

protected:
    enum PreviewType
    {
        PT_NONE,
        PT_TEXT,
        PT_IMAGE,
        PT_UNSUPPORTED
    };

    PreviewType m_ePreviewType;
    CString m_strCurrentFile;
    CString m_strTextContent;
    CImage m_Image;
    CFont m_font;

    GdiplusStartupInput m_gdiplusStartupInput;
    ULONG_PTR m_gdiplusToken;

public:
    virtual ~CFilePreview();

    void PreviewFile(LPCTSTR lpszFilePath);
    void ClearPreview();

protected:
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    DECLARE_MESSAGE_MAP()

private:
    PreviewType DeterminePreviewType(LPCTSTR lpszFilePath);
    BOOL LoadTextFile(LPCTSTR lpszFilePath);
    BOOL LoadImageFile(LPCTSTR lpszFilePath);
    void DrawTextPreview(CDC* pDC);
    void DrawImagePreview(CDC* pDC);
    void DrawUnsupportedPreview(CDC* pDC);
    CString GetFileExtension(LPCTSTR lpszFilePath);
    BOOL IsTextFile(LPCTSTR lpszExt);
    BOOL IsImageFile(LPCTSTR lpszExt);
};
