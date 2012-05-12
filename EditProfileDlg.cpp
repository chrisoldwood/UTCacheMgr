/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		EDITPROFILEDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CEditProfileDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "EditProfileDlg.hpp"
#include "UTCMGRApp.hpp"
#include "HelpTopics.h"

/******************************************************************************
**
** Local variables.
**
*******************************************************************************
*/

static const tchar* SEL_FOLDER_MSG = TXT("Select The UT/UT2003/UT2004/Mod Base Folder\n")
									 TXT("e.g. C:\\UnrealTournament");

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

CEditProfileDlg::CEditProfileDlg()
	: CDialog(IDD_EDIT_PROFILE)
	, m_strLastPath(CProfile::DEF_ROOT_DIR)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_NAME,			&m_ebName      )
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
		CMD_CTRLMSG(IDC_FORMAT,				CBN_SELCHANGE,	&CEditProfileDlg::OnSelectFormat  )
		CMD_CTRLMSG(IDC_BROWSE_CACHE,		BN_CLICKED,		&CEditProfileDlg::OnBrowseCache   )
		CMD_CTRLMSG(IDC_BROWSE_SYSTEM,		BN_CLICKED,		&CEditProfileDlg::OnBrowseSystem  )
		CMD_CTRLMSG(IDC_BROWSE_MAPS,		BN_CLICKED,		&CEditProfileDlg::OnBrowseMaps    )
		CMD_CTRLMSG(IDC_BROWSE_TEXTURES,	BN_CLICKED,		&CEditProfileDlg::OnBrowseTextures)
		CMD_CTRLMSG(IDC_BROWSE_SOUNDS,		BN_CLICKED,		&CEditProfileDlg::OnBrowseSounds  )
		CMD_CTRLMSG(IDC_BROWSE_MUSIC,		BN_CLICKED,		&CEditProfileDlg::OnBrowseMusic   )
		CMD_CTRLMSG(IDC_BROWSE_MESHS,		BN_CLICKED,		&CEditProfileDlg::OnBrowseMeshes  )
		CMD_CTRLMSG(IDC_BROWSE_ANIMS,		BN_CLICKED,		&CEditProfileDlg::OnBrowseAnims   )
		CMD_CTRLMSG(IDC_BROWSE_KARMA,		BN_CLICKED,		&CEditProfileDlg::OnBrowseKarma   )
		CMD_CTRLMSG(IDC_BROWSE_CONFIG,		BN_CLICKED,		&CEditProfileDlg::OnBrowseCfgFile )
		CMD_CTRLMSG(IDC_QUICK_SETUP,		BN_CLICKED,		&CEditProfileDlg::OnQuickSetup    )
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

