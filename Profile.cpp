/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PROFILE.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CProfile class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"

/******************************************************************************
**
** Class constants.
**
*******************************************************************************
*/

const char* CProfile::DEF_PROFILE_NAME   = "Default";
const char* CProfile::DEF_ROOT_DIR       = "C:\\UnrealTournament";
const char* CProfile::DEF_CACHE_DIR      = "C:\\UnrealTournament\\Cache";
const char* CProfile::DEF_CACHE_IDX_FILE = "cache.ini";
const char* CProfile::DEF_CACHE_FILE_EXT = "uxx";
const char* CProfile::DEF_SYSTEM_DIR     = "C:\\UnrealTournament\\System";
const char* CProfile::DEF_MAPS_DIR       = "C:\\UnrealTournament\\Maps";
const char* CProfile::DEF_TEXTURES_DIR   = "C:\\UnrealTournament\\Textures";
const char* CProfile::DEF_SOUNDS_DIR     = "C:\\UnrealTournament\\Sounds";
const char* CProfile::DEF_MUSIC_DIR      = "C:\\UnrealTournament\\Music";
const char* CProfile::DEF_CONFIG_FILE    = "UnrealTournament.ini";

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

CProfile::CProfile()
{
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

CProfile::~CProfile()
{
}
