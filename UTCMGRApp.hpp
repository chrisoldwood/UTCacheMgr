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
	CHelpFile	m_oHelpFile;		// The help file.
	CIniFile	m_oIniFile;			// Config File.
	uint		m_nModified;		// Config modified flags.

	CMDB		m_oMDB;				// Memory database.
	CCache		m_oCache;			// MDB Cache table.

	CProfiles	m_aoProfiles;		// The list of profiles.
	CProfile*	m_pProfile;			// Current profile.
	CString		m_strDefProfile;	// Default profile.

	CString		m_strCacheIndex;	// Cache index file name.
	CString		m_strCacheExt;		// Cache file extensions.

	CStrArray	m_astrPinned;		// The list of pinned files.

	bool		m_bScanOnStart;		// Scan on startup?
	bool		m_bScanOnSwitch;	// Scan when switching profiles?
	bool		m_bScanForTmp;		// Scan for old .tmp files?
	bool		m_bScanIndex;		// Scan index for old entries?
	bool		m_bShowAllFiles;	// Show all files?
	bool		m_bLogEdits;		// Log edit operations to a file?

	CRect		m_rcLastWndPos;		// Main window position.
	CRect		m_rcLastDlgPos;		// Import/Restore dialog position.

	CString		m_strDefStatus;		// The default status bar text.

	//
	// Profile methods.
	//
	CProfile* FindProfile(const char* pszName) const;
	int       GetProfileIndex(CProfile* pProfile) const;
	void      BuildProfileMenu();

	//
	// Formatting methods.
	//
	CString FormatType(char cType) const;
	CString FormatSize(int nSize) const;
	CString FormatStatus(char cStatus) const;
	int     IconIndex(char cType) const;

	//
	// Utility methods.
	//
	void UpdateCacheStatus();

	//
	// Constants.
	//
	static const char* VERSION;

	//
	// Modified flags.
	//
	enum
	{
		NONE	 = 0x0000,
		SETTINGS = 0x0001,
		PROFILES = 0x0002,
		PIN_LIST = 0x0004,
	};

protected:
	//
	// Startup and Shutdown template methods.
	//
	virtual	bool OnOpen();
	virtual	bool OnClose();

	//
	// Internal methods.
	//
	void LoadConfig();
	void SaveConfig();

	//
	// Constants.
	//
	static const char* INI_FILE_VER_10;
	static const char* INI_FILE_VER_20;
	static const char* INI_FILE_VER_25;
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
