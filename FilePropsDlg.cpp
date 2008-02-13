/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FILEPROPSDLG.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CFilePropsDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "FilePropsDlg.hpp"
#include "UTCMGRApp.hpp"
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

CFilePropsDlg::CFilePropsDlg()
	: CDialog(IDD_PROPERTIES)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_FILE_ICON,  &m_scIcon      )
		CTRL(IDC_REAL_NAME,  &m_txtRealName )
		CTRL(IDC_CACHE_NAME, &m_txtCacheName)
		CTRL(IDC_FILE_DATE,  &m_txtDate     )
		CTRL(IDC_FILE_TYPE,  &m_txtType     )
		CTRL(IDC_FILE_SIZE,  &m_txtSize     )
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
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

void CFilePropsDlg::OnInitDialog()
{
	// Initialise the controls.
	m_scIcon.Icon(FileIcon(m_cType));
	m_txtRealName.Text(m_strRealName);
	m_txtCacheName.Text(m_strCacheName);
	m_txtDate.Text(m_strDate);
	m_txtType.Text(m_strType);
	m_txtSize.Text(m_strSize);
}

/******************************************************************************
** Method:		FileIcon()
**
** Description:	Get the icon for the file type.
**
** Parameters:	cType	The file type.
**
** Returns:		The icon ID.
**
*******************************************************************************
*/

uint CFilePropsDlg::FileIcon(tchar cType)
{
	switch (cType)
	{
		case SYSTEM_FILE:	return IDI_SYSTEM;
		case MAP_FILE:		return IDI_MAP;
		case TEXTURE_FILE:	return IDI_TEXTURE;
		case SOUND_FILE:	return IDI_SOUND;
		case MUSIC_FILE:	return IDI_MUSIC;
		case MESH_FILE:		return IDI_MESH;
		case ANIM_FILE:		return IDI_ANIM;
		case KARMA_FILE:	return IDI_KARMA;
	}

	ASSERT_FALSE();

	return IDR_APPICON;
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

void CFilePropsDlg::OnHelp(HELPINFO& /*oInfo*/)
{
	// Show the dialogs help topic.
	App.m_oHelpFile.Topic(IDH_PROPSDLG);
}
