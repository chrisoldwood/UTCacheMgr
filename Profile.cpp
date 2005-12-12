/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PROFILE.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CProfile class definition.
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
** Class constants.
**
*******************************************************************************
*/

const char* CProfile::DEF_UT_PROFILE_NAME     = "UT";
const char* CProfile::DEF_UT2003_PROFILE_NAME = "UT2003";
const char* CProfile::DEF_UT2004_PROFILE_NAME = "UT2004";
const char* CProfile::DEF_TO_PROFILE_NAME     = "Tactical Ops";

const char* CProfile::DEF_ROOT_DIR       = "C:\\UnrealTournament";
const char* CProfile::DEF_CACHE_DIR      = "Cache";
const char* CProfile::DEF_CACHE_IDX_FILE = "cache.ini";
const char* CProfile::DEF_CACHE_FILE_EXT = "uxx";
const char* CProfile::DEF_SYSTEM_DIR     = "System";
const char* CProfile::DEF_MAPS_DIR       = "Maps";
const char* CProfile::DEF_TEXTURES_DIR   = "Textures";
const char* CProfile::DEF_SOUNDS_DIR     = "Sounds";
const char* CProfile::DEF_MUSIC_DIR      = "Music";
const char* CProfile::DEF_CONFIG_FILE    = "UnrealTournament.ini";
const char* CProfile::DEF_CACHE_TMP_DIR  = "..\\.";
const char* CProfile::DEF_CACHE_TMP_MASK = "????.tmp";

const char* CProfile::DEF_MESH_DIR         = "StaticMeshes";
const char* CProfile::DEF_ANIM_DIR         = "Animations";
const char* CProfile::DEF_KARMA_DIR        = "KarmaData";
const char* CProfile::DEF_2003_CONFIG_FILE = "UT2003.ini";

const char* CProfile::DEF_2004_CONFIG_FILE = "UT2004.ini";

const char* CProfile::DEF_TO_CONFIG_FILE   = "TacticalOps.ini";

const char* CProfile::s_pszFormats[3] = { "UT", "UT2003", "UT2004" };

/******************************************************************************
** Method:		Constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CProfile::CProfile()
	: m_nFormat(UT_FORMAT)
	, m_bReadOnly(false)
{
}

/******************************************************************************
** Method:		Destructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CProfile::~CProfile()
{
}

/******************************************************************************
** Method:		IsValidType()
**
** Description:	Queries if the file extension is a valid UT file extension.
**
** Parameters:	strExt		The file extension.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CProfile::IsValidType(const CString& strExt)
{
	return ( (strExt == ".u"  ) || (strExt == ".int")
		  || (strExt == ".unr") || (strExt == ".ut2")
		  || (strExt == ".utx") || (strExt == ".uax")
		  || (strExt == ".umx") || (strExt == ".ogg")
		  || (strExt == ".usx") || (strExt == ".ukx")
		  || (strExt == ".ka" )  );
}

/******************************************************************************
** Method:		GetFileType()
**
** Description:	Gets the file type from the files extension.
**
** Parameters:	strExt		The file extension.
**
** Returns:		The file type or NULL.
**
*******************************************************************************
*/

char CProfile::GetFileType(const CString& strExt)
{
	if ((strExt == ".u") || (strExt == ".int") || (strExt == ""))
	{
		return SYSTEM_FILE;
	}
	else if ((strExt == ".unr") || (strExt == ".ut2"))
	{
		return MAP_FILE;
	}
	else if (strExt == ".utx")
	{
		return TEXTURE_FILE;
	}
	else if (strExt == ".uax")
	{
		return SOUND_FILE;
	}
	else if ((strExt == ".umx") || (strExt == ".ogg"))
	{
		return MUSIC_FILE;
	}
	else if (strExt == ".usx")
	{
		return MESH_FILE;
	}
	else if (strExt == ".ukx")
	{
		return ANIM_FILE;
	}
	else if (strExt == ".ka")
	{
		return KARMA_FILE;
	}

	ASSERT_FALSE();

	return NULL;
}

/******************************************************************************
** Method:		GetTypeDir()
**
** Description:	Gets the directory which stores the given file type.
**
** Parameters:	cType	The file type.
**
** Returns:		The directory.
**
*******************************************************************************
*/

