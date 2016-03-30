/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CACHE.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CCache class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef CACHE_HPP
#define CACHE_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <MDBL/Table.hpp>

/******************************************************************************
** 
** Constants.
**
*******************************************************************************
*/

// File types.
const tchar UNKNOWN_FILE = 0;
const tchar SYSTEM_FILE  = 'U';
const tchar MAP_FILE     = 'P';
const tchar TEXTURE_FILE = 'T';
const tchar SOUND_FILE   = 'A';
const tchar MUSIC_FILE   = 'M';
const tchar MESH_FILE    = 'S';
const tchar ANIM_FILE    = 'K';
const tchar KARMA_FILE   = 'D';

// File status.
const tchar NEW_FILE = 'N';
const tchar OLD_FILE = 'O';
const tchar PIN_FILE = 'P';

/******************************************************************************
** 
** The table used to store the cached files details.
**
*******************************************************************************
*/

class CCache : public CTable
{
public:
	//
	// Constructors/Destructor.
	//
	CCache();
	~CCache();
	
	//
	// Column indices.
	//
	enum
	{
		ID,
		CACHE_FILENAME,
		INDEX_KEY,
		REAL_FILENAME,
		FILE_TYPE,
		FILE_DATE,
		FILE_SIZE,
		STATUS,
	};

	//
	// Column lengths.
	//
	enum
	{
		CACHE_FILENAME_LEN = MAX_PATH,
		INDEX_KEY_LEN      = MAX_PATH,
		REAL_FILENAME_LEN  = MAX_PATH,
	};

	//
	// Methods (overriden).
	//
	virtual CRow& CreateRow();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //CACHE_HPP
