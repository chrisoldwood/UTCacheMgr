/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		UTCMGRAPP.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CUTCMGRApp class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

/******************************************************************************
**
** Global variables.
**
*******************************************************************************
*/

// "The" application object.
CUTCMGRApp App;

/******************************************************************************
**
** Class constants.
**
*******************************************************************************
*/

#ifdef _DEBUG
const char* CUTCMGRApp::VERSION      = "v2.5 [Debug]";
#else
const char* CUTCMGRApp::VERSION      = "v2.5";
#endif
const char* CUTCMGRApp::INI_FILE_VER_10 = "1.0";
const char* CUTCMGRApp::INI_FILE_VER_20 = "2.0";
const char* CUTCMGRApp::INI_FILE_VER_25 = "2.5";

/******************************************************************************
** Method:		Constructor
**
** Description:	Default constructor.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CUTCMGRApp::CUTCMGRApp()
	: CApp(m_AppWnd, m_AppCmds)
	, m_oHelpFile(m_AppWnd)
	, m_nModified(NONE)
	, m_oCache(m_oMDB)
	, m_pProfile(NULL)
	, m_bScanOnStart(true)
	, m_bScanOnSwitch(true)
	, m_bScanForTmp(true)
	, m_bScanIndex(true)
	, m_bShowAllFiles(false)
	, m_bLogEdits(true)
{
	// Create the MDB.
	m_oMDB.AddTable(m_oCache);

	// Set the help file path.
	m_oHelpFile.m_HelpFile  = CPath(CPath::ApplicationDir(), "UTCacheMgr.hlp");
}

/******************************************************************************
** Method:		Destructor
**
** Description:	Cleanup.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CUTCMGRApp::~CUTCMGRApp()
{
	m_aoProfiles.DeleteAll();
}

/******************************************************************************
** Method:		OnOpen()
**
** Description:	Initialises the application.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CUTCMGRApp::OnOpen()
{
	// Set the app title.
	m_strTitle = "UT Cache Manager";

	// Load settings.
	LoadConfig();
	
	// Load the toolbar bitmap.
	m_rCmdControl.CmdBitmap().LoadRsc(IDR_APPTOOLBAR);

	// Create the main window.
	if (!m_AppWnd.Create())
		return false;

	// Show it.
	if ( (m_iCmdShow == SW_SHOWNORMAL) && (m_rcLastWndPos.Empty() == false) )
		m_AppWnd.Move(m_rcLastWndPos);

	m_AppWnd.Show(m_iCmdShow);

	// Initialise UI.
	BuildProfileMenu();
	m_AppCmds.UpdateUI();

	// Scan on startup?
	if (m_bScanOnStart)
		m_AppWnd.PostCommand(ID_CACHE_RESCAN);

	return true;
}

/******************************************************************************
** Method:		OnClose()
**
** Description:	Cleans up the application resources.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CUTCMGRApp::OnClose()
{
	// Save settings.
	SaveConfig();

	return true;
}

/******************************************************************************
** Method:		LoadConfig()
**
** Description:	Load the app configuration.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CUTCMGRApp::LoadConfig()
{
	// Read the file version.
	CString strVer = m_oIniFile.ReadString("Version", "Version", "");

	// If first run, mark all settings as modified.
	if (strVer == "")
		m_nModified = SETTINGS | PROFILES | PIN_LIST;

	// Read the cache general settings.
	m_strCacheIndex = m_oIniFile.ReadString("Cache", "Index",        CProfile::DEF_CACHE_IDX_FILE);
	m_strCacheExt   = m_oIniFile.ReadString("Cache", "Ext",          CProfile::DEF_CACHE_FILE_EXT);
	m_bScanOnStart  = m_oIniFile.ReadBool  ("Cache", "ScanOnStart",  m_bScanOnStart);
	m_bScanOnSwitch = m_oIniFile.ReadBool  ("Cache", "ScanOnSwitch", m_bScanOnSwitch);
	m_bScanForTmp   = m_oIniFile.ReadBool  ("Cache", "ScanForTmp",   m_bScanForTmp);
	m_bScanIndex    = m_oIniFile.ReadBool  ("Cache", "ScanIndex",    m_bScanIndex);
	m_bShowAllFiles = m_oIniFile.ReadBool  ("Cache", "ShowAllFiles", m_bShowAllFiles);
	m_bLogEdits     = m_oIniFile.ReadBool  ("Cache", "LogEdits",     m_bLogEdits);

	// Read the number of profiles.
	uint nProfiles  = m_oIniFile.ReadInt("Profiles", "Count",   0);

	// Read the profiles.
	for (uint i = 0; i < nProfiles; ++i)
	{
		CString  strSection;
		CProfile oProfile;

		// Create section name.
		strSection.Format("Profile%d", i);

		// Read profile details.
		oProfile.m_strName       = m_oIniFile.ReadString(strSection, "Name",       "");
		oProfile.m_nFormat       = m_oIniFile.ReadInt   (strSection, "Format",     CProfile::UT_FORMAT);
		oProfile.m_strCacheDir   = m_oIniFile.ReadString(strSection, "CacheDir",   "");
		oProfile.m_bReadOnly     = m_oIniFile.ReadBool  (strSection, "ReadOnly",   false);
		oProfile.m_strSystemDir  = m_oIniFile.ReadString(strSection, "SystemDir",  "");
		oProfile.m_strMapDir     = m_oIniFile.ReadString(strSection, "MapDir",     "");
		oProfile.m_strTextureDir = m_oIniFile.ReadString(strSection, "TextureDir", "");
		oProfile.m_strSoundDir   = m_oIniFile.ReadString(strSection, "SoundDir",   "");
		oProfile.m_strMusicDir   = m_oIniFile.ReadString(strSection, "MusicDir",   "");
		oProfile.m_strMeshDir    = m_oIniFile.ReadString(strSection, "MeshDir",    "");
		oProfile.m_strAnimDir    = m_oIniFile.ReadString(strSection, "AnimDir",    "");
		oProfile.m_strConfigFile = m_oIniFile.ReadString(strSection, "ConfigFile", "");
		oProfile.m_strLastCopyTo = m_oIniFile.ReadString(strSection, "LastCopyTo", "");
		oProfile.m_strLastImport  = m_oIniFile.ReadString(strSection, "LastImport",  "");
		oProfile.m_strLastInstall = m_oIniFile.ReadString(strSection, "LastInstall", "");

		// If valid, add to collection.
		if (oProfile.m_strName.Length() > 0)
			m_aoProfiles.Add(new CProfile(oProfile));
	}

	// Read the default profile.
	m_strDefProfile = m_oIniFile.ReadString("Profiles", "Default", "");

	// Find the default profile.
	if (m_strDefProfile != "")
		m_pProfile = FindProfile(m_strDefProfile);

	// New installation?.
	if (m_pProfile == NULL)
	{
		CProfile* pProfile = NULL;

		// Look for a UT installation.
		if ((pProfile = CProfile::DetectUT()) != NULL)
			m_aoProfiles.Add(pProfile);

		// Look for a UT2003 installation.
		if ((pProfile = CProfile::DetectUT2003()) != NULL)
			m_aoProfiles.Add(pProfile);

		// Look for a Tactical Ops installation.
		if ((pProfile = CProfile::DetectTacOps()) != NULL)
			m_aoProfiles.Add(pProfile);

		// If nothing detected, create a default UT one.
		if (m_aoProfiles.Size() == 0)
		{
			// Warn user.
			AlertMsg("Your UT/UT2003 installation could not be detected.\n\n"
					 "The inital cache scan may fail as it is performed on\n"
					 "the default UT installation folder.");

			m_pProfile = new CProfile();

			// Initialise profile with sensible defaults.
			m_pProfile->m_strName       = CProfile::DEF_UT_PROFILE_NAME;
			m_pProfile->m_nFormat       = CProfile::UT_FORMAT;
			m_pProfile->m_strCacheDir   = CPath(CProfile::DEF_ROOT_DIR,     CProfile::DEF_CACHE_DIR   );
			m_pProfile->m_bReadOnly     = false;
			m_pProfile->m_strSystemDir  = CPath(CProfile::DEF_ROOT_DIR,     CProfile::DEF_SYSTEM_DIR  );
			m_pProfile->m_strMapDir     = CPath(CProfile::DEF_ROOT_DIR,     CProfile::DEF_MAPS_DIR    );
			m_pProfile->m_strTextureDir = CPath(CProfile::DEF_ROOT_DIR,     CProfile::DEF_TEXTURES_DIR);
			m_pProfile->m_strSoundDir   = CPath(CProfile::DEF_ROOT_DIR,     CProfile::DEF_SOUNDS_DIR  );
			m_pProfile->m_strMusicDir   = CPath(CProfile::DEF_ROOT_DIR,     CProfile::DEF_MUSIC_DIR   );
			m_pProfile->m_strConfigFile = CPath(m_pProfile->m_strSystemDir, CProfile::DEF_CONFIG_FILE );

			m_aoProfiles.Add(m_pProfile);
		}

		ASSERT(m_aoProfiles.Size() > 0);

		// Set the default profile.
		m_pProfile      = m_aoProfiles[0];
		m_strDefProfile = m_pProfile->m_strName;
	}

	ASSERT(m_pProfile != NULL);

	// Read the number of pinned files.
	uint nPinned = m_oIniFile.ReadInt("Pinned", "Count",   0);

	// Read the pinned files.
	for (i = 0; i < nPinned; ++i)
	{
		CString strEntry;
		CString strName;

		// Create entry name.
		strEntry.Format("File%d", i);

		strName = m_oIniFile.ReadString("Pinned", strEntry, "");

		// Add if valid and not already listed.
		if ((strName.Length() > 0) && (m_astrPinned.Find(strName, false) == -1))
			m_astrPinned.Add(strName);
	}

	// Read the previous UI settings.
	m_rcLastWndPos = m_oIniFile.ReadRect("UI", "MainWindow",  CRect());
	m_rcLastDlgPos = m_oIniFile.ReadRect("UI", "SelFilesDlg", CRect());

	if ( (strVer == INI_FILE_VER_10) || (strVer == INI_FILE_VER_20) )
	{
		m_rcLastWndPos.left   = m_oIniFile.ReadInt("UI", "Left",   0);
		m_rcLastWndPos.top    = m_oIniFile.ReadInt("UI", "Top",    0);
		m_rcLastWndPos.right  = m_oIniFile.ReadInt("UI", "Right",  0);
		m_rcLastWndPos.bottom = m_oIniFile.ReadInt("UI", "Bottom", 0);
	}
}

/******************************************************************************
** Method:		SaveConfig()
**
** Description:	Save the app configuration.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CUTCMGRApp::SaveConfig()
{
	// Write the file version.
	m_oIniFile.WriteString("Version", "Version", INI_FILE_VER_25);

	// Application settings changed?
	if (m_nModified & SETTINGS)
	{
		// Write the cache general settings.
		m_oIniFile.WriteString("Cache",    "Index",        m_strCacheIndex);
		m_oIniFile.WriteString("Cache",    "Ext",          m_strCacheExt);
		m_oIniFile.WriteBool  ("Cache",    "ScanOnStart",  m_bScanOnStart);
		m_oIniFile.WriteBool  ("Cache",    "ScanOnSwitch", m_bScanOnSwitch);
		m_oIniFile.WriteBool  ("Cache",    "ScanForTmp",   m_bScanForTmp);
		m_oIniFile.WriteBool  ("Cache",    "ScanIndex",    m_bScanIndex);
		m_oIniFile.WriteBool  ("Cache",    "ShowAllFiles", m_bShowAllFiles);
		m_oIniFile.WriteBool  ("Cache",    "LogEdits",     m_bLogEdits);
		m_oIniFile.WriteString("Profiles", "Default",      m_strDefProfile);
	}

	// Profiles changed?
	if (m_nModified & PROFILES)
	{
		// Write the profiles.
		m_oIniFile.WriteInt("Profiles", "Count", m_aoProfiles.Size());

		for (int i = 0; i < m_aoProfiles.Size(); ++i)
		{
			CString   strSection;
			CProfile* pProfile = m_aoProfiles[i];

			// Create section name.
			strSection.Format("Profile%d", i);

			// Write profile details.
			m_oIniFile.WriteString(strSection, "Name",       pProfile->m_strName      );
			m_oIniFile.WriteInt   (strSection, "Format",     pProfile->m_nFormat      );
			m_oIniFile.WriteString(strSection, "CacheDir",   pProfile->m_strCacheDir  );
			m_oIniFile.WriteBool  (strSection, "ReadOnly",   pProfile->m_bReadOnly    );
			m_oIniFile.WriteString(strSection, "SystemDir",  pProfile->m_strSystemDir );
			m_oIniFile.WriteString(strSection, "MapDir",     pProfile->m_strMapDir    );
			m_oIniFile.WriteString(strSection, "TextureDir", pProfile->m_strTextureDir);
			m_oIniFile.WriteString(strSection, "SoundDir",   pProfile->m_strSoundDir  );
			m_oIniFile.WriteString(strSection, "MusicDir",   pProfile->m_strMusicDir  );
			m_oIniFile.WriteString(strSection, "MeshDir",    pProfile->m_strMeshDir   );
			m_oIniFile.WriteString(strSection, "AnimDir",    pProfile->m_strAnimDir   );
			m_oIniFile.WriteString(strSection, "ConfigFile", pProfile->m_strConfigFile);
			m_oIniFile.WriteString(strSection, "LastCopyTo", pProfile->m_strLastCopyTo);
			m_oIniFile.WriteString(strSection, "LastImport",  pProfile->m_strLastImport);
			m_oIniFile.WriteString(strSection, "LastInstall", pProfile->m_strLastInstall);
		}
	}

	// Pinned files list changed?
	if (m_nModified & PIN_LIST)
	{
		// Write the list of pinned files.
		m_oIniFile.DeleteSection("Pinned");
		m_oIniFile.WriteInt ("Pinned", "Count", m_astrPinned.Size());

		for (int i = 0; i < m_astrPinned.Size(); ++i)
		{
			CString strEntry;

			// Create entry name.
			strEntry.Format("File%d", i);

			m_oIniFile.WriteString("Pinned", strEntry, m_astrPinned[i]);
		}
	}

	// Save the current UI settings.
	m_oIniFile.WriteRect("UI", "MainWindow",  m_rcLastWndPos);
	m_oIniFile.WriteRect("UI", "SelFilesDlg", m_rcLastDlgPos);
}

/******************************************************************************
** Method:		FindProfile()
**
** Description:	Finds a profile by name.
**
** Parameters:	None.
**
** Returns:		The profile or NULL.
**
*******************************************************************************
*/

