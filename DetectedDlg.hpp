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

/******************************************************************************
** 
** .
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
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // DETECTEDDLG_HPP
