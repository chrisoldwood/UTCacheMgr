/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPWND.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CAppWnd class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "AppWnd.hpp"
#include "UTCMGRApp.hpp"

/******************************************************************************
** Method:		Default constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CAppWnd::CAppWnd()
	: CDlgFrame(IDR_APPICON, m_AppDlg, false)
{
}

/******************************************************************************
** Method:		Destructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CAppWnd::~CAppWnd()
{
}

/******************************************************************************
** Method:		OnCreate()
**
** Description:	Creates the toolbar and status bars.
**
** Parameters:	rcClient	The client rectangle.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppWnd::OnCreate(const CRect& rcClient)
{
	//
	// Create and attach the components.
	//
	m_Accel.LoadRsc(IDR_APPACCEL);
	Accel(&m_Accel);

	m_Menu.LoadRsc(IDR_APPMENU);
	Menu(&m_Menu);

	m_ToolBar.Create(*this, IDC_TOOL_BAR, rcClient);
	ToolBar(&m_ToolBar);

	m_StatusBar.Create(*this, IDC_STATUS_BAR, rcClient);
	StatusBar(&m_StatusBar);

	m_AppDlg.RunModeless(*this);
	m_AppDlg.m_bShowAllFiles = App.m_bShowAllFiles;

	UpdateTitle();

	// Call base class.
	CDlgFrame::OnCreate(rcClient);
}

/******************************************************************************
** Method:		OnFocus()
**
** Description:	Divert the focus to the active view window.
**				
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppWnd::OnFocus()
{
	App.m_AppWnd.m_AppDlg.m_lvGrid.Focus();
}

/******************************************************************************
** Method:		OnQueryClose()
**
** Description:	Check if the app can close.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CAppWnd::OnQueryClose()
{
	// Save windows final position.
	App.m_rcLastWndPos = Placement();

	// Close help file.
	App.m_oHelpFile.Close();

	return true;
}

/******************************************************************************
** Method:		ProcessMsg()
**
** Description:	Show default status bar text if moving over main window/dialog
**				or its children.
**
** Parameters:	rMsg	The message.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CAppWnd::ProcessMsg(MSG& rMsg)
{
	// Only interested in mouse moves over the main window/dialog
	if ( (rMsg.message == WM_MOUSEMOVE)
	  && ((rMsg.hwnd == Handle()) || (rMsg.hwnd == m_AppDlg.Handle())
	   || (::GetParent(rMsg.hwnd) == m_AppDlg.Handle())) )
	{
		// Show default status message.
		m_StatusBar.Hint(App.m_strDefStatus);
	}

	// Forward to base class.
	return CDlgFrame::ProcessMsg(rMsg);
}

/******************************************************************************
** Method:		UpdateTitle()
**
** Description:	Updates the title bar by using the connection and query file
**				strings.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppWnd::UpdateTitle()
{
	ASSERT(App.m_pProfile != NULL);

	// Start with app name.
	CString strTitle = App.m_strTitle;

	// Append profile
	strTitle += TXT(" [");
	strTitle += App.m_pProfile->m_strName;
	strTitle += TXT("]");

	Title(strTitle);
}
