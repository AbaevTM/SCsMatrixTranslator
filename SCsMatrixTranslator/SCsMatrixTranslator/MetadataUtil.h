#ifndef MetadataUtil_H
#define MetadataUtil_H

	#include "type.h"

#ifdef __cplusplus
extern "C" {
#endif

	extern PREFIX * defaultPrefixInstance;

	/*
		TODO: Maybe other fields of default prefix should be parameterized as well.
	*/
	PREFIX* getDefaultPrefixData();

	void destroyDefaultPrefix();

	/*
		returns : instance of PREFIX. Existing if prefix having provided ID has been
		found. Newly created and appended to the end of prefix data of METADATA PREFIX if 
		prefix having provided ID has not been found.
	*/
	PREFIX* getOrCreatePrefixById(
		METADATA * metadata,
		BYTE * prefixIdentifier,
		int prefixIdentifierSize,
		int prefixId
		);

	/*
		returns: NULL if there are no prefix having given id in METADATA's prefix_data.
	*/
	PREFIX* getPrefixById(METADATA * metadata, int id);

	void copyPrefixes(PREFIX * target, PREFIX * source, int numberOfPrefixes);

#ifdef __cplusplus
	}
#endif

#endif