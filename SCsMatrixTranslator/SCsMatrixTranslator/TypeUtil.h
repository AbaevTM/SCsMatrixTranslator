#ifndef TypeUtil_H
#define TypeUtil_H

	#include "type.h"
	#include "Vector.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ELEMENT_CANNOT_BE_FOUND_ID -1

	extern char * typeIdentifiers[TYPES_CNT];

	extern char ** scsTypeIdentifiers;

	extern char ** scsArcSymbols;

	extern IntVectorVector * stringTypeCorrespondence;

	extern IntVectorVector * incompatibleTypes;

	extern int * incompatibleTypesToListCorresondence;

	/*
	returns : id of element using METADATA's DATA instances or ELEMENT_CANNOT_BE_FOUND_ID if element cannot be found.
	*/
	int getElementIdByIdentifier(DataVector * vertexData, const char * identifier);

	/*
	returns : id of element using METADATA's DATA instances or ELEMENT_CANNOT_BE_FOUND_ID if element cannot be found.
	*/
	int getElementIdByData(DataVector * vertexData, const char * data);

	/*
	returns : id of element using METADATA's DATA instances or ELEMENT_CANNOT_BE_FOUND_ID if element cannot be found.
	First searches element using its identifier, then if nothing has been found tries to search element by its data.
	*/
	int getElementId(DataVector * vertexData, const char * identifier, const char * data);


	/*
		Initializes type correspondence structures.
		If pathToTypeConfig points to NULL, then default type correspondence should be used.
		*/
	void initializeTypeCorrespondence(char * pathToTypeConfig);

	int checkElementTypesForIncompatibleness(IntVector* elementTypes, int elementId);

	/*
		Destroys type correspondence structures.
		*/
	void destroyTypeCorrespondence();

	/*
		returnes : vector of corresponding types for given scs type. If info about corresponding types
		is missing then NULL is returned.
		*/
	IntVector * getCorrespondingTypesForScsType(const char * scsTypeStringRepresentation);

	/*
		returnes : vector of corresponding types for given set of scs types. If info about corresponding types
		is missing then empty vector is returned.
		*/
	IntVector* getTypesByMergingSeveral(char ** typeNames, int numberOfTypes);

	/*
		Updates info about types of element creating arcs having type nodes as a source and the element as a target.
		*/
	int updateTypeInfo(
		IntVectorVector * outputArcTargetsPerElement,
		int elementId,
		int * typeIdentifiersArg,
		int numberOfTypeIdentifiers
		);

	/*
		Updates vectors of output arc targets of elements.
		*/
	void saveArc(IntVectorVector * outputArcTargetsPerElement, int sourceElementId, int targetElementId);

	/*
		Assuming logic that is used currently in sc-machine for processing sentences of level 1
		first and third elements have predefined types. So this function should return vector of
		corresponding types for such elements.
		*/
	IntVector * getTypeIdsOfNodesAtSentenceLevel1();

	/*
		Assuming logic that is used currently in sc-machine for processing sentences of level 1
		arcs can have one of 4 types, that can be configured using '#' symbol as a separator
		of string representation of a type and system identifier of a node.
		So this function should return vector of corresponding types for such elements.
		*/
	IntVector * getTypeIdsOfArcsAtSentenceLevel1(char * type);

	/*
		returns : id of assignment group by given element's identifier
		or ELEMENT_CANNOT_BE_FOUND_ID if such assignment group cannot be found.
		*/
	int getAssignmentsVectorIdByIdentifier(IntVectorVector * assignments, DataVector * vertexData, char * identifier);

#ifdef __cplusplus
	}
#endif

#endif