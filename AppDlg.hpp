/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CAppDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef APPDLG_HPP
#define APPDLG_HPP

/******************************************************************************
** 
** This is the main application dialog.
**
*******************************************************************************
*/

class CAppDlg : public CMainDlg
{
public:
	//
	// Constructors/Destructor.
	//
	CAppDlg();
	
	//
	// Methods.
	//
	void RefreshView();
	void SelectNew();
	void SelectAll();
	void GetSelectedFiles(CResultSet& oRS);

	//
	// Members.
	//
	CListView			m_lvGrid;
	uint				m_nSortColumn;
	CSortColumns::Dir	m_eSortOrder;
	bool				m_bShowAllFiles;

	// Column indices.
	enum Column
	{
		FILE_COLUMN,
		TYPE_COLUMN,
		DATE_COLUMN,
		SIZE_COLUMN,
		STATUS_COLUMN,
	};

protected:
	//
	// Message processors.
	//
	virtual void OnInitDialog();
	LRESULT OnGridSelchange(NMHDR&);
	LRESULT OnGridRightClick(NMHDR&);
	LRESULT OnGridClickColumn(NMHDR&);

	//
	// Internal methods.
	//
	CRow& GetRow(int nRow);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CRow& CAppDlg::GetRow(int nRow)
{
	ASSERT((nRow >= 0) && (nRow < m_lvGrid.ItemCount()));

	return *((CRow*) m_lvGrid.ItemPtr(nRow));
}

#endif //APPDLG_HPP
