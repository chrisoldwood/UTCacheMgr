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
	CAppCmds();
	~CAppCmds();

	//
	// Commands.
	//

	// Cache menu.
	void OnCacheProfile();
	void OnCacheRescan();
	void OnCacheRestore();
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
	void OnViewSortByColumn(uint nColumn, CSortColumns::Dir eDefDir);
	void OnViewShowAll();

	// Options menu.
	void OnOptionsProfiles();
	void OnOptionsPrefs();
	void OnOptionsUTConfig();

	// Help menu.
	void OnHelpContents();
	void OnHelpAbout();

	//
	// UI handlers.
	//
	void OnUICacheRestore();
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
