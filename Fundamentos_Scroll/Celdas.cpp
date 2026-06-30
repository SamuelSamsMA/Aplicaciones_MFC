#include <afxwin.h>
#include "Celdas.h"


CApp app;

BOOL CApp::InitInstance()
{
	m_pMainWnd = new CMainWindow;
	m_pMainWnd->ShowWindow(m_nCmdShow);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}


BEGIN_MESSAGE_MAP(CMainWindow, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_PAINT()
END_MESSAGE_MAP()


CMainWindow::CMainWindow()
{
	Create(NULL, _T("Celdas"), WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL);
}

int CMainWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CClientDC dc(this);

	// LOGPIXELSX: Número de pixeles por pulgada lógica horizontalmente
	// LOGPIXELSY: Número de pixeles por pulgada lógica verticalmente
	cellWidth = dc.GetDeviceCaps(LOGPIXELSX);
	cellHeight = dc.GetDeviceCaps(LOGPIXELSY) / 4;
	ribbonWidth = cellWidth / 2;
	viewWidth = (26 * cellWidth) + ribbonWidth;
	viewHeight = cellHeight * 100;
	return 0;
}

void CMainWindow::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	//
	// Establece los parámetros de scroll horizontal
	//
	int hScrollMax = 0;
	hScrollPos = hPageSize = 0;

	if (cx < viewWidth) {
		hScrollMax = viewWidth - 1;
		hPageSize = cx;
		hScrollPos = min(hScrollPos, viewWidth - hPageSize - 1);
	}

	SCROLLINFO si;
	si.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
	si.nMin = 0;
	si.nMax = hScrollMax;
	si.nPos = hScrollPos;
	si.nPage = hPageSize;

	SetScrollInfo(SB_HORZ, &si, TRUE);

	TRACE(_T("hScrollMax: %d\n"), hScrollMax);
	TRACE(_T("hScrollPos: %d\n"), hScrollPos);
	TRACE(_T("hPageSize: %d\n"), hPageSize);

	//
	// Establece los parámetros de scroll vertical
	//
	int vScrollMax = 0;
	vScrollPos = vPageSize = 0;

	if (cy < viewHeight) {
		vScrollMax = viewHeight - 1;
		vPageSize = cy;
		vScrollPos = min(vScrollPos, viewHeight - vPageSize - 1);
	}

	si.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
	si.nMin = 0;
	si.nMax = vScrollMax;
	si.nPos = vScrollPos;
	si.nPage = vPageSize;

	SetScrollInfo(SB_VERT, &si, TRUE);
}

void CMainWindow::OnHScroll(UINT nCode, UINT nPos, CScrollBar* pScrollBar)
{
	int delta;

	switch (nCode) {
	case SB_LINELEFT:
		delta = -LINESIZE;
		break;

	case SB_PAGELEFT:
		delta = -hPageSize;
		break;

	case SB_THUMBTRACK:
		delta = (int)nPos - hScrollPos;
		break;

	case SB_PAGERIGHT:
		delta = hPageSize;
		break;

	case SB_LINERIGHT:
		delta = LINESIZE;
		break;

	default:
		return;
	}

	int scrollPos = hScrollPos + delta;
	int maxPos = viewWidth - hPageSize;

	// Por ejemplo: Si hScrollPos = 10 y delta = -100, entonces scrollPos = -90 ¡Imposible!
	// Entonces limita a delta al mismo valor que hScrollPos pero con sigo opuesto, es decir: -10
	// así, cuando se actualize el hScrollPos se tendrá: 10 + (-10) = 0 ¡OK!
	// Algo similar hace para limitar en la posición máxima
	if (scrollPos < 0)
		delta = -hScrollPos;
	else if (scrollPos > maxPos)
		delta = maxPos - hScrollPos;
	
	if (delta != 0) {
		hScrollPos += delta;
		SetScrollPos(SB_HORZ, hScrollPos, TRUE);
		ScrollWindow(-delta, 0);
	}
}

void CMainWindow::OnVScroll(UINT nCode, UINT nPos, CScrollBar* pScrollBar)
{
	int delta;

	switch (nCode) {
	case SB_LINEUP:
		delta = -LINESIZE;
		break;

	case SB_PAGEUP:
		delta = -vPageSize;
		break;

	case SB_THUMBTRACK:
		delta = (int)nPos - vScrollPos;
		break;

	case SB_PAGEDOWN:
		delta = vPageSize;
		break;

	case SB_LINEDOWN:
		delta = LINESIZE;
		break;

	default:
		return;
	}

	int scrollPos = vScrollPos + delta;
	int maxPos = viewHeight - vPageSize;

	if (scrollPos < 0)
		delta = -vScrollPos;
	else if (scrollPos > maxPos)
		delta = maxPos - vScrollPos;

	if (delta != 0) {
		vScrollPos += delta;
		SetScrollPos(SB_VERT, vScrollPos, TRUE);
		ScrollWindow(0, -delta);
	}
}

void CMainWindow::OnPaint()
{
	CPaintDC dc(this);

	// Cambia el origen de la pantalla de acuerdo a la posición actual del scroll
	dc.SetWindowOrg(hScrollPos, vScrollPos);

	// Dibuja la cuadrícula
	CPen pen(PS_SOLID, 0, RGB(192, 192, 192));
	CPen* oldPen = dc.SelectObject(&pen);

	for (int i = 0; i < 99; i++) {
		int y = (i * cellHeight) + cellHeight;
		dc.MoveTo(0, y);
		dc.LineTo(viewWidth, y);
	}

	for (int i = 0; i < 26; i++) {
		int x = (i * cellWidth) + ribbonWidth;
		dc.MoveTo(x, 0);
		dc.LineTo(x, viewHeight);
	}

	dc.SelectObject(oldPen);

	// Dibuja la fila y columna principales
	CBrush brush;
	brush.CreateStockObject(LTGRAY_BRUSH);

	CRect rcTop(0, 0, viewWidth, cellHeight);
	dc.FillRect(rcTop, &brush);
	CRect rcLeft(0, 0, ribbonWidth, viewHeight);
	dc.FillRect(rcLeft, &brush);

	dc.MoveTo(0, cellHeight);
	dc.LineTo(viewWidth, cellHeight);
	dc.MoveTo(ribbonWidth, 0);
	dc.LineTo(ribbonWidth, viewHeight);

	dc.SetBkMode(TRANSPARENT);

	// Agrega los números y las líneas divisorias para las filas
	for (int i = 0; i < 99; i++) {
		int y = (i * cellHeight) + cellHeight;
		dc.MoveTo(0, y);
		dc.LineTo(ribbonWidth, y);

		CString str;
		str.Format(_T("%d"), i + 1);

		CRect rect(0, y, ribbonWidth, y + cellHeight);
		dc.DrawText(str, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		rect.top++;
		dc.Draw3dRect(rect, RGB(255, 255, 255), RGB(128, 128, 128));
	}

	// Agrega las letras y las líneas divisorias para las columnas
	for (int i = 0; i < 26; i++) {
		int x = (i * cellWidth) + ribbonWidth;
		dc.MoveTo(x, 0);
		dc.LineTo(x, cellHeight);

		CString str;
		str.Format(_T("%c"), i + 'A');

		CRect rect(x, 0, x + cellWidth, cellHeight);
		dc.DrawText(str, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		rect.left++;
		dc.Draw3dRect(rect, RGB(255, 255, 255), RGB(128, 128, 128));
	}
}