/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CACHE.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CCache class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "Cache.hpp"

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

CCache::CCache(CMDB& oDB)
	: CTable(oDB, "Cache")
{
	// Create the table schema.
	AddColumn("ID",            MDCT_IDENTITY, 0,                  CColumn::IDENTITY);
	AddColumn("CacheFileName", MDCT_FXDSTR,   CACHE_FILENAME_LEN, CColumn::DEFAULTS);
	AddColumn("IndexKey",      MDCT_FXDSTR,   INDEX_KEY_LEN,      CColumn::DEFAULTS);
	AddColumn("RealFileName",  MDCT_FXDSTR,   REAL_FILENAME_LEN,  CColumn::DEFAULTS);
	AddColumn("FileType",      MDCT_CHAR,     0,                  CColumn::DEFAULTS);
	AddColumn("FileDate",      MDCT_DATETIME, 0,                  CColumn::DEFAULTS);
	AddColumn("FileSize",      MDCT_INT,      0,                  CColumn::DEFAULTS);
	AddColumn("Status",        MDCT_CHAR,     0,                  CColumn::DEFAULTS);
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

CCache::~CCache()
{
}

/******************************************************************************
** Method:		CreateRow()
**
** Description:	Creates a new row and initialises it.
**
** Parameters:	None.
**
** Returns:		The new row.
**
*******************************************************************************
*/

CRow& CCache::CreateRow()
{
	CRow& oRow = CTable::CreateRow();

//	oRow[ID]             = 
//	oRow[CACHE_FILENAME] =
//	oRow[INDEX_KEY]      =
//	oRow[REAL_FILENAME]  =
//	oRow[FILE_TYPE]      =
//	oRow[FILE_DATE]      =
//	oRow[FILE_SIZE]      = 
//	oRow[STATUS]         =
	
	return oRow;
}
