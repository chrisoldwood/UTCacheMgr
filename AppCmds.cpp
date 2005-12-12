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
#include "SelFilesDlg.hpp"
#include "ErrorsDlg.hpp"
#include "FilePropsDlg.hpp"
#include "ConflictDlg.hpp"
#include "HelpTopics.h"

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
		CMD_RANGE(ID_CACHE_FIRST_PROFILE, ID_CACHE_LAST_PROFILE,
										OnCacheProfile,		OnUICacheProfile,		-1)
		CMD_ENTRY(ID_CACHE_PROFILE,		OnCacheProfileDlg,	NULL,					 3)
		CMD_ENTRY(ID_CACHE_RESCAN,		OnCacheRescan,		NULL,					 0)
		CMD_ENTRY(ID_CACHE_RESTORE,		OnCacheRestore,		OnUICacheRestore,		-1)
		CMD_ENTRY(ID_CACHE_IMPORT,		OnCacheImport,		NULL,					-1)
		CMD_ENTRY(ID_CACHE_UT_CONFIG,	OnCacheUTConfig,	NULL,					-1)
		CMD_ENTRY(ID_CACHE_PROPERTIES,	OnCacheProperties,	OnUICacheProperties,	-1)
		CMD_ENTRY(ID_CACHE_EXIT,		OnCacheExit,		NULL,					-1)
		// Edit menu.
		CMD_ENTRY(ID_EDIT_PIN,			OnEditPin,			OnUIEditPin,			-1)
		CMD_ENTRY(ID_EDIT_MOVE,			OnEditMove,			OnUIEditMove,			 5)
		CMD_ENTRY(ID_EDIT_COPY,			OnEditCopy,			OnUIEditCopy,			 6)
		CMD_ENTRY(ID_EDIT_DELETE,		OnEditDelete,		OnUIEditDelete,			 7)
		CMD_ENTRY(ID_EDIT_COPY_TO,		OnEditCopyTo,		OnUIEditCopyTo,			 9)
		// View menu.
		CMD_ENTRY(ID_VIEW_SELECT_NEW,	OnViewSelectNew,	NULL,					-1)
		CMD_ENTRY(ID_VIEW_SELECT_ALL,	OnViewSelectAll,	NULL,					-1)
		CMD_ENTRY(ID_VIEW_SORT_NAME,	OnViewSortByName,	OnUIViewSortByName,		-1)
		CMD_ENTRY(ID_VIEW_SORT_TYPE,	OnViewSortByType,	OnUIViewSortByType,		-1)
		CMD_ENTRY(ID_VIEW_SORT_DATE,	OnViewSortByDate,	OnUIViewSortByDate,		-1)
		CMD_ENTRY(ID_VIEW_SORT_SIZE,	OnViewSortBySize,	OnUIViewSortBySize,		-1)
		CMD_ENTRY(ID_VIEW_SORT_STATUS,	OnViewSortByStatus,	OnUIViewSortByStatus,	-1)
		CMD_ENTRY(ID_VIEW_SORT_CACHE,	OnViewSortByCache,	OnUIViewSortByCache,	-1)
		CMD_ENTRY(ID_VIEW_SHOW_ALL,		OnViewShowAll,		OnUIViewShowAll,		-1)
		// Tools menu.
		CMD_ENTRY(ID_TOOLS_INSTALL,		OnToolsInstall,		NULL,					-1)
		// Options menu.
		CMD_ENTRY(ID_OPTIONS_PROFILES,	OnOptionsProfiles,	NULL,					-1)
		CMD_ENTRY(ID_OPTIONS_PREFS,		OnOptionsPrefs,		NULL,					-1)
		// Help menu.
		CMD_ENTRY(ID_HELP_CONTENTS,		OnHelpContents,		NULL,					 1)
		CMD_ENTRY(ID_HELP_ABOUT,		OnHelpAbout,		NULL,					-1)
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
** Description:	Cache profile selected from the Profile menu.
**
** Parameters:	nCmdID		The menu ID of the profile.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnCacheProfile(uint nCmdID)
{
	ASSERT(App.m_pProfile != NULL);

	// Convert ID to profile index.
	uint nProfile = nCmdID - ID_CACHE_FIRST_PROFILE;

	// New profile selected?
	if (App.m_aoProfiles[nProfile] != App.m_pProfile)
	{
		// Switch profiles...
		App.m_pProfile = App.m_aoProfiles[nProfile];

		// Reset the scan details.
		App.m_oCache.Truncate();

		// Update UI.
		App.m_AppWnd.UpdateTitle();
		App.m_AppWnd.m_AppDlg.RefreshView();
		App.UpdateCacheStatus();
		UpdateUI();

		// Scan on profile change?
		if (App.m_bScanOnSwitch)
			App.m_AppWnd.PostCommand(ID_CACHE_RESCAN);
	}
}

