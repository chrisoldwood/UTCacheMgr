/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PROFILECFGDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CProfileCfgDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "ProfileCfgDlg.hpp"
#include "EditProfileDlg.hpp"
#include "HelpTopics.h"
#include "DetectedDlg.hpp"

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

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

CProfileCfgDlg::CProfileCfgDlg()
	: CDialog(IDD_PROFILES)
	, m_bReScan(false)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_PROFILES,	&m_lbProfiles)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_ADD,      BN_CLICKED, OnAdd   )
		CMD_CTRLMSG(IDC_EDIT,     BN_CLICKED, OnEdit  )
		CMD_CTRLMSG(IDC_REMOVE,   BN_CLICKED, OnRemove)
		CMD_CTRLMSG(IDC_DETECT,   BN_CLICKED, OnDetect)
		CMD_CTRLMSG(IDC_PROFILES, LBN_DBLCLK, OnDblClkProfile)
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

void CProfileCfgDlg::OnInitDialog()
{
	// Load the profiles listbox.
	for (int i = 0; i < App.m_aoProfiles.Size(); ++i)
	{
		CProfile* pProfile = App.m_aoProfiles[i];

		int n = m_lbProfiles.Add(pProfile->m_strName);
		m_lbProfiles.ItemPtr(n, pProfile);
	}

	// Select the first by default.
	m_lbProfiles.CurSel(0);
}

/******************************************************************************
** Method:		OnAdd()
**
** Description:	Add a new profile.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CProfileCfgDlg::OnAdd()
{
	CEditProfileDlg Dlg;

	// Show profile editing dialog.
	if (Dlg.RunModal(*this) == IDOK)
	{
		CProfile* pProfile = new CProfile(Dlg.m_oProfile);

		// Add to collection.
		App.m_aoProfiles.Add(pProfile);

		// Add to view.
		int n = m_lbProfiles.Add(pProfile->m_strName);
		m_lbProfiles.ItemPtr(n, pProfile);

		m_lbProfiles.CurSel(n);

		// Mark profiles as modified.
		App.m_nModified |= App.PROFILES;
	}
}

/******************************************************************************
** Method:		OnEdit()
**
** Description:	Edit the selected profile.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CProfileCfgDlg::OnEdit()
{
	int nSel = m_lbProfiles.CurSel();

	// No selection?
	if (nSel == CB_ERR)
		return;

	CEditProfileDlg Dlg;
	CProfile*       pProfile = (CProfile*) m_lbProfiles.ItemPtr(nSel);

	// Get selected profile details.
	Dlg.m_oProfile = *pProfile;

	// Show profile editing dialog.
	if (Dlg.RunModal(*this) == IDOK)
	{
		// Update profile.
		*pProfile = Dlg.m_oProfile;

		// Rescan, if the active one.
		if (pProfile == App.m_pProfile)
			m_bReScan = true;

		// Mark profiles as modified.
		App.m_nModified |= App.PROFILES;
	}
}

/******************************************************************************
** Method:		OnRemove()
**
** Description:	Remove the selected profile.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CProfileCfgDlg::OnRemove()
{
	int nSel = m_lbProfiles.CurSel();

	// No selection?
	if (nSel == CB_ERR)
		return;

	// Get selected profile details.
	CProfile* pProfile = (CProfile*) m_lbProfiles.ItemPtr(nSel);

	// Check we're not deleting the active profile.
	if (pProfile == App.m_pProfile)
	{
		AlertMsg("You cannot delete the active profile.");
		return;
	}

	// Check we're not deleting the default.
	if (pProfile->m_strName == App.m_strDefProfile)
	{
		AlertMsg("You cannot delete the default profile.");
		return;
	}

	// Remove from collection.
	App.m_aoProfiles.Delete(App.m_aoProfiles.Find(pProfile));

	// Remove from view.
	m_lbProfiles.Delete(nSel);

	if (nSel == m_lbProfiles.Count())
		nSel--;

	m_lbProfiles.CurSel(nSel);

	// Mark profiles as modified.
	App.m_nModified |= App.PROFILES;
}

/******************************************************************************
** Method:		OnDblClkProfile()
**
** Description:	Double-clicked a profile, edit it.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CProfileCfgDlg::OnDblClkProfile()
{
	OnEdit();
}

/******************************************************************************
** Method:		OnDetect()
**
** Description:	Detect any new UT installations.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CProfileCfgDlg::OnDetect()
{
	CBusyCursor oBusyCursor;

	// Template shorthands.
	typedef TPtrArray<CProfile> CProfiles;

	CProfile* pProfile = NULL;
	CProfiles aoProfiles;

	// Look for a UT installation.
	if ((pProfile = CProfile::DetectUT()) != NULL)
		aoProfiles.Add(pProfile);

	// Look for a UT2003 installation.
	if ((pProfile = CProfile::DetectUT2003()) != NULL)
		aoProfiles.Add(pProfile);

	// Look for a UT2004 installation.
	if ((pProfile = CProfile::DetectUT2004()) != NULL)
		aoProfiles.Add(pProfile);

	// Look for a Tactical Ops installation.
	if ((pProfile = CProfile::DetectTacOps()) != NULL)
		aoProfiles.Add(pProfile);

	// Remove any duplicates.
	for (int i = aoProfiles.Size()-1; i >= 0; --i)
	{
		CProfile* pProfile = aoProfiles[i];

		// Profile name already used OR already configured?
		if ( (App.FindProfile(pProfile->m_strName) != NULL)
		  || (App.FindProfileByCfgFile(pProfile->m_strConfigFile) != NULL) )
		{
			aoProfiles.Delete(i);
		}
	}

	// Nothing new detected?
	if (aoProfiles.Size() == 0)
	{
		NotifyMsg("No new installations were detected.");
		return;
	}

	CDetectedDlg Dlg;

	// App profiles to dialog.
	for (i = 0; i < aoProfiles.Size(); ++i)
	{
		CProfile* pProfile = aoProfiles[i];

		Dlg.m_astrNames.Add(pProfile->m_strName);
		Dlg.m_astrFolders.Add(pProfile->m_strConfigFile.Directory().Directory());
	}

	// Query user to confirm new profiles.
	if (Dlg.RunModal(*this) == IDOK)
	{
		// Copy to main profiles array.
		for (int i = aoProfiles.Size()-1; i >= 0; --i)
		{
			CProfile* pProfile = aoProfiles[i];

			// Add to collection.
			App.m_aoProfiles.Add(pProfile);

			// Add to view.
			int n = m_lbProfiles.Add(pProfile->m_strName);
			m_lbProfiles.ItemPtr(n, pProfile);

			m_lbProfiles.CurSel(n);
		}

		// Detach from temporary array.
		aoProfiles.RemoveAll();

		// Mark profiles as modified.
		App.m_nModified |= App.PROFILES;
	}

	// Cleanup.
	aoProfiles.DeleteAll();
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

void CProfileCfgDlg::OnHelp(HELPINFO& /*oInfo*/)
{
	// Show the dialogs help topic.
	App.m_oHelpFile.Topic(IDH_PROFILESDLG);
}
