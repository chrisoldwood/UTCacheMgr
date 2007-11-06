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

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>
#include "Profile.hpp"

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
	// Message processors.
	//
	virtual void OnInitDialog();
	virtual bool OnOk();
	virtual void OnHelp(HELPINFO& oInfo);

	void OnSelectProfile();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //SELPROFILEDLG_HPP
