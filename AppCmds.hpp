/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPCMDS.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CAppCmds class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef APPCMDS_HPP
#define APPCMDS_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CmdCtrl.hpp>
#include <MDBL/FwdDecls.hpp>
#include <MDBL/SortColumns.hpp>

// Forward declarations.
class CAppWnd;

/******************************************************************************
** 
** Application command controller.
**
*******************************************************************************
*/

class CAppCmds : public CCmdControl
{
public:
	//
	// Constructors/Destructor.
	//
	CAppCmds(CAppWnd& appWnd);
	~CAppCmds();

	//
	// Commands.
	//

	// Cache menu.
	void OnCacheProfile(uint nCmdID);
	void OnCacheProfileDlg();
	void OnCacheRescan();
	void OnCacheRestore();
	void OnCacheImport();
	void OnCacheUTConfig();
	void OnCacheProperties();
	void OnCacheExit();

	// Edit menu.
	void OnEditPin();
	void OnEditMove();
	void OnEditCopy();
	void OnEditDelete();
	void OnEditCopyTo();

	// View menu.
	void OnViewSelectNew();
	void OnViewSelectAll();
	void OnViewSortByName();
	void OnViewSortByType();
	void OnViewSortByDate();
	void OnViewSortBySize();
	void OnViewSortByStatus();
	void OnViewSortByCache();
	void OnViewSortByColumn(uint nColumn, CSortColumns::Dir eDefDir);
	void OnViewShowAll();

	// Tools menu.
	void OnToolsInstall();

	// Options menu.
	void OnOptionsProfiles();
	void OnOptionsPrefs();

	// Help menu.
	void OnHelpContents();
	void OnHelpAbout();

	//
	// UI handlers.
	//
	void OnUICacheProfile();
	void OnUICacheRestore();
	void OnUICacheProperties();
	void OnUIEditPin();
	void OnUIEditMove();
	void OnUIEditCopy();
	void OnUIEditDelete();
	void OnUIEditCopyTo();
	void OnUIViewShowAll();
	void OnUIViewSortByName();
	void OnUIViewSortByType();
	void OnUIViewSortByDate();
	void OnUIViewSortBySize();
	void OnUIViewSortByStatus();
	void OnUIViewSortByCache();

	//
	// Command property methods.
	//
	virtual CString CmdHintStr(uint iCmdID) const;

protected:
	//
	// Internal methods.
	//
	void LogEdits(CResultSet& oRS);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //APPCMDS_HPP
