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
**
** Local variables.
**
*******************************************************************************
*/

static const char* CFG_SECTION      = "Core.System";
static const char* FOLDER_CFG_ENTRY = "CachePath";
static const char* EXPIRY_CFG_ENTRY = "PurgeCacheDays";

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
	m_ebFolder.Text(m_oIniFile.ReadString(CFG_SECTION, FOLDER_CFG_ENTRY, ""));
	m_ebExpiry.Text(m_oIniFile.ReadString(CFG_SECTION, EXPIRY_CFG_ENTRY, ""));

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