CProfile* CUTCMGRApp::FindProfile(const char* pszName) const
{
	ASSERT(pszName != NULL);

	// For all profiles...
	for (int i = 0; i < m_aoProfiles.Size(); ++i)
	{
		if (m_aoProfiles[i]->m_strName.Compare(pszName, true) == 0)
			return m_aoProfiles[i];
	}

	return NULL;
}

/******************************************************************************
** Method:		GetProfileIndex()
**
** Description:	Gets the index of a profile.
**
** Parameters:	pProfile	The profile.
**
** Returns:		The index or -1 if not found.
**
*******************************************************************************
*/

int CUTCMGRApp::GetProfileIndex(CProfile* pProfile) const
{
	ASSERT(pProfile != NULL);

	// For all profiles...
	for (int i = 0; i < m_aoProfiles.Size(); ++i)
	{
		if (m_aoProfiles[i] == pProfile)
			return i;
	}

	return -1;
}

/******************************************************************************
** Method:		BuildProfileMenu()
**
** Description:	Builds the Cache | Profile sub-menu.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CUTCMGRApp::BuildProfileMenu()
{
	// Ensure profile list is sorted.
	m_aoProfiles.Sort((TPtrArray<CProfile>::PFNCOMPARE)CProfile::Compare);

	// Delete old menu.
	for (int i = ID_CACHE_FIRST_PROFILE; i <= ID_CACHE_LAST_PROFILE; ++i)
		App.m_AppWnd.m_Menu.RemoveCmd(i);

	CPopupMenu oCacheMenu = App.m_AppWnd.m_Menu.GetItemPopup(0).GetItemPopup(0);

	// Build new menu.
	for (i = 0; i < m_aoProfiles.Size(); ++i)
		oCacheMenu.InsertCmd(i, ID_CACHE_FIRST_PROFILE + i, m_aoProfiles[i]->m_strName);
}

/******************************************************************************
** Method:		FormatType()
**
** Description:	Convert the file type to a string.
**
** Parameters:	cType	The file type.
**
** Returns:		The type as a string.
**
*******************************************************************************
*/

