/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		UTCONFIGDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CUTConfigDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef UTCONFIGDLG_HPP
#define UTCONFIGDLG_HPP

/******************************************************************************
** 
** The dialog to edit the UT cache settings.
**
*******************************************************************************
*/

class CUTConfigDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CUTConfigDlg();
	
protected:
	//
	// Members.
	//
	CIniFile	m_oIniFile;

	//
	// Message processors.
	//
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //UTCONFIGDLG_HPP
