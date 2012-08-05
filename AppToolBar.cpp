/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPTOOLBAR.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CAppToolBar class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "AppToolbar.hpp"

/******************************************************************************
** Method:		Constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CAppToolBar::CAppToolBar(WCL::IMsgThread& thread, WCL::ICmdController& controller)
	: CToolBar(thread, controller)
{
	// Define the toolbar.
	DEFINE_TOOLBAR
		TBCTL(&m_btnSelProf,	ID_CACHE_PROFILE)
		TBCTL(&m_btnRescan,		ID_CACHE_RESCAN )
		TBGAP()
		TBCTL(&m_btnMove,		ID_EDIT_MOVE    )
		TBCTL(&m_btnCopy,		ID_EDIT_COPY    )
		TBCTL(&m_btnDelete,		ID_EDIT_DELETE  )
		TBCTL(&m_btnCopyTo,		ID_EDIT_COPY_TO )
		TBGAP()
		TBCTL(&m_btnHelp,		ID_HELP_CONTENTS)
	END_TOOLBAR
}

/******************************************************************************
** Method:		Destructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CAppToolBar::~CAppToolBar()
{
}
