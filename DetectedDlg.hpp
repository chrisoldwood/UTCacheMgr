/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DETECTEDDLG.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CDetectedDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef DETECTEDDLG_HPP
#define DETECTEDDLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>
#include <WCL/StrArray.hpp>

/******************************************************************************
** 
** The dialog used to display the detected new UT installations and mods.
**
*******************************************************************************
*/

class CDetectedDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CDetectedDlg();
	
	//
	// Members.
	//
	CStrArray	m_astrNames;
	CStrArray	m_astrFolders;
	
protected:
	//
	// Controls.
	//
	CListView	m_lvProfiles;

	//
	// Message handlers.
	//
	virtual void OnInitDialog();
	virtual bool OnOk();
	virtual void OnHelp(HELPINFO& oInfo);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // DETECTEDDLG_HPP
