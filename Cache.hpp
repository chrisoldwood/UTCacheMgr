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
const char SYSTEM_FILE  = 'U';
const char MAP_FILE     = 'P';
const char TEXTURE_FILE = 'T';
const char SOUND_FILE   = 'A';
const char MUSIC_FILE   = 'M';
const char MESH_FILE    = 'S';
const char ANIM_FILE    = 'K';
const char KARMA_FILE   = 'D';

// File status.
const char NEW_FILE = 'N';
const char OLD_FILE = 'O';
const char PIN_FILE = 'P';

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
	CCache(CMDB& oDB);
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
