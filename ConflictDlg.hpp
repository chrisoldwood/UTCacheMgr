/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CONFLICTDLG.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CConflictDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef CONFLICTDLG_HPP
#define CONFLICTDLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>

/******************************************************************************
** 
** The dialog used to resolve a conflict when installing a file from a map
** pack into the profile folder.
**
*******************************************************************************
*/

class CConflictDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CConflictDlg();
	
	//
	// Members.
	//
	CString	m_strFileName1;
	CString	m_strFileInfo1;
	CString	m_strFileName2;
	CString	m_strFileInfo2;
	
protected:
	//
	// Controls.
	//
	CLabel	m_ssFileName1;
	CLabel	m_ssFileInfo1;
	CLabel	m_ssFileName2;
	CLabel	m_ssFileInfo2;

	//
	// Message handlers.
	//
	virtual void OnInitDialog();
	virtual void OnHelp(HELPINFO& oInfo);

	void OnYes();
	void OnYesAll();
	void OnNo();
	void OnNoAll();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // CONFLICTDLG_HPP
