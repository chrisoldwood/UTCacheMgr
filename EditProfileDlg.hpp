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
	CComboBox	m_cbFormat;
	CEditBox	m_ebCacheDir;
	CCheckBox	m_ckReadOnly;
	CEditBox	m_ebSystemDir;
	CEditBox	m_ebMapDir;
	CEditBox	m_ebTextureDir;
	CEditBox	m_ebSoundDir;
	CEditBox	m_ebMusicDir;
	CEditBox	m_ebMeshDir;
	CEditBox	m_ebAnimDir;
	CEditBox	m_ebKarmaDir;
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

	void OnSelectFormat();
	void OnBrowseCache();
	void OnBrowseSystem();
	void OnBrowseMaps();
	void OnBrowseTextures();
	void OnBrowseSounds();
	void OnBrowseMusic();
	void OnBrowseMeshes();
	void OnBrowseAnims();
	void OnBrowseKarma();
	void OnBrowseDir(CEditBox& ebPath);
	void OnBrowseCfgFile();
	void OnQuickSetup();

	bool ValidatePath(const CPath& strPath, CEditBox& ebControl, const char* pszName);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //EDITPROFILEDLG_HPP
