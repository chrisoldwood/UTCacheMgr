/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FILEPROPSDLG.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CFilePropsDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef FILEPROPSDLG_HPP
#define FILEPROPSDLG_HPP

/******************************************************************************
** 
** The dialog used to display a cache files' properties.
**
*******************************************************************************
*/

class CFilePropsDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CFilePropsDlg();
	
	//
	// Members.
	//
	char	m_cType;
	CString	m_strRealName;
	CString m_strCacheName;
	CString m_strDate;
	CString m_strType;
	CString m_strSize;
	
protected:
	//
	// Controls.
	//
	CStaticCtrl	m_scIcon;
	CLabel		m_txtRealName;
	CLabel		m_txtCacheName;
	CLabel		m_txtDate;
	CLabel		m_txtType;
	CLabel		m_txtSize;

	//
	// Message handlers.
	//
	virtual void OnInitDialog();

	//
	// Internal methods.
	//
	uint FileIcon(char cType);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // FILEPROPSDLG_HPP
