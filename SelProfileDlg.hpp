/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SELPROFILEDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CSelProfileDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef SELPROFILEDLG_HPP
#define SELPROFILEDLG_HPP

/******************************************************************************
** 
** This is the dialog used to select a profile.
**
*******************************************************************************
*/

class CSelProfileDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CSelProfileDlg();
	
	//
	// Members.
	//
	CProfiles*	m_pProfiles;
	CProfile*	m_pChoice;
	
protected:
	//
	// Controls.
	//
	CComboBox	m_cbProfiles;
	CEditBox	m_ebCacheDir;
	CEditBox	m_ebSystemDir;
	CEditBox	m_ebMapDir;
	CEditBox	m_ebTextureDir;
	CEditBox	m_ebSoundDir;
	CEditBox	m_ebMusicDir;
	CEditBox	m_ebConfigFile;

	//
	// Message processors.
	//
	virtual void OnInitDialog();
	virtual bool OnOk();

	void OnSelectProfile();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //SELPROFILEDLG_HPP
