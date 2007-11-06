/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PROGRESSDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CProgressDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef PROGRESSDLG_HPP
#define PROGRESSDLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>

/******************************************************************************
** 
** This is the dialog to display the scanning progress.
**
*******************************************************************************
*/

class CProgressDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CProgressDlg();
	
	//
	// Members.
	//
	void InitMeter(uint nSteps);
	void UpdateLabel(const char* pszLabel);
	void UpdateMeter(uint nSteps);
	void UpdateLabelAndMeter(const char* pszLabel, uint nSteps);

protected:
	//
	// Members.
	//
	CLabel			m_txtLabel;
	CProgressBar	m_barMeter;
	
	//
	// Message processors.
	//
	virtual void OnInitDialog();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //PROGRESSDLG_HPP
