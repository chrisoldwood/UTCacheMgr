/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		EDITPROFILEDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CEditProfileDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "EditProfileDlg.hpp"
#include "HelpTopics.h"

/******************************************************************************
**
** Local variables.
**
*******************************************************************************
*/

static const char* SEL_FOLDER_MSG = "Select The UT/UT2003/Mod Base Folder\n"
									"e.g. C:\\UnrealTournament";

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
		CMD_CTRLMSG(IDC_FORMAT,				CBN_SELCHANGE,	OnSelectFormat  )
		CMD_CTRLMSG(IDC_BROWSE_CACHE,		BN_CLICKED,		OnBrowseCache   )
		CMD_CTRLMSG(IDC_BROWSE_SYSTEM,		BN_CLICKED,		OnBrowseSystem  )
		CMD_CTRLMSG(IDC_BROWSE_MAPS,		BN_CLICKED,		OnBrowseMaps    )
		CMD_CTRLMSG(IDC_BROWSE_TEXTURES,	BN_CLICKED,		OnBrowseTextures)
		CMD_CTRLMSG(IDC_BROWSE_SOUNDS,		BN_CLICKED,		OnBrowseSounds  )
		CMD_CTRLMSG(IDC_BROWSE_MUSIC,		BN_CLICKED,		OnBrowseMusic   )
		CMD_CTRLMSG(IDC_BROWSE_MESHS,		BN_CLICKED,		OnBrowseMeshes  )
		CMD_CTRLMSG(IDC_BROWSE_ANIMS,		BN_CLICKED,		OnBrowseAnims   )
		CMD_CTRLMSG(IDC_BROWSE_KARMA,		BN_CLICKED,		OnBrowseKarma   )
		CMD_CTRLMSG(IDC_BROWSE_CONFIG,		BN_CLICKED,		OnBrowseCfgFile )
		CMD_CTRLMSG(IDC_QUICK_SETUP,		BN_CLICKED,		OnQuickSetup    )
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
	m_cbFormat.Add("UT");
	m_cbFormat.Add("UT2003");

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
	if (m_oProfile.m_strName != "")
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
	int     nFormat       = m_cbFormat.CurSel();
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

	// Validate name.
	if (strName.Length() == 0)
	{
		AlertMsg("Please supply a name for the profile.");
		m_ebName.Focus();
		return false;
	}

	if ( (m_oProfile.m_strName == "") && (App.FindProfile(strName) != NULL) )
	{
		AlertMsg("The profile name should be unique.");
		m_ebName.Focus();
		return false;
	}

	// Validate paths.
	if (!strCacheDir.Exists())
	{
		AlertMsg("The cache folder is invalid.");
		m_ebCacheDir.Focus();
		return false;
	}

	if (!strSystemDir.Exists())
	{
		AlertMsg("The system folder is invalid.");
		m_ebSystemDir.Focus();
		return false;
	}

	if (!strMapDir.Exists())
	{
		AlertMsg("The maps folder is invalid.");
		m_ebMapDir.Focus();
		return false;
	}

	if (!strTextureDir.Exists())
	{
		AlertMsg("The textures folder is invalid.");
		m_ebTextureDir.Focus();
		return false;
	}

	if (!strSoundDir.Exists())
	{
		AlertMsg("The sounds folder is invalid.");
		m_ebSoundDir.Focus();
		return false;
	}

	if (!strMusicDir.Exists())
	{
		AlertMsg("The music folder is invalid.");
		m_ebMusicDir.Focus();
		return false;
	}

	if ( (nFormat == CProfile::UT2003_FORMAT) && (!strMeshDir.Exists()) )
	{
		AlertMsg("The static meshes folder is invalid.");
		m_ebMeshDir.Focus();
		return false;
	}

	if ( (nFormat == CProfile::UT2003_FORMAT) && (!strAnimDir.Exists()) )
	{
		AlertMsg("The animations folder is invalid.");
		m_ebAnimDir.Focus();
		return false;
	}

	if ( (nFormat == CProfile::UT2003_FORMAT) && (!strKarmaDir.Exists()) )
	{
		AlertMsg("The karma data folder is invalid.");
		m_ebKarmaDir.Focus();
		return false;
	}

	if (!strConfigFile.Exists())
	{
		AlertMsg("The config file is invalid.");
		m_ebConfigFile.Focus();
		return false;
	}

	// Update profile.
	m_oProfile.m_strName       = strName;
	m_oProfile.m_nFormat       = nFormat;
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
	int nFormat = m_cbFormat.CurSel();

	// Enable/Disable depending on UT/UT2003.
	m_ebMeshDir.ReadOnly(nFormat == CProfile::UT_FORMAT);
	m_ebAnimDir.ReadOnly(nFormat == CProfile::UT_FORMAT);
	m_ebKarmaDir.ReadOnly(nFormat == CProfile::UT_FORMAT);

	// Clear non UT controls.
	if (nFormat == CProfile::UT_FORMAT)
	{
		m_ebMeshDir.Text("");
		m_ebAnimDir.Text("");
		m_ebKarmaDir.Text("");
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
	if (strDir == "")
		strDir = m_strLastPath;

	// Show Select Directory common dialog.
	if (strDir.SelectDir(*this, "Select Folder", strDir))
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
	static char szExts[] = {	"INI Files (*.ini)\0*.ini\0"
								"All Files (*.*)\0*.*\0"
								"\0\0"							};

	CPath strDir = m_ebConfigFile.Text();

	// If empty start from last path.
	if (strDir == "")
		strDir = m_strLastPath;

	CPath strFile;

	// Show Select File common dialog.
	if (strFile.Select(*this, CPath::SelectFile, szExts, "ini", strDir))
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

	// Check for UT2003.
	CPath strUT2003Cfg(strSysDir, CProfile::DEF_2003_CONFIG_FILE);

	if (strUT2003Cfg.Exists())
	{
		m_cbFormat.CurSel  (CProfile::UT2003_FORMAT);
		m_ebConfigFile.Text(strUT2003Cfg);
		m_ebMeshDir.Text   (CPath(strBaseDir, CProfile::DEF_MESH_DIR   ));
		m_ebAnimDir.Text   (CPath(strBaseDir, CProfile::DEF_ANIM_DIR   ));
		m_ebKarmaDir.Text  (CPath(strBaseDir, CProfile::DEF_KARMA_DIR  ));
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
