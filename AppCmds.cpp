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
#include "RestoreDlg.hpp"
#include "ErrorsDlg.hpp"

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
		CMD_ENTRY(ID_CACHE_PROFILE,		OnCacheProfile,		NULL,				 3)
		CMD_ENTRY(ID_CACHE_RESCAN,		OnCacheRescan,		NULL,				 0)
		CMD_ENTRY(ID_CACHE_RESTORE,		OnCacheRestore,		OnUICacheRestore,	-1)
		CMD_ENTRY(ID_CACHE_EXIT,		OnCacheExit,		NULL,				-1)
		// Edit menu.
		CMD_ENTRY(ID_EDIT_PIN,			OnEditPin,			OnUIEditPin,		-1)
		CMD_ENTRY(ID_EDIT_MOVE,			OnEditMove,			OnUIEditMove,		 5)
		CMD_ENTRY(ID_EDIT_COPY,			OnEditCopy,			OnUIEditCopy,		 6)
		CMD_ENTRY(ID_EDIT_DELETE,		OnEditDelete,		OnUIEditDelete,		 7)
		CMD_ENTRY(ID_EDIT_COPY_TO,		OnEditCopyTo,		OnUIEditCopyTo,		 9)
		// View menu.
		CMD_ENTRY(ID_VIEW_SELECT_NEW,	OnViewSelectNew,	NULL,				-1)
		CMD_ENTRY(ID_VIEW_SELECT_ALL,	OnViewSelectAll,	NULL,				-1)
		CMD_ENTRY(ID_VIEW_SORT_NAME,	OnViewSortByName,	OnUIViewSortByName,	-1)
		CMD_ENTRY(ID_VIEW_SORT_TYPE,	OnViewSortByType,	OnUIViewSortByType,	-1)
		CMD_ENTRY(ID_VIEW_SORT_DATE,	OnViewSortByDate,	OnUIViewSortByDate,	-1)
		CMD_ENTRY(ID_VIEW_SORT_SIZE,	OnViewSortBySize,	OnUIViewSortBySize,	-1)
		CMD_ENTRY(ID_VIEW_SHOW_ALL,		OnViewShowAll,		OnUIViewShowAll,	-1)
		// Options menu.
		CMD_ENTRY(ID_OPTIONS_PROFILES,	OnOptionsProfiles,	NULL,				-1)
		CMD_ENTRY(ID_OPTIONS_PREFS,		OnOptionsPrefs,		NULL,				-1)
		CMD_ENTRY(ID_OPTIONS_UT_CONFIG,	OnOptionsUTConfig,	NULL,				-1)
		// Help menu.
		CMD_ENTRY(ID_HELP_CONTENTS,		OnHelpContents,		NULL,				 1)
		CMD_ENTRY(ID_HELP_ABOUT,		OnHelpAbout,		NULL,				-1)
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
			UpdateUI();

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

	// Create errors dialog.
	CErrorsDlg dlgErrors;

	CString	strFindMask;

	// Create 'find files' mask.
	strFindMask.Format("*.%s", App.m_strCacheExt);

	CFileFinder oFinder;
	CFileTree	oFiles;

	// Search cache for files.
	oFinder.Find(strCacheDir, strFindMask, false, oFiles);

	CStrArray& astrFiles = oFiles.Root()->m_oData.m_astrFiles;
	CIniFile   oIdxFile(strCacheFile);

	Dlg.InitMeter(astrFiles.Size());

	// For all files found...
	for (int i = 0; i < astrFiles.Size(); ++i)
	{
		// Get file name, index key and real name.
		CPath   strCacheName = astrFiles[i];
		CString strIndexName = strCacheName.FileTitle();
		CPath   strRealName  = oIdxFile.ReadString("Cache", strIndexName, "");

		// File not in index?
		if (strRealName.Length() == 0)
		{
			dlgErrors.m_astrFiles.Add(strCacheName);
			dlgErrors.m_astrErrors.Add("Index entry missing");
			continue;
		}

		Dlg.UpdateLabelAndMeter("Processing file: " + (CString)strRealName, i);

		// Get file type from extension.
		CString strExt = strRealName.FileExt().ToLower();
		char    cType  = App.m_pProfile->GetFileType(strExt);

		// Unknown file type?
		if (cType == NULL)
		{
			dlgErrors.m_astrFiles.Add(strRealName);
			dlgErrors.m_astrErrors.Add("Unknown file type");
			continue;
		}

		// File already in relevant UT folder?
		CPath strUTDir  = App.m_pProfile->GetTypeDir(cType);
		CPath strUTFile = strUTDir + strRealName;
		bool  bExists   = strUTFile.Exists();

		// File pinned in cache?
		bool bPinned = (App.m_astrPinned.Find(strRealName, true) != -1);

		// Get other file details.
		CPath strFile = strCacheDir + strCacheName;
		struct _stat oInfo;

		if (!CFile::QueryInfo(strFile, oInfo))
		{
			dlgErrors.m_astrFiles.Add(strCacheName);
			dlgErrors.m_astrErrors.Add("Size query failed");
			continue;
		}

		// Add file details to the table.
		CRow& oRow = App.m_oCache.CreateRow();

		oRow[CCache::CACHE_FILENAME] = strCacheName;
		oRow[CCache::INDEX_KEY]      = strIndexName;
		oRow[CCache::REAL_FILENAME]  = strRealName;
		oRow[CCache::FILE_TYPE]      = cType;
		oRow[CCache::FILE_DATE]      = oInfo.st_mtime;
		oRow[CCache::FILE_SIZE]      = (int)oInfo.st_size;
		oRow[CCache::STATUS]         = (bPinned) ? PIN_FILE : (bExists) ? OLD_FILE : NEW_FILE;

		App.m_oCache.InsertRow(oRow);
	}

	// Update UI.
	App.m_AppWnd.m_AppDlg.RefreshView();

	// Remove progress dialog.
	Dlg.Destroy();

	// Report any errors.
	if (dlgErrors.m_astrFiles.Size()  > 0)
	{
		dlgErrors.m_strTitle = "Cache Scan Errors";
		dlgErrors.RunModal(App.m_AppWnd);
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

	// Search index for old entries?
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

	UpdateUI();
}