CPath CProfile::GetTypeDir(char cType)
{
	switch (cType)
	{
		case SYSTEM_FILE:	return m_strSystemDir;
		case MAP_FILE:		return m_strMapDir;
		case TEXTURE_FILE:	return m_strTextureDir;
		case SOUND_FILE:	return m_strSoundDir;
		case MUSIC_FILE:	return m_strMusicDir;
		case MESH_FILE:		return m_strMeshDir;
		case ANIM_FILE:		return m_strAnimDir;
		case KARMA_FILE:	return m_strKarmaDir;
	}

	ASSERT_FALSE();

	return "";
}

/******************************************************************************
** Method:		Compare()
**
** Description:	Compare function used to sort the Profiles collection.
**
** Parameters:	See qsort().
**
** Returns:		See qsort().
**
*******************************************************************************
*/

bool CProfile::Compare(const CProfile* pProfile1, const CProfile* pProfile2)
{
	ASSERT((pProfile1 != NULL) && (pProfile2 != NULL));

	return (strcmp((pProfile1)->m_strName, (pProfile2)->m_strName) < 0);
}

/******************************************************************************
** Method:		DetectUT()
**
** Description:	Attempt to detect a UT installation.
**
** Parameters:	None.
**
** Returns:		A profile or NULL, if none found.
**
*******************************************************************************
*/

CProfile* CProfile::DetectUT()
{
	CRegKey oKey;

	// Try and find the regkey that contains the UT base path.
	if (!oKey.Open(HKEY_LOCAL_MACHINE, "SOFTWARE\\Unreal Technology\\Installed Apps\\UnrealTournament"))
		return NULL;

	// Get the UT base path.
	CPath strBaseDir = oKey.QueryString("Folder", "");

	if (strBaseDir == "")
		return NULL;

	CProfile* pProfile = new CProfile();

	// Create a profile for UT.
	pProfile->m_strName       = DEF_UT_PROFILE_NAME;
	pProfile->m_nFormat       = UT_FORMAT;
	pProfile->m_strCacheDir   = CPath(strBaseDir, DEF_CACHE_DIR   );
	pProfile->m_bReadOnly     = false;
	pProfile->m_strSystemDir  = CPath(strBaseDir, DEF_SYSTEM_DIR  );
	pProfile->m_strMapDir     = CPath(strBaseDir, DEF_MAPS_DIR    );
	pProfile->m_strTextureDir = CPath(strBaseDir, DEF_TEXTURES_DIR);
	pProfile->m_strSoundDir   = CPath(strBaseDir, DEF_SOUNDS_DIR  );
	pProfile->m_strMusicDir   = CPath(strBaseDir, DEF_MUSIC_DIR   );
	pProfile->m_strConfigFile = CPath(pProfile->m_strSystemDir, DEF_CONFIG_FILE);

	return pProfile;
}

/******************************************************************************
** Method:		DetectUT2003()
**
** Description:	Attempt to detect a UT2003 installation.
**
** Parameters:	None.
**
** Returns:		A profile or NULL, if none found.
**
*******************************************************************************
*/

CProfile* CProfile::DetectUT2003()
{
	CRegKey oKey;

	// Try and find the regkey that contains the UT2003 base path.
	if (!oKey.Open(HKEY_LOCAL_MACHINE, "SOFTWARE\\Unreal Technology\\Installed Apps\\UT2003"))
		return NULL;

	// Get the UT2003 base path.
	CPath strBaseDir = oKey.QueryString("Folder", "");

	if (strBaseDir == "")
		return NULL;

	CProfile* pProfile = new CProfile();

	// Create a profile for UT2003.
	pProfile->m_strName       = DEF_UT2003_PROFILE_NAME;
	pProfile->m_nFormat       = UT2003_FORMAT;
	pProfile->m_strCacheDir   = CPath(strBaseDir, DEF_CACHE_DIR   );
	pProfile->m_bReadOnly     = false;
	pProfile->m_strSystemDir  = CPath(strBaseDir, DEF_SYSTEM_DIR  );
	pProfile->m_strMapDir     = CPath(strBaseDir, DEF_MAPS_DIR    );
	pProfile->m_strTextureDir = CPath(strBaseDir, DEF_TEXTURES_DIR);
	pProfile->m_strSoundDir   = CPath(strBaseDir, DEF_SOUNDS_DIR  );
	pProfile->m_strMusicDir   = CPath(strBaseDir, DEF_MUSIC_DIR   );
	pProfile->m_strMeshDir    = CPath(strBaseDir, DEF_MESH_DIR    );
	pProfile->m_strAnimDir    = CPath(strBaseDir, DEF_ANIM_DIR    );
	pProfile->m_strKarmaDir   = CPath(strBaseDir, DEF_KARMA_DIR   );
	pProfile->m_strConfigFile = CPath(pProfile->m_strSystemDir, DEF_2003_CONFIG_FILE);

	return pProfile;
}

