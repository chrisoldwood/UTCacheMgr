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
	static int Compare(const CProfile** ppProfile1, const CProfile** ppProfile2);

	static CProfile* DetectUT();
	static CProfile* DetectUT2003();
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

	// Tactical Ops specific settings.
	static const char* DEF_TO_CONFIG_FILE;

	// Cache formats.
	static const int UT_FORMAT;
	static const int UT2003_FORMAT;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //PROFILE_HPP
