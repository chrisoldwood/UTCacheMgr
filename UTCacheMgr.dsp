# Microsoft Developer Studio Project File - Name="UTCacheMgr" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=UTCacheMgr - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "UTCacheMgr.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "UTCacheMgr.mak" CFG="UTCacheMgr - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "UTCacheMgr - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "UTCacheMgr - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Win32/UTCacheMgr", QICAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "UTCacheMgr - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W4 /WX /GX /O2 /I "..\WCL" /I "..\MDBL" /I ".\HelpFile" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX"AppHeaders.hpp" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /i "..\WCL" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "UTCacheMgr - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /WX /Gm /GX /ZI /Od /I "..\WCL" /I "..\MDBL" /I ".\HelpFile" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX"AppHeaders.hpp" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /i "..\WCL" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "UTCacheMgr - Win32 Release"
# Name "UTCacheMgr - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\App.rc
# End Source File
# Begin Source File

SOURCE=.\AppCmds.cpp
# End Source File
# Begin Source File

SOURCE=.\AppDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AppToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\AppWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Cache.cpp
# End Source File
# Begin Source File

SOURCE=.\EditProfileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ErrorsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FilePropsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PrefsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Profile.cpp
# End Source File
# Begin Source File

SOURCE=.\ProfileCfgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelFilesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelProfileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UTCMGRApp.cpp
# End Source File
# Begin Source File

SOURCE=.\UTConfigDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AboutDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\AppCmds.hpp
# End Source File
# Begin Source File

SOURCE=.\AppDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\AppHeaders.hpp
# End Source File
# Begin Source File

SOURCE=.\AppToolBar.hpp
# End Source File
# Begin Source File

SOURCE=.\AppWnd.hpp
# End Source File
# Begin Source File

SOURCE=.\Cache.hpp
# End Source File
# Begin Source File

SOURCE=.\EditProfileDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\ErrorsDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\FilePropsDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\PrefsDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\Profile.hpp
# End Source File
# Begin Source File

SOURCE=.\ProfileCfgDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SelFilesDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\SelProfileDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\UTCMGRApp.hpp
# End Source File
# Begin Source File

SOURCE=.\UTConfigDlg.hpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Anim.ico
# End Source File
# Begin Source File

SOURCE=.\App.ico
# End Source File
# Begin Source File

SOURCE=.\App.rcv
# End Source File
# Begin Source File

SOURCE=.\ListIcons.bmp
# End Source File
# Begin Source File

SOURCE=.\Map.ico
# End Source File
# Begin Source File

SOURCE=.\Mesh.ico
# End Source File
# Begin Source File

SOURCE=.\Music.ico
# End Source File
# Begin Source File

SOURCE=.\Sound.ico
# End Source File
# Begin Source File

SOURCE=.\System.ico
# End Source File
# Begin Source File

SOURCE=.\Texture.ico
# End Source File
# Begin Source File

SOURCE=.\Toolbar.bmp
# End Source File
# End Group
# Begin Group "HelpFile"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HelpFile\AboutDlg.bmp
# End Source File
# Begin Source File

SOURCE=.\HelpFile\CacheMenu.bmp
# End Source File
# Begin Source File

SOURCE=.\HelpFile\ContextMenu.bmp
# End Source File
# Begin Source File

SOURCE=.\HelpFile\DetectMsg.bmp
# End Source File
# Begin Source File

SOURCE=.\HelpFile\EditMenu.bmp
# End Source File
# Begin Source File

SOURCE=.\HelpFile\EditProfDlg.bmp
# End Source File
# Begin Source File

SOURCE=.\HelpFile\FailedMsg.bmp
# End Source File
# Begin Source File

SOURCE=.\HelpFile\HelpMenu.bmp
# End Source File
# Begin Source File

SOURCE=.\HelpFile\HelpTopics.h
# End Source File
# Begin Source File

SOURCE=.\HelpFile\MainView.bmp
# End Source File
# Begin Source File

SOURCE=.\HelpFile\OptionsMenu.bmp
# End Source File
# Begin Source File

SOURCE=.\HelpFile\PrefsDlg.bmp
# End Source File
# Begin Source File

SOURCE=.\HelpFile\ProfDlg.bmp
# End Source File
# Begin Source File

SOURCE=.\HelpFile\PropsDlg.bmp
# End Source File
# Begin Source File

SOURCE=.\HelpFile\RestoreDlg.bmp
# End Source File
# Begin Source File

SOURCE=.\HelpFile\SelFolder.bmp
# End Source File
# Begin Source File

SOURCE=.\HelpFile\SelProfDlg.bmp
# End Source File
# Begin Source File

SOURCE=.\HelpFile\UTCacheMgr.hpj
# End Source File
# Begin Source File

SOURCE=.\HelpFile\UTCacheMgr.rtf
# End Source File
# Begin Source File

SOURCE=.\HelpFile\UTCfgDlg.bmp
# End Source File
# Begin Source File

SOURCE=.\HelpFile\ViewMenu.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\RelNotes.txt
# End Source File
# Begin Source File

SOURCE=.\TODO.Txt
# End Source File
# End Target
# End Project
