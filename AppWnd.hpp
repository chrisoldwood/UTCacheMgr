/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPWND.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CAppWnd class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef APPWND_HPP
#define APPWND_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/DlgFrame.hpp>
#include "AppDlg.hpp"
#include <WCL/FrameMenu.hpp>
#include "AppToolbar.hpp"
#include <WCL/StatusBar.hpp>
#include <WCL/Accel.hpp>

/******************************************************************************
**
** This is the the applications main window.
**
*******************************************************************************
*/

class CAppWnd : public CDlgFrame
{
public:
	//
	// Constructors/Destructor.
	//
	CAppWnd(WCL::IMsgThread& thread, WCL::ICmdController& controller);
	~CAppWnd();

	//
	// Methods.
	//
	void UpdateTitle();

	//
	// Members.
	//
	CAppDlg		m_AppDlg;
	CFrameMenu	m_Menu;
	CAppToolBar	m_ToolBar;
	CStatusBar	m_StatusBar;

protected:
	//
	// Internal members.
	//
	CAccel		m_Accel;

	// Child window IDs.
	enum
	{
		IDC_TOOL_BAR   = 100,
		IDC_STATUS_BAR = 101
	};

	//
	// Message processors.
	//
	virtual void OnCreate(const CRect& rcClient);
	virtual void OnFocus();
	virtual bool OnQueryClose();
	virtual bool ProcessMsg(MSG& rMsg);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //APPWND_HPP
