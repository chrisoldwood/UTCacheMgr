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

/******************************************************************************
**
** Class constants.
**
*******************************************************************************
*/

// Defaults for UT.
const char* CProfile::DEF_PROFILE_NAME   = "Default";
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
const char* CProfile::DEF_CACHE_TMP_MASK = "Cache????.tmp";

// Additional defaults for UT2003.
const char* CProfile::DEF_2003_CONFIG_FILE = "UT2003.ini";


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
	: m_bReadOnly(false)
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
	else if (strExt == ".umx")
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

	ASSERT(false);

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
	}

	ASSERT(false);

	return "";
}
