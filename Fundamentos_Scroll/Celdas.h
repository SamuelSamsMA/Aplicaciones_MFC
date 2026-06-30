#pragma once

#define LINESIZE	8

class CApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
};


class CMainWindow : public CFrameWnd
{
public:
	CMainWindow();

protected:
	int cellWidth;		// Ancho de una celda
	int cellHeight;		// Alto de una celda
	int ribbonWidth;	// Ancho de la primer columna
	int viewWidth;		// Ancho del espacio de impresión
	int viewHeight;		// Alto del espacio de impresión
	int hScrollPos;		// Posición horizontal del scroll
	int vScrollPos;		// Posición vertical del scroll
	int hPageSize;		// Tamaño horizontal de la página
	int vPageSize;		// Tamaño vertical de la página

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP();
};