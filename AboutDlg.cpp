/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		ABOUTDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CAboutDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "AboutDlg.hpp"

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

CAboutDlg::CAboutDlg()
	: CDialog(IDD_ABOUT)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_EMAIL,		&m_txtEmail  )
		CTRL(IDC_WEBSITE,	&m_txtWebSite)
	END_CTRL_TABLE

	// Set the URL label protocols.
	m_txtEmail.Protocol("mailto:");
	m_txtWebSite.Protocol("http://");
}