void CEditProfileDlg::OnInitDialog()
{
	// Initialise formats combo.
	for (int i = 0; i < CProfile::NUM_FORMATS; ++i)
		m_cbFormat.Add(CProfile::s_pszFormats[i]);

	// Load the current config.
	m_ebName.Text(m_oProfile.m_strName);
	m_cbFormat.CurSel(m_oProfile.m_nFormat);
	m_ebCacheDir.Text(m_oProfile.m_strCacheDir);
	m_ckReadOnly.Check(m_oProfile.m_bReadOnly);
	m_ebSystemDir.Text(m_oProfile.m_strSystemDir);
	m_ebMapDir.Text(m_oProfile.m_strMapDir);
	m_ebTextureDir.Text(m_oProfile.m_strTextureDir);
	m_ebSoundDir.Text(m_oProfile.m_strSoundDir);
	m_ebMusicDir.Text(m_oProfile.m_strMusicDir);
	m_ebMeshDir.Text(m_oProfile.m_strMeshDir);
	m_ebAnimDir.Text(m_oProfile.m_strAnimDir);
	m_ebKarmaDir.Text(m_oProfile.m_strKarmaDir);
	m_ebConfigFile.Text(m_oProfile.m_strConfigFile);

	// Initialise control string lengths.
	m_ebName.TextLimit(MAX_PATH);
	m_ebCacheDir.TextLimit(MAX_PATH);
	m_ebSystemDir.TextLimit(MAX_PATH);
	m_ebMapDir.TextLimit(MAX_PATH);
	m_ebTextureDir.TextLimit(MAX_PATH);
	m_ebSoundDir.TextLimit(MAX_PATH);
	m_ebMusicDir.TextLimit(MAX_PATH);
	m_ebMeshDir.TextLimit(MAX_PATH);
	m_ebAnimDir.TextLimit(MAX_PATH);
	m_ebKarmaDir.TextLimit(MAX_PATH);
	m_ebConfigFile.TextLimit(MAX_PATH);

	// Cannot edit profile name.
	if (m_oProfile.m_strName != TXT(""))
		m_ebName.Enable(false);

	OnSelectFormat();
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

bool CEditProfileDlg::OnOk()
{
	// Get settings.
	CString strName       = m_ebName.Text();
	size_t  nFormat       = m_cbFormat.CurSel();
	CPath   strCacheDir   = m_ebCacheDir.Text();
	CPath   strSystemDir  = m_ebSystemDir.Text();
	CPath   strMapDir     = m_ebMapDir.Text();
	CPath   strTextureDir = m_ebTextureDir.Text();
	CPath   strSoundDir   = m_ebSoundDir.Text();
	CPath   strMusicDir   = m_ebMusicDir.Text();
	CPath   strMeshDir    = m_ebMeshDir.Text();
	CPath   strAnimDir    = m_ebAnimDir.Text();
	CPath   strKarmaDir   = m_ebKarmaDir.Text();
	CPath   strConfigFile = m_ebConfigFile.Text();

	// Validate profile name.
	if (strName.Length() == 0)
	{
		AlertMsg(TXT("Please supply a name for the profile."));
		m_ebName.Focus();
		return false;
	}

	if ( (m_oProfile.m_strName == TXT("")) && (App.FindProfile(strName) != NULL) )
	{
		AlertMsg(TXT("The profile name should be unique."));
		m_ebName.Focus();
		return false;
	}

	// Validate common paths.
	if (!ValidatePath(strCacheDir, m_ebCacheDir, TXT("cache folder")))
		return false;

	if (!ValidatePath(strSystemDir, m_ebSystemDir, TXT("system folder")))
		return false;

	if (!ValidatePath(strMapDir, m_ebMapDir, TXT("maps folder")))
		return false;

	if (!ValidatePath(strTextureDir, m_ebTextureDir, TXT("textures folder")))
		return false;

	if (!ValidatePath(strSoundDir, m_ebSoundDir, TXT("sounds folder")))
		return false;

	if (!ValidatePath(strMusicDir, m_ebMusicDir, TXT("music folder")))
		return false;

	// Validate UT2003/4 specific paths.
	if ((nFormat == CProfile::UT2003_FORMAT) || (nFormat == CProfile::UT2004_FORMAT))
	{
		if (!ValidatePath(strMeshDir, m_ebMeshDir, TXT("static meshes folder")))
			return false;

		if (!ValidatePath(strAnimDir, m_ebAnimDir, TXT("animations folder")))
			return false;

		if (!ValidatePath(strKarmaDir, m_ebKarmaDir, TXT("karma data folder")))
			return false;
	}

	if (!ValidatePath(strConfigFile, m_ebConfigFile, TXT("config file")))
		return false;

	// Update profile.
	m_oProfile.m_strName       = strName;
	m_oProfile.m_nFormat       = static_cast<int>(nFormat);
	m_oProfile.m_strCacheDir   = strCacheDir;
	m_oProfile.m_bReadOnly     = m_ckReadOnly.IsChecked();
	m_oProfile.m_strSystemDir  = strSystemDir;
	m_oProfile.m_strMapDir     = strMapDir;
	m_oProfile.m_strTextureDir = strTextureDir;
	m_oProfile.m_strSoundDir   = strSoundDir;
	m_oProfile.m_strMusicDir   = strMusicDir;
	m_oProfile.m_strMeshDir    = strMeshDir;
	m_oProfile.m_strAnimDir    = strAnimDir;
	m_oProfile.m_strKarmaDir   = strKarmaDir;
	m_oProfile.m_strConfigFile = strConfigFile;

	return true;
}

/******************************************************************************
** Method:		OnSelectFormat()
**
** Description:	New format selected, update UI.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CEditProfileDlg::OnSelectFormat()
{
	size_t nFormat = m_cbFormat.CurSel();

	// Enable/Disable depending on UT/UT2003.
	m_ebMeshDir.ReadOnly(nFormat == CProfile::UT_FORMAT);
	m_ebAnimDir.ReadOnly(nFormat == CProfile::UT_FORMAT);
	m_ebKarmaDir.ReadOnly(nFormat == CProfile::UT_FORMAT);

	// Clear non UT controls.
	if (nFormat == CProfile::UT_FORMAT)
	{
		m_ebMeshDir.Text(TXT(""));
		m_ebAnimDir.Text(TXT(""));
		m_ebKarmaDir.Text(TXT(""));
	}
}

/******************************************************************************
** Method:		OnBrowse*()
**
** Description:	Handlers to show directory/file selection dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CEditProfileDlg::OnBrowseCache()
{
	OnBrowseDir(m_ebCacheDir);
}

void CEditProfileDlg::OnBrowseSystem()
{
	OnBrowseDir(m_ebSystemDir);
}

void CEditProfileDlg::OnBrowseMaps()
{
	OnBrowseDir(m_ebMapDir);
}

void CEditProfileDlg::OnBrowseTextures()
{
	OnBrowseDir(m_ebTextureDir);
}

void CEditProfileDlg::OnBrowseSounds()
{
	OnBrowseDir(m_ebSoundDir);
}

void CEditProfileDlg::OnBrowseMusic()
{
	OnBrowseDir(m_ebMusicDir);
}

void CEditProfileDlg::OnBrowseMeshes()
{
	OnBrowseDir(m_ebMeshDir);
}

void CEditProfileDlg::OnBrowseAnims()
{
	OnBrowseDir(m_ebAnimDir);
}

void CEditProfileDlg::OnBrowseKarma()
{
	OnBrowseDir(m_ebKarmaDir);
}

void CEditProfileDlg::OnBrowseDir(CEditBox& ebPath)
{
	CPath strDir = ebPath.Text();

	// If empty start from last path.
	if (strDir == TXT(""))
		strDir = m_strLastPath;

	// Show Select Directory common dialog.
	if (strDir.SelectDir(*this, TXT("Select Folder"), strDir))
	{
		// Display path.
		ebPath.Text(strDir);

		// Use path as starting point for next selection.
		m_strLastPath = strDir;
	}
}

void CEditProfileDlg::OnBrowseCfgFile()
{
	// File extensions.
	static tchar szExts[] = {	TXT("INI Files (*.ini)\0*.ini\0")
								TXT("All Files (*.*)\0*.*\0")
								TXT("\0\0")							};

	CPath strDir = m_ebConfigFile.Text();

	// If empty start from last path.
	if (strDir == TXT(""))
		strDir = m_strLastPath;

	CPath strFile;

	// Show Select File common dialog.
	if (strFile.Select(*this, CPath::SelectFile, szExts, TXT("ini"), strDir))
	{
		// Display path.
		m_ebConfigFile.Text(strFile);
	}
}

/******************************************************************************
** Method:		OnQuickSetup()
**
** Description:	Fills in all paths based on an inital base directory.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CEditProfileDlg::OnQuickSetup()
{
	CPath strBaseDir;

	// Select the base directory.
	if (!strBaseDir.SelectDir(*this, SEL_FOLDER_MSG))
		return;

	// Create the full path to the System folder.
	CPath strSysDir(strBaseDir, CProfile::DEF_SYSTEM_DIR);

	// Set common paths.
	m_cbFormat.CurSel  (CProfile::UT_FORMAT);
	m_ebCacheDir.Text  (CPath(strBaseDir, CProfile::DEF_CACHE_DIR   ));
	m_ebSystemDir.Text (CPath(strBaseDir, CProfile::DEF_SYSTEM_DIR  ));
	m_ebMapDir.Text    (CPath(strBaseDir, CProfile::DEF_MAPS_DIR    ));
	m_ebTextureDir.Text(CPath(strBaseDir, CProfile::DEF_TEXTURES_DIR));
	m_ebSoundDir.Text  (CPath(strBaseDir, CProfile::DEF_SOUNDS_DIR  ));
	m_ebMusicDir.Text  (CPath(strBaseDir, CProfile::DEF_MUSIC_DIR   ));
	m_ebConfigFile.Text(CPath(strSysDir,  CProfile::DEF_CONFIG_FILE ));

	// Check for UT2004.
	CPath strUT2003Cfg(strSysDir, CProfile::DEF_2003_CONFIG_FILE);
	CPath strUT2004Cfg(strSysDir, CProfile::DEF_2004_CONFIG_FILE);

	if (strUT2003Cfg.Exists() || strUT2004Cfg.Exists())
	{
		// Set common paths.
		m_ebMeshDir.Text (CPath(strBaseDir, CProfile::DEF_MESH_DIR ));
		m_ebAnimDir.Text (CPath(strBaseDir, CProfile::DEF_ANIM_DIR ));
		m_ebKarmaDir.Text(CPath(strBaseDir, CProfile::DEF_KARMA_DIR));

		// Set UT2003 paths.
		if (strUT2003Cfg.Exists())
		{
			m_ebConfigFile.Text(strUT2003Cfg);
			m_cbFormat.CurSel(CProfile::UT2003_FORMAT);
		}

		// Set UT2004 paths.
		if (strUT2004Cfg.Exists())
		{
			m_ebConfigFile.Text(strUT2004Cfg);
			m_cbFormat.CurSel(CProfile::UT2004_FORMAT);
		}
	}

	OnSelectFormat();
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

void CEditProfileDlg::OnHelp(HELPINFO& /*oInfo*/)
{
	// Show the dialogs help topic.
	App.m_oHelpFile.Topic(IDH_EDITPROFDLG);
}

/******************************************************************************
** Method:		ValidatePath()
**
** Description:	Checks if the path is valid and if not queries the user if they
**				want to allow it anyway.
**
** Parameters:	strPath		The path.
**				ebControl	The control where the path is enetered.
**				pszName		The name for the path.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CEditProfileDlg::ValidatePath(const CPath& strPath, CEditBox& ebControl, const tchar* pszName)
{
	ASSERT(pszName != NULL);

	// Path valid?
	if (strPath.Exists())
		return true;

	// User allowing path anyway?
	if (QueryMsg(TXT("The %s is invalid.\n\nDo you want to allow it?"), pszName) == IDYES)
		return true;
		
	ebControl.Focus();

	return false;
}
