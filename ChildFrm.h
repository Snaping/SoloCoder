#pragma once

class CChildFrame : public CMDIChildWnd
{
    DECLARE_DYNCREATE(CChildFrame)

public:
    CChildFrame() noexcept;

protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    DECLARE_MESSAGE_MAP()

public:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};
