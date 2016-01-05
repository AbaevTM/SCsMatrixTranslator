#include "stdafx.h"

	static BYTE *** matrix;

	static METADATA * metadata;

	/*
		Contains array of vertex data.
		Index corresponds to index of matrix.
	*/
	static DataVector * vertexData = NULL;

	/*
		Contains array of vectors of integers.
		Array's index corresponds to element's index in matrix.
		Integers contained in array corresonds to elements that are targets of arcs which source is current element.

		1->2;
		1->3;
		1->4;
		2->5;

		=

		2,3,4
		5
	*/
	static IntVectorVector * outputArcTargetsPerElement = NULL;

	/*
		Number of parsed elements.
		Also vertexData and outputArcsPerElement have the same size 
		and the size of assignments vector is limited by this value.
	*/
	static int numberOfElements;

	/*
		Contains array of vectors of integers.
		Array's index corresponds to element's index in matrix.
		Integers contained in array corresonds to elements that are synonymous to current element.

		1=2;
		1=3;
		12=14;
		5=7;

		=

		1,2,3
		12,14
		5,7
	*/
	static IntVectorVector * assignments = NULL;

	/*
		Default number of prefixes.
	*/
	int numberOfPrefixes = 1;

	/*

		Parameterized values.

	*/
	BYTE * defaultPrefix;

	/*
		Assuming ASSIGNMENT_PROCESSING_TYPE_GENERATE_SYMMETRY is used.
		Corresponds to identifier of synonymy node.
		Otherwise this option is ignored.
	*/
	char * synonymyRelationNodeId;

	/*

		Functions.

	*/

	int translateSCsToMatrix(
		char * fileName,
		char * typeCorrespondenceConfig,
		char * logFileName,
		unsigned char compatibilityMode,
		unsigned char assignmentProcessingType,
		unsigned char verificationEnablingFlag,
		BYTE * defaultPrefix,
		char * synonymyRelationNodeId,
		BYTE *** matrixArg,
		METADATA * metadataArg) {
		int result = initialize(fileName, typeCorrespondenceConfig, logFileName, assignmentProcessingType, matrixArg, metadataArg, defaultPrefix);
		if (result == OK) {
			result = translate(compatibilityMode);
			prepareResultMatrixAndMetadata();
			printResults();
			if (verificationEnablingFlag == VERIFICATION_ENABLED) {
				result = verify();
			}
		}
		cleanup();
		return result;
	}

	int initialize(
			char * fileName,
			char * typeCorrespondenceConfig,
			char * logFileName,
			unsigned char assignmentProcessingType,
			BYTE *** matrixArg,
			METADATA * metadataArg,
			BYTE * defaultPrefix
		) {
		matrix = matrixArg;
		metadata = metadataArg;
		initializeLogger(logFileName);
		initializeTypeCorrespondence(typeCorrespondenceConfig);
		assignParameters(assignmentProcessingType, defaultPrefix, synonymyRelationNodeId);
		initDataVector(&vertexData);
		initIntVectorVector(&outputArcTargetsPerElement);
		initIntVectorVector(&assignments);
		// Metadata is allocated but vertex data has been initialized with NULL and 0 as the number of elements.
		allocate_metadata(metadata, numberOfPrefixes, 0);
		copyPrefixes(metadata->prefix_data, getDefaultPrefixData(), numberOfPrefixes);
		initializeTypes(metadata);
		initializeAntlrParser(fileName);
		return OK;
	}

	int translate(unsigned char compatibilityMode) {
		int result = UNDEF_ERROR;
		switch (compatibilityMode) {
			case COMPATIBILITY_MODE_FULL: {
				result = translateWithFullCompatibility();
				break;
			}
			case COMPATIBILITY_MODE_PARTIAL: {
				result = translateWithPartialCompatibility();
				break;
			}
			default: {
				result = UNDEF_ERROR;
				printMessage("Unsupported type of compatibility mode has been spotted.");
			}
		}
		return result;
	}

	void prepareResultMatrixAndMetadata() {
		// Point metadata->vertex_data to already built array and set appropriate number of elements.
		metadata->vertex_data = vertexData->data;
		metadata->vertex_num = numberOfElements;
		// Allocating matrix filled with 0s.
		allocate_matrix(matrix, metadata);
		// Add info about arcs.
		fillMatrixWithArcs(matrix);
	}

	void cleanup() {
		destroyAntlrParser();
		if (assignments) {
			destroyIntVectorVector(assignments, true);
		}
		if (outputArcTargetsPerElement) {
			destroyIntVectorVector(outputArcTargetsPerElement, true);
		}
		if (vertexData) {
			destroyDataVector(vertexData, false);
		}
		destroyDefaultPrefix();
		destroyTypeCorrespondence();
		vertexData = NULL;
		outputArcTargetsPerElement = NULL;
		numberOfElements = 0;
		assignments = NULL;
		numberOfPrefixes = 1;
		defaultPrefix = NULL;
		synonymyRelationNodeId = NULL;
	}

	void printResults() {
		printMetadata(metadata);
		printMetadataToLog(metadata);
		printMatrix(*matrix, metadata);
		printMatrixToLog(*matrix, metadata);
		closeLogger();
	}

	int verify() {
		int result = OK;
		IntVector* currentElementTypes;
		int numberOfElements = metadata->vertex_num;
		int currentValue;
		/*
			Here right top corner of the matrix is processed.
			
			00|10101  <- first element has type 0, second 1, third 0, fourth 1, fifth 0 and 1
			00|01011
			--------
			00|00000
			00|00000
		
		*/
		for (int j = 0; j < numberOfElements; ++j) {
			initIntVector(&currentElementTypes);
			for (int i = 0; i < TYPES_CNT; ++i) {
				GET_TYPE(*matrix, i, j, currentValue);
				if (currentValue) {
					appendIntVector(currentElementTypes, i);
				}
			}
			result = checkElementTypesForIncompatibleness(currentElementTypes, j);
			destroyIntVector(currentElementTypes, true);
			if (result != OK) {
				break;
			}
		}
		return result;
	}

	void assignParameters(
		unsigned char assignmentProcessingType,
		BYTE * defaultPrefixParameter,
		char * synonymyRelationNodeIdParameter
		) {
		printMessage("Assigning parameters");
		printMessageAssignmentProcessingType(assignmentProcessingType);
		defaultPrefix = defaultPrefixParameter;
		printMessage("\t[PARAM] Default prefix parameter : %s", defaultPrefix);
		synonymyRelationNodeId = synonymyRelationNodeIdParameter;
		printMessage("\t[PARAM] Synonymy relation node identifier parameter : %s", synonymyRelationNodeId);
	}

	void initializeTypes(METADATA * metadata) {
		int index;
		int defaultPrefixId = metadata->prefix_data->id;
		IntVector * typeAssignments;
		IntVector * typeOutputArcTargets;
		for (index = 0; index < TYPES_CNT; ++index) {
			printMessage("Initializing type node %s", typeIdentifiers[index]);
			// Initialize vector of target elements for output arcs of the type element.
			initIntVector(&typeOutputArcTargets);
			appendIntVectorVector(
				outputArcTargetsPerElement,
				typeOutputArcTargets);
			// Initialize assignments vector for the type element 
			// (in case somebody wants to use custom synonyms of types or use them directly in SCs files).
			initIntVector(&typeAssignments);
			appendIntVector(typeAssignments, index);
			appendIntVectorVector(
				assignments,
				typeAssignments);
		}
		numberOfElements = 0;
	}

	void arcDiscovered(int sourceElementId, int targetElementId, int arcElementId) {
		printMessage(
			"Arc discovered. Source element : %d Target element : %d Arc element : %d", 
			sourceElementId, 
			targetElementId, 
			arcElementId);
		saveArc(outputArcTargetsPerElement, sourceElementId, arcElementId);
		saveArc(outputArcTargetsPerElement, arcElementId, sourceElementId);
		saveArc(outputArcTargetsPerElement, arcElementId, targetElementId);
	}

	void fillMatrixWithArcs(BYTE *** matrix) {
		int sourceElementId;
		int targetElementId;
		IntVector* outputArcTargets;
		for (sourceElementId = 0; sourceElementId < outputArcTargetsPerElement->size; ++sourceElementId) {
			outputArcTargets = outputArcTargetsPerElement->data[sourceElementId];
			for (targetElementId = 0; targetElementId < outputArcTargets->size; ++targetElementId) {
				generateArc(*matrix, sourceElementId, outputArcTargets->data[targetElementId]);
			}
		}
	}

	/*
	TODO: review implementation after multiple prefixes support is implemented.
	*/
	int elementDiscovered(
		const char * identifier,
		int identifierSize,
		const char * data,
		int dataSize,
		int dataType,
		int * typeIds,
		int numberOfTypeIds,
		BYTE * prefixIdentifier,
		int prefixIdentifierSize,
		int prefixId
		) {
		IntVector * elementOutputArcTargets;
		PREFIX * prefix;
		DATA * dataStructure;
		char * elementStatus;
		int elementId = getElementId(vertexData, identifier, data);
		if (elementId != ELEMENT_CANNOT_BE_FOUND_ID) {
			elementStatus = "Recognized";
			// Element has been found.
			// TODO: maybe some additional steps should be performed here. E.g. overwriting of element's data, or prefix.
		}
		else {
			elementStatus = "New";
			// Element has not been found.
			prefix = getOrCreatePrefixById(metadata, prefixIdentifier, prefixIdentifierSize, prefixId);
			elementId = numberOfElements++ + TYPES_CNT;
			createData(
				&dataStructure,
				identifier,
				identifierSize,
				data,
				dataSize,
				dataType,
				prefix->id
				);
			appendDataVector(vertexData, dataStructure);
			initIntVector(&elementOutputArcTargets);
			appendIntVectorVector(
				outputArcTargetsPerElement,
				elementOutputArcTargets);
			// Info about assignments should not be directly processed here.
			// While processing assignments this function is called and
			// only ID of created element should be returned.
		}
		updateTypeInfo(outputArcTargetsPerElement, elementId, typeIds, numberOfTypeIds);
		printElementDiscoveredInfo(
			elementStatus, identifier, identifierSize, data, dataSize, dataType, typeIds,
			numberOfTypeIds, prefixIdentifier, prefixIdentifierSize, prefixId
			);
		return elementId;
	}

	int elementDiscoveredDefault(
		const char * identifier,
		int identifierSize,
		int * typeIds,
		int numberOfTypeIds
		) {
		PREFIX * defaultPrefixInstance = getOrCreatePrefixById(metadata, defaultPrefix, strlen((const char*)defaultPrefix), 0);
		return elementDiscovered(
			identifier,
			identifierSize,
			NULL,
			0,
			DEFAULT_VERTEX_DATA_TYPE,
			typeIds,
			numberOfTypeIds,
			defaultPrefixInstance->idtf,
			defaultPrefixInstance->idtf_size,
			defaultPrefixInstance->id);
	}

	int elementDiscoveredDefaultLink(
		const char * identifier,
		int identifierSize,
		const char * data,
		int dataSize,
		int dataType
		) {
		PREFIX * defaultPrefixInstance = getOrCreatePrefixById(metadata, defaultPrefix, strlen((const char *)defaultPrefix), 0);
		IntVector * correspondingTypes = getCorrespondingTypesForScsType("sc_link");
		return elementDiscovered(
			identifier,
			identifierSize,
			data,
			dataSize,
			dataType,
			correspondingTypes->data,
			correspondingTypes->size,
			defaultPrefixInstance->idtf,
			defaultPrefixInstance->idtf_size,
			defaultPrefixInstance->id);
	}