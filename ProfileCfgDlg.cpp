/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PROFILECFGDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CProfileCfgDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "ProfileCfgDlg.hpp"
#include "EditProfileDlg.hpp"
#include "HelpTopics.h"
#include "DetectedDlg.hpp"
#include <Core/Algorithm.hpp>
#include "UTCMGRApp.hpp"
#include <WCL/BusyCursor.hpp>

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
		CMD_CTRLMSG(IDC_ADD,      BN_CLICKED, &CProfileCfgDlg::OnAdd   )
		CMD_CTRLMSG(IDC_EDIT,     BN_CLICKED, &CProfileCfgDlg::OnEdit  )
		CMD_CTRLMSG(IDC_REMOVE,   BN_CLICKED, &CProfileCfgDlg::OnRemove)
		CMD_CTRLMSG(IDC_DETECT,   BN_CLICKED, &CProfileCfgDlg::OnDetect)
		CMD_CTRLMSG(IDC_PROFILES, LBN_DBLCLK, &CProfileCfgDlg::OnDblClkProfile)
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
	const size_t first = 0;

	// Load the profiles listbox.
	for (uint i = 0; i < App.m_aoProfiles.size(); ++i)
	{
		CProfile* pProfile = App.m_aoProfiles[i];

		size_t n = m_lbProfiles.Add(pProfile->m_strName);
		m_lbProfiles.ItemPtr(n, pProfile);
	}

	// Select the first by default.
	m_lbProfiles.CurSel(first);
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
		App.m_aoProfiles.push_back(pProfile);

		// Add to view.
		size_t n = m_lbProfiles.Add(pProfile->m_strName);
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
	size_t nSel = m_lbProfiles.CurSel();

	// No selection?
	if (nSel == CB_ERR)
		return;

	CEditProfileDlg Dlg;
	CProfile*       pProfile = static_cast<CProfile*>(m_lbProfiles.ItemPtr(nSel));

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
	size_t nSel = m_lbProfiles.CurSel();

	// No selection?
	if (nSel == CB_ERR)
		return;

	// Get selected profile details.
	CProfile* pProfile = static_cast<CProfile*>(m_lbProfiles.ItemPtr(nSel));

	// Check we're not deleting the active profile.
	if (pProfile == App.m_pProfile)
	{
		AlertMsg(TXT("You cannot delete the active profile."));
		return;
	}

	// Check we're not deleting the default.
	if (pProfile->m_strName == App.m_strDefProfile)
	{
		AlertMsg(TXT("You cannot delete the default profile."));
		return;
	}

	// Remove from collection.
	Core::deleteValue(App.m_aoProfiles, pProfile);

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

	CProfile* profile = nullptr;
	CProfiles aoProfiles;

	// Look for a UT installation.
	if ((profile = CProfile::DetectUT()) != nullptr)
		aoProfiles.push_back(profile);

	// Look for a UT2003 installation.
	if ((profile = CProfile::DetectUT2003()) != nullptr)
		aoProfiles.push_back(profile);

	// Look for a UT2004 installation.
	if ((profile = CProfile::DetectUT2004()) != nullptr)
		aoProfiles.push_back(profile);

	// Look for a Tactical Ops installation.
	if ((profile = CProfile::DetectTacOps()) != nullptr)
		aoProfiles.push_back(profile);

	// Remove any duplicates.
	for (CProfiles::reverse_iterator oIter = aoProfiles.rbegin(); oIter != aoProfiles.rend(); ++oIter)
	{
		CProfile* pProfile = *oIter;

		// Profile name already used OR already configured?
		if ( (App.FindProfile(pProfile->m_strName) != nullptr)
		  || (App.FindProfileByCfgFile(pProfile->m_strConfigFile) != nullptr) )
		{
			CProfiles::iterator oPos = (oIter+1).base();

			delete *oPos;
			aoProfiles.erase(oPos);
		}
	}

	// Nothing new detected?
	if (aoProfiles.empty())
	{
		NotifyMsg(TXT("No new installations were detected."));
		return;
	}

	CDetectedDlg Dlg;

	// App profiles to dialog.
	for (uint i = 0; i < aoProfiles.size(); ++i)
	{
		CProfile* pProfile = aoProfiles[i];

		Dlg.m_astrNames.Add(pProfile->m_strName);
		Dlg.m_astrFolders.Add(pProfile->m_strConfigFile.Directory().Directory());
	}

	// Query user to confirm new profiles.
	if (Dlg.RunModal(*this) == IDOK)
	{
		// Copy to main profiles array.
		for (CProfiles::reverse_iterator oIter = aoProfiles.rbegin(); oIter != aoProfiles.rend(); ++oIter)
		{
			CProfile* pProfile = *oIter;

			// Add to collection.
			App.m_aoProfiles.push_back(pProfile);

			// Add to view.
			size_t n = m_lbProfiles.Add(pProfile->m_strName);
			m_lbProfiles.ItemPtr(n, pProfile);

			m_lbProfiles.CurSel(n);
		}

		// Detach from temporary array.
		aoProfiles.clear();

		// Mark profiles as modified.
		App.m_nModified |= App.PROFILES;
	}

	// Cleanup.
	Core::deleteAll(aoProfiles);
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