/******************************************************************************
** Method:		OnCacheProfileDlg()
**
** Description:	Select the cache profile using the dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnCacheProfileDlg()
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
			App.UpdateCacheStatus();
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
	App.m_AppWnd.Enable(false);

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

		// Workaround for UT2003 v2166/86 patch bug which appends a "-1"
		// after the 32 char file name, but the index entry remains the same.
		if ( (strRealName.Empty()) && (strIndexName.Length() > 32) )
		{
			strIndexName = strIndexName.Left(32);
			strRealName  = oIdxFile.ReadString("Cache", strIndexName, "");
		}

		// File not in index?
		if (strRealName.Empty())
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
			dlgErrors.m_astrErrors.Add("Unknown file type.");
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
			dlgErrors.m_astrErrors.Add("Size query failed.");
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
	App.m_AppWnd.Enable(true);
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
		CString   strMask   = strCacheDir.FileTitle() + CProfile::DEF_CACHE_TMP_MASK;
		CFileTree oTmpFiles;

		// Do search...
		oFinder.Find(strTmpDir, strMask, false, oTmpFiles);

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

			// Workaround for UT2003 v2166/86 patch bug which appends a "-1"
			// after the 32 char file name, but the index entry remains the same.
			CPath strAltFile = strCacheDir + (astrKeys[i] + "-1." + CProfile::DEF_CACHE_FILE_EXT);

			if ( (!strFile.Exists()) && (!strAltFile.Exists()) )
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

	// Update UI.
	App.UpdateCacheStatus();
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

	// Get logfile path.
	CPath   strLogFile(CPath::ApplicationDir(), "UTCacheMgr.log");
	CFile   fLogFile;

	// Logfile not created yet?
	if (!strLogFile.Exists())
	{
		App.NotifyMsg("There are no files that can be restored.");
		return;
	}

	// Show the progress dialog.
	CProgressDlg Dlg;

	Dlg.RunModeless(App.m_AppWnd);
	Dlg.Title("Loading Restore Log");
	Dlg.InitMeter(CFile::Size(strLogFile));
	App.m_AppWnd.Enable(false);

	try
	{
		fLogFile.Open(strLogFile, GENERIC_READ);

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
			if (CStrTok::Split(strLine, ',', astrFields) != 3)
				continue;

			CPath strRealName  = astrFields[0];
			CPath strCacheName = astrFields[1];
			int   nFileSize    = atoi(astrFields[2]);

			Dlg.UpdateLabelAndMeter("Checking file: " + (CString)strRealName, fLogFile.Seek(0, FILE_CURRENT));

			// Duplicate entry?
			if (oRestore.Exists(CWhereCmp(CCache::REAL_FILENAME, CWhereCmp::EQUALS, strRealName)))
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

		fLogFile.Close();
	}
	catch (CStreamException& e)
	{
		// Report error.
		App.AlertMsg(e.ErrorText());

		// Remove progress dialog.
		App.m_AppWnd.Enable(true);
		Dlg.Destroy();

		return;
	}

	// Remove progress dialog.
	App.m_AppWnd.Enable(true);
	Dlg.Destroy();

	// Nothing to restore?
	if (oRestore.RowCount() == 0)
	{
		App.NotifyMsg("There are no files that can be restored.");
		return;
	}

	CSelFilesDlg oSelFilesDlg;

	oSelFilesDlg.m_pTable   = &oRestore;
	oSelFilesDlg.m_strTitle = "Restore Files To Cache";
	oSelFilesDlg.m_dwHelpID = IDH_RESTOREDLG;

	// Get files to restore from user.
	if (oSelFilesDlg.RunModal(App.m_AppWnd) != IDOK)
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
	App.m_AppWnd.Enable(true);
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
** Method:		OnCacheImport()
**
** Description:	Import files into the cache from another cache folder.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnCacheImport()
{
	CPath strSrcDir = App.m_pProfile->m_strLastImport;

	// Get the directory to import from.
	if (!strSrcDir.SelectDir(App.m_AppWnd, "Select Folder To Import From", strSrcDir))
		return;

	// If changed, update profiles' default folder.
	if (strSrcDir != App.m_pProfile->m_strLastImport)
	{
		App.m_pProfile->m_strLastImport = strSrcDir;

		App.m_nModified |= App.PROFILES;
	}

	// Get cache + index file full paths.
	CPath strSrcCacheDir  = strSrcDir;
	CPath strSrcCacheFile = strSrcCacheDir + App.m_strCacheIndex;

	// Check the cache index file exists.
	if (!strSrcCacheFile.Exists())
	{
		App.AlertMsg("Failed to access the cache index file:\n\n%s", strSrcCacheFile);
		return;
	}

	CBusyCursor oCursor;

	// Tmp table to store files we can import.
	CCache oImport(App.m_oMDB);

	// Show the progress dialog.
	CProgressDlg Dlg;

	Dlg.RunModeless(App.m_AppWnd);
	Dlg.Title("Scanning Cache");
	Dlg.UpdateLabel("Searching cache for files...");
	App.m_AppWnd.Enable(false);

	CString	strFindMask;

	// Create 'find files' mask.
	strFindMask.Format("*.%s", App.m_strCacheExt);

	CFileFinder oFinder;
	CFileTree	oFiles;

	// Search cache for files.
	oFinder.Find(strSrcCacheDir, strFindMask, false, oFiles);

	CStrArray& astrFiles = oFiles.Root()->m_oData.m_astrFiles;
	CIniFile   oSrcIdxFile(strSrcCacheFile);

	Dlg.InitMeter(astrFiles.Size());

	// For all files found...
	for (int i = 0; i < astrFiles.Size(); ++i)
	{
		// Get file name, index key and real name.
		CPath   strCacheName = astrFiles[i];
		CString strIndexName = strCacheName.FileTitle();
		CPath   strRealName  = oSrcIdxFile.ReadString("Cache", strIndexName, "");

		// Workaround for UT2003 v2166/86 patch bug which appends a "-1"
		// after the 32 char file name, but the index entry remains the same.
		if ( (strRealName.Empty()) && (strIndexName.Length() > 32) )
		{
			strIndexName = strIndexName.Left(32);
			strRealName  = oSrcIdxFile.ReadString("Cache", strIndexName, "");
		}

		// File not in index?
		if (strRealName.Empty())
			continue;

		Dlg.UpdateLabelAndMeter("Processing file: " + (CString)strRealName, i);

		// File already in cache?
		if (App.m_oCache.Exists(CWhereCmp(CCache::CACHE_FILENAME, CWhereCmp::EQUALS, strCacheName)))
			continue;

		// Get file type from extension.
		CString strExt = strRealName.FileExt().ToLower();
		char    cType  = CProfile::GetFileType(strExt);

		// Unknown file type?
		if (cType == NULL)
			continue;

		// Get other file details.
		CPath strFile = strSrcCacheDir + strCacheName;
		struct _stat oInfo;

		if (!CFile::QueryInfo(strFile, oInfo))
			continue;

		// Add file details to the table.
		CRow& oRow = oImport.CreateRow();

		oRow[CCache::CACHE_FILENAME] = strCacheName;
		oRow[CCache::INDEX_KEY]      = strIndexName;
		oRow[CCache::REAL_FILENAME]  = strRealName;
		oRow[CCache::FILE_TYPE]      = cType;
		oRow[CCache::FILE_DATE]      = oInfo.st_mtime;
		oRow[CCache::FILE_SIZE]      = (int)oInfo.st_size;
		oRow[CCache::STATUS]         = NEW_FILE;

		oImport.InsertRow(oRow);
	}

	// Remove progress dialog.
	App.m_AppWnd.Enable(true);
	Dlg.Destroy();

	// Nothing to import?
	if (oImport.RowCount() == 0)
	{
		App.NotifyMsg("There are no files that can be imported.");
		return;
	}

	CSelFilesDlg oSelFilesDlg;

	oSelFilesDlg.m_pTable   = &oImport;
	oSelFilesDlg.m_strTitle = "Import Files Into Cache";
	oSelFilesDlg.m_dwHelpID = IDH_IMPORTDLG;

	// Get files to import from user.
	if (oSelFilesDlg.RunModal(App.m_AppWnd) != IDOK)
		return;

	int nFiles = oImport.RowCount();

	// No files selected?
	if (nFiles == 0)
		return;

	// Get cache + index file full paths.
	CPath    strDstCacheDir  = App.m_pProfile->m_strCacheDir;
	CPath    strDstCacheFile = strDstCacheDir + App.m_strCacheIndex;
	CIniFile oDstIdxFile(strDstCacheFile);

	// Show the progress dialog.
	Dlg.RunModeless(App.m_AppWnd);
	Dlg.Title("Importing Files");
	Dlg.InitMeter(nFiles);

	// Create errors dialog.
	CErrorsDlg dlgErrors;

	// For all files to import...
	for (int i = 0; i < nFiles; ++i)
	{
		CRow& oRow         = oImport[i];
		CPath strRealName  = oRow[CCache::REAL_FILENAME];
		CPath strCacheName = oRow[CCache::CACHE_FILENAME];

		Dlg.UpdateLabelAndMeter("Importing file: " + (CString)strRealName, i);

		// Create the src and dst full paths.
		CString	strSrcFile = strSrcCacheDir + strCacheName;
		CString	strDstFile = strDstCacheDir + strCacheName;

		// Copy it...
		if (!CFile::Copy(strSrcFile, strDstFile))
		{
			dlgErrors.m_astrFiles.Add(oRow[CCache::REAL_FILENAME].GetString());
			dlgErrors.m_astrErrors.Add(App.FormatError());
			continue;
		}

		// Add to index.
		oDstIdxFile.WriteString("Cache", oRow[CCache::INDEX_KEY], strRealName);
	}

	// Remove progress dialog.
	App.m_AppWnd.Enable(true);
	Dlg.Destroy();

	// Report any errors.
	if (dlgErrors.m_astrFiles.Size() > 0)
	{
		dlgErrors.m_strTitle = "Import Errors";
		dlgErrors.RunModal(App.m_AppWnd);
	}

	// Rescan cache to pick up imported files.
	OnCacheRescan();
	UpdateUI();
}

/******************************************************************************
** Method:		OnCacheUTConfig()
**
** Description:	Show the dialog for editing the UT cache settings.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnCacheUTConfig()
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
** Method:		OnCacheProperties()
**
** Description:	Show the cache files' properties.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnCacheProperties()
{
	ASSERT(App.m_pProfile != NULL);

	CResultSet oRS(App.m_oCache);

	// Get the current selection.
	App.m_AppWnd.m_AppDlg.GetSelectedFiles(oRS);

	// Ignore if nothing to do.
	if (oRS.Count() == 0)
		return;

	// Only show details for the 1st file.
	CRow& oRow = oRS[0];

	CFilePropsDlg Dlg;

	Dlg.m_cType        = oRow[CCache::FILE_TYPE];
	Dlg.m_strRealName  = oRow[CCache::REAL_FILENAME];
	Dlg.m_strCacheName = oRow[CCache::CACHE_FILENAME];
	Dlg.m_strDate      = oRow[CCache::FILE_DATE].Format();
	Dlg.m_strType      = App.FormatType(oRow[CCache::FILE_TYPE]);
	Dlg.m_strSize      = oRow[CCache::FILE_SIZE].Format();

	Dlg.RunModal(App.m_rMainWnd);
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

	// For all selected rows...
	for (int i = 0; i < nFiles; ++i)
	{
		CRow&       oRow    = oRS[i];
		char        cStatus = oRow[CCache::STATUS];
		const char* pszName = oRow[CCache::REAL_FILENAME];

		// Add to pinned list?
		if (cStatus == NEW_FILE)
		{
			if (App.m_astrPinned.Find(pszName, true) == -1)
				App.m_astrPinned.Add(pszName);
		}

		// Remove from pinned list?
		if (cStatus == PIN_FILE)
		{
			if (App.m_astrPinned.Find(pszName, true) != -1)
				App.m_astrPinned.Delete(App.m_astrPinned.Find(pszName, true));
		}
	}

	// For all cache table rows...
	for (int i = 0; i < App.m_oCache.RowCount(); ++i)
	{
		CRow&       oRow     = App.m_oCache[i];
		char        cStatus  = oRow[CCache::STATUS];
		const char* pszName  = oRow[CCache::REAL_FILENAME];
		int         nListIdx = App.m_astrPinned.Find(pszName, true);

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

	// Mark pinned list as modified.
	App.m_nModified |= App.PIN_LIST;

	// Update UI.
	App.m_AppWnd.m_AppDlg.RefreshView();
	App.UpdateCacheStatus();
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
	App.m_AppWnd.Enable(false);

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
	App.m_AppWnd.Enable(true);
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
	App.UpdateCacheStatus();
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
	App.m_AppWnd.Enable(false);

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
	App.m_AppWnd.Enable(true);
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
	App.UpdateCacheStatus();
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
	App.UpdateCacheStatus();
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
	ASSERT(App.m_pProfile != NULL);

	CResultSet oRS(App.m_oCache);

	// Get the current selection.
	App.m_AppWnd.m_AppDlg.GetSelectedFiles(oRS);

	int nFiles = oRS.Count();

	// Ignore if nothing to copy.
	if (nFiles == 0)
		return;

	CPath strDstDir = App.m_pProfile->m_strLastCopyTo;

	// Get the directory to copy to.
	if (!strDstDir.SelectDir(App.m_AppWnd, "Select Folder To Copy To", strDstDir))
		return;

	// If changed, update profiles' default folder.
	if (strDstDir != App.m_pProfile->m_strLastCopyTo)
	{
		App.m_pProfile->m_strLastCopyTo = strDstDir;

		App.m_nModified |= App.PROFILES;
	}

	// Get cache + index file full paths.
	CPath    strCacheDir  = App.m_pProfile->m_strCacheDir;
	CPath    strCacheFile = strCacheDir + App.m_strCacheIndex;
	CIniFile oIdxFile(strCacheFile);

	// Show the progress dialog.
	CProgressDlg Dlg;

	Dlg.RunModeless(App.m_AppWnd);
	Dlg.Title("Copying Files");
	Dlg.InitMeter(nFiles);
	App.m_AppWnd.Enable(false);

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
	App.m_AppWnd.Enable(true);
	Dlg.Destroy();

	// Report any errors.
	if (dlgErrors.m_astrFiles.Size() > 0)
	{
		dlgErrors.m_strTitle = "Copy Errors";
		dlgErrors.RunModal(App.m_AppWnd);
	}

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

void CAppCmds::OnViewSortByStatus()
{
	OnViewSortByColumn(CCache::STATUS, CSortColumns::ASC);
}

void CAppCmds::OnViewSortByCache()
{
	OnViewSortByColumn(CCache::CACHE_FILENAME, CSortColumns::ASC);
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
** Method:		OnToolsInstall()
**
** Description:	Copy file sets such as map packs into the profile folders.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnToolsInstall()
{
	CPath strSrcDir = App.m_pProfile->m_strLastInstall;

	// Get the directory to install from.
	if (!strSrcDir.SelectDir(App.m_AppWnd, "Select Folder To Install From", strSrcDir))
		return;

	// If changed, update profiles' default folder.
	if (strSrcDir != App.m_pProfile->m_strLastInstall)
	{
		App.m_pProfile->m_strLastInstall = strSrcDir;

		App.m_nModified |= App.PROFILES;
	}

	CBusyCursor oCursor;

	// Show the progress dialog.
	CProgressDlg Dlg;

	Dlg.RunModeless(App.m_AppWnd);
	Dlg.Title("Scanning Folder");
	Dlg.UpdateLabel("Searching folder for files...");
	App.m_AppWnd.Enable(false);

	CFileFinder oFinder;
	CFileTree   oFiles;

	// Search folder for files.
	oFinder.Find(strSrcDir, "*.*", true, oFiles);

	CFileTreeIter  oCntIter(oFiles);
	CFileTreeNode* pNode  = NULL;
	int            nFiles = 0;

	// Count all files found...
	while ((pNode = oCntIter.Next()) != NULL)
		nFiles += pNode->m_oData.m_astrFiles.Size();

	// Nothing to install?
	if (nFiles == 0)
	{
		// Remove progress dialog.
		App.m_AppWnd.Enable(true);
		Dlg.Destroy();

		App.NotifyMsg("There are no files to install.");
		return;
	}

	// Create errors dialog.
	CErrorsDlg dlgErrors;

	Dlg.Title("Installing Files");
	Dlg.InitMeter(nFiles);

	CFileTreeIter oInsIter(oFiles);
	int           nFile      = 0;
	bool          bAborted   = false;
	bool          bDefYesAll = false;
	bool          bDefNoAll  = false;

	// For all folders...
	while (((pNode = oInsIter.Next()) != NULL) && (!bAborted))
	{
		// For all files in the folder...
		for (int i = 0; ((i < pNode->m_oData.m_astrFiles.Size()) && (!bAborted)); ++i, ++nFile)
		{
			CPath   strSrcDir   = pNode->m_oData.m_strPath;
			CPath   strFileName = pNode->m_oData.m_astrFiles[i];
			CString strFileExt  = strFileName.FileExt().ToLower();

			dlgErrors.m_astrFiles.Add(strFileName);

			// Not a UT file?
			if (!CProfile::IsValidType(strFileExt))
			{
				Dlg.UpdateLabelAndMeter("Skipping file: " + (CString)strFileName, nFile);
				dlgErrors.m_astrErrors.Add("Ignored.");
				continue;
			}

			Dlg.UpdateLabelAndMeter("Installing file: " + (CString)strFileName, nFile);

			// Get the folder to copy to.
			char  cType     = App.m_pProfile->GetFileType(strFileExt);
			CPath strDstDir = App.m_pProfile->GetTypeDir(cType);

			// Create the src and dst full paths.
			CPath   strSrcFile = strSrcDir + strFileName;
			CPath   strDstFile = strDstDir + strFileName;

			// File already installed?
			if (strDstFile.Exists())
			{
				struct _stat oSrcInfo, oDstInfo;

				// Query both files details.
				if ( (!CFile::QueryInfo(strSrcFile, oSrcInfo))
				  || (!CFile::QueryInfo(strDstFile, oDstInfo)) )
				{
					dlgErrors.m_astrErrors.Add("Size query failed.");
					continue;
				}

				// The same file?
				if (  (oSrcInfo.st_size  == oDstInfo.st_size )
				  && ((oSrcInfo.st_mtime == oDstInfo.st_mtime) || (App.m_bIgnoreDates)) )
				{
					dlgErrors.m_astrErrors.Add("Already installed.");
					continue;
				}

				// Default is don't overwrite?
				if (bDefNoAll)
				{
					dlgErrors.m_astrErrors.Add("Not installed.");
					continue;
				}

				// Default is to query overwrites?
				if (!bDefYesAll)
				{
					CConflictDlg oQueryDlg;

					oQueryDlg.m_strFileName1.Format("%s", strDstFile);
					oQueryDlg.m_strFileInfo1.Format("%s - %u Bytes", CStrCvt::FormatDateTime(oDstInfo.st_mtime), oDstInfo.st_size);
					oQueryDlg.m_strFileName2.Format("%s", strSrcFile);
					oQueryDlg.m_strFileInfo2.Format("%s - %u Bytes", CStrCvt::FormatDateTime(oSrcInfo.st_mtime), oSrcInfo.st_size);

					// Query user for action.
					int nResult = oQueryDlg.RunModal(App.m_AppWnd);

					// Default answer returned?
					if (nResult == IDYESALL)	bDefYesAll = true;
					if (nResult == IDNOALL)		bDefNoAll  = true;

					// Ignore file?
					if ((nResult == IDNO) || (nResult == IDNOALL))
					{
						dlgErrors.m_astrErrors.Add("Not installed.");
						continue;
					}

					// Abort install?
					if (nResult == IDCANCEL)
					{
						dlgErrors.m_astrErrors.Add("Installed aborted.");
						bAborted = true;
						continue;
					}
				}
			}

			// Install it.
			if (CFile::Copy(strSrcFile, strDstFile, true) == 0)
			{
				dlgErrors.m_astrErrors.Add(App.FormatError());
				continue;
			}

			dlgErrors.m_astrErrors.Add("Installed.");
		}
	}

	// Remove progress dialog.
	App.m_AppWnd.Enable(true);
	Dlg.Destroy();

	// Report any errors.
	if (dlgErrors.m_astrFiles.Size() > 0)
	{
		dlgErrors.m_strTitle = "Install Results";
		dlgErrors.RunModal(App.m_AppWnd);
	}
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

	if (Dlg.m_bReScan)
		App.m_AppWnd.PostCommand(ID_CACHE_RESCAN);

	App.BuildProfileMenu();

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

void CAppCmds::OnUICacheProfile()
{
	ASSERT(App.m_pProfile != NULL);

	CMenu&       oMenu    = App.m_AppWnd.m_Menu;
//	CAppToolBar& oToolBar = App.m_AppWnd.m_ToolBar;
//	CAppDlg&     oAppDlg  = App.m_AppWnd.m_AppDlg;

	// Remove all check marks.
	for (uint i = 0; i < App.m_aoProfiles.size(); ++i)
		oMenu.CheckCmd(i + ID_CACHE_FIRST_PROFILE, false);

	ASSERT(App.GetProfileIndex(App.m_pProfile) != -1);

	// Place check next to active profile.
	oMenu.CheckCmd(App.GetProfileIndex(App.m_pProfile) + ID_CACHE_FIRST_PROFILE, true);
}

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

void CAppCmds::OnUICacheProperties()
{
	ASSERT(App.m_pProfile != NULL);

	CMenu&       oMenu    = App.m_AppWnd.m_Menu;
//	CAppToolBar& oToolBar = App.m_AppWnd.m_ToolBar;
	CAppDlg&     oAppDlg  = App.m_AppWnd.m_AppDlg;

	bool bSelection = oAppDlg.m_lvGrid.IsSelection();
//	bool bReadOnly  = App.m_pProfile->m_bReadOnly;

	oMenu.EnableCmd(ID_CACHE_PROPERTIES, bSelection);
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

void CAppCmds::OnUIViewSortByStatus()
{
	CMenu&   oMenu   = App.m_AppWnd.m_Menu;
	CAppDlg& oAppDlg = App.m_AppWnd.m_AppDlg;

	oMenu.CheckCmd(ID_VIEW_SORT_STATUS, (oAppDlg.m_nSortColumn == CCache::STATUS));
}

void CAppCmds::OnUIViewSortByCache()
{
	CMenu&   oMenu   = App.m_AppWnd.m_Menu;
	CAppDlg& oAppDlg = App.m_AppWnd.m_AppDlg;

	oMenu.CheckCmd(ID_VIEW_SORT_CACHE, (oAppDlg.m_nSortColumn == CCache::CACHE_FILENAME));
}

/******************************************************************************
** Method:		CmdHintStr()
**
** Description:	Get the status bar hint for the command.
**
** Parameters:	iCmdID		The command ID.
**
** Returns:		The hint string.
**
*******************************************************************************
*/

CString CAppCmds::CmdHintStr(uint iCmdID) const
{
	// Remap all Cache profiles to first ID.
	if ( (iCmdID >= ID_CACHE_FIRST_PROFILE) && (iCmdID <= ID_CACHE_LAST_PROFILE) )
		iCmdID = ID_CACHE_FIRST_PROFILE;

	// Forward to base class.
	return CCmdControl::CmdHintStr(iCmdID);
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
	CPath strLogFile(CPath::ApplicationDir(), "UTCacheMgr.log");
	CFile fLogFile;

	try
	{
		// Open logfile.
		if (strLogFile.Exists())
		{
			// Open existing file and seek to end.
			fLogFile.Open(strLogFile, GENERIC_WRITE);
			fLogFile.Seek(0, FILE_END);
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
