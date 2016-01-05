// SCsMatrixTranslatorClient.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>

#include "SCsMatrixTranslator.h"
#include "test.h"
#include "matrix.h"
#include "metadata.h"

#define MAX_TEST_MATRIX_SIZE 200

int const NUMBER_OF_TESTS = 3;

int testResults[NUMBER_OF_TESTS];

char * const TYPES_CONFIG_FILE = "D:\\temp\\typesConfig";

char * const LOG_FILE = "D:\\temp\\log";

char * const DEFAULT_PREFIX = "default prefix";

char * const SYNONYMY_RELATION_NODE_ID = "synonymy";

void test0();

void test1();

void test2();

void test3();

void test4();

void test5();

void test6();

void test7();

void verifyResults(
	int status,
	BYTE ** matrix,
	METADATA * metadata,
	int expectedStatus,
	int expectedMatrix[][MAX_TEST_MATRIX_SIZE],
	int expectedMatrixSize,
	int testNumber);

int matrixesAreEqual(
	BYTE ** matrix,
	METADATA * metadata,
	int expectedMatrix[][MAX_TEST_MATRIX_SIZE],
	int expectedMatrixSize);

void printTestResults();

int _tmain(int argc, _TCHAR* argv[])
{
	int index;
	for (index = 0; index < NUMBER_OF_TESTS; ++index) {
		testResults[index] = 0;
	}
	test0();
	test1();
	test2();
	test3();
	test4();
	test5();
	test6();
	test7();
	printTestResults();
	getchar();
	return 0;
}

void verifyResults(
	int status,
	BYTE ** matrix,
	METADATA * metadata,
	int expectedStatus,
	int expectedMatrix[][MAX_TEST_MATRIX_SIZE],
	int expectedMatrixSize,
	int testNumber) {
	int result;
	if (status == expectedStatus) {
		if (expectedStatus == OK) {
			result = matrixesAreEqual(matrix, metadata, expectedMatrix, expectedMatrixSize);
		}
		else {
			result = 1;
		}		
	} else {
		printf("\nStatuses are not equal, expected : %d, actual : %d\n", expectedStatus, status);
		result = 0;
	}
	testResults[testNumber] = result;
}

int matrixesAreEqual(
	BYTE ** matrix,
	METADATA * metadata,
	int expectedMatrix[][MAX_TEST_MATRIX_SIZE],
	int expectedMatrixSize) {
	int result = 1;
	int i = 0;
	int j = 0;
	int currentValue;
	int currentExpectedValue;
	int matrixSize = metadata->vertex_num + TYPES_CNT;
	if (matrixSize == expectedMatrixSize) {
		for (i; i < matrixSize && result; ++i) {
			for (j = 0; j < matrixSize && result; ++j) {
				currentExpectedValue = expectedMatrix[i][j];
				GET_ELEMENT(matrix, i - TYPES_CNT, j - TYPES_CNT, currentValue);
				if (currentValue != currentExpectedValue) {
					printf(
						"\nMatrix values are not equal, (i: %d j: %d) expected : %d, actual : %d\n", 
						i, 
						j, 
						currentExpectedValue, 
						currentValue);
					result = 0;
					break;
				}
			}
		}
	} else {
		printf(
			"\nMatrixes has different sizes expected : %d, actual : %d\n",
			expectedMatrixSize,
			matrixSize);
		result = 0;
	}
	return result;
}

void test0() {
	BYTE ** matrix = NULL;
	METADATA * metadata = (METADATA*)malloc(sizeof(METADATA));
	int status = translateSCsToMatrix(
		"D:\\temp\\scs_level_1\\test0.scs",
		TYPES_CONFIG_FILE,
		LOG_FILE,
		COMPATIBILITY_MODE_FULL,
		ASSIGNMENT_PROCESSING_TYPE_GENERATE_SYMMETRY,
		VERIFICATION_DISABLED,
		(BYTE *)"default prefix",
		"synonymy",
		&matrix,
		metadata
		);
	int const expectedMatrixSize = 23;
	int expectedMatrix[][MAX_TEST_MATRIX_SIZE]
		= {
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0 } };
	verifyResults(status, matrix, metadata, OK, expectedMatrix, expectedMatrixSize, 0);
	delete_matrix(&matrix, metadata);
	delete_metadata(metadata);
}

void test1() {
	BYTE ** matrix = NULL;
	METADATA * metadata = (METADATA*)malloc(sizeof(METADATA));
	int status = translateSCsToMatrix(
		"D:\\temp\\scs_level_1\\test1.scs",
		TYPES_CONFIG_FILE,
		LOG_FILE,
		COMPATIBILITY_MODE_FULL,
		ASSIGNMENT_PROCESSING_TYPE_GENERATE_SYMMETRY,
		VERIFICATION_DISABLED,
		(BYTE *)"default prefix",
		"synonymy",
		&matrix,
		metadata
		);
	int const expectedMatrixSize = 22;
	int expectedMatrix[][MAX_TEST_MATRIX_SIZE]
		= {
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0 } };
	verifyResults(status, matrix, metadata, OK, expectedMatrix, expectedMatrixSize, 1);
	delete_matrix(&matrix, metadata);
	delete_metadata(metadata);
}

void test2() {
	BYTE ** matrix = NULL;
	METADATA * metadata = (METADATA*)malloc(sizeof(METADATA));
	int status = translateSCsToMatrix(
		"D:\\temp\\scs_level_1\\test1.scs",
		TYPES_CONFIG_FILE,
		LOG_FILE,
		COMPATIBILITY_MODE_FULL,
		ASSIGNMENT_PROCESSING_TYPE_GENERATE_SYMMETRY,
		VERIFICATION_ENABLED,
		(BYTE *)"default prefix",
		"synonymy",
		&matrix,
		metadata
		);
	int const expectedMatrixSize = 22;
	int expectedMatrix[][MAX_TEST_MATRIX_SIZE] = {{}};
	verifyResults(status, matrix, metadata, UNDEF_ERROR, expectedMatrix, expectedMatrixSize, 2);
	delete_matrix(&matrix, metadata);
	delete_metadata(metadata);
}

void test3() {

}

void test4() {

}

void test5() {

}

void test6() {

}

void test7() {

}

void printTestResults() {
	int index;
	int numberOfPassedTests = 0;
	for (index = 0; index < NUMBER_OF_TESTS; ++index) {
		if (testResults[index]) {
			++numberOfPassedTests;
		};
	}
	printf("\n----------------------------------------\n");
	printf("Number of passed tests: %d out of %d", numberOfPassedTests, NUMBER_OF_TESTS);
}