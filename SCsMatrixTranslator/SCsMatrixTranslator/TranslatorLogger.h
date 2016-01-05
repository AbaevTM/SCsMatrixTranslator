#ifndef TranslatorLogger_H
#define TranslatorLogger_H

	#include "metadata.h"
	#include "SCsMatrixTranslatorConstants.h"	

#ifdef __cplusplus
extern "C" {
#endif

	/*
		Initizlizes logger with given log file name.
		*/
	void initializeLogger(const char * logFileName);

	/*
		Prints message of specified format to logFile if logger is initialized.
		Otherwise echoes it to standard output.
		Adds a newline symbol to the end of the message.

		Type specifiers for format string:

		%s		char *
		%p		void *
		%d,%i	int, size_t, signed char, short int
		%u		usigned int, unsigned char
		%c		int, wchar_t*
		*/
	void printMessage(char *fmt, ...);

	/*
		Prints message containing info about selected assignment processing type.
		*/
	void printMessageAssignmentProcessingType(unsigned char assignmentProcessingType);

	/*
		Prints given matrix to standard out.
		*/
	void printMatrix(BYTE ** matrix, METADATA * metadata);

	/*
		Prints given metadata to standard out.
		*/
	void printMetadata(METADATA * metadata);

	/*
		Prints given matrix to log file.
		*/
	void printMatrixToLog(BYTE ** matrix, METADATA * metadata);

	/*
		Prints given metadata to log file.
		*/
	void printMetadataToLog(METADATA * metadata);

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
		BYTE * prefixIdentifier,
		int prefixIdentifierSize,
		int prefixId
		);

	/*
		Prints identifiers of types one by one using their ids.
		*/
	void printTypeArray(int * typeArray, int arraySize);

	/*
		Closes the logger and releases logFile.
		*/
	void closeLogger();

#ifdef __cplusplus
	}
#endif

#endif

