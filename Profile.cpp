/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PROFILE.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CProfile class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "Profile.hpp"
#include "Cache.hpp"
#include <WCL/RegKey.hpp>

/******************************************************************************
**
** Class constants.
**
*******************************************************************************
*/

const tchar* CProfile::DEF_UT_PROFILE_NAME     = TXT("UT");
const tchar* CProfile::DEF_UT2003_PROFILE_NAME = TXT("UT2003");
const tchar* CProfile::DEF_UT2004_PROFILE_NAME = TXT("UT2004");
const tchar* CProfile::DEF_TO_PROFILE_NAME     = TXT("Tactical Ops");

const tchar* CProfile::DEF_ROOT_DIR       = TXT("C:\\UnrealTournament");
const tchar* CProfile::DEF_CACHE_DIR      = TXT("Cache");
const tchar* CProfile::DEF_CACHE_IDX_FILE = TXT("cache.ini");
const tchar* CProfile::DEF_CACHE_FILE_EXT = TXT("uxx");
const tchar* CProfile::DEF_SYSTEM_DIR     = TXT("System");
const tchar* CProfile::DEF_MAPS_DIR       = TXT("Maps");
const tchar* CProfile::DEF_TEXTURES_DIR   = TXT("Textures");
const tchar* CProfile::DEF_SOUNDS_DIR     = TXT("Sounds");
const tchar* CProfile::DEF_MUSIC_DIR      = TXT("Music");
const tchar* CProfile::DEF_CONFIG_FILE    = TXT("UnrealTournament.ini");
const tchar* CProfile::DEF_CACHE_TMP_DIR  = TXT("..\\.");
const tchar* CProfile::DEF_CACHE_TMP_MASK = TXT("????.tmp");

const tchar* CProfile::DEF_MESH_DIR         = TXT("StaticMeshes");
const tchar* CProfile::DEF_ANIM_DIR         = TXT("Animations");
const tchar* CProfile::DEF_KARMA_DIR        = TXT("KarmaData");
const tchar* CProfile::DEF_2003_CONFIG_FILE = TXT("UT2003.ini");

const tchar* CProfile::DEF_2004_CONFIG_FILE = TXT("UT2004.ini");

const tchar* CProfile::DEF_TO_CONFIG_FILE   = TXT("TacticalOps.ini");

const tchar* CProfile::s_pszFormats[3] = { TXT("UT"), TXT("UT2003"), TXT("UT2004") };

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
	return ( (strExt == TXT(".u")  ) || (strExt == TXT(".int"))
		  || (strExt == TXT(".unr")) || (strExt == TXT(".ut2"))
		  || (strExt == TXT(".utx")) || (strExt == TXT(".uax"))
		  || (strExt == TXT(".umx")) || (strExt == TXT(".ogg"))
		  || (strExt == TXT(".usx")) || (strExt == TXT(".ukx"))
		  || (strExt == TXT(".ka") )  );
}

/******************************************************************************
** Method:		GetFileType()
**
** Description:	Gets the file type from the files extension.
**
** Parameters:	strExt		The file extension.
**
** Returns:		The file type or UNKNOWN_FILE.
**
*******************************************************************************
*/

tchar CProfile::GetFileType(const CString& strExt)
{
	if ((strExt == TXT(".u")) || (strExt == TXT(".int")) || (strExt == TXT("")))
	{
		return SYSTEM_FILE;
	}
	else if ((strExt == TXT(".unr")) || (strExt == TXT(".ut2")))
	{
		return MAP_FILE;
	}
	else if (strExt == TXT(".utx"))
	{
		return TEXTURE_FILE;
	}
	else if (strExt == TXT(".uax"))
	{
		return SOUND_FILE;
	}
	else if ((strExt == TXT(".umx")) || (strExt == TXT(".ogg")))
	{
		return MUSIC_FILE;
	}
	else if (strExt == TXT(".usx"))
	{
		return MESH_FILE;
	}
	else if (strExt == TXT(".ukx"))
	{
		return ANIM_FILE;
	}
	else if (strExt == TXT(".ka"))
	{
		return KARMA_FILE;
	}

	ASSERT_FALSE();

	return UNKNOWN_FILE;
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

CPath CProfile::GetTypeDir(tchar cType)
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

	return TXT("");
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
	ASSERT((pProfile1 != nullptr) && (pProfile2 != nullptr));

	return (tstrcmp((pProfile1)->m_strName, (pProfile2)->m_strName) < 0);
}

