/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CONFLICTDLG.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CConflictDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "ConflictDlg.hpp"

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

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

CConflictDlg::CConflictDlg()
	: CDialog(IDD_CONFLICT)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_FILENAME_1, &m_ssFileName1)
		CTRL(IDC_FILEINFO_1, &m_ssFileInfo1)
		CTRL(IDC_FILENAME_2, &m_ssFileName2)
		CTRL(IDC_FILEINFO_2, &m_ssFileInfo2)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDYES,    BN_CLICKED, OnYes   )
		CMD_CTRLMSG(IDYESALL, BN_CLICKED, OnYesAll)
		CMD_CTRLMSG(IDNO,     BN_CLICKED, OnNo    )
		CMD_CTRLMSG(IDNOALL,  BN_CLICKED, OnNoAll )
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

void CConflictDlg::OnInitDialog()
{
	// Set the filename and info strings.
	m_ssFileName1.Text(m_strFileName1);
	m_ssFileInfo1.Text(m_strFileInfo1);
	m_ssFileName2.Text(m_strFileName2);
	m_ssFileInfo2.Text(m_strFileInfo2);
}

/******************************************************************************
** Methods:		OnYes()/OnYesAll()/OnMo()/OnNoAll()
**
** Description:	Close the dialog, returning the button code.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CConflictDlg::OnYes()
{
	EndDialog(IDYES);
}

void CConflictDlg::OnYesAll()
{
	EndDialog(IDYESALL);
}

void CConflictDlg::OnNo()
{
	EndDialog(IDNO);
}

void CConflictDlg::OnNoAll()
{
	EndDialog(IDNOALL);
}
