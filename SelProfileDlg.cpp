/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SELPROFILEDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CSelProfileDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "SelProfileDlg.hpp"
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

CSelProfileDlg::CSelProfileDlg()
	: CDialog(IDD_SEL_PROFILE)
	, m_pProfiles(NULL)
	, m_pChoice(NULL)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_PROFILES,		&m_cbProfiles  )
		CTRL(IDC_FORMAT,		&m_cbFormat    )
		CTRL(IDC_CACHE_DIR,		&m_ebCacheDir  )
		CTRL(IDC_READ_ONLY,		&m_ckReadOnly  )
		CTRL(IDC_SYSTEM_DIR,	&m_ebSystemDir )
		CTRL(IDC_MAPS_DIR,		&m_ebMapDir    )
		CTRL(IDC_TEXTURES_DIR,	&m_ebTextureDir)
		CTRL(IDC_SOUNDS_DIR,	&m_ebSoundDir  )
		CTRL(IDC_MUSIC_DIR,		&m_ebMusicDir  )
		CTRL(IDC_MESH_DIR,		&m_ebMeshDir   )
		CTRL(IDC_ANIM_DIR,		&m_ebAnimDir   )
		CTRL(IDC_KARMA_DIR,		&m_ebKarmaDir  )
		CTRL(IDC_CONFIG_FILE,	&m_ebConfigFile)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_PROFILES, CBN_SELCHANGE, OnSelectProfile)
	END_CTRLMSG_TABLE
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

void CSelProfileDlg::OnInitDialog()
{
	ASSERT(m_pProfiles != NULL);
	ASSERT(m_pChoice   != NULL);

	// Load the names combo.
	for (int i = 0; i < m_pProfiles->Size(); ++i)
	{
		CProfile* pProfile = m_pProfiles->At(i);

		int n = m_cbProfiles.Add(pProfile->m_strName);
		m_cbProfiles.ItemPtr(n, pProfile);
	}

	// Load formats combo.
	for (int i = 0; i < CProfile::NUM_FORMATS; ++i)
		m_cbFormat.Add(CProfile::s_pszFormats[i]);

	// Select the default.
	m_cbProfiles.CurSel(m_cbProfiles.FindExact(m_pChoice->m_strName, -1));
	OnSelectProfile();
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

bool CSelProfileDlg::OnOk()
{
	// Get the selection.
	int       nSel     = m_cbProfiles.CurSel();
	CProfile* pProfile = (CProfile*) m_cbProfiles.ItemPtr(nSel);

	ASSERT((nSel != CB_ERR) && (pProfile != NULL));

	// Save selection.
	m_pChoice = pProfile;

	return true;
}

/******************************************************************************
** Method:		OnSelectProfile()
**
** Description:	User selected a profile, display profile details.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CSelProfileDlg::OnSelectProfile()
{
	// Get the selected profile.
	int       nSel     = m_cbProfiles.CurSel();
	CProfile* pProfile = (CProfile*) m_cbProfiles.ItemPtr(nSel);

	ASSERT((nSel != CB_ERR) && (pProfile != NULL));

	// Load details into controls.
	m_cbFormat.CurSel(pProfile->m_nFormat);
	m_ebCacheDir.Text(pProfile->m_strCacheDir);
	m_ckReadOnly.Check(pProfile->m_bReadOnly);
	m_ebSystemDir.Text(pProfile->m_strSystemDir);
	m_ebMapDir.Text(pProfile->m_strMapDir);
	m_ebTextureDir.Text(pProfile->m_strTextureDir);
	m_ebSoundDir.Text(pProfile->m_strSoundDir);
	m_ebMusicDir.Text(pProfile->m_strMusicDir);
	m_ebMeshDir.Text(pProfile->m_strMeshDir);
	m_ebAnimDir.Text(pProfile->m_strAnimDir);
	m_ebKarmaDir.Text(pProfile->m_strKarmaDir);
	m_ebConfigFile.Text(pProfile->m_strConfigFile);
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

void CSelProfileDlg::OnHelp(HELPINFO& /*oInfo*/)
{
	// Show the dialogs help topic.
	App.m_oHelpFile.Topic(IDH_SELPROFDLG);
}
