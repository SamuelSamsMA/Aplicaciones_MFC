#include <afxwin.h>
#include "App.h"


// Único objeto global que crea la aplicación.
CApp app;


/*
* Función utilizada para inicializar los recursos necesarios cuando
* arranca la aplicación.
* 
* En este caso, inicia la ventana principal y la muestra en pantalla.
* 
* @return
* - TRUE: Inicialización correcta y continua la aplicación.
* @return
* - FALSE: Error al iniciar y cierra la aplicación.
*/
BOOL CApp::InitInstance()
{
	m_pMainWnd = new CMainWindow;
	m_pMainWnd->ShowWindow(m_nCmdShow);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}


/*
* Mapa de Mensajes
* 
* Vincula los mensajes del sistema con las funciones que los procesan
*/
BEGIN_MESSAGE_MAP(CMainWindow, CFrameWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()


/*
* Constructor de la clase CMainWindow
* 
* Crea la ventana principal con los valores por defecto
*/
CMainWindow::CMainWindow()
{
	Create(NULL, _T("Iniciando MFC"));
}


/*
* Procesador del mensaje WM_PAINT
* 
* Dibuja la frase "Microsoft Fundation Class" en el centro de la ventana.
*/
void CMainWindow::OnPaint()
{
	CPaintDC dc(this);

	CRect rect;
	GetClientRect(&rect);

	dc.DrawText(_T("Microsoft Fundation Class"), -1, &rect,
		DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}