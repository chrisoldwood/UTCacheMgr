/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		EDITPROFILEDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CEditProfileDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef EDITPROFILEDLG_HPP
#define EDITPROFILEDLG_HPP

/******************************************************************************
** 
** This is the dialog used to create/edit a profile.
**
*******************************************************************************
*/

class CEditProfileDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CEditProfileDlg();
	
	//
	// Members.
	//
	CProfile	m_oProfile;
	
protected:
	//
	// Controls.
	//
	CEditBox	m_ebName;
	CEditBox	m_ebCacheDir;
	CEditBox	m_ebSystemDir;
	CEditBox	m_ebMapDir;
	CEditBox	m_ebTextureDir;
	CEditBox	m_ebSoundDir;
	CEditBox	m_ebMusicDir;
	CEditBox	m_ebConfigFile;

	//
	// Members.
	//
	CString		m_strLastPath;

	//
	// Message processors.
	//
	virtual void OnInitDialog();
	virtual bool OnOk();
	virtual void OnHelp(HELPINFO& oInfo);

	void OnBrowseCache();
	void OnBrowseSystem();
	void OnBrowseMaps();
	void OnBrowseTextures();
	void OnBrowseSounds();
	void OnBrowseMusic();
	void OnBrowseDir(CEditBox& ebPath);
	void OnBrowseCfgFile();
	void OnQuickSetup();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //EDITPROFILEDLG_HPP
