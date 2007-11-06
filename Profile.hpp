/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PROFILE.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CProfile class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef PROFILE_HPP
#define PROFILE_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/Path.hpp>

/******************************************************************************
** 
** Data class used to store a profiles details.
**
*******************************************************************************
*/

class CProfile
{
public:
	//
	// Constructors/Destructor.
	//
	CProfile();
	~CProfile();
	
	//
	// Methods.
	//
	CPath GetTypeDir(char cType);

	static bool IsValidType(const CString& strExt);
	static char GetFileType(const CString& strExt);

	//
	// Helper methods.
	//
	static bool Compare(const CProfile* pProfile1, const CProfile* pProfile2);

	static CProfile* DetectUT();
	static CProfile* DetectUT2003();
	static CProfile* DetectUT2004();
	static CProfile* DetectTacOps();

	//
	// Members.
	//
	CString	m_strName;			// The profile name.
	int		m_nFormat;			// Cache format.

	CPath	m_strCacheDir;		// The cache directory.
	bool	m_bReadOnly;

	CPath	m_strSystemDir;		// System directory.
	CPath	m_strMapDir;		// Maps directory.
	CPath	m_strTextureDir;	// Textures directory.
	CPath	m_strSoundDir;		// Sounds directory.
	CPath	m_strMusicDir;		// Music directory.
	CPath	m_strMeshDir;		// StaticMeshes directory.
	CPath	m_strAnimDir;		// Animations directory.
	CPath	m_strKarmaDir;		// Karma Data directory.

	CPath	m_strConfigFile;	// UT Config file.

	CPath	m_strLastCopyTo;	// Last "Copy To..." path.
	CPath	m_strLastImport;	// Last "Import..." path.
	CPath	m_strLastInstall;	// Last "Install..." path.

	//
	// Constants.
	//

	// Default profiles.
	static const char* DEF_UT_PROFILE_NAME;
	static const char* DEF_UT2003_PROFILE_NAME;
	static const char* DEF_UT2004_PROFILE_NAME;
	static const char* DEF_TO_PROFILE_NAME;

	static const char* DEF_ROOT_DIR;

	// Cache folder settings.
	static const char* DEF_CACHE_DIR;

	static const char* DEF_CACHE_IDX_FILE;
	static const char* DEF_CACHE_FILE_EXT;

	static const char* DEF_CACHE_TMP_DIR;
	static const char* DEF_CACHE_TMP_MASK;

	// Main folder settings.
	static const char* DEF_SYSTEM_DIR;
	static const char* DEF_MAPS_DIR;
	static const char* DEF_TEXTURES_DIR;
	static const char* DEF_SOUNDS_DIR;
	static const char* DEF_MUSIC_DIR;

	static const char* DEF_CONFIG_FILE;

	// UT2003 specific settings.
	static const char* DEF_MESH_DIR;
	static const char* DEF_ANIM_DIR;
	static const char* DEF_KARMA_DIR;
	static const char* DEF_2003_CONFIG_FILE;

	// UT2004 specific settings.
	static const char* DEF_2004_CONFIG_FILE;

	// Tactical Ops specific settings.
	static const char* DEF_TO_CONFIG_FILE;

	// Cache formats.
	enum Format
	{
		UT_FORMAT     = 0,
		UT2003_FORMAT = 1,
		UT2004_FORMAT = 2,

		NUM_FORMATS	  = 3,
	};

	static const char* s_pszFormats[3];
};

// Template shorthands.
typedef std::vector<CProfile*> CProfiles;

#endif //PROFILE_HPP
