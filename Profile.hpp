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
	char  GetFileType(const CString& strExt);
	CPath GetTypeDir(char cType);

	//
	// Members.
	//
	CString	m_strName;			// The profile name.

	CPath	m_strCacheDir;		// The cache directory.
	bool	m_bReadOnly;

	CPath	m_strSystemDir;		// System directory.
	CPath	m_strMapDir;		// Maps directory.
	CPath	m_strTextureDir;	// Textures directory.
	CPath	m_strSoundDir;		// Sounds directory.
	CPath	m_strMusicDir;		// Music directory.

	CPath	m_strConfigFile;	// UT Config file.

	//
	// Constants.
	//
	static const char* DEF_PROFILE_NAME;

	static const char* DEF_ROOT_DIR;

	static const char* DEF_CACHE_DIR;

	static const char* DEF_CACHE_IDX_FILE;
	static const char* DEF_CACHE_FILE_EXT;

	static const char* DEF_CACHE_TMP_DIR;
	static const char* DEF_CACHE_TMP_MASK;

	static const char* DEF_SYSTEM_DIR;
	static const char* DEF_MAPS_DIR;
	static const char* DEF_TEXTURES_DIR;
	static const char* DEF_SOUNDS_DIR;
	static const char* DEF_MUSIC_DIR;

	static const char* DEF_CONFIG_FILE;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //PROFILE_HPP
