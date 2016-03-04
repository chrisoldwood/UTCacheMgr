/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		UTCMGRAPP.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CUTCMGRApp class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "UTCMGRApp.hpp"
#include <Core/Algorithm.hpp>

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
const tchar* CUTCMGRApp::VERSION = TXT("v2.5 [Debug]");
#else
const tchar* CUTCMGRApp::VERSION = TXT("v2.5");
#endif
const tchar* CUTCMGRApp::INI_FILE_VER_10 = TXT("1.0");
const tchar* CUTCMGRApp::INI_FILE_VER_20 = TXT("2.0");
const tchar* CUTCMGRApp::INI_FILE_VER_25 = TXT("2.5");

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
	, m_AppWnd(m_MainThread, m_AppCmds)
	, m_AppCmds(m_AppWnd)
	, m_oHelpFile(m_AppWnd)
	, m_nModified(NONE)
	, m_oCache()
	, m_pProfile(NULL)
	, m_bScanOnStart(true)
	, m_bScanOnSwitch(true)
	, m_bScanForTmp(true)
	, m_bScanIndex(true)
	, m_bShowAllFiles(false)
	, m_bLogEdits(true)
	, m_bIgnoreDates(true)
{
	// Set the help file path.
	m_oHelpFile.m_HelpFile  = CPath(CPath::ApplicationDir(), TXT("UTCacheMgr.hlp"));
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
	Core::deleteAll(m_aoProfiles);
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
	m_strTitle = TXT("UT Cache Manager");

	// Load settings.
	LoadConfig();
	
	// Create the main window.
	if (!m_AppWnd.Create())
		return false;

	// Show it.
	if (!m_rcLastWndPos.Empty())
		m_AppWnd.Move(m_rcLastWndPos);

	m_AppWnd.Show(m_iCmdShow);

	// Initialise UI.
	BuildProfileMenu();
	m_AppCmds.InitialiseUI();

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
	CString strVer = m_oIniFile.ReadString(TXT("Version"), TXT("Version"), TXT(""));

	// If first run, mark all settings as modified.
	if (strVer == TXT(""))
		m_nModified = SETTINGS | PROFILES | PIN_LIST;

	// Read the cache general settings.
	m_strCacheIndex = m_oIniFile.ReadString(TXT("Cache"), TXT("Index"),        CProfile::DEF_CACHE_IDX_FILE);
	m_strCacheExt   = m_oIniFile.ReadString(TXT("Cache"), TXT("Ext"),          CProfile::DEF_CACHE_FILE_EXT);
	m_bScanOnStart  = m_oIniFile.ReadBool  (TXT("Cache"), TXT("ScanOnStart"),  m_bScanOnStart);
	m_bScanOnSwitch = m_oIniFile.ReadBool  (TXT("Cache"), TXT("ScanOnSwitch"), m_bScanOnSwitch);
	m_bScanForTmp   = m_oIniFile.ReadBool  (TXT("Cache"), TXT("ScanForTmp"),   m_bScanForTmp);
	m_bScanIndex    = m_oIniFile.ReadBool  (TXT("Cache"), TXT("ScanIndex"),    m_bScanIndex);
	m_bShowAllFiles = m_oIniFile.ReadBool  (TXT("Cache"), TXT("ShowAllFiles"), m_bShowAllFiles);
	m_bLogEdits     = m_oIniFile.ReadBool  (TXT("Cache"), TXT("LogEdits"),     m_bLogEdits);
	m_bIgnoreDates  = m_oIniFile.ReadBool  (TXT("Cache"), TXT("IgnoreDates"),  m_bIgnoreDates);

	// Read the number of profiles.
	uint nProfiles  = m_oIniFile.ReadInt(TXT("Profiles"), TXT("Count"),   0);

	// Read the profiles.
	for (uint i = 0; i < nProfiles; ++i)
	{
		CString  strSection;
		CProfile oProfile;

		// Create section name.
		strSection.Format(TXT("Profile%d"), i);

		// Read profile details.
		oProfile.m_strName       = m_oIniFile.ReadString(strSection, TXT("Name"),       TXT(""));
		oProfile.m_nFormat       = m_oIniFile.ReadInt   (strSection, TXT("Format"),     CProfile::UT_FORMAT);
		oProfile.m_strCacheDir   = m_oIniFile.ReadString(strSection, TXT("CacheDir"),   TXT(""));
		oProfile.m_bReadOnly     = m_oIniFile.ReadBool  (strSection, TXT("ReadOnly"),   false);
		oProfile.m_strSystemDir  = m_oIniFile.ReadString(strSection, TXT("SystemDir"),  TXT(""));
		oProfile.m_strMapDir     = m_oIniFile.ReadString(strSection, TXT("MapDir"),     TXT(""));
		oProfile.m_strTextureDir = m_oIniFile.ReadString(strSection, TXT("TextureDir"), TXT(""));
		oProfile.m_strSoundDir   = m_oIniFile.ReadString(strSection, TXT("SoundDir"),   TXT(""));
		oProfile.m_strMusicDir   = m_oIniFile.ReadString(strSection, TXT("MusicDir"),   TXT(""));
		oProfile.m_strMeshDir    = m_oIniFile.ReadString(strSection, TXT("MeshDir"),    TXT(""));
		oProfile.m_strAnimDir    = m_oIniFile.ReadString(strSection, TXT("AnimDir"),    TXT(""));
		oProfile.m_strKarmaDir   = m_oIniFile.ReadString(strSection, TXT("KarmaDir"),   TXT(""));
		oProfile.m_strConfigFile = m_oIniFile.ReadString(strSection, TXT("ConfigFile"), TXT(""));
		oProfile.m_strLastCopyTo = m_oIniFile.ReadString(strSection, TXT("LastCopyTo"), TXT(""));
		oProfile.m_strLastImport  = m_oIniFile.ReadString(strSection, TXT("LastImport"),  TXT(""));
		oProfile.m_strLastInstall = m_oIniFile.ReadString(strSection, TXT("LastInstall"), TXT(""));

		// If valid, add to collection.
		if (oProfile.m_strName.Length() > 0)
			m_aoProfiles.push_back(new CProfile(oProfile));
	}

	// Read the default profile.
	m_strDefProfile = m_oIniFile.ReadString(TXT("Profiles"), TXT("Default"), TXT(""));

	// Find the default profile.
	if (m_strDefProfile != TXT(""))
		m_pProfile = FindProfile(m_strDefProfile);

	// New installation?.
	if (m_pProfile == NULL)
	{
		CProfile* pProfile = NULL;

		// Look for a UT installation.
		if ((pProfile = CProfile::DetectUT()) != NULL)
			m_aoProfiles.push_back(pProfile);

		// Look for a UT2003 installation.
		if ((pProfile = CProfile::DetectUT2003()) != NULL)
			m_aoProfiles.push_back(pProfile);

		// Look for a UT2004 installation.
		if ((pProfile = CProfile::DetectUT2004()) != NULL)
			m_aoProfiles.push_back(pProfile);

		// Look for a Tactical Ops installation.
		if ((pProfile = CProfile::DetectTacOps()) != NULL)
			m_aoProfiles.push_back(pProfile);

		// If nothing detected, create a default UT one.
		if (m_aoProfiles.empty())
		{
			// Warn user.
			AlertMsg(TXT("No UT/TO/UT2003/UT2004 installation could not be detected.\n\n")
					 TXT("The inital cache scan may fail as it is performed on\n")
					 TXT("the default UT installation folder."));

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

			m_aoProfiles.push_back(m_pProfile);
		}

		ASSERT(!m_aoProfiles.empty());

		// Set the default profile.
		m_pProfile      = m_aoProfiles[0];
		m_strDefProfile = m_pProfile->m_strName;
	}

	ASSERT(m_pProfile != NULL);

	// Read the number of pinned files.
	uint nPinned = m_oIniFile.ReadInt(TXT("Pinned"), TXT("Count"), 0);

	// Read the pinned files.
	for (uint i = 0; i < nPinned; ++i)
	{
		CString strEntry;
		CString strName;

		// Create entry name.
		strEntry.Format(TXT("File%d"), i);

		strName = m_oIniFile.ReadString(TXT("Pinned"), strEntry, TXT(""));

		// Add if valid and not already listed.
		if ((strName.Length() > 0) && (m_astrPinned.Find(strName, false) == -1))
			m_astrPinned.Add(strName);
	}

	// Read the previous UI settings.
	m_rcLastWndPos = m_oIniFile.ReadRect(TXT("UI"), TXT("MainWindow"),  CRect());
	m_rcLastDlgPos = m_oIniFile.ReadRect(TXT("UI"), TXT("SelFilesDlg"), CRect());

	if ( (strVer == INI_FILE_VER_10) || (strVer == INI_FILE_VER_20) )
	{
		m_rcLastWndPos.left   = m_oIniFile.ReadInt(TXT("UI"), TXT("Left"),   0);
		m_rcLastWndPos.top    = m_oIniFile.ReadInt(TXT("UI"), TXT("Top"),    0);
		m_rcLastWndPos.right  = m_oIniFile.ReadInt(TXT("UI"), TXT("Right"),  0);
		m_rcLastWndPos.bottom = m_oIniFile.ReadInt(TXT("UI"), TXT("Bottom"), 0);
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
	m_oIniFile.WriteString(TXT("Version"), TXT("Version"), INI_FILE_VER_25);

	// Application settings changed?
	if (m_nModified & SETTINGS)
	{
		// Write the cache general settings.
		m_oIniFile.WriteString(TXT("Cache"),    TXT("Index"),        m_strCacheIndex);
		m_oIniFile.WriteString(TXT("Cache"),    TXT("Ext"),          m_strCacheExt);
		m_oIniFile.WriteBool  (TXT("Cache"),    TXT("ScanOnStart"),  m_bScanOnStart);
		m_oIniFile.WriteBool  (TXT("Cache"),    TXT("ScanOnSwitch"), m_bScanOnSwitch);
		m_oIniFile.WriteBool  (TXT("Cache"),    TXT("ScanForTmp"),   m_bScanForTmp);
		m_oIniFile.WriteBool  (TXT("Cache"),    TXT("ScanIndex"),    m_bScanIndex);
		m_oIniFile.WriteBool  (TXT("Cache"),    TXT("ShowAllFiles"), m_bShowAllFiles);
		m_oIniFile.WriteBool  (TXT("Cache"),    TXT("LogEdits"),     m_bLogEdits);
		m_oIniFile.WriteBool  (TXT("Cache"),    TXT("IgnoreDates"),  m_bIgnoreDates);
		m_oIniFile.WriteString(TXT("Profiles"), TXT("Default"),      m_strDefProfile);
	}

	// Profiles changed?
	if (m_nModified & PROFILES)
	{
		// Write the profiles.
		m_oIniFile.WriteInt(TXT("Profiles"), TXT("Count"), static_cast<int>(m_aoProfiles.size()));

		for (uint i = 0; i < m_aoProfiles.size(); ++i)
		{
			CString   strSection;
			CProfile* pProfile = m_aoProfiles[i];

			// Create section name.
			strSection.Format(TXT("Profile%d"), i);

			// Write profile details.
			m_oIniFile.WriteString(strSection, TXT("Name"),       pProfile->m_strName      );
			m_oIniFile.WriteInt   (strSection, TXT("Format"),     pProfile->m_nFormat      );
			m_oIniFile.WriteString(strSection, TXT("CacheDir"),   pProfile->m_strCacheDir  );
			m_oIniFile.WriteBool  (strSection, TXT("ReadOnly"),   pProfile->m_bReadOnly    );
			m_oIniFile.WriteString(strSection, TXT("SystemDir"),  pProfile->m_strSystemDir );
			m_oIniFile.WriteString(strSection, TXT("MapDir"),     pProfile->m_strMapDir    );
			m_oIniFile.WriteString(strSection, TXT("TextureDir"), pProfile->m_strTextureDir);
			m_oIniFile.WriteString(strSection, TXT("SoundDir"),   pProfile->m_strSoundDir  );
			m_oIniFile.WriteString(strSection, TXT("MusicDir"),   pProfile->m_strMusicDir  );
			m_oIniFile.WriteString(strSection, TXT("MeshDir"),    pProfile->m_strMeshDir   );
			m_oIniFile.WriteString(strSection, TXT("AnimDir"),    pProfile->m_strAnimDir   );
			m_oIniFile.WriteString(strSection, TXT("KarmaDir"),   pProfile->m_strKarmaDir  );
			m_oIniFile.WriteString(strSection, TXT("ConfigFile"), pProfile->m_strConfigFile);
			m_oIniFile.WriteString(strSection, TXT("LastCopyTo"), pProfile->m_strLastCopyTo);
			m_oIniFile.WriteString(strSection, TXT("LastImport"),  pProfile->m_strLastImport);
			m_oIniFile.WriteString(strSection, TXT("LastInstall"), pProfile->m_strLastInstall);
		}
	}

	// Pinned files list changed?
	if (m_nModified & PIN_LIST)
	{
		// Write the list of pinned files.
		m_oIniFile.DeleteSection(TXT("Pinned"));
		m_oIniFile.WriteInt (TXT("Pinned"), TXT("Count"), static_cast<int>(m_astrPinned.Size()));

		for (size_t i = 0; i < m_astrPinned.Size(); ++i)
		{
			CString strEntry;

			// Create entry name.
			strEntry.Format(TXT("File%d"), i);

			m_oIniFile.WriteString(TXT("Pinned"), strEntry, m_astrPinned[i]);
		}
	}

	// Save the current UI settings.
	m_oIniFile.WriteRect(TXT("UI"), TXT("MainWindow"),  m_rcLastWndPos);
	m_oIniFile.WriteRect(TXT("UI"), TXT("SelFilesDlg"), m_rcLastDlgPos);
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

CProfile* CUTCMGRApp::FindProfile(const tchar* pszName) const
{
	ASSERT(pszName != NULL);

	// For all profiles...
	for (uint i = 0; i < m_aoProfiles.size(); ++i)
	{
		if (m_aoProfiles[i]->m_strName.Compare(pszName, true) == 0)
			return m_aoProfiles[i];
	}

	return NULL;
}

/******************************************************************************
** Method:		FindProfileByCfgFile()
**
** Description:	Finds a profile by its config (.ini) file.
**
** Parameters:	strCfgFile.
**
** Returns:		The profile or NULL.
**
*******************************************************************************
*/

CProfile* CUTCMGRApp::FindProfileByCfgFile(const CPath& strCfgFile) const
{
	// For all profiles...
	for (uint i = 0; i < m_aoProfiles.size(); ++i)
	{
		if (m_aoProfiles[i]->m_strConfigFile.Compare(strCfgFile, true) == 0)
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
	for (uint i = 0; i < m_aoProfiles.size(); ++i)
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
	std::sort(m_aoProfiles.begin(), m_aoProfiles.end(), CProfile::Compare);

	// Delete old menu.
	for (int i = ID_CACHE_FIRST_PROFILE; i <= ID_CACHE_LAST_PROFILE; ++i)
		App.m_AppWnd.m_Menu.RemoveCmd(i);

	CPopupMenu oCacheMenu = App.m_AppWnd.m_Menu.GetItemPopup(0).GetItemPopup(0);

	// Build new menu.
	for (uint i = 0; i < m_aoProfiles.size(); ++i)
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

CString CUTCMGRApp::FormatType(tchar cType) const
{
	switch (cType)
	{
		case SYSTEM_FILE:	return TXT("System");
		case MAP_FILE:		return TXT("Map");
		case TEXTURE_FILE:	return TXT("Texture");
		case SOUND_FILE:	return TXT("Sound");
		case MUSIC_FILE:	return TXT("Music");
		case MESH_FILE:		return TXT("Mesh");
		case ANIM_FILE:		return TXT("Anim");
		case KARMA_FILE:	return TXT("Karma");
	}

	ASSERT_FALSE();

	return TXT("");
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
	nSize = std::max(1024, nSize);

	str.Format(TXT("%d K"), nSize/1024);

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

CString CUTCMGRApp::FormatStatus(tchar cStatus) const
{
	switch (cStatus)
	{
		case NEW_FILE :	return TXT("New");
		case OLD_FILE :	return TXT("Old");
		case PIN_FILE :	return TXT("Pinned");
	}

	ASSERT_FALSE();

	return TXT("");
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

int CUTCMGRApp::IconIndex(tchar cType) const
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
		case KARMA_FILE:	return 7;
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
	size_t nFiles = m_oCache.RowCount();
	double dSize  = 0.0;

	// Sum file sizes...
	for (size_t i = 0; i < nFiles; ++i)
		dSize += m_oCache[i][CCache::FILE_SIZE].GetInt();

	// Format and display it.
	App.m_strDefStatus.Format(TXT("Total: %d Files - %.2f MB"), nFiles, dSize / (1024.0*1024.0));

	App.m_AppWnd.m_StatusBar.Hint(App.m_strDefStatus);
}
