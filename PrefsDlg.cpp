/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PREFSDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CPrefsDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "PrefsDlg.hpp"
#include "HelpTopics.h"

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

CPrefsDlg::CPrefsDlg()
	: CDialog(IDD_PREFS)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_DEF_PROFILE,		&m_cbProfiles  )
		CTRL(IDC_SCAN_ON_START,		&m_ckScanStart )
		CTRL(IDC_SCAN_ON_SWITCH,	&m_ckScanSwitch)
		CTRL(IDC_SCAN_TMP_FILES,	&m_ckScanForTmp)
		CTRL(IDC_SCAN_INDEX,		&m_ckScanIndex )
		CTRL(IDC_SHOW_ALL,			&m_ckShowAll   )
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

void CPrefsDlg::OnInitDialog()
{
	// Load the default profile combo.
	for (int i = 0; i < App.m_aoProfiles.Size(); ++i)
	{
		CProfile* pProfile = App.m_aoProfiles[i];

		int n = m_cbProfiles.Add(pProfile->m_strName);
		m_cbProfiles.ItemPtr(n, pProfile);
	}

	// Select the current default.
	int nDefProfile = m_cbProfiles.FindExact(App.m_strDefProfile, -1);

	if (nDefProfile == CB_ERR)
		nDefProfile = m_cbProfiles.FindExact(CProfile::DEF_PROFILE_NAME, -1);

	ASSERT(nDefProfile != CB_ERR);

	m_cbProfiles.CurSel(nDefProfile);

	// Initialise scanning defaults.
	m_ckScanStart.Check(App.m_bScanOnStart);
	m_ckScanSwitch.Check(App.m_bScanOnSwitch);
	m_ckScanForTmp.Check(App.m_bScanForTmp);
	m_ckScanIndex.Check(App.m_bScanIndex);
	m_ckShowAll.Check(App.m_bShowAllFiles);
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

bool CPrefsDlg::OnOk()
{
	// Get the default profile.
	int       nSel     = m_cbProfiles.CurSel();
	CProfile* pProfile = (CProfile*) m_cbProfiles.ItemPtr(nSel);

	ASSERT((nSel != CB_ERR) && (pProfile != NULL));

	App.m_strDefProfile = pProfile->m_strName;

	// Get the scanning defaults.
	App.m_bScanOnStart  = m_ckScanStart.IsChecked();
	App.m_bScanOnSwitch = m_ckScanSwitch.IsChecked();
	App.m_bScanForTmp   = m_ckScanForTmp.IsChecked();
	App.m_bScanIndex    = m_ckScanIndex.IsChecked();
	App.m_bShowAllFiles = m_ckShowAll.IsChecked();

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

void CPrefsDlg::OnHelp(HELPINFO& /*oInfo*/)
{
	// Show the dialogs help topic.
	App.m_oHelpFile.Topic(IDH_PREFSDLG);
}
