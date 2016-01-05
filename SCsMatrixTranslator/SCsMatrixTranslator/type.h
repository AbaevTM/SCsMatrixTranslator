///////////////////////////////////////////////////////////////////////
// File:	types.h
// Date:	8.03.2007
// Author:	Valerian Ivashenko	
// Coded by:	CORSAIR
///////////////////////////////////////////////////////////////////////
// Description:	unified semantic knowledge representation model utility

#ifndef _TYPES_H_
#define _TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

	// types
	typedef unsigned int UINT;
#define BYTE			unsigned char
#define WORD			unsigned short
#define DWORD			unsigned int

	// formats
#define F3_FORMAT		1
#define DG6_FORMAT		2
#define G6_FORMAT		3
#define S6_FORMAT		4
#define INVALID_FORMAT	-1

	// error codes
#define OK				1
#define MEMORY_ERROR	-1
#define FILE_ERROR		-2
#define CRC_NOT_OK		-3
#define UNDEF_ERROR		-4
#define INVALID_HEADER	-5
#ifdef  FAILED
#undef  FAILED
#endif//FAILED
#define FAILED			UNDEF_ERROR

	// number of types
#define TYPES_CNT		13


	typedef struct _VERTEX_DATA_
	{
		BYTE *idtf;
		BYTE *data;
		int prefix_id;
		int data_type;

		int idtf_size;
		int data_size;
	}
	DATA;

	typedef struct _PREFIX_
	{
		BYTE *idtf;
		int id;

		int idtf_size;
	}
	PREFIX;

	typedef struct _METADATA_
	{
		int vertex_num;
		int prefix_num;
		DATA *vertex_data;
		PREFIX *prefix_data;
	}
	METADATA;

#ifdef __cplusplus
	}
#endif

#endif //_TYPES_H_