/******************************************************************************
** Method:		DetectUT()
**
** Description:	Attempt to detect a UT installation.
**
** Parameters:	None.
**
** Returns:		A profile or nullptr, if none found.
**
*******************************************************************************
*/

CProfile* CProfile::DetectUT()
{
	WCL::RegKey oKey;

	// Try and find the regkey that contains the UT base path.
	if (!WCL::RegKey::Exists(HKEY_LOCAL_MACHINE, TXT("SOFTWARE\\Unreal Technology\\Installed Apps\\UnrealTournament")))
		return nullptr;

	oKey.Open(HKEY_LOCAL_MACHINE, TXT("SOFTWARE\\Unreal Technology\\Installed Apps\\UnrealTournament"), KEY_READ);

	// Get the UT base path.
	CPath strBaseDir = oKey.ReadStringValue(TXT("Folder"), TXT(""));

	if (strBaseDir == TXT(""))
		return nullptr;

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
** Returns:		A profile or nullptr, if none found.
**
*******************************************************************************
*/

CProfile* CProfile::DetectUT2003()
{
	WCL::RegKey oKey;

	// Try and find the regkey that contains the UT2003 base path.
	if (!WCL::RegKey::Exists(HKEY_LOCAL_MACHINE, TXT("SOFTWARE\\Unreal Technology\\Installed Apps\\UT2003")))
		return nullptr;

	oKey.Open(HKEY_LOCAL_MACHINE, TXT("SOFTWARE\\Unreal Technology\\Installed Apps\\UT2003"), KEY_READ);

	// Get the UT2003 base path.
	CPath strBaseDir = oKey.ReadStringValue(TXT("Folder"), TXT(""));

	if (strBaseDir == TXT(""))
		return nullptr;

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
** Returns:		A profile or nullptr, if none found.
**
*******************************************************************************
*/

CProfile* CProfile::DetectUT2004()
{
	WCL::RegKey oKey;

	// Try and find the regkey that contains the UT2004 base path.
	if (!WCL::RegKey::Exists(HKEY_LOCAL_MACHINE, TXT("SOFTWARE\\Unreal Technology\\Installed Apps\\UT2004")))
		return nullptr;

	oKey.Open(HKEY_LOCAL_MACHINE, TXT("SOFTWARE\\Unreal Technology\\Installed Apps\\UT2004"), KEY_READ);

	// Get the UT2004 base path.
	CPath strBaseDir = oKey.ReadStringValue(TXT("Folder"), TXT(""));

	if (strBaseDir == TXT(""))
		return nullptr;

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
** Returns:		A profile or nullptr, if none found.
**
*******************************************************************************
*/

CProfile* CProfile::DetectTacOps()
{
	WCL::RegKey oKey;

	// Try and find the regkey that contains the TO base path.
	if (!WCL::RegKey::Exists(HKEY_LOCAL_MACHINE, TXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Tactical Ops")))
		return nullptr;

	oKey.Open(HKEY_LOCAL_MACHINE, TXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Tactical Ops"), KEY_READ);

	// Get the TO base path.
	CPath strBaseDir = oKey.ReadStringValue(TXT("InstallLocation"), TXT(""));

	if (strBaseDir == TXT(""))
		return nullptr;

	CProfile* pProfile = new CProfile();

	// Create a profile for TO.
	pProfile->m_strName       = DEF_TO_PROFILE_NAME;
	pProfile->m_nFormat       = UT_FORMAT;
	pProfile->m_strCacheDir   = CPath(strBaseDir, DEF_CACHE_DIR   );
	pProfile->m_bReadOnly     = false;
	pProfile->m_strConfigFile = CPath(strBaseDir / DEF_SYSTEM_DIR, DEF_TO_CONFIG_FILE);

	strBaseDir /= TXT("TacticalOps");

	pProfile->m_strSystemDir  = CPath(strBaseDir, DEF_SYSTEM_DIR  );
	pProfile->m_strMapDir     = CPath(strBaseDir, DEF_MAPS_DIR    );
	pProfile->m_strTextureDir = CPath(strBaseDir, DEF_TEXTURES_DIR);
	pProfile->m_strSoundDir   = CPath(strBaseDir, DEF_SOUNDS_DIR  );
	pProfile->m_strMusicDir   = CPath(strBaseDir, DEF_MUSIC_DIR   );

	return pProfile;
}
