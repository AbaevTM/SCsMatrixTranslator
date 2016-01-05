#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "TypeUtil.h"
#include "MatrixUtil.h"
#include "TranslatorLogger.h"

	#define DEFAULT_NUMBER_OF_SCS_STRING_TYPES 6

	#define DEFAULT_NUMBER_OF_SCS_ARC_SYMBOLS 0

	#define ARC_IDENTIFIER_TYPE_SEPARATOR_SYMBOL '#'

	char * typeIdentifiers[TYPES_CNT] = {
		"CONST", "VAR", "META",
		"UNCERTAINED", "ARC", "NODE",
		"POSITIVE", "NEGATIVE", "FUZZY",
		"TEMPORARY", "PERMANENT", "ACTUAL",
		"PHANTOM"
	};

	int * incompatibleTypesToListCorresondence;

	char ** scsTypeIdentifiers;

	char * defaultScsTypeIdentifiers[DEFAULT_NUMBER_OF_SCS_STRING_TYPES] = {
		"sc_node", "sc_link", "sc_arc_common", "sc_arc_main", "sc_arc_access", "sc_edge_common"
	};

	char ** scsArcSymbols;

	char * pathToTypeConfig = NULL;

	IntVectorVector * stringTypeCorrespondence;

	IntVectorVector * incompatibleTypes;

	void initializeDefaultTypeCorrespondence() {
		int result = UNDEF_ERROR;
		IntVector * scsTypeCorrespondingTypes;
		printMessage("Initizlizing type correspondence with default values.");
		scsTypeIdentifiers = defaultScsTypeIdentifiers;
		result = initIntVectorVector(&stringTypeCorrespondence);
		if (result == OK) {
			// sc_node
			initIntVector(&scsTypeCorrespondingTypes);
			appendIntVector(scsTypeCorrespondingTypes, 3);
			appendIntVectorVector(stringTypeCorrespondence, scsTypeCorrespondingTypes);
			destroyIntVector(scsTypeCorrespondingTypes, false);
			// sc_link
			initIntVector(&scsTypeCorrespondingTypes);
			appendIntVector(scsTypeCorrespondingTypes, 3);
			appendIntVectorVector(stringTypeCorrespondence, scsTypeCorrespondingTypes);
			destroyIntVector(scsTypeCorrespondingTypes, false);
			// sc_arc_common
			initIntVector(&scsTypeCorrespondingTypes);
			appendIntVector(scsTypeCorrespondingTypes, 3);
			appendIntVectorVector(stringTypeCorrespondence, scsTypeCorrespondingTypes);
			destroyIntVector(scsTypeCorrespondingTypes, false);
			// sc_arc_main
			initIntVector(&scsTypeCorrespondingTypes);
			appendIntVector(scsTypeCorrespondingTypes, 3);
			appendIntVectorVector(stringTypeCorrespondence, scsTypeCorrespondingTypes);
			destroyIntVector(scsTypeCorrespondingTypes, false);
			// sc_arc_access
			initIntVector(&scsTypeCorrespondingTypes);
			appendIntVector(scsTypeCorrespondingTypes, 3);
			appendIntVectorVector(stringTypeCorrespondence, scsTypeCorrespondingTypes);
			destroyIntVector(scsTypeCorrespondingTypes, false);
			// sc_edge_common
			initIntVector(&scsTypeCorrespondingTypes);
			appendIntVector(scsTypeCorrespondingTypes, 3);
			appendIntVectorVector(stringTypeCorrespondence, scsTypeCorrespondingTypes);
			destroyIntVector(scsTypeCorrespondingTypes, false);
		}
	}

	void initializeTypeCorrespondence(char * pathToTypeConfigArg) {
		IntVector * scsTypeCorrespondingTypes;
		FILE * typeCorrespondenceConfigFile;
		int numberOfNamedTypes;
		int numberOfCorrespondingTypesForNamedType;
		int type;
		int index;
		int index2;
		int numberOfIncompatibleTypesLists;
		int numberOfIncompatibleTypesForNamedType;
		IntVector * incompatibleTypesForType;
		int incompatibleType;
		char * stringTypeRepresentation;
		pathToTypeConfig = pathToTypeConfigArg;
		if (pathToTypeConfig != NULL) {
			printMessage("Initizlizing type correspondence using config file : %s", pathToTypeConfig);
			typeCorrespondenceConfigFile = fopen(pathToTypeConfig, "rt");
			if (typeCorrespondenceConfigFile) {
				printf("Type correspondence config file is opened.");
				// Read number of scs string types.
				fscanf(typeCorrespondenceConfigFile, "%d", &numberOfNamedTypes);
				printMessage("Number of named type representations : %d", numberOfNamedTypes);
				scsTypeIdentifiers = (char**)malloc(sizeof(char *)*numberOfNamedTypes);
				initIntVectorVector(&stringTypeCorrespondence);
				for (index = 0; index < numberOfNamedTypes; ++index) {
					fscanf(typeCorrespondenceConfigFile, "\n");
					stringTypeRepresentation = (char *) calloc(100,sizeof(char));
					fscanf(typeCorrespondenceConfigFile, "%[^\:]", stringTypeRepresentation);
					fscanf(typeCorrespondenceConfigFile, ":");
					scsTypeIdentifiers[index] = stringTypeRepresentation;
					// Read number of corresponding types and create vector.
					fscanf(typeCorrespondenceConfigFile, "%d", &numberOfCorrespondingTypesForNamedType);
					printMessage(
						"\tReading %d corresponding types for type : %s", 
						numberOfCorrespondingTypesForNamedType, 
						stringTypeRepresentation);
					fscanf(typeCorrespondenceConfigFile, ":");
					initIntVector(&scsTypeCorrespondingTypes);
					fscanf(typeCorrespondenceConfigFile, "%d", &type);
					appendIntVector(scsTypeCorrespondingTypes, type);
					printMessage("\t\t%s (%d)", typeIdentifiers[type], type);
					for (index2 = 1; index2 < numberOfCorrespondingTypesForNamedType; ++index2) {
						fscanf(typeCorrespondenceConfigFile, ",%d", &type);
						appendIntVector(scsTypeCorrespondingTypes, type);
						printMessage("\t\t%s (%d)", typeIdentifiers[type], type);
					}
					appendIntVectorVector(stringTypeCorrespondence, scsTypeCorrespondingTypes);
				}
				// Reading incompatible types lists.
				fscanf(typeCorrespondenceConfigFile, "%d", &numberOfIncompatibleTypesLists);
				printMessage("Number of incompatible types lists : %d", numberOfIncompatibleTypesLists);
				initIntVectorVector(&incompatibleTypes);
				incompatibleTypesToListCorresondence = (int *)malloc(sizeof(int)*TYPES_CNT);
				for (int index3 = 0; index3 < TYPES_CNT; ++index3){
					incompatibleTypesToListCorresondence[index3] = -1;
				}
				for (index = 0; index < numberOfIncompatibleTypesLists; ++index) {
					fscanf(typeCorrespondenceConfigFile, "\n");
					fscanf(typeCorrespondenceConfigFile, "%d", &type);
					incompatibleTypesToListCorresondence[type] = index;
					fscanf(typeCorrespondenceConfigFile, ":");
					// Read number of incompatible types and create vector.
					fscanf(typeCorrespondenceConfigFile, "%d", &numberOfIncompatibleTypesForNamedType);
					printMessage(
						"\tReading %d corresponding types for type : %d",
						numberOfIncompatibleTypesForNamedType,
						type);
					fscanf(typeCorrespondenceConfigFile, ":");
					initIntVector(&incompatibleTypesForType);
					fscanf(typeCorrespondenceConfigFile, "%d", &incompatibleType);
					appendIntVector(incompatibleTypesForType, incompatibleType);
					printMessage("\t\t%d", typeIdentifiers[type], incompatibleType);
					for (index2 = 1; index2 < numberOfIncompatibleTypesForNamedType; ++index2) {
						fscanf(typeCorrespondenceConfigFile, "%d", &incompatibleType);
						appendIntVector(incompatibleTypesForType, incompatibleType);
						printMessage("\t\t%d", typeIdentifiers[type], incompatibleType);
					}
					appendIntVectorVector(incompatibleTypes, incompatibleTypesForType);
				}
				fclose(typeCorrespondenceConfigFile);
			}
			else {
				printf("Cannot open type correspondence config file.");
				initializeDefaultTypeCorrespondence();
			}
		} else {
			initializeDefaultTypeCorrespondence();
		}
	}

	int checkElementTypesForIncompatibleness(IntVector* elementTypes, int elementId) {
		int result = OK;
		int currentType;
		int incompatibleTypesListIndex;
		IntVector* incompatibleTypesForType;
		for (int index = 0; index < elementTypes->size; ++index) {
			currentType = elementTypes->data[index];
			incompatibleTypesListIndex = incompatibleTypesToListCorresondence[currentType];
			if (incompatibleTypesListIndex != -1) {
				incompatibleTypesForType = incompatibleTypes->data[index];
				if (intVectorsHaveCommonElements(incompatibleTypesForType, elementTypes)) {
					printMessage("[WARNING] Element #%d has incompatible types. Checked type %d.", elementId, currentType);
					result = UNDEF_ERROR;
					break;
				}
			}
		}
		return result;
	}

	void destroyTypeCorrespondence() {
		int index;
		if (pathToTypeConfig) {
			for (index = 0; index < stringTypeCorrespondence->size; ++index) {
				free(scsTypeIdentifiers[index]);
			}
			free(scsTypeIdentifiers);
			if (incompatibleTypesToListCorresondence) {
				free(incompatibleTypesToListCorresondence);
			}
		}
		destroyIntVectorVector(stringTypeCorrespondence, true);
		destroyIntVectorVector(incompatibleTypes, true);
	}

	IntVector * getCorrespondingTypesForScsType(const char * scsTypeStringRepresentation) {
		int index;
		IntVector * result;
		for (index = 0; index < stringTypeCorrespondence->size; ++index) {
			if (strcmp(scsTypeStringRepresentation, scsTypeIdentifiers[index]) == 0) {
				initIntVectorUsingVectorToCopy(
					&result,
					*(stringTypeCorrespondence->data + index)
					);
				return result;
			}
		}
		printMessage("[WARNING] Cannot find corresponding types for SCs type : %s", scsTypeStringRepresentation);
		return NULL;
	}

	IntVector* getTypesByMergingSeveral(char ** typeNames, int numberOfTypes) {
		IntVector* result;
		initIntVector(&result);
		IntVector* temporaryType;
		for (int index = 0; index < numberOfTypes; ++index) {
			temporaryType = getCorrespondingTypesForScsType(typeNames[index]);
			appendIntVectorToIntVector(result, temporaryType);
			destroyIntVector(temporaryType, true);
		}
		return result;
	}

	int getElementIdByIdentifier(DataVector * vertexData, const char * identifier) {
		int index;
		BYTE * currentIdentifier;
		if (identifier != NULL) {
			for (index = 0; index < vertexData->size; ++index) {
				currentIdentifier = vertexData->data[index].idtf;
				if (currentIdentifier != NULL && strcmp(identifier, (const char*)currentIdentifier) == 0) {
					return index + TYPES_CNT;
				}
			}
		}
		return ELEMENT_CANNOT_BE_FOUND_ID;
	}

	int getElementIdByData(DataVector * vertexData, const char * data) {
		int index;
		BYTE * currentData;
		if (data != NULL) {
			for (index = 0; index < vertexData->size; ++index) {
				currentData = vertexData->data[index].data;
				if (currentData != NULL && strcmp(data, (const char*)currentData) == 0) {
					return index + TYPES_CNT;
				}
			}
		}
		return ELEMENT_CANNOT_BE_FOUND_ID;
	}

	int getElementId(DataVector * vertexData, const char * identifier, const char * data) {
		int id;
		id = getElementIdByIdentifier(vertexData, identifier);
		if (id == ELEMENT_CANNOT_BE_FOUND_ID) {
			return getElementIdByData(vertexData, data);
		} else {
			return id;
		}
	}	

	int updateTypeInfo(
		IntVectorVector * outputArcTargetsPerElement, 
		int elementId, 
		int * typeIdentifiersArg, 
		int numberOfTypeIdentifiers
		) {
		int index;
		for (index = 0; index < numberOfTypeIdentifiers; ++index) {
			saveArc(outputArcTargetsPerElement, typeIdentifiersArg[index], elementId);
		}
		return OK;
	}

	void saveArc(IntVectorVector * outputArcTargetsPerElement, int sourceElementId, int targetElementId) {
		int index;
		for (index = 0; index < outputArcTargetsPerElement->size; ++index) {
			if (index == sourceElementId) {
				appendIntVector(outputArcTargetsPerElement->data[index], targetElementId);
				return;
			}
		}
	}

	IntVector * getTypeIdsOfNodesAtSentenceLevel1() {
		return getCorrespondingTypesForScsType("sc_node");
	}

	IntVector * getTypeIdsOfArcsAtSentenceLevel1(char * type) {
		if (strcmp(type, "sc_arc_common") == 0) {
			return getCorrespondingTypesForScsType("sc_arc_common");
		}
		if (strcmp(type, "sc_arc_main") == 0) {
			return getCorrespondingTypesForScsType("sc_arc_main");
		}
		if (strcmp(type, "sc_arc_access") == 0) {
			return getCorrespondingTypesForScsType("sc_arc_access");
		}		
		return getCorrespondingTypesForScsType("sc_edge_common");
	}

	int getAssignmentsVectorIdByIdentifier(IntVectorVector * assignments, DataVector * vertexData, char * identifier) {
		int index;
		int index2;
		int elementId = getElementIdByIdentifier(vertexData, identifier);
		IntVector * assignmentGroup;
		if (elementId != ELEMENT_CANNOT_BE_FOUND_ID) {
			for (index = 0; index < assignments->size; ++index) {
				assignmentGroup = *(assignments->data + index);
				for (index2 = 0; index2 < assignmentGroup->size; ++index2) {
					if (assignmentGroup->data[index2] == elementId) {
						return index;
					}
				}
			}
		} 
		return ELEMENT_CANNOT_BE_FOUND_ID;
	}