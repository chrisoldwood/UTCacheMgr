/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SELFILESDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CSelFilesDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef SELFILESDLG_HPP
#define SELFILESDLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>
#include <MDBL/FwdDecls.hpp>
#include <MDBL/SortColumns.hpp>

/******************************************************************************
** 
** Dialog used to select which files should be imported/restored into the cache.
**
*******************************************************************************
*/

class CSelFilesDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CSelFilesDlg();
	
	//
	// Members.
	//
	CTable*	m_pTable;
	CString	m_strTitle;
	DWORD	m_dwHelpID;

protected:
	//
	// Members.
	//
	CListView			m_lvGrid;
	uint				m_nSortColumn;
	CSortColumns::Dir	m_eSortOrder;
	
	// Column indices.
	enum Column
	{
		FILE_COLUMN,
		TYPE_COLUMN,
		DATE_COLUMN,
		SIZE_COLUMN,
		CACHE_COLUMN,
		
		NUM_COLUMNS,
	};

	//
	// Message processors.
	//
	virtual void OnInitDialog();
	virtual void OnDestroy();
	virtual bool OnOk();
	virtual void OnHelp(HELPINFO& oInfo);
	LRESULT OnGridClickColumn(NMHDR& oHdr);
	        void OnSelectAll();

	//
	// Internal methods.
	//
	void  RefreshView();
	CRow& GetRow(size_t nRow);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CRow& CSelFilesDlg::GetRow(size_t nRow)
{
	ASSERT(nRow < m_lvGrid.ItemCount());

	return *(static_cast<CRow*>(m_lvGrid.ItemPtr(nRow)));
}

#endif //SELFILESDLG_HPP
