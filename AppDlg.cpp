/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CAppDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"

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

CAppDlg::CAppDlg()
	: CMainDlg(IDD_MAIN)
	, m_nSortColumn(CCache::FILE_DATE)
	, m_eSortOrder(CSortColumns::DESC)
	, m_bShowAllFiles(false)
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

void CAppDlg::OnInitDialog()
{
	// Set grid style.
//	m_lvGrid.Font(CFont(ANSI_FIXED_FONT));
	m_lvGrid.FullRowSelect(true);
//	m_lvGrid.GridLines(true);

	// Create grid columns.
	m_lvGrid.InsertColumn(0, "File",   200, LVCFMT_LEFT  );
	m_lvGrid.InsertColumn(1, "Type",    50, LVCFMT_LEFT  );
	m_lvGrid.InsertColumn(2, "Date",   125, LVCFMT_LEFT  );
	m_lvGrid.InsertColumn(3, "Size",    70, LVCFMT_RIGHT );
	m_lvGrid.InsertColumn(4, "Status",  55, LVCFMT_CENTER);
}

/******************************************************************************
** Method:		RefreshView()
**
** Description:	Load the grid with the cache details.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppDlg::RefreshView()
{
	// Reset grid.
	m_lvGrid.DeleteAllItems();

	// Get the cache details and sort.
	CResultSet oRS = App.m_oCache.SelectAll();

	oRS.OrderBy(CSortColumns(m_nSortColumn, m_eSortOrder));

	// For all rows.
	for (int i = 0; i < oRS.Count(); ++i)
	{
		CRow& oRow = oRS[i];

		// Check filter.
		if ( (m_bShowAllFiles) || (oRow[CCache::STATUS] == NEW_FILE) )
		{
			int n = m_lvGrid.ItemCount();

			// Add to the grid.
			m_lvGrid.InsertItem(n,    oRow[CCache::REAL_FILENAME], &oRow);
			m_lvGrid.ItemText  (n, 1, App.FormatType(oRow[CCache::FILE_TYPE]));
			m_lvGrid.ItemText  (n, 2, oRow[CCache::FILE_DATE].Format());
			m_lvGrid.ItemText  (n, 3, App.FormatSize(oRow[CCache::FILE_SIZE]));
			m_lvGrid.ItemText  (n, 4, App.FormatStatus(oRow[CCache::STATUS]));
		}
	}
}

/******************************************************************************
** Method:		SelectNew()
**
** Description:	Select all new files in the grid.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppDlg::SelectNew()
{
	int nFiles = m_lvGrid.ItemCount();

	for (int i = 0; i < nFiles; ++i)
		m_lvGrid.Select(i, (GetRow(i)[CCache::STATUS] == NEW_FILE));
}

/******************************************************************************
** Method:		SelectAll()
**
** Description:	Select all files in the grid.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppDlg::SelectAll()
{
	int nFiles = m_lvGrid.ItemCount();

	for (int i = 0; i < nFiles; ++i)
		m_lvGrid.Select(i);
}

/******************************************************************************
** Method:		GetSelectedFiles()
**
** Description:	Gets all the files selected in the grid.
**
** Parameters:	oRS		The collection for the selection.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppDlg::GetSelectedFiles(CResultSet& oRS)
{
	int nFiles = m_lvGrid.ItemCount();

	// For all rows.
	for (int i = 0; i < nFiles; ++i)
	{
		if (m_lvGrid.IsSelected(i))
			oRS.Add(GetRow(i));
	}
}
