/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		ERRORSDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CErrorsDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef ERRORSDLG_HPP
#define ERRORSDLG_HPP

/******************************************************************************
** 
** Dialog used to display errors that occurred during an operation.
**
*******************************************************************************
*/

class CErrorsDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CErrorsDlg();
	
	//
	// Members.
	//
	CString		m_strTitle;
	CStrArray	m_astrFiles;
	CStrArray	m_astrErrors;
	
protected:
	//
	// Controls.
	//
	CListView	m_lvGrid;

	//
	// Message processors.
	//
	virtual void OnInitDialog();
	LRESULT OnDblClick(NMHDR&);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //ERRORSDLG_HPP
