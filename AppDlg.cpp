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

	DEFINE_CTRLMSG_TABLE
		NFY_CTRLMSG(IDC_GRID, LVN_ITEMCHANGED, OnGridSelchange)
		NFY_CTRLMSG(IDC_GRID, NM_RCLICK,       OnGridRightClick)
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

void CAppDlg::OnInitDialog()
{
	// Set grid style.
//	m_lvGrid.Font(CFont(ANSI_FIXED_FONT));
	m_lvGrid.FullRowSelect(true);
//	m_lvGrid.GridLines(true);

	// Set the ListView icons.
	m_lvGrid.ImageList(LVSIL_SMALL, IDB_LIST_ICONS, 16, RGB(255, 0, 255));

	// Create grid columns.
	m_lvGrid.InsertColumn(FILE_COLUMN,   "File",   200, LVCFMT_LEFT  );
	m_lvGrid.InsertColumn(TYPE_COLUMN,   "Type",    50, LVCFMT_LEFT  );
	m_lvGrid.InsertColumn(DATE_COLUMN,   "Date",   125, LVCFMT_LEFT  );
	m_lvGrid.InsertColumn(SIZE_COLUMN,   "Size",    70, LVCFMT_RIGHT );
	m_lvGrid.InsertColumn(STATUS_COLUMN, "Status",  55, LVCFMT_CENTER);
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
			m_lvGrid.InsertItem(n,                oRow[CCache::REAL_FILENAME], IconIndex(oRow[CCache::FILE_TYPE]));
			m_lvGrid.ItemText  (n, TYPE_COLUMN,   App.FormatType(oRow[CCache::FILE_TYPE]));
			m_lvGrid.ItemText  (n, DATE_COLUMN,   oRow[CCache::FILE_DATE].Format());
			m_lvGrid.ItemText  (n, SIZE_COLUMN,   App.FormatSize(oRow[CCache::FILE_SIZE]));
			m_lvGrid.ItemText  (n, STATUS_COLUMN, App.FormatStatus(oRow[CCache::STATUS]));
			m_lvGrid.ItemPtr   (n,                &oRow);
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

/******************************************************************************
** Method:		OnGridSelchange()
**
** Description:	Grid item (de)selected, update the main UI.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

LRESULT CAppDlg::OnGridSelchange(NMHDR&)
{
	App.m_AppCmds.UpdateUI();

	return 0;
}

/******************************************************************************
** Method:		OnGridRightClick()
**
** Description:	Right click on the grid, show context menu.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

LRESULT CAppDlg::OnGridRightClick(NMHDR&)
{
	// Only show menu, if a selection.
	if (m_lvGrid.IsSelection())
	{
		CPopupMenu oMenu(IDR_GRIDMENU);

		bool bReadOnly = App.m_pProfile->m_bReadOnly;

		// Enable/Disable relevant menu commands.
		oMenu.EnableCmd(ID_EDIT_MOVE,        !bReadOnly);
		oMenu.EnableCmd(ID_EDIT_DELETE,      !bReadOnly);
		oMenu.EnableCmd(ID_CACHE_PROPERTIES, true);

		// Get co-ordinates of mouse click.
		const MSG& oCurrMsg = App.m_MainThread.CurrentMsg();

		// Show menu.
		uint nCmdID = oMenu.TrackMenu(m_lvGrid, CPoint(oCurrMsg.pt.x, oCurrMsg.pt.y));

		// Dispatch command.
		if (nCmdID != NULL)
			App.m_AppWnd.PostCommand(nCmdID);
	}

	return 0;
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

LRESULT CAppDlg::OnGridClickColumn(NMHDR& oHdr)
{
	NMLISTVIEW& oParam = (NMLISTVIEW&) oHdr;

	// Decode column.
	switch (oParam.iSubItem)
	{
		case FILE_COLUMN:		App.m_AppCmds.OnViewSortByName();	break;
		case TYPE_COLUMN:		App.m_AppCmds.OnViewSortByType();	break;
		case DATE_COLUMN:		App.m_AppCmds.OnViewSortByDate();	break;
		case SIZE_COLUMN:		App.m_AppCmds.OnViewSortBySize();	break;
		case STATUS_COLUMN:											break;
		default:				ASSERT(false);						break;
	}

	return 0;
}

/******************************************************************************
** Method:		IconIndex()
**
** Description:	Gets the index of the icon for the given file type.
**
** Parameters:	cType	The file type.
**
** Returns:		The icon index.
**
*******************************************************************************
*/

int CAppDlg::IconIndex(char cType)
{
	switch (cType)
	{
		case SYSTEM_FILE:	return 0;
		case MAP_FILE:		return 1;
		case TEXTURE_FILE:	return 2;
		case SOUND_FILE:	return 3;
		case MUSIC_FILE:	return 4;
		case MESH_FILE:		return 5;
		case ANIM_FILE:		return 6;
	}

	ASSERT(false);

	return -1;
}
