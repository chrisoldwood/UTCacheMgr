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
	void OnCacheExit();

	// Edit menu.
	void OnEditMove();
	void OnEditCopy();
	void OnEditDelete();
	void OnEditMoveTo();
	void OnEditCopyTo();

	// View menu.
	void OnViewSelectAll();
	void OnViewSortByName();
	void OnViewSortByType();
	void OnViewSortByDate();
	void OnViewSortBySize();
	void OnViewSortByColumn(uint nColumn);

	// Options menu.
	void OnOptionsProfiles();
	void OnOptionsPrefs();
	void OnOptionsUTConfig();

	// Help menu.
	void OnHelpAbout();

	//
	// UI handlers.
	//

	//
	// Command property methods.
	//

protected:
	//
	// Members.
	//
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //APPCMDS_HPP