CString CUTCMGRApp::FormatType(char cType) const
{
	switch (cType)
	{
		case SYSTEM_FILE:	return "System";
		case MAP_FILE:		return "Map";
		case TEXTURE_FILE:	return "Texture";
		case SOUND_FILE:	return "Sound";
		case MUSIC_FILE:	return "Music";
		case MESH_FILE:		return "Mesh";
		case ANIM_FILE:		return "Anim";
	}

	ASSERT_FALSE();

	return "";
}

/******************************************************************************
** Method:		FormatSize()
**
** Description:	Convert the file size to a string.
**
** Parameters:	nSize	The file size.
**
** Returns:		The size as a string.
**
*******************************************************************************
*/

CString CUTCMGRApp::FormatSize(int nSize) const
{
	CString str;

	// Ensure we report at least 1K.
	nSize = max(1024, nSize);

	str.Format("%d K", nSize/1024);

	return str;
}

/******************************************************************************
** Method:		FormatStatus()
**
** Description:	Convert the file status to a string.
**
** Parameters:	cStatus		The file status.
**
** Returns:		The status as a string.
**
*******************************************************************************
*/

CString CUTCMGRApp::FormatStatus(char cStatus) const
{
	switch (cStatus)
	{
		case NEW_FILE :	return "New";
		case OLD_FILE :	return "Old";
		case PIN_FILE :	return "Pinned";
	}

	ASSERT_FALSE();

	return "";
}

