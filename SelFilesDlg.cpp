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
	, m_nSortColumn(CCache::REAL_FILENAME)
	, m_eSortOrder(CSortColumns::ASC)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_GRID,	&m_lvGrid)
	END_CTRL_TABLE

	DEFINE_GRAVITY_TABLE
		CTRLGRAV(IDC_GRID, LEFT_EDGE,  TOP_EDGE,    RIGHT_EDGE, BOTTOM_EDGE)
		CTRLGRAV(IDOK,     RIGHT_EDGE, BOTTOM_EDGE, RIGHT_EDGE, BOTTOM_EDGE)
		CTRLGRAV(IDCANCEL, RIGHT_EDGE, BOTTOM_EDGE, RIGHT_EDGE, BOTTOM_EDGE)
	END_GRAVITY_TABLE

	DEFINE_CTRLMSG_TABLE
		NFY_CTRLMSG(IDC_GRID, LVN_COLUMNCLICK, OnGridClickColumn)
	END_CTRLMSG_TABLE
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

	// Set the ListView icons.
	m_lvGrid.ImageList(LVSIL_SMALL, IDB_LIST_ICONS, 16, RGB(255, 0, 255));

	// Create grid columns.
	m_lvGrid.InsertColumn(FILE_COLUMN,  "File",       175, LVCFMT_LEFT );
	m_lvGrid.InsertColumn(TYPE_COLUMN,  "Type",        50, LVCFMT_LEFT );
	m_lvGrid.InsertColumn(DATE_COLUMN,  "Date",       115, LVCFMT_LEFT );
	m_lvGrid.InsertColumn(SIZE_COLUMN,  "Size",        70, LVCFMT_RIGHT);
	m_lvGrid.InsertColumn(CACHE_COLUMN, "Cache Name", 170, LVCFMT_LEFT );

	// Load grid.
	RefreshView();
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
		// Remove unselected rows from table.
		if (!m_lvGrid.IsSelected(i))
			m_pTable->DeleteRow(GetRow(i));
	}

	return true;
}

/******************************************************************************
** Method:		RefreshView()
**
** Description:	Reload the grid with the new sort order.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CRestoreDlg::RefreshView()
{
	// Trash old grid.
	m_lvGrid.DeleteAllItems();

	// Get the file details and sort.
	CResultSet oRS = m_pTable->SelectAll();

	oRS.OrderBy(CSortColumns(m_nSortColumn, m_eSortOrder));

	// For all rows.
	for (int i = 0; i < oRS.Count(); ++i)
	{
		CRow& oRow = oRS[i];

		int n = m_lvGrid.ItemCount();

		// Add to the grid.
		m_lvGrid.InsertItem(n,                oRow[CCache::REAL_FILENAME], App.IconIndex(oRow[CCache::FILE_TYPE]));
		m_lvGrid.ItemText  (n, TYPE_COLUMN,   App.FormatType(oRow[CCache::FILE_TYPE]));
		m_lvGrid.ItemText  (n, DATE_COLUMN,   oRow[CCache::FILE_DATE].Format());
		m_lvGrid.ItemText  (n, SIZE_COLUMN,   App.FormatSize(oRow[CCache::FILE_SIZE]));
		m_lvGrid.ItemText  (n, CACHE_COLUMN,  oRow[CCache::CACHE_FILENAME]);
		m_lvGrid.ItemPtr   (n,                &oRow);
	}
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


/******************************************************************************
** Method:		OnGridClickColumn()
**
** Description:	Grid column clicked, resort the grid.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

LRESULT CRestoreDlg::OnGridClickColumn(NMHDR& oHdr)
{
	NMLISTVIEW& oParam = (NMLISTVIEW&) oHdr;

	// Default to first column.
	uint nColumn = FILE_COLUMN;
	int  eOrder  = CSortColumns::ASC;

	// Decode column.
	switch (oParam.iSubItem)
	{
		case FILE_COLUMN:
			nColumn = CCache::REAL_FILENAME;
			eOrder  = CSortColumns::ASC;
			break;

		case TYPE_COLUMN:		
			nColumn = CCache::FILE_TYPE;
			eOrder  = CSortColumns::ASC;
			break;

		case DATE_COLUMN:		
			nColumn = CCache::FILE_DATE;
			eOrder  = CSortColumns::DESC;
			break;

		case SIZE_COLUMN:		
			nColumn = CCache::FILE_SIZE;
			eOrder  = CSortColumns::DESC;
			break;

		case CACHE_COLUMN:		
			nColumn = CCache::CACHE_FILENAME;
			eOrder  = CSortColumns::ASC;
			break;

		default:
			ASSERT_FALSE();
			break;
	}

	// Different column?
	if (nColumn != m_nSortColumn)
	{
		m_nSortColumn = nColumn;
		m_eSortOrder  = (CSortColumns::Dir) eOrder;
	}
	// Same column.
	else
	{
		// Reverse sort.
		m_eSortOrder = (CSortColumns::Dir) -m_eSortOrder;
	}

	// Reload grid.
	RefreshView();

	return 0;
}