/******************************************************************************
** Method:		DetectUT2004()
**
** Description:	Attempt to detect a UT2004 installation.
**
** Parameters:	None.
**
** Returns:		A profile or NULL, if none found.
**
*******************************************************************************
*/

CProfile* CProfile::DetectUT2004()
{
	CRegKey oKey;

	// Try and find the regkey that contains the UT2004 base path.
	if (!oKey.Open(HKEY_LOCAL_MACHINE, "SOFTWARE\\Unreal Technology\\Installed Apps\\UT2004"))
		return NULL;

	// Get the UT2004 base path.
	CPath strBaseDir = oKey.QueryString("Folder", "");

	if (strBaseDir == "")
		return NULL;

	CProfile* pProfile = new CProfile();

	// Create a profile for UT2004.
	pProfile->m_strName       = DEF_UT2004_PROFILE_NAME;
	pProfile->m_nFormat       = UT2004_FORMAT;
	pProfile->m_strCacheDir   = CPath(strBaseDir, DEF_CACHE_DIR   );
	pProfile->m_bReadOnly     = false;
	pProfile->m_strSystemDir  = CPath(strBaseDir, DEF_SYSTEM_DIR  );
	pProfile->m_strMapDir     = CPath(strBaseDir, DEF_MAPS_DIR    );
	pProfile->m_strTextureDir = CPath(strBaseDir, DEF_TEXTURES_DIR);
	pProfile->m_strSoundDir   = CPath(strBaseDir, DEF_SOUNDS_DIR  );
	pProfile->m_strMusicDir   = CPath(strBaseDir, DEF_MUSIC_DIR   );
	pProfile->m_strMeshDir    = CPath(strBaseDir, DEF_MESH_DIR    );
	pProfile->m_strAnimDir    = CPath(strBaseDir, DEF_ANIM_DIR    );
	pProfile->m_strKarmaDir   = CPath(strBaseDir, DEF_KARMA_DIR   );
	pProfile->m_strConfigFile = CPath(pProfile->m_strSystemDir, DEF_2004_CONFIG_FILE);

	return pProfile;
}

/******************************************************************************
** Method:		DetectTacOps()
**
** Description:	Attempt to detect a Tactical Ops installation.
**
** Parameters:	None.
**
** Returns:		A profile or NULL, if none found.
**
*******************************************************************************
*/

CProfile* CProfile::DetectTacOps()
{
	CRegKey oKey;

	// Try and find the regkey that contains the TO base path.
	if (!oKey.Open(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Tactical Ops"))
		return NULL;

	// Get the TO base path.
	CPath strBaseDir = oKey.QueryString("InstallLocation", "");

	if (strBaseDir == "")
		return NULL;

	CProfile* pProfile = new CProfile();

	// Create a profile for TO.
	pProfile->m_strName       = DEF_TO_PROFILE_NAME;
	pProfile->m_nFormat       = UT_FORMAT;
	pProfile->m_strCacheDir   = CPath(strBaseDir, DEF_CACHE_DIR   );
	pProfile->m_bReadOnly     = false;
	pProfile->m_strConfigFile = CPath(strBaseDir + DEF_SYSTEM_DIR, DEF_TO_CONFIG_FILE);

	strBaseDir += "TacticalOps";

	pProfile->m_strSystemDir  = CPath(strBaseDir, DEF_SYSTEM_DIR  );
	pProfile->m_strMapDir     = CPath(strBaseDir, DEF_MAPS_DIR    );
	pProfile->m_strTextureDir = CPath(strBaseDir, DEF_TEXTURES_DIR);
	pProfile->m_strSoundDir   = CPath(strBaseDir, DEF_SOUNDS_DIR  );
	pProfile->m_strMusicDir   = CPath(strBaseDir, DEF_MUSIC_DIR   );

	return pProfile;
}
