/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		ERRORSDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CErrorsDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "ErrorsDlg.hpp"

/******************************************************************************
** Method:		Default constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CErrorsDlg::CErrorsDlg()
	: CDialog(IDD_ERRORS)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_GRID,	&m_lvGrid)
	END_CTRL_TABLE

	DEFINE_GRAVITY_TABLE
		CTRLGRAV(IDC_GRID,  LEFT_EDGE,  TOP_EDGE,    RIGHT_EDGE, BOTTOM_EDGE)
		CTRLGRAV(IDCANCEL,  RIGHT_EDGE, BOTTOM_EDGE, RIGHT_EDGE, BOTTOM_EDGE)
	END_GRAVITY_TABLE
}

/******************************************************************************
** Method:		OnInitDialog()
**
** Description:	Initialise the dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CErrorsDlg::OnInitDialog()
{
	ASSERT(m_astrFiles.Size() == m_astrErrors.Size());

	// Restore dialog size to last time.
	if (!App.m_rcLastDlgPos.Empty())
		Move(App.m_rcLastDlgPos);

	// Set the dialog title.
	Title(m_strTitle);

	// Set grid style.
//	m_lvGrid.Font(CFont(ANSI_FIXED_FONT));
	m_lvGrid.FullRowSelect(true);
//	m_lvGrid.GridLines(true);

	// Create grid columns.
	m_lvGrid.InsertColumn(0, "File",   250, LVCFMT_LEFT);
	m_lvGrid.InsertColumn(1, "Status", 125, LVCFMT_LEFT);

	// Add errors to grid.
	for (int i = 0; i < m_astrFiles.Size(); ++i)
	{
		int n = m_lvGrid.ItemCount();

		m_lvGrid.InsertItem(n,    m_astrFiles[i] );
		m_lvGrid.ItemText  (n, 1, m_astrErrors[i]);
	}
}

/******************************************************************************
** Method:		OnDestroy()
**
** Description:	Dialog closing.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CErrorsDlg::OnDestroy()
{
	// Save windows final position.
	App.m_rcLastDlgPos = Placement();
}
