/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		UTCONFIGDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CUTConfigDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "UTConfigDlg.hpp"

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

CUTConfigDlg::CUTConfigDlg()
	: CDialog(IDD_UT_CONFIG)
	, m_oIniFile(App.m_pProfile->m_strConfigFile)
{
}
