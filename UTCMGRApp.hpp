/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		UTCMGR.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CUTCMGRApp class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef UTCMGR_HPP
#define UTCMGR_HPP

// Template shorthands.
typedef TPtrArray<CProfile> CProfiles;

/******************************************************************************
** 
** The application class.
**
*******************************************************************************
*/

class CUTCMGRApp : public CApp
{
public:
	//
	// Constructors/Destructor.
	//
	CUTCMGRApp();
	~CUTCMGRApp();

	//
	// Members
	//
	CAppWnd		m_AppWnd;			// Main window.
	CAppCmds	m_AppCmds;			// Command handler.

	CMDB		m_oMDB;				// Memory database.
	CCache		m_oCache;			// MDB Cache table.

	CProfiles	m_aoProfiles;		// The list of profiles.
	CProfile*	m_pProfile;			// Current profile.
	CString		m_strDefProfile;	// Default profile.

	CString		m_strCacheIndex;	// Cache index file name.
	CString		m_strCacheExt;		// Cache file extensions.

	bool		m_bScanOnStart;		// Scan on startup?
	bool		m_bScanOnSwitch;	// Scan when switching profiles?
	bool		m_bScanForTmp;		// Scan for old .tmp files?
	bool		m_bScanIndex;		// Scan index for old entries?
	bool		m_bShowAllFiles;	// Show all files?
	CPath		m_strLastCopyTo;	// Last "Copy To..." path.

	CRect		m_rcLastPos;		// Main window position.

	//
	// Methods.
	//
	CProfile* FindProfile(const char* pszName) const;

protected:
	//
	// Startup and Shutdown template methods.
	//
	virtual	bool OnOpen();
	virtual	bool OnClose();

	//
	// Preferences.
	//
	CIniFile	m_oIniFile;		// .INI FIle

	//
	// Internal methods.
	//
	void LoadConfig();
	void SaveConfig();

	//
	// Constants.
	//
	static const char* INI_FILE_VER;
};

/******************************************************************************
**
** Global variables.
**
*******************************************************************************
*/

// The application object.
extern CUTCMGRApp App;

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/


#endif //UTCMGR_HPP
