/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PREFSDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CPrefsDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef PREFSDLG_HPP
#define PREFSDLG_HPP

/******************************************************************************
** 
** This is the dialog used to edit the general preferences.
**
*******************************************************************************
*/

class CPrefsDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CPrefsDlg();
	
protected:
	//
	// Controls.
	//
	CComboBox	m_cbProfiles;
	CCheckBox	m_ckScanStart;
	CCheckBox	m_ckScanSwitch;

	//
	// Message processors.
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

#endif //PREFSDLG_HPP
