/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPCMDS.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CAppCmds class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "AboutDlg.hpp"
#include "SelProfileDlg.hpp"
#include "ProfileCfgDlg.hpp"
#include "PrefsDlg.hpp"
#include "ProfileCfgDlg.hpp"
#include "PrefsDlg.hpp"
#include "ProgressDlg.hpp"
#include "UTConfigDlg.hpp"

/******************************************************************************
** Method:		Constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CAppCmds::CAppCmds()
{
	// Define the command table.
	DEFINE_CMD_TABLE
		// File menu.
		CMD_ENTRY(ID_CACHE_PROFILE,		OnCacheProfile,		NULL,	 3)
		CMD_ENTRY(ID_CACHE_RESCAN,		OnCacheRescan,		NULL,	 0)
		CMD_ENTRY(ID_CACHE_EXIT,		OnCacheExit,		NULL,	-1)
		// Edit menu.
		CMD_ENTRY(ID_EDIT_MOVE,			OnEditMove,			NULL,	 5)
		CMD_ENTRY(ID_EDIT_COPY,			OnEditCopy,			NULL,	 6)
		CMD_ENTRY(ID_EDIT_DELETE,		OnEditDelete,		NULL,	 7)
		CMD_ENTRY(ID_EDIT_COPY_TO,		OnEditCopyTo,		NULL,	 9)
		// View menu.
		CMD_ENTRY(ID_VIEW_SELECT_ALL,	OnViewSelectAll,	NULL,	-1)
		CMD_ENTRY(ID_VIEW_SORT_NAME,	OnViewSortByName,	NULL,	-1)
		CMD_ENTRY(ID_VIEW_SORT_TYPE,	OnViewSortByType,	NULL,	-1)
		CMD_ENTRY(ID_VIEW_SORT_DATE,	OnViewSortByDate,	NULL,	-1)
		CMD_ENTRY(ID_VIEW_SORT_SIZE,	OnViewSortBySize,	NULL,	-1)
		CMD_ENTRY(ID_VIEW_SHOW_ALL,		OnViewShowAll,		NULL,	-1)
		// Options menu.
		CMD_ENTRY(ID_OPTIONS_PROFILES,	OnOptionsProfiles,	NULL,	-1)
		CMD_ENTRY(ID_OPTIONS_PREFS,		OnOptionsPrefs,		NULL,	-1)
		CMD_ENTRY(ID_OPTIONS_UT_CONFIG,	OnOptionsUTConfig,	NULL,	-1)
		// Help menu.
		CMD_ENTRY(ID_HELP_CONTENTS,		OnHelpContents,		NULL,	 1)
		CMD_ENTRY(ID_HELP_ABOUT,		OnHelpAbout,		NULL,	-1)
	END_CMD_TABLE
}

/******************************************************************************
** Method:		Destructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CAppCmds::~CAppCmds()
{
}

/******************************************************************************
** Method:		OnCacheProfile()
**
** Description:	Select the cache profile to use.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnCacheProfile()
{
	ASSERT(App.m_pProfile != NULL);

	CSelProfileDlg	Dlg;

	Dlg.m_pProfiles = &App.m_aoProfiles;
	Dlg.m_pChoice   = App.m_pProfile;

	// Show selection dialog.
	if (Dlg.RunModal(App.m_AppWnd) == IDOK)
	{
		// New profile selected?
		if (Dlg.m_pChoice != App.m_pProfile)
		{
			// Switch profiles...
			App.m_pProfile = Dlg.m_pChoice;

			// Reset the scan details.
			App.m_oCache.Truncate();

			// Update UI.
			App.m_AppWnd.UpdateTitle();
			App.m_AppWnd.m_AppDlg.RefreshView();

			// Scan on profile change?
			if (App.m_bScanOnSwitch)
				App.m_AppWnd.PostCommand(ID_CACHE_RESCAN);
		}
	}
}

/******************************************************************************
** Method:		OnCacheRescan()
**
** Description:	Rescan the cache.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnCacheRescan()
{
	ASSERT(App.m_pProfile != NULL);

	CBusyCursor oCursor;

	// Get cache + index file full paths.
	CPath   strCacheDir  = App.m_pProfile->m_strCacheDir;
	CPath   strCacheFile = strCacheDir + App.m_strCacheIndex;

	// Check the cache path exists.
	if (!strCacheDir.Exists())
	{
		App.AlertMsg("Failed to access the cache folder:\n\n%s\n\nPlease check the profile setup in Options | Profiles.", strCacheDir);
		return;
	}

	// Check the cache index file exists.
	if (!strCacheFile.Exists())
	{
		App.AlertMsg("Failed to access the cache index file:\n\n%s", strCacheFile);
		return;
	}

	// Reset the scan details.
	App.m_oCache.Truncate();

	// Show the progress dialog.
	CProgressDlg Dlg;

	Dlg.RunModeless(App.m_AppWnd);
	Dlg.Title("Scanning Cache");
	Dlg.UpdateLabel("Searching cache for files...");

	CString	strFindMask;

	// Create 'find files' mask.
	strFindMask.Format("*.%s", App.m_strCacheExt);

	CFileFinder oFinder;
	CFileTree	oFiles;

	// Search cache for files.
	oFinder.Find(strCacheDir, strFindMask, false, oFiles);

	CStrArray& astrFiles = oFiles.Root()->m_oData.m_astrFiles;
	CIniFile   oIdxFile(strCacheFile);
	uint       nIndexErrs = 0;
	uint       nInfoErrs  = 0;
	uint       nTypeErrs  = 0;

	Dlg.InitMeter(astrFiles.Size());

	// For all files found...
	for (int i = 0; i < astrFiles.Size(); ++i)
	{
		// Get file name, index key and real name.
		CPath   strCacheName = astrFiles[i];
		CString strIndexName = strCacheName.FileTitle();
		CPath   strRealName  = oIdxFile.ReadString("Cache", strIndexName, "");

		Dlg.UpdateLabelAndMeter("Processing file: " + (CString)strRealName, i);

		// File in index?
		if (strRealName.Length() == 0)
		{
			nIndexErrs++;
			continue;
		}

		// Get file extension in lowercase.
		CString strExt = strRealName.FileExt().ToLower();
		char    cType  = '\0';
		CPath   strUTDir;

		// Determine file type and relevant UT folder.
		if ((strExt == ".u") || (strExt == ".int"))
		{
			cType  = SYSTEM_FILE;
			strUTDir = App.m_pProfile->m_strSystemDir;
		}
		else if (strExt == ".unr")
		{
			cType = MAP_FILE;
			strUTDir = App.m_pProfile->m_strMapDir;
		}
		else if (strExt == ".utx")
		{
			cType = TEXTURE_FILE;
			strUTDir = App.m_pProfile->m_strTextureDir;
		}
		else if (strExt == ".uax")
		{
			cType = SOUND_FILE;
			strUTDir = App.m_pProfile->m_strSoundDir;
		}
		else if (strExt == ".umx")
		{
			cType = MUSIC_FILE;
			strUTDir = App.m_pProfile->m_strMusicDir;
		}
		else
		{
			nTypeErrs++;
			continue;
		}

		CPath strUTFile = strUTDir + strRealName;

		// File already in relevant UT folder?
		bool bExists = strUTFile.Exists();

		CPath strFile = strCacheDir + strCacheName;
		struct _stat oInfo;

		// Get other file details.
		if (!CFile::QueryInfo(strFile, oInfo))
		{
			nInfoErrs++;
			continue;
		}

		// Add file details to the table.
		CRow& oRow = App.m_oCache.CreateRow();

		oRow[CCache::CACHE_FILENAME] = strCacheName;
		oRow[CCache::INDEX_KEY]      = strIndexName;
		oRow[CCache::REAL_FILENAME]  = strRealName;
		oRow[CCache::FILE_TYPE]      = cType;
		oRow[CCache::FILE_DATE]      = oInfo.st_ctime;
		oRow[CCache::FILE_SIZE]      = (int)oInfo.st_size;
		oRow[CCache::STATUS]         = (bExists) ? OLD_FILE : NEW_FILE;

		App.m_oCache.InsertRow(oRow);
	}

	// Update UI.
	App.m_AppWnd.m_AppDlg.RefreshView();

	// Remove progress dialog.
	Dlg.Destroy();

	// Report any errors.
	uint nErrors = nIndexErrs + nInfoErrs + nTypeErrs;

	if (nErrors > 0)
	{
		App.AlertMsg("Encountered %d error(s) during the scan.\n\n"
					 "%d Index Lookup Error(s)\n%d Status Query Error(s)\n%d File Extension Error(s)\n",
					nErrors, nIndexErrs, nInfoErrs, nTypeErrs);
	}

	// Search for old .tmp files?
	if (App.m_bScanForTmp)
	{
		CPath     strTmpDir = strCacheDir + CProfile::DEF_CACHE_TMP_DIR;
		CFileTree oTmpFiles;

		// Do search...
		oFinder.Find(strTmpDir, CProfile::DEF_CACHE_TMP_MASK, false, oTmpFiles);

		int nTmpFiles = oTmpFiles.Root()->m_oData.m_astrFiles.Size();

		// Found any AND user wants to delete them?
		if ( (nTmpFiles > 0) && (App.QueryMsg("Found %d .tmp cache file(s)?\n\nDo you want to delete them?", nTmpFiles) == IDYES) )
		{
			CStrArray& astrFiles = oTmpFiles.Root()->m_oData.m_astrFiles;
			int        nErrors   = 0;

			// Delete all files found...
			for (int i = 0; i < astrFiles.Size(); ++i)
			{
				CPath strTmpFile = strTmpDir + astrFiles[i];

				if (!CFile::Delete(strTmpFile))
					nErrors++;
			}

			// Report any errors.
			if (nErrors > 0)
				App.AlertMsg("Failed to delete %d file(s).", nErrors);
		}
	}

	// Search index old entries?
	if (App.m_bScanIndex)
	{
		CStrArray astrKeys, astrValues;
		CStrArray astrInvalid;

		// Read all cache index entries.
		oIdxFile.ReadSection("Cache", astrKeys, astrValues);

		// Find invalid entries.
		for (int i = 0; i < astrKeys.Size(); ++i)
		{
			CPath strFile = strCacheDir + (astrKeys[i] + "." + CProfile::DEF_CACHE_FILE_EXT);

			if (!strFile.Exists())
				astrInvalid.Add(astrKeys[i]);
		}

		int nEntries = astrInvalid.Size();

		// Found any AND user wants to delete them?
		if ( (nEntries > 0) && (App.QueryMsg("Found %d invalid cache index entries?\n\nDo you want to delete them?", nEntries) == IDYES) )
		{
			// Delete the invalid entries...
			for (int i = 0; i < astrInvalid.Size(); ++i)
				oIdxFile.DeleteEntry("Cache", astrInvalid[i]);
		}
	}
}

/******************************************************************************
** Method:		OnCacheExit()
**
** Description:	Terminate the app.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnCacheExit()
{
	App.m_AppWnd.Close();
}

/******************************************************************************
** Method:		OnEditMove()
**
** Description:	Move the selected files to the relevant folder.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnEditMove()
{
	ASSERT(App.m_pProfile != NULL);

	CResultSet oRS(App.m_oCache);

	// Get the current selection.
	App.m_AppWnd.m_AppDlg.GetSelectedFiles(oRS);

	int   nFiles      = oRS.Count();
	int   nErrors     = 0;
	DWORD dwLastError = 0;

	// Ignore if nothing to copy.
	if (nFiles == 0)
		return;

	// Get cache + index file full paths.
	CPath    strCacheDir  = App.m_pProfile->m_strCacheDir;
	CPath    strCacheFile = strCacheDir + App.m_strCacheIndex;
	CIniFile oIdxFile(strCacheFile);

	// Show the progress dialog.
	CProgressDlg Dlg;

	Dlg.RunModeless(App.m_AppWnd);
	Dlg.Title("Moving Files");
	Dlg.InitMeter(nFiles);

	// For all rows...
	for (int i = 0; i < nFiles; ++i)
	{
		CRow&	oRow        = oRS[i];
		CPath	strSrcDir   = App.m_pProfile->m_strCacheDir;
		CPath   strRealName = oRow[CCache::REAL_FILENAME];
		CPath	strDstDir;

		Dlg.UpdateLabelAndMeter("Moving file: " + (CString)strRealName, i);

		// Already moved?
		if (oRow[CCache::STATUS] == OLD_FILE)
			continue;

		// Get the folder to copy to.
		switch (oRow[CCache::FILE_TYPE].GetChar())
		{
			case SYSTEM_FILE :	strDstDir = App.m_pProfile->m_strSystemDir;		break;
			case MAP_FILE    :	strDstDir = App.m_pProfile->m_strMapDir;		break;
			case TEXTURE_FILE:	strDstDir = App.m_pProfile->m_strTextureDir;	break;
			case SOUND_FILE  :	strDstDir = App.m_pProfile->m_strSoundDir;		break;
			case MUSIC_FILE  :	strDstDir = App.m_pProfile->m_strMusicDir;		break;
			default:			ASSERT(false);									break;
		}

		// Create the src and dst full paths.
		CString	strSrcFile = strSrcDir + oRow[CCache::CACHE_FILENAME];
		CString	strDstFile = strDstDir + oRow[CCache::REAL_FILENAME];

		// Move it...
		if (::MoveFile(strSrcFile, strDstFile) == 0)
		{
			nErrors++;
			dwLastError = ::GetLastError();
			continue;
		}

		// Delete from index.
		oIdxFile.DeleteEntry("Cache", oRow[CCache::INDEX_KEY]);

		// Delete from cache table.
		App.m_oCache.DeleteRow(oRow);
	}

	// Remove progress dialog.
	Dlg.Destroy();

	// Report any errors.
	if (nErrors > 0)
		App.AlertMsg("Failed to move %d of %d file(s).", nErrors, nFiles);

	// Update UI.
	App.m_AppWnd.m_AppDlg.RefreshView();
}

/******************************************************************************
** Method:		OnEditCopy()
**
** Description:	Copy the selected files to the relevant folder.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnEditCopy()
{
	ASSERT(App.m_pProfile != NULL);

	CResultSet oRS(App.m_oCache);

	// Get the current selection.
	App.m_AppWnd.m_AppDlg.GetSelectedFiles(oRS);

	int   nFiles      = oRS.Count();
	int   nErrors     = 0;
	DWORD dwLastError = 0;

	// Ignore if nothing to copy.
	if (nFiles == 0)
		return;

	// Get cache + index file full paths.
	CPath    strCacheDir  = App.m_pProfile->m_strCacheDir;
	CPath    strCacheFile = strCacheDir + App.m_strCacheIndex;
	CIniFile oIdxFile(strCacheFile);

	// Show the progress dialog.
	CProgressDlg Dlg;

	Dlg.RunModeless(App.m_AppWnd);
	Dlg.Title("Copying Files");
	Dlg.InitMeter(nFiles);

	// For all rows...
	for (int i = 0; i < nFiles; ++i)
	{
		CRow&	oRow        = oRS[i];
		CPath	strSrcDir   = App.m_pProfile->m_strCacheDir;
		CPath   strRealName = oRow[CCache::REAL_FILENAME];
		CPath	strDstDir;

		Dlg.UpdateLabelAndMeter("Copying file: " + (CString)strRealName, i);

		// Already copied?
		if (oRow[CCache::STATUS] == OLD_FILE)
			continue;

		// Get the folder to copy to.
		switch (oRow[CCache::FILE_TYPE].GetChar())
		{
			case SYSTEM_FILE :	strDstDir = App.m_pProfile->m_strSystemDir;		break;
			case MAP_FILE    :	strDstDir = App.m_pProfile->m_strMapDir;		break;
			case TEXTURE_FILE:	strDstDir = App.m_pProfile->m_strTextureDir;	break;
			case SOUND_FILE  :	strDstDir = App.m_pProfile->m_strSoundDir;		break;
			case MUSIC_FILE  :	strDstDir = App.m_pProfile->m_strMusicDir;		break;
			default:			ASSERT(false);									break;
		}

		// Create the src and dst full paths.
		CString	strSrcFile = strSrcDir + oRow[CCache::CACHE_FILENAME];
		CString	strDstFile = strDstDir + oRow[CCache::REAL_FILENAME];

		// Copy it...
		if (::CopyFile(strSrcFile, strDstFile, TRUE) == 0)
		{
			nErrors++;
			dwLastError = ::GetLastError();
			continue;
		}

		// Update cache table status.
		oRow[CCache::STATUS] = OLD_FILE;
	}

	// Remove progress dialog.
	Dlg.Destroy();

	// Report any errors.
	if (nErrors > 0)
		App.AlertMsg("Failed to copy %d of %d file(s).", nErrors, nFiles);

	// Update UI.
	App.m_AppWnd.m_AppDlg.RefreshView();
}

/******************************************************************************
** Method:		OnEditDelete()
**
** Description:	Delete the selected files from the cache.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnEditDelete()
{
	ASSERT(App.m_pProfile != NULL);

	CResultSet oRS(App.m_oCache);

	// Get the current selection.
	App.m_AppWnd.m_AppDlg.GetSelectedFiles(oRS);

	int   nFiles      = oRS.Count();
	int   nErrors     = 0;
	DWORD dwLastError = 0;

	// Ignore if nothing to copy.
	if (nFiles == 0)
		return;

	// Get cache + index file full paths.
	CPath    strCacheDir  = App.m_pProfile->m_strCacheDir;
	CPath    strCacheFile = strCacheDir + App.m_strCacheIndex;
	CIniFile oIdxFile(strCacheFile);

	// For all rows...
	for (int i = 0; i < nFiles; ++i)
	{
		CRow&	oRow       = oRS[i];
		CPath	strSrcDir  = App.m_pProfile->m_strCacheDir;
		CString	strSrcFile = strSrcDir + oRow[CCache::CACHE_FILENAME];

		// Delete it...
		if (::DeleteFile(strSrcFile) == 0)
		{
			nErrors++;
			dwLastError = ::GetLastError();
			continue;
		}

		// Delete from index.
		oIdxFile.DeleteEntry("Cache", oRow[CCache::INDEX_KEY]);

		// Delete from cache table.
		App.m_oCache.DeleteRow(oRow);
	}

	// Report any errors.
	if (nErrors > 0)
		App.AlertMsg("Failed to delete %d of %d file(s).", nErrors, nFiles);

	// Update UI.
	App.m_AppWnd.m_AppDlg.RefreshView();
}

/******************************************************************************
** Method:		OnEditCopyTo()
**
** Description:	Copy the selected files to different folder.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnEditCopyTo()
{
	CResultSet oRS(App.m_oCache);

	// Get the current selection.
	App.m_AppWnd.m_AppDlg.GetSelectedFiles(oRS);

	int   nFiles      = oRS.Count();
	int   nErrors     = 0;
	DWORD dwLastError = 0;

	// Ignore if nothing to copy.
	if (nFiles == 0)
		return;

	CPath strDstDir = App.m_strLastCopyTo;

	// Get the directory to copy to.
	if (!strDstDir.SelectDir(App.m_AppWnd, "Select Folder To Copy To", strDstDir))
		return;

	// Get cache + index file full paths.
	CPath    strCacheDir  = App.m_pProfile->m_strCacheDir;
	CPath    strCacheFile = strCacheDir + App.m_strCacheIndex;
	CIniFile oIdxFile(strCacheFile);

	// Show the progress dialog.
	CProgressDlg Dlg;

	Dlg.RunModeless(App.m_AppWnd);
	Dlg.Title("Copying Files");
	Dlg.InitMeter(nFiles);

	// For all rows...
	for (int i = 0; i < nFiles; ++i)
	{
		CRow&	oRow        = oRS[i];
		CPath	strSrcDir   = App.m_pProfile->m_strCacheDir;
		CPath   strRealName = oRow[CCache::REAL_FILENAME];

		Dlg.UpdateLabelAndMeter("Copying file: " + (CString)strRealName, i);

		// Create the src and dst full paths.
		CPath strSrcFile = strSrcDir + oRow[CCache::CACHE_FILENAME];
		CPath strDstFile = strDstDir + oRow[CCache::REAL_FILENAME];

		// Ignore if destination already exists.
		if (strDstFile.Exists())
			continue;

		// Copy it...
		if (::CopyFile(strSrcFile, strDstFile, TRUE) == 0)
		{
			nErrors++;
			dwLastError = ::GetLastError();
			continue;
		}
	}

	// Remove progress dialog.
	Dlg.Destroy();

	// Report any errors.
	if (nErrors > 0)
		App.AlertMsg("Failed to copy %d of %d file(s).", nErrors, nFiles);

	// Save folder as default.
	App.m_strLastCopyTo = strDstDir;
}

/******************************************************************************
** Method:		OnViewSelectAll()
**
** Description:	Select all files in the view.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnViewSelectAll()
{
	App.m_AppWnd.m_AppDlg.SelectAll();
}

/******************************************************************************
** Method:		OnSortBy*()
**
** Description:	Change the sort order of the grid.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnViewSortByName()
{
	OnViewSortByColumn(CCache::REAL_FILENAME, CSortColumns::ASC);
}

void CAppCmds::OnViewSortByType()
{
	OnViewSortByColumn(CCache::FILE_TYPE, CSortColumns::ASC);
}

void CAppCmds::OnViewSortByDate()
{
	OnViewSortByColumn(CCache::FILE_DATE, CSortColumns::DESC);
}

void CAppCmds::OnViewSortBySize()
{
	OnViewSortByColumn(CCache::FILE_SIZE, CSortColumns::DESC);
}

void CAppCmds::OnViewSortByColumn(uint nColumn, CSortColumns::Dir eDefDir)
{
	CAppDlg& oAppDlg = App.m_AppWnd.m_AppDlg;

	// Already sorted by column?
	if (oAppDlg.m_nSortColumn == nColumn)
	{
		// Swap sort order.
		oAppDlg.m_eSortOrder = (CSortColumns::Dir) -(oAppDlg.m_eSortOrder);
	}
	else
	{
		// Set column and initial sort order.
		oAppDlg.m_nSortColumn = nColumn;
		oAppDlg.m_eSortOrder  = eDefDir;
	}

	// Refresh grid.
	oAppDlg.RefreshView();
}

/******************************************************************************
** Method:		OnViewShowAll()
**
** Description:	Toggles showing of all files in the view.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnViewShowAll()
{
	CAppDlg& oAppDlg = App.m_AppWnd.m_AppDlg;

	// Toggle setting.
	oAppDlg.m_bShowAllFiles = !oAppDlg.m_bShowAllFiles;

	// Refresh grid.
	App.m_AppWnd.m_AppDlg.RefreshView();
}

/******************************************************************************
** Method:		OnOptionsProfiles()
**
** Description:	Show the profiles editing dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnOptionsProfiles()
{
	CProfileCfgDlg Dlg;

	Dlg.RunModal(App.m_rMainWnd);
}

/******************************************************************************
** Method:		OnOptionsPrefs()
**
** Description:	Show the preferences editing dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnOptionsPrefs()
{
	CPrefsDlg Dlg;

	Dlg.RunModal(App.m_rMainWnd);
}

/******************************************************************************
** Method:		OnOptionsUTConfig()
**
** Description:	Show the dialog for editing the UT settings.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnOptionsUTConfig()
{
	ASSERT(App.m_pProfile != NULL);

	// Check config file exists.
	if (!App.m_pProfile->m_strConfigFile.Exists())
	{
		App.AlertMsg("The config file is missing:\n\n%s", App.m_pProfile->m_strConfigFile);
		return;
	}

	CUTConfigDlg Dlg;

	Dlg.RunModal(App.m_rMainWnd);
}

/******************************************************************************
** Method:		OnHelpContents()
**
** Description:	Show the help file.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnHelpContents()
{
	App.m_oHelpFile.Contents();
}

/******************************************************************************
** Method:		OnHelpAbout()
**
** Description:	Show the about dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnHelpAbout()
{
	CAboutDlg Dlg;

	Dlg.RunModal(App.m_rMainWnd);
}

/******************************************************************************
** Method:		OnUI...()
**
** Description:	UI update handlers.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

