/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		UTCONFIGDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CUTConfigDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "UTConfigDlg.hpp"
#include "UTCMGRApp.hpp"
#include "HelpTopics.h"

/******************************************************************************
**
** Local variables.
**
*******************************************************************************
*/

static const tchar* CFG_SECTION      = TXT("Core.System");
static const tchar* FOLDER_CFG_ENTRY = TXT("CachePath");
static const tchar* EXPIRY_CFG_ENTRY = TXT("PurgeCacheDays");

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
	DEFINE_CTRL_TABLE
		CTRL(IDC_FOLDER,	&m_ebFolder)
		CTRL(IDC_EXPIRY,	&m_ebExpiry)
	END_CTRL_TABLE
}

/******************************************************************************
** Method:		OnInitDialog()
**
** Description:	Initialise the dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CUTConfigDlg::OnInitDialog()
{
	// Load the current config.
	m_ebFolder.Text(m_oIniFile.ReadString(CFG_SECTION, FOLDER_CFG_ENTRY, TXT("")));
	m_ebExpiry.Text(m_oIniFile.ReadString(CFG_SECTION, EXPIRY_CFG_ENTRY, TXT("")));

	// Initialise control string lengths.
	m_ebFolder.TextLimit(MAX_PATH);
	m_ebExpiry.TextLimit(4);
}

/******************************************************************************
** Method:		OnOk()
**
** Description:	User pressed OK.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CUTConfigDlg::OnOk()
{
	// Save changes.
	m_oIniFile.WriteString(CFG_SECTION, FOLDER_CFG_ENTRY, m_ebFolder.Text());
	m_oIniFile.WriteString(CFG_SECTION, EXPIRY_CFG_ENTRY, m_ebExpiry.Text());

	return true;
}

/******************************************************************************
** Method:		OnHelp()
**
** Description:	Help requested for the dialog.
**
** Parameters:	See HELPINFO.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CUTConfigDlg::OnHelp(HELPINFO& /*oInfo*/)
{
	// Show the dialogs help topic.
	App.m_oHelpFile.Topic(IDH_UTCFGDLG);
}
