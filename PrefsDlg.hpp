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

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>

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
	CCheckBox	m_ckScanForTmp;
	CCheckBox	m_ckScanIndex;
	CCheckBox	m_ckShowAll;
	CCheckBox	m_ckLogEdits;
	CCheckBox	m_ckIgnoreDates;

	//
	// Message processors.
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

#endif //PREFSDLG_HPP
