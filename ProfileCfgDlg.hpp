/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PROFILECFGDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CProfileCfgDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef PROFILECFGDLG_HPP
#define PROFILECFGDLG_HPP

/******************************************************************************
** 
** This is the master dialog used to edit the profiles.
**
*******************************************************************************
*/

class CProfileCfgDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CProfileCfgDlg();
	
	//
	// Members.
	//
	bool m_bReScan;

protected:
	//
	// Controls.
	//
	CListBox	m_lbProfiles;

	//
	// Message processors.
	//
	virtual void OnInitDialog();
	virtual void OnHelp(HELPINFO& oInfo);

	void OnAdd();
	void OnEdit();
	void OnRemove();
	void OnDblClkProfile();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //PROFILECFGDLG_HPP
