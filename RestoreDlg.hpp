/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		RESTOREDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CRestoreDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef RESTOREDLG_HPP
#define RESTOREDLG_HPP

/******************************************************************************
** 
** The dialog used to select which files should be restored to the cache.
**
*******************************************************************************
*/

class CRestoreDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CRestoreDlg();
	
	//
	// Members.
	//
	CTable*	m_pTable;

protected:
	//
	// Members.
	//
	CListView	m_lvGrid;
	
	//
	// Message processors.
	//
	virtual void OnInitDialog();
	virtual bool OnOk();
	virtual void OnHelp(HELPINFO& oInfo);

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

inline CRow& CRestoreDlg::GetRow(int nRow)
{
	ASSERT((nRow >= 0) && (nRow < m_lvGrid.ItemCount()));

	return *((CRow*) m_lvGrid.ItemPtr(nRow));
}

#endif //RESTOREDLG_HPP