/******************************************************************************
** Method:		OnCacheRestore()
**
** Description:	Restore files to the cache which were previously extracted.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnCacheRestore()
{
	ASSERT(App.m_pProfile != NULL);

	CBusyCursor oCursor;

	// Tmp table to store files we can restore.
	CCache oRestore(App.m_oMDB);

	try
	{
		// Get logfile path.
		CPath   strLogFile(CPath::AppDir(), "UTCacheMgr.log");
		CFile   fLogFile;

		// Logfile not created yet?
		if (!strLogFile.Exists())
		{
			App.NotifyMsg("There are no files that can be restored.");
			return;
		}

		fLogFile.Open(strLogFile, CFile::ReadOnly);

		// Show the progress dialog.
		CProgressDlg Dlg;

		Dlg.RunModeless(App.m_AppWnd);
		Dlg.Title("Loading Restore Log");
		Dlg.InitMeter(CFile::Size(strLogFile));

		// For all lines.
		while (!fLogFile.IsEOF())
		{
			CString strLine = fLogFile.ReadLine();

			// Ignore blank and commented lines.
			if ( (strLine.Empty()) || (strLine[0] == '#') )
				continue;

			CStrArray astrFields;

			// Split line into separate fields.
			// NB: Fields are RealFileName, CacheFileName, Size.
			if (strLine.Split(",", astrFields) != 3)
				continue;

			CPath strRealName  = astrFields[0];
			CPath strCacheName = astrFields[1];
			int   nFileSize    = atoi(astrFields[2]);

			Dlg.UpdateLabelAndMeter("Checking file: " + (CString)strRealName, fLogFile.CurPos());

			// Duplicate entry?
			if (oRestore.Exists(CWhereEqual(CCache::REAL_FILENAME, strRealName)))
				continue;

			// Get file type from extension.
			CString strExt = strRealName.FileExt().ToLower();
			char    cType  = App.m_pProfile->GetFileType(strExt);

			// Unknown file type?
			if (cType == NULL)
				continue;

			CPath strUTDir  = App.m_pProfile->GetTypeDir(cType);
			CPath strUTFile = strUTDir + strRealName;

			// File missing from relevant UT folder?
			if (!strUTFile.Exists())
				continue;

			struct _stat oInfo;

			// File has different size?
			if ( (!CFile::QueryInfo(strUTFile, oInfo))
			  || (oInfo.st_size != nFileSize) )
				continue;

			// Add file details to the table.
			CRow& oRow = oRestore.CreateRow();

			oRow[CCache::CACHE_FILENAME] = strCacheName;
			oRow[CCache::INDEX_KEY]      = strCacheName.FileTitle();
			oRow[CCache::REAL_FILENAME]  = strRealName;
			oRow[CCache::FILE_TYPE]      = cType;
			oRow[CCache::FILE_DATE]      = oInfo.st_ctime;
			oRow[CCache::FILE_SIZE]      = nFileSize;
			oRow[CCache::STATUS]         = OLD_FILE;

			oRestore.InsertRow(oRow);
		}

		// Remove progress dialog.
		Dlg.Destroy();

		fLogFile.Close();
	}
	catch (CStreamException& e)
	{
		// Report error.
		App.AlertMsg(e.ErrorText());
		return;
	}

	// Nothing to restore?
	if (oRestore.RowCount() == 0)
	{
		App.NotifyMsg("There are no files that can be restored.");
		return;
	}

	CRestoreDlg oRestoreDlg;

	oRestoreDlg.m_pTable = &oRestore;

	// Get files to restore from user.
	if (oRestoreDlg.RunModal(App.m_AppWnd) != IDOK)
		return;

	int nFiles = oRestore.RowCount();

	// No files selected?
	if (nFiles == 0)
		return;

	// Get cache + index file full paths.
	CPath    strCacheDir  = App.m_pProfile->m_strCacheDir;
	CPath    strCacheFile = strCacheDir + App.m_strCacheIndex;
	CIniFile oIdxFile(strCacheFile);

	// Show the progress dialog.
	CProgressDlg Dlg;

	Dlg.RunModeless(App.m_AppWnd);
	Dlg.Title("Restoring Files");
	Dlg.InitMeter(nFiles);

	// Create errors dialog.
	CErrorsDlg dlgErrors;

	// For all files to restore...
	for (int i = 0; i < nFiles; ++i)
	{
		CRow& oRow        = oRestore[i];
		CPath strRealName = oRow[CCache::REAL_FILENAME];
		CPath strDstDir   = strCacheDir;

		Dlg.UpdateLabelAndMeter("Restoring file: " + (CString)strRealName, i);

		// Get the folder to move from.
		CPath strSrcDir = App.m_pProfile->GetTypeDir(oRow[CCache::FILE_TYPE]);

		// Create the src and dst full paths.
		CString	strSrcFile = strSrcDir + oRow[CCache::REAL_FILENAME];
		CString	strDstFile = strDstDir + oRow[CCache::CACHE_FILENAME];

		// Move it...
		if (::MoveFile(strSrcFile, strDstFile) == 0)
		{
			dlgErrors.m_astrFiles.Add(oRow[CCache::REAL_FILENAME].GetString());
			dlgErrors.m_astrErrors.Add(App.FormatError());
			continue;
		}

		// Add to index.
		oIdxFile.WriteString("Cache", oRow[CCache::INDEX_KEY], strRealName);
	}

	// Remove progress dialog.
	Dlg.Destroy();

	// Report any errors.
	if (dlgErrors.m_astrFiles.Size() > 0)
	{
		dlgErrors.m_strTitle = "Restore Errors";
		dlgErrors.RunModal(App.m_AppWnd);
	}

	// Rescan cache to pick up restored files.
	OnCacheRescan();
	UpdateUI();
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
** Method:		OnEditPin()
**
** Description:	Mark the selected files as pinned or unpinned within the cache.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnEditPin()
{
	ASSERT(App.m_pProfile != NULL);

	CResultSet oRS(App.m_oCache);

	// Get the current selection.
	App.m_AppWnd.m_AppDlg.GetSelectedFiles(oRS);

	int nFiles = oRS.Count();

	// Ignore if nothing to do.
	if (nFiles == 0)
		return;

	CStrArray& astrPinned = App.m_astrPinned;

	// For all selected rows...
	for (int i = 0; i < nFiles; ++i)
	{
		CRow&       oRow    = oRS[i];
		char        cStatus = oRow[CCache::STATUS];
		const char* pszName = oRow[CCache::REAL_FILENAME];

		// Add to pinned list?
		if (cStatus == NEW_FILE)
		{
			if (astrPinned.Find(pszName, true) == -1)
				astrPinned.Add(pszName);
		}

		// Remove from pinned list?
		if (cStatus == PIN_FILE)
		{
			if (astrPinned.Find(pszName, true) != -1)
				astrPinned.Delete(astrPinned.Find(pszName, true));
		}
	}

	// For all cache table rows...
	for (i = 0; i < App.m_oCache.RowCount(); ++i)
	{
		CRow&       oRow     = App.m_oCache[i];
		char        cStatus  = oRow[CCache::STATUS];
		const char* pszName  = oRow[CCache::REAL_FILENAME];
		int         nListIdx = astrPinned.Find(pszName, true);

		// Pin, if in pinned list AND not already pinned.
		if ( (nListIdx != -1) && (cStatus != PIN_FILE) )
		{
			oRow[CCache::STATUS] = PIN_FILE;
		}
		// Unpin, if not in pinned list AND currently pinned.
		else if ( (nListIdx == -1) && (cStatus == PIN_FILE) )
		{
			oRow[CCache::STATUS] = NEW_FILE;
		}
	}

	// Update UI.
	App.m_AppWnd.m_AppDlg.RefreshView();
	UpdateUI();
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
	CResultSet oEditsRS(App.m_oCache);

	// Get the current selection.
	App.m_AppWnd.m_AppDlg.GetSelectedFiles(oRS);

	int nFiles = oRS.Count();

	// Ignore if nothing to move.
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

	// Create errors dialog.
	CErrorsDlg dlgErrors;

	// For all rows...
	for (int i = 0; i < nFiles; ++i)
	{
		CRow&	oRow        = oRS[i];
		CPath	strSrcDir   = App.m_pProfile->m_strCacheDir;
		CPath   strRealName = oRow[CCache::REAL_FILENAME];

		Dlg.UpdateLabelAndMeter("Moving file: " + (CString)strRealName, i);

		// Shouldn't move?
		if (oRow[CCache::STATUS] != NEW_FILE)
			continue;

		// Get the folder to copy to.
		CPath strDstDir = App.m_pProfile->GetTypeDir(oRow[CCache::FILE_TYPE]);

		// Create the src and dst full paths.
		CString	strSrcFile = strSrcDir + oRow[CCache::CACHE_FILENAME];
		CString	strDstFile = strDstDir + oRow[CCache::REAL_FILENAME];

		// Move it...
		if (::MoveFile(strSrcFile, strDstFile) == 0)
		{
			dlgErrors.m_astrFiles.Add(oRow[CCache::REAL_FILENAME].GetString());
			dlgErrors.m_astrErrors.Add(App.FormatError());
			continue;
		}

		// Delete from index.
		oIdxFile.DeleteEntry("Cache", oRow[CCache::INDEX_KEY]);

		// Add to list of edits.
		oEditsRS.Add(oRow);
	}

	// Remove progress dialog.
	Dlg.Destroy();

	// Log cache changes.
	if (App.m_bLogEdits)
		LogEdits(oEditsRS);

	// Delete rows from cache table.
	App.m_oCache.DeleteRows(oEditsRS);

	// Report any errors.
	if (dlgErrors.m_astrFiles.Size() > 0)
	{
		dlgErrors.m_strTitle = "Move Errors";
		dlgErrors.RunModal(App.m_AppWnd);
	}

	// Update UI.
	App.m_AppWnd.m_AppDlg.RefreshView();
	UpdateUI();
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
	CResultSet oEditsRS(App.m_oCache);

	// Get the current selection.
	App.m_AppWnd.m_AppDlg.GetSelectedFiles(oRS);

	int nFiles = oRS.Count();

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

	// Create errors dialog.
	CErrorsDlg dlgErrors;

	// For all rows...
	for (int i = 0; i < nFiles; ++i)
	{
		CRow&	oRow        = oRS[i];
		CPath	strSrcDir   = App.m_pProfile->m_strCacheDir;
		CPath   strRealName = oRow[CCache::REAL_FILENAME];

		Dlg.UpdateLabelAndMeter("Copying file: " + (CString)strRealName, i);

		// Shouldn't copy?
		if (oRow[CCache::STATUS] != NEW_FILE)
			continue;

		// Get the folder to copy to.
		CPath strDstDir = App.m_pProfile->GetTypeDir(oRow[CCache::FILE_TYPE]);

		// Create the src and dst full paths.
		CString	strSrcFile = strSrcDir + oRow[CCache::CACHE_FILENAME];
		CString	strDstFile = strDstDir + oRow[CCache::REAL_FILENAME];

		// Copy it...
		if (::CopyFile(strSrcFile, strDstFile, TRUE) == 0)
		{
			dlgErrors.m_astrFiles.Add(oRow[CCache::REAL_FILENAME].GetString());
			dlgErrors.m_astrErrors.Add(App.FormatError());
			continue;
		}

		// Update cache table status.
		oRow[CCache::STATUS] = OLD_FILE;

		// Add to list of edits.
		oEditsRS.Add(oRow);
	}

	// Remove progress dialog.
	Dlg.Destroy();

	// Log cache changes.
	if (App.m_bLogEdits)
		LogEdits(oEditsRS);

	// Report any errors.
	if (dlgErrors.m_astrFiles.Size() > 0)
	{
		dlgErrors.m_strTitle = "Copy Errors";
		dlgErrors.RunModal(App.m_AppWnd);
	}

	// Update UI.
	App.m_AppWnd.m_AppDlg.RefreshView();
	UpdateUI();
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
	CResultSet oEditsRS(App.m_oCache);

	// Get the current selection.
	App.m_AppWnd.m_AppDlg.GetSelectedFiles(oRS);

	int nFiles = oRS.Count();

	// Ignore if nothing to copy.
	if (nFiles == 0)
		return;

	// Get cache + index file full paths.
	CPath    strCacheDir  = App.m_pProfile->m_strCacheDir;
	CPath    strCacheFile = strCacheDir + App.m_strCacheIndex;
	CIniFile oIdxFile(strCacheFile);

	// Create errors dialog.
	CErrorsDlg dlgErrors;

	// For all rows...
	for (int i = 0; i < nFiles; ++i)
	{
		CRow&	oRow       = oRS[i];
		CPath	strSrcDir  = App.m_pProfile->m_strCacheDir;
		CString	strSrcFile = strSrcDir + oRow[CCache::CACHE_FILENAME];

		// Delete it...
		if (::DeleteFile(strSrcFile) == 0)
		{
			dlgErrors.m_astrFiles.Add(oRow[CCache::REAL_FILENAME].GetString());
			dlgErrors.m_astrErrors.Add(App.FormatError());
			continue;
		}

		// Delete from index.
		oIdxFile.DeleteEntry("Cache", oRow[CCache::INDEX_KEY]);

		// Add to list of edits.
		oEditsRS.Add(oRow);
	}

	// Delete rows from cache table.
	App.m_oCache.DeleteRows(oEditsRS);

	// Report any errors.
	if (dlgErrors.m_astrFiles.Size() > 0)
	{
		dlgErrors.m_strTitle = "Delete Errors";
		dlgErrors.RunModal(App.m_AppWnd);
	}

	// Update UI.
	App.m_AppWnd.m_AppDlg.RefreshView();
	UpdateUI();
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

	int nFiles = oRS.Count();

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

	// Create errors dialog.
	CErrorsDlg dlgErrors;

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
			dlgErrors.m_astrFiles.Add(oRow[CCache::REAL_FILENAME].GetString());
			dlgErrors.m_astrErrors.Add(App.FormatError());
			continue;
		}
	}

	// Remove progress dialog.
	Dlg.Destroy();

	// Report any errors.
	if (dlgErrors.m_astrFiles.Size() > 0)
	{
		dlgErrors.m_strTitle = "Copy Errors";
		dlgErrors.RunModal(App.m_AppWnd);
	}

	// Save folder as default.
	App.m_strLastCopyTo = strDstDir;

	UpdateUI();
}

/******************************************************************************
** Method:		OnViewSelectNew()
**
** Description:	Select all new files in the view.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnViewSelectNew()
{
	App.m_AppWnd.m_AppDlg.SelectNew();
	UpdateUI();
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
	UpdateUI();
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

	// Update UI.
	UpdateUI();
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

	// Update UI.
	UpdateUI();
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

	UpdateUI();
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

void CAppCmds::OnUICacheRestore()
{
	ASSERT(App.m_pProfile != NULL);

	CMenu&       oMenu    = App.m_AppWnd.m_Menu;
//	CAppToolBar& oToolBar = App.m_AppWnd.m_ToolBar;
//	CAppDlg&     oAppDlg  = App.m_AppWnd.m_AppDlg;

	bool bReadOnly = App.m_pProfile->m_bReadOnly;

	oMenu.EnableCmd(ID_CACHE_RESTORE, !bReadOnly);
//	oToolBar.m_btnMove.Enable(bSelection && !bReadOnly);
}

void CAppCmds::OnUIEditPin()
{
	CMenu&       oMenu    = App.m_AppWnd.m_Menu;
//	CAppToolBar& oToolBar = App.m_AppWnd.m_ToolBar;
	CAppDlg&     oAppDlg  = App.m_AppWnd.m_AppDlg;

	bool bSelection = oAppDlg.m_lvGrid.IsSelection();

	oMenu.EnableCmd(ID_EDIT_PIN, bSelection);
//	oToolBar.m_btnPin.Enable(bSelection);
}

void CAppCmds::OnUIEditMove()
{
	ASSERT(App.m_pProfile != NULL);

	CMenu&       oMenu    = App.m_AppWnd.m_Menu;
	CAppToolBar& oToolBar = App.m_AppWnd.m_ToolBar;
	CAppDlg&     oAppDlg  = App.m_AppWnd.m_AppDlg;

	bool bSelection = oAppDlg.m_lvGrid.IsSelection();
	bool bReadOnly  = App.m_pProfile->m_bReadOnly;

	oMenu.EnableCmd(ID_EDIT_MOVE, bSelection && !bReadOnly);
	oToolBar.m_btnMove.Enable(bSelection && !bReadOnly);
}

void CAppCmds::OnUIEditCopy()
{
	CMenu&       oMenu    = App.m_AppWnd.m_Menu;
	CAppToolBar& oToolBar = App.m_AppWnd.m_ToolBar;
	CAppDlg&     oAppDlg  = App.m_AppWnd.m_AppDlg;

	bool bSelection = oAppDlg.m_lvGrid.IsSelection();

	oMenu.EnableCmd(ID_EDIT_COPY, bSelection);
	oToolBar.m_btnCopy.Enable(bSelection);
}

void CAppCmds::OnUIEditDelete()
{
	ASSERT(App.m_pProfile != NULL);

	CMenu&       oMenu    = App.m_AppWnd.m_Menu;
	CAppToolBar& oToolBar = App.m_AppWnd.m_ToolBar;
	CAppDlg&     oAppDlg  = App.m_AppWnd.m_AppDlg;

	bool bSelection = oAppDlg.m_lvGrid.IsSelection();
	bool bReadOnly  = App.m_pProfile->m_bReadOnly;

	oMenu.EnableCmd(ID_EDIT_DELETE, bSelection && !bReadOnly);
	oToolBar.m_btnDelete.Enable(bSelection && !bReadOnly);
}

void CAppCmds::OnUIEditCopyTo()
{
	CMenu&       oMenu    = App.m_AppWnd.m_Menu;
	CAppToolBar& oToolBar = App.m_AppWnd.m_ToolBar;
	CAppDlg&     oAppDlg  = App.m_AppWnd.m_AppDlg;

	bool bSelection = oAppDlg.m_lvGrid.IsSelection();

	oMenu.EnableCmd(ID_EDIT_COPY_TO, bSelection);
	oToolBar.m_btnCopyTo.Enable(bSelection);
}

void CAppCmds::OnUIViewShowAll()
{
	CMenu&   oMenu   = App.m_AppWnd.m_Menu;
	CAppDlg& oAppDlg = App.m_AppWnd.m_AppDlg;

	oMenu.CheckCmd(ID_VIEW_SHOW_ALL, oAppDlg.m_bShowAllFiles);
}

void CAppCmds::OnUIViewSortByName()
{
	CMenu&   oMenu   = App.m_AppWnd.m_Menu;
	CAppDlg& oAppDlg = App.m_AppWnd.m_AppDlg;

	oMenu.CheckCmd(ID_VIEW_SORT_NAME, (oAppDlg.m_nSortColumn == CCache::REAL_FILENAME));
}

void CAppCmds::OnUIViewSortByType()
{
	CMenu&   oMenu   = App.m_AppWnd.m_Menu;
	CAppDlg& oAppDlg = App.m_AppWnd.m_AppDlg;

	oMenu.CheckCmd(ID_VIEW_SORT_TYPE, (oAppDlg.m_nSortColumn == CCache::FILE_TYPE));
}

void CAppCmds::OnUIViewSortByDate()
{
	CMenu&   oMenu   = App.m_AppWnd.m_Menu;
	CAppDlg& oAppDlg = App.m_AppWnd.m_AppDlg;

	oMenu.CheckCmd(ID_VIEW_SORT_DATE, (oAppDlg.m_nSortColumn == CCache::FILE_DATE));
}

void CAppCmds::OnUIViewSortBySize()
{
	CMenu&   oMenu   = App.m_AppWnd.m_Menu;
	CAppDlg& oAppDlg = App.m_AppWnd.m_AppDlg;

	oMenu.CheckCmd(ID_VIEW_SORT_SIZE, (oAppDlg.m_nSortColumn == CCache::FILE_SIZE));
}

/******************************************************************************
** Method:		LogEdits()
**
** Description:	Log the edited cache files.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::LogEdits(CResultSet& oRS)
{
	ASSERT(App.m_bLogEdits);

	// Get logfile path.
	CPath strLogFile(CPath::AppDir(), "UTCacheMgr.log");
	CFile fLogFile;

	try
	{
		// Open logfile.
		if (strLogFile.Exists())
		{
			// Open existing file and seek to end.
			fLogFile.Open(strLogFile, CFile::WriteOnly);
			fLogFile.Seek(0, CFile::End);
		}
		else
		{
			// Create new file and add column header.
			fLogFile.Create(strLogFile);
			fLogFile.WriteLine("# RealFileName, CacheFileName, Size");
		}

		// For all edited rows...
		for (int i = 0; i < oRS.Count(); ++i)
		{
			CRow& oRow = oRS[i];

			// Create log entry.
			CString strLogEntry;

			strLogEntry  = oRow[CCache::REAL_FILENAME].Format();
			strLogEntry += ',';
			strLogEntry += oRow[CCache::CACHE_FILENAME].Format();
			strLogEntry += ',';
			strLogEntry += oRow[CCache::FILE_SIZE].Format();

			// Add to logfile.
			fLogFile.WriteLine(strLogEntry);
		}
	}
	catch (CStreamException& e)
	{
		// Report error.
		App.AlertMsg(e.ErrorText());
	}
}
