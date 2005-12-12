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
		CTRL(IDC_DEF_PROFILE,		&m_cbProfiles   )
		CTRL(IDC_SCAN_ON_START,		&m_ckScanStart  )
		CTRL(IDC_SCAN_ON_SWITCH,	&m_ckScanSwitch )
		CTRL(IDC_SCAN_TMP_FILES,	&m_ckScanForTmp )
		CTRL(IDC_SCAN_INDEX,		&m_ckScanIndex  )
		CTRL(IDC_SHOW_ALL,			&m_ckShowAll    )
		CTRL(IDC_LOG_EDITS,			&m_ckLogEdits   )
		CTRL(IDC_IGNORE_DATES,		&m_ckIgnoreDates)
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
	for (uint i = 0; i < App.m_aoProfiles.size(); ++i)
	{
		CProfile* pProfile = App.m_aoProfiles[i];

		int n = m_cbProfiles.Add(pProfile->m_strName);
		m_cbProfiles.ItemPtr(n, pProfile);
	}

	// Select the current default.
	int nDefProfile = m_cbProfiles.FindExact(App.m_strDefProfile, -1);

	ASSERT(nDefProfile != CB_ERR);

	m_cbProfiles.CurSel(nDefProfile);

	// Initialise scanning defaults.
	m_ckScanStart.Check(App.m_bScanOnStart);
	m_ckScanSwitch.Check(App.m_bScanOnSwitch);
	m_ckScanForTmp.Check(App.m_bScanForTmp);
	m_ckScanIndex.Check(App.m_bScanIndex);
	m_ckShowAll.Check(App.m_bShowAllFiles);
	m_ckLogEdits.Check(App.m_bLogEdits);
	m_ckIgnoreDates.Check(App.m_bIgnoreDates);
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
	App.m_bLogEdits     = m_ckLogEdits.IsChecked();
	App.m_bIgnoreDates  = m_ckIgnoreDates.IsChecked();

	// Mark settings as modified.
	App.m_nModified |= App.SETTINGS;

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
