#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "TranslatorLogger.h"
#include "matrix.h"
#include "metadata.h"
#include "SCsMatrixTranslator.h"

	/*
		Concatenates given strings dynamically allocating new one.

		returns: const char * to result that should be freed ones it is useless.
	*/
	char * concatStrings(char * string1, char * string2);

	FILE * logFile;

	const char * logFileName;

	void initializeLogger(const char * logFileNameArg){
		logFileName = logFileNameArg;
		if (logFileName) {
			logFile = fopen(logFileName, "at");
			if (logFile) {
				printf("Logging enabled. Log file : %s", logFileName);
			}
			else {
				printf("Cannot open logfile : %s", logFileName);
			}
		}
		else {
			printf("Logging disabled.");
		}
		printf("\n\r");
	}

	void printMessage(char *fmt, ...) {
		char * format = concatStrings(fmt,"\n");
		if (logFile) {
			va_list args;
			va_start(args, fmt);
			vfprintf(logFile, format, args);
			va_end(args);
			fflush(logFile);
		}
		else {
			va_list args;
			va_start(args, fmt);
			vprintf(format, args);
			va_end(args);
		}
		free(format);
	}

	void printMessageAssignmentProcessingType(unsigned char assignmentProcessingType) {
		switch (assignmentProcessingType) {
			case ASSIGNMENT_PROCESSING_TYPE_GENERATE_SYMMETRY :
				printMessage("\t[PARAM] Assignment processing type : ASSIGNMENT_PROCESSING_TYPE_GENERATE_SYMMETRY");
				break;
			case ASSIGNMENT_PROCESSING_TYPE_MERGE_ELEMENTS:
				printMessage("\t[PARAM] Assignment processing type : ASSIGNMENT_PROCESSING_TYPE_MERGE_ELEMENTS");
				break;
		}
	}

	void printMatrix(BYTE ** matrix, METADATA * metadata) {
		int i;
		int j;
		int numberOfElements = metadata->vertex_num + TYPES_CNT;
		int currentValue;
		printf("MATRIX\n");
		for (i = 0; i < numberOfElements; ++i) {
			printf("\t");
			if (i == TYPES_CNT) {
				for (j = 0; j <= numberOfElements; ++j) {
					printf("-");
				}
				printf("\n\t");
			}
			for (j = 0; j < numberOfElements; ++j) {
				if (j < TYPES_CNT) {
					GET_TYPE(matrix, i, j - TYPES_CNT, currentValue);
				}
				else {
					if (j == TYPES_CNT) {
						printf("|");
					}
					GET_ELEMENT(matrix, i - TYPES_CNT, j - TYPES_CNT, currentValue);
				}
				printf("%d", currentValue);
			}
			if (i >= TYPES_CNT
				&& metadata->vertex_data[i - TYPES_CNT].idtf != NULL) {
				printf(" %s", metadata->vertex_data[i - TYPES_CNT].idtf);
			}
			printf("\n");
		}
	}

	void printMetadata(METADATA * metadata) {
		int index;
		DATA vertexData;
		printf("METADATA\n");
		for (index = 0; index < metadata->vertex_num; ++index) {
			printf("\tElement %d\n", index);
			vertexData = (metadata->vertex_data)[index];
			printf("\t\tIdentifier %s\n", vertexData.idtf);
			printf("\t\tData %s\n", vertexData.data);
		}
	}

	void printMatrixToLog(BYTE ** matrix, METADATA * metadata) {
		int i;
		int j;
		int numberOfElements = metadata->vertex_num + TYPES_CNT;
		int currentValue = 0;
		fprintf(logFile, "MATRIX\n");
		for (i = 0; i < numberOfElements; ++i) {
			fprintf(logFile, "\t");
			if (i == TYPES_CNT) {
				for (j = 0; j <= numberOfElements; ++j) {
					fprintf(logFile, "-");
				}
				fprintf(logFile, "\n\t");
			}
			for (j = 0; j < numberOfElements; ++j) {
				if (j < TYPES_CNT) {
					GET_TYPE(matrix, i, j - TYPES_CNT, currentValue);
				}
				else {
					if (j == TYPES_CNT) {
						fprintf(logFile, "|");
					}
					GET_ELEMENT(matrix, i - TYPES_CNT, j - TYPES_CNT, currentValue);
				}
				fprintf(logFile, "%d", currentValue);
			}
			if (i >= TYPES_CNT
				&& metadata->vertex_data[i - TYPES_CNT].idtf != NULL) {
				fprintf(logFile, " %s", metadata->vertex_data[i - TYPES_CNT].idtf);
			}
			fprintf(logFile, "\n");
		}
		fflush(logFile);
	}

	void printMetadataToLog(METADATA * metadata) {
		int index;
		DATA vertexData;
		fprintf(logFile, "METADATA\n");
		for (index = 0; index < metadata->vertex_num; ++index) {
			fprintf(logFile, "\tElement %d\n", index);
			vertexData = (metadata->vertex_data)[index];
			fprintf(logFile, "\t\tIdentifier %s\n", vertexData.idtf);
			fprintf(logFile, "\t\tData %s\n", vertexData.data);
		}
		fflush(logFile);
	}

	/*
	Prints info about discovered element.
	*/
	void printElementDiscoveredInfo(
		char * elementStatus,
		const char *  identifier,
		int identifierSize,
		const char *  data,
		int dataSize,
		int dataType,
		int * typeIds,
		int numberOfTypeIds,
		BYTE *  prefixIdentifier,
		int prefixIdentifierSize,
		int prefixId
		) {
		printMessage("%s element has been discovered.", elementStatus);
		printMessage("\tIdentifier : %s", identifier);
		printMessage("\tIdentifier size : %d", identifierSize);
		printMessage("\tData : %s", data);
		printMessage("\tData size : %d", dataSize);
		printMessage("\tData type : %d", dataType);
		printMessage("\tPrefix identifier : %s", prefixIdentifier);
		printMessage("\tPrefix identifier size : %d", prefixIdentifierSize);
		printMessage("\tPrefix id : %d", prefixId);
		if (numberOfTypeIds) {
			printMessage("Element's types:");
			printTypeArray(typeIds, numberOfTypeIds);
		}
		else {
			printMessage("Type info is missing.");
		}
	}

	void printTypeArray(int * typeArray, int arraySize) {
		int index;
		for (index = 0; index < arraySize; ++index) {
			printMessage("\t%d", typeArray[index]);
		}
	}

	void closeLogger() {
		fclose(logFile);
		printf("Closing logger. Log file has been saved.");
	}

	char * concatStrings(char * string1, char * string2) {
		size_t message_len = strlen(string1) + strlen(string2) + 1;
		char * message = (char*)malloc(message_len);
		strcpy(message, string1);
		strcat(message, string2);
		return message;
	}
	

	