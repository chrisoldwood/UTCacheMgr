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
	, m_nSortColumn(CCache::REAL_FILENAME)
	, m_eSortOrder(CSortColumns::ASC)
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
	m_lvGrid.InsertColumn(3, "Size",    75, LVCFMT_RIGHT );
	m_lvGrid.InsertColumn(4, "Status",  50, LVCFMT_CENTER);
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
			m_lvGrid.ItemText  (n, 1, FormatType(oRow[CCache::FILE_TYPE]));
			m_lvGrid.ItemText  (n, 2, oRow[CCache::FILE_DATE].Format());
			m_lvGrid.ItemText  (n, 3, FormatSize(oRow[CCache::FILE_SIZE]));
			m_lvGrid.ItemText  (n, 4, FormatStatus(oRow[CCache::STATUS]));
		}
	}
}

/******************************************************************************
** Method:		FormatType()
**
** Description:	Convert the file type to a string.
**
** Parameters:	cType	The file type.
**
** Returns:		The type as a string.
**
*******************************************************************************
*/

CString CAppDlg::FormatType(char cType) const
{
	switch (cType)
	{
		case SYSTEM_FILE :	return "System";
		case MAP_FILE    :	return "Map";
		case TEXTURE_FILE:	return "Texture";
		case SOUND_FILE  :	return "Sound";
		case MUSIC_FILE  :	return "Music";
	}

	ASSERT(false);

	return "";
}

/******************************************************************************
** Method:		FormatSize()
**
** Description:	Convert the file size to a string.
**
** Parameters:	nSize	The file size.
**
** Returns:		The size as a string.
**
*******************************************************************************
*/

CString CAppDlg::FormatSize(int nSize) const
{
	CString str;

	// Ensure we report at least 1K.
	nSize = max(1024, nSize);

	str.Format("%d K", nSize/1024);

	return str;
}

/******************************************************************************
** Method:		FormatStatus()
**
** Description:	Convert the file status to a string.
**
** Parameters:	cStatus		The file status.
**
** Returns:		The status as a string.
**
*******************************************************************************
*/

CString CAppDlg::FormatStatus(char cStatus) const
{
	switch (cStatus)
	{
		case NEW_FILE :	return "New";
		case OLD_FILE :	return "Old";
	}

	ASSERT(false);

	return "";
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
		if (m_lvGrid.ItemState(i) & LVIS_SELECTED)
			oRS.Add(*((CRow*) m_lvGrid.ItemPtr(i)));
	}
}
