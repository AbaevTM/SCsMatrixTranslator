#ifndef Data_H
#define Data_H

	#include "type.h"

#ifdef __cplusplus
extern "C" {
#endif

	/*
		Creates instance of DATA structure.
	*/
	int createData(
		DATA ** dataPointer,
		const char * identifier,
		int identifierSize,
		const char * data,
		int dataSize,
		int dataType,
		int prefixId		
		);

#ifdef __cplusplus
	}
#endif

#endif