/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		RESTOREDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CRestoreDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "RestoreDlg.hpp"
#include "HelpTopics.h"

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

CRestoreDlg::CRestoreDlg()
	: CDialog(IDD_RESTORE)
	, m_pTable(NULL)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_GRID,	&m_lvGrid)
	END_CTRL_TABLE

	DEFINE_GRAVITY_TABLE
		CTRLGRAV(IDC_GRID, LEFT_EDGE, TOP_EDGE, RIGHT_EDGE, BOTTOM_EDGE)
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

void CRestoreDlg::OnInitDialog()
{
	ASSERT(m_pTable != NULL);

	// Set grid style.
//	m_lvGrid.Font(CFont(ANSI_FIXED_FONT));
	m_lvGrid.FullRowSelect(true);
//	m_lvGrid.GridLines(true);

	// Create grid columns.
	m_lvGrid.InsertColumn(0, "File",   200, LVCFMT_LEFT  );
	m_lvGrid.InsertColumn(1, "Type",    50, LVCFMT_LEFT  );
	m_lvGrid.InsertColumn(3, "Size",    70, LVCFMT_RIGHT );

	// Get the file details and sort.
	CResultSet oRS = m_pTable->SelectAll();

	oRS.OrderBy(CSortColumns(CCache::REAL_FILENAME, CSortColumns::ASC));

	// For all rows.
	for (int i = 0; i < oRS.Count(); ++i)
	{
		CRow& oRow = oRS[i];

		int n = m_lvGrid.ItemCount();

		// Add to the grid.
		m_lvGrid.InsertItem(n,    oRow[CCache::REAL_FILENAME], &oRow);
		m_lvGrid.ItemText  (n, 1, App.FormatType(oRow[CCache::FILE_TYPE]));
		m_lvGrid.ItemText  (n, 2, App.FormatSize(oRow[CCache::FILE_SIZE]));
	}
}

/******************************************************************************
** Method:		OnOk()
**
** Description:	User pressed OK.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CRestoreDlg::OnOk()
{
	int nFiles = m_lvGrid.ItemCount();

	// For all rows...
	for (int i = 0; i < nFiles; ++i)
	{
		// Remove unselected one from table.
		if (!m_lvGrid.IsSelected(i))
			m_pTable->DeleteRow(GetRow(i));
	}

	return true;
}

/******************************************************************************
** Method:		OnHelp()
**
** Description:	Help requested for the dialog.
**
** Parameters:	See HELPINFO.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CRestoreDlg::OnHelp(HELPINFO& /*oInfo*/)
{
	// Show the dialogs help topic.
	App.m_oHelpFile.Topic(IDH_RESTOREDLG);
}
