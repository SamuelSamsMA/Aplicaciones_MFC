#pragma once

//
// Clase de la aplicación
//
class CApp : public CWinApp
{
private:
	virtual BOOL InitInstance();
};


//
// Clase de la ventana principal de la aplicación
//
class CMainWindow : public CFrameWnd
{
public:
	CMainWindow();

	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP();
};
