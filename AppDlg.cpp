/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CAppDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "AppDlg.hpp"
#include "UTCMGRApp.hpp"
#include <MDBL/ResultSet.hpp>

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
		NFY_CTRLMSG(IDC_GRID, LVN_ITEMCHANGED, &CAppDlg::OnGridSelchange)
		NFY_CTRLMSG(IDC_GRID, NM_RCLICK,       &CAppDlg::OnGridRightClick)
		NFY_CTRLMSG(IDC_GRID, LVN_COLUMNCLICK, &CAppDlg::OnGridClickColumn)
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

	int anColWidths[NUM_COLUMNS];

	// Load the columns widths config.
	anColWidths[FILE_COLUMN]   = App.m_oIniFile.ReadInt(TXT("UI"), TXT("Column0"), 200);
	anColWidths[TYPE_COLUMN]   = App.m_oIniFile.ReadInt(TXT("UI"), TXT("Column1"),  50);
	anColWidths[DATE_COLUMN]   = App.m_oIniFile.ReadInt(TXT("UI"), TXT("Column2"), 125);
	anColWidths[SIZE_COLUMN]   = App.m_oIniFile.ReadInt(TXT("UI"), TXT("Column3"),  70);
	anColWidths[STATUS_COLUMN] = App.m_oIniFile.ReadInt(TXT("UI"), TXT("Column4"),  55);
	anColWidths[CACHE_COLUMN]  = App.m_oIniFile.ReadInt(TXT("UI"), TXT("Column5"), 275);

	// Create grid columns.
	m_lvGrid.InsertColumn(FILE_COLUMN,   TXT("File"),       anColWidths[FILE_COLUMN],   LVCFMT_LEFT  );
	m_lvGrid.InsertColumn(TYPE_COLUMN,   TXT("Type"),       anColWidths[TYPE_COLUMN],   LVCFMT_LEFT  );
	m_lvGrid.InsertColumn(DATE_COLUMN,   TXT("Date"),       anColWidths[DATE_COLUMN],   LVCFMT_LEFT  );
	m_lvGrid.InsertColumn(SIZE_COLUMN,   TXT("Size"),       anColWidths[SIZE_COLUMN],   LVCFMT_RIGHT );
	m_lvGrid.InsertColumn(STATUS_COLUMN, TXT("Status"),     anColWidths[STATUS_COLUMN], LVCFMT_CENTER);
	m_lvGrid.InsertColumn(CACHE_COLUMN,  TXT("Cache Name"), anColWidths[CACHE_COLUMN],  LVCFMT_LEFT  );
}

/******************************************************************************
** Method:		OnDestroy()
**
** Description:	Dialog being destroyed, save the column widths.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppDlg::OnDestroy()
{
	App.m_oIniFile.WriteInt(TXT("UI"), TXT("Column0"), m_lvGrid.ColumnWidth(FILE_COLUMN));
	App.m_oIniFile.WriteInt(TXT("UI"), TXT("Column1"), m_lvGrid.ColumnWidth(TYPE_COLUMN));
	App.m_oIniFile.WriteInt(TXT("UI"), TXT("Column2"), m_lvGrid.ColumnWidth(DATE_COLUMN));
	App.m_oIniFile.WriteInt(TXT("UI"), TXT("Column3"), m_lvGrid.ColumnWidth(SIZE_COLUMN));
	App.m_oIniFile.WriteInt(TXT("UI"), TXT("Column4"), m_lvGrid.ColumnWidth(STATUS_COLUMN));
	App.m_oIniFile.WriteInt(TXT("UI"), TXT("Column5"), m_lvGrid.ColumnWidth(CACHE_COLUMN));
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
	for (size_t i = 0; i < oRS.Count(); ++i)
	{
		CRow& oRow = oRS[i];

		// Check filter.
		if ( (m_bShowAllFiles) || (oRow[CCache::STATUS] == NEW_FILE) )
		{
			int n = m_lvGrid.ItemCount();

			// Add to the grid.
			m_lvGrid.InsertItem(n,                oRow[CCache::REAL_FILENAME], App.IconIndex(oRow[CCache::FILE_TYPE]));
			m_lvGrid.ItemText  (n, TYPE_COLUMN,   App.FormatType(oRow[CCache::FILE_TYPE]));
			m_lvGrid.ItemText  (n, DATE_COLUMN,   oRow[CCache::FILE_DATE].Format());
			m_lvGrid.ItemText  (n, SIZE_COLUMN,   App.FormatSize(oRow[CCache::FILE_SIZE]));
			m_lvGrid.ItemText  (n, STATUS_COLUMN, App.FormatStatus(oRow[CCache::STATUS]));
			m_lvGrid.ItemText  (n, CACHE_COLUMN,  oRow[CCache::CACHE_FILENAME]);
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

LRESULT CAppDlg::OnGridSelchange(NMHDR& oNMHdr)
{
	NMLISTVIEW& oMsgHdr = reinterpret_cast<NMLISTVIEW&>(oNMHdr);

	if (oMsgHdr.uChanged & LVIF_STATE)
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

LRESULT CAppDlg::OnGridRightClick(NMHDR& /*oHdr*/)
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
		case STATUS_COLUMN:		App.m_AppCmds.OnViewSortByStatus();	break;
		case CACHE_COLUMN:		App.m_AppCmds.OnViewSortByCache();	break;
		default:				ASSERT_FALSE();						break;
	}

	return 0;
}