/******************************************************************************
** Method:		IconIndex()
**
** Description:	Gets the index of the icon for the given file type.
**
** Parameters:	cType	The file type.
**
** Returns:		The icon index.
**
*******************************************************************************
*/

int CUTCMGRApp::IconIndex(char cType) const
{
	switch (cType)
	{
		case SYSTEM_FILE:	return 0;
		case MAP_FILE:		return 1;
		case TEXTURE_FILE:	return 2;
		case SOUND_FILE:	return 3;
		case MUSIC_FILE:	return 4;
		case MESH_FILE:		return 5;
		case ANIM_FILE:		return 6;
	}

	ASSERT_FALSE();

	return -1;
}

/******************************************************************************
** Method:		UpdateCacheStatus()
**
** Description:	Updates the cache status bar message.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CUTCMGRApp::UpdateCacheStatus()
{
	int    nFiles = m_oCache.RowCount();
	double dSize  = 0.0;

	// Sum file sizes...
	for (int i = 0; i < nFiles; ++i)
		dSize += m_oCache[i][CCache::FILE_SIZE].GetInt();

	// Format and display it.
	App.m_strDefStatus.Format("Total: %d Files - %.2f MB", nFiles, dSize / (1024.0*1024.0));

	App.m_AppWnd.m_StatusBar.Hint(App.m_strDefStatus);
}
