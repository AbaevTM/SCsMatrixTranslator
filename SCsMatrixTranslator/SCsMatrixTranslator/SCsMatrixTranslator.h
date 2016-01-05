#ifndef SCsMatrixTranslator
#define SCsMatrixTranslator

#ifdef __cplusplus
extern "C" {
#endif


#include "SCsMatrixTranslatorConstants.h"
#include "type.h"
#include "Vector.h"
#include "TypeUtil.h"

	/*
		Assuming all elements have the same prefix.
		Otherwise this option is ignored.
	*/
	extern BYTE * defaultPrefix;

	/*
		Translates SCs file to adjacency matrix.

		TODO: add full comments.

	*/
	extern int  translateSCsToMatrix(
		char * fileName,
		char * typeCorrespondenceConfig,
		char * logFileName,
		unsigned char compatibilityMode,
		unsigned char assignmentProcessingType,
		unsigned char verificationEnablingFlag,
		BYTE * defaultPrefixParameter,
		char * synonymyRelationNodeIdParameter,
		BYTE *** matrix, 
		METADATA * metadata
		);

	/*
		Should be triggered every time sc-element is discovered.
		For unrecognized elements generates METADATA and arcs between type nodes and element.
		For recognized elements updates arcs if additional info about types has been discovered.
		Tries to find prefix by provided id. If there is no such prefix, creates it adding to
		METADATA's prefixes.

		elementDiscovered("A" , [t1, t2, t3], 3);

		=
			    A
		t1 o--->o<--
				^	|
		t2 o----| 	|
					|
		t3 o--------|

		returns : id of element in the matrix.
	*/
	int elementDiscovered(
		const BYTE * identifier,
		int identifierSize,
		const BYTE * data,
		int dataSize,
		int dataType,
		int * typeIds,
		int numberOfTypeIds,
		BYTE * prefixIdentifier,
		int prefixIdentifierSize,
		int prefixId
		);

	/*
		Version of elementDiscovered() for elements that do not have any data and have default prefix.
	*/
	int elementDiscoveredDefault(
		const char * identifier,
		int identifierSize,
		int * typeIds,
		int numberOfTypeIds
		);

	/*
		Version of elementDiscovered() for link elements that have default prefix.
	*/
	int elementDiscoveredDefaultLink(
		const char * identifier,
		int identifierSize,
		const char * data,
		int dataSize,
		int dataType
		);

	/*
		Should be triggered after discovering of source, target and arc elements.
		Saves info about graph 3 arcs that should be generated to represent given sc-arc.
	*/
	void arcDiscovered(int sourceElementId, int targetElementId, int arcElementId);

	int initialize(
		char * fileName,
		char * typeCorrespondenceConfig,
		char * logFileName,
		unsigned char assignmentProcessingType,
		BYTE *** matrixArg,
		METADATA * metadataArg,
		BYTE * defaultPrefix);

	/*
		Translates document using different modes specified by compatibilityMode value.
	*/
	int translate(unsigned char compatibilityMode);

	void cleanup();

	void prepareResultMatrixAndMetadata();

	void printResults();

	/*
		Verifies generated matrix and returns results of verification. Looks for elements having types marked as incompatible.
	*/
	int verify();

	/*
		Assignes all given parameters.
	*/
	void assignParameters(
		unsigned char assignmentProcessingType,
		BYTE * defaultPrefix,
		char * synonymyRelationNodeId
		);

	/*
	Creates type nodes. Fills metadata of these nodes.
	*/
	void initializeTypes(METADATA * metadata);

	/*
	Receives empty matrix and fills it  whith 1s
	using previously collected info about arcs.
	*/
	void fillMatrixWithArcs(BYTE *** matrix);

#ifdef __cplusplus
}
#endif

#endif

