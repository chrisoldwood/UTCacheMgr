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

/******************************************************************************
** 
** .
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
