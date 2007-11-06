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

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/MainDlg.hpp>
#include <WCL/ListView.hpp>
#include <MDBL/FwdDecls.hpp>
#include <MDBL/SortColumns.hpp>

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
		CACHE_COLUMN,
		
		NUM_COLUMNS,
	};

protected:
	//
	// Message processors.
	//
	virtual void OnInitDialog();
	virtual void OnDestroy();
	LRESULT OnGridSelchange(NMHDR& oHdr);
	LRESULT OnGridRightClick(NMHDR& oHdr);
	LRESULT OnGridClickColumn(NMHDR& oHdr);

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
