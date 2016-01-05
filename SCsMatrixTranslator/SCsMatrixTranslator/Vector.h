#ifndef Vector_H
#define Vector_H

	#include <stdbool.h>
	#include "type.h"
	
	#define INITIAL_CAPACITY 10	

	#define CAPACITY_MULTIPLIER 2	
#ifdef __cplusplus
extern "C" {
#endif

	/*
	
		Vector of integers

	*/

	typedef struct IntVectorStructure{
	
		int * data;

		int size;

		int capacity;

	} IntVector;

	int initIntVector(IntVector ** vector);

	int initIntVectorUsingVectorToCopy(IntVector ** vector, IntVector * vectorToCopy);

	int appendIntVector(IntVector *vector, int value);

	int appendIntVectorToIntVector(IntVector * vector, IntVector * value);

	void removeFromIntVector(IntVector *vector, int value);

	void removeFromIntVectorAllValuesOfIntVector(IntVector * vector, IntVector * value);

	bool intVectorsAreEqual(IntVector * firstVector, IntVector * secondVector);

	bool intVectorsHaveCommonElements(IntVector * firstVector, IntVector * secondVector);

	IntVector* getCommonElementsOfIntVectorsContainedInFirst(IntVector * firstVector, IntVector * secondVector);

	bool intVectorContainsValue(IntVector * vector, int value);

	int increaseCapacityIfNeededIntVector(IntVector *vector);

	void destroyIntVector(IntVector * vector, bool destroyData);

	/*

		Vector of vectors of integers

	*/

	typedef struct IntVectorVectorStructure{

		IntVector ** data;

		int size;

		int capacity;

	} IntVectorVector;

	int initIntVectorVector(IntVectorVector ** vector);

	int appendIntVectorVector(IntVectorVector * vector, IntVector * value);

	int increaseCapacityIfNeededIntVectorVector(IntVectorVector *vector);

	void destroyIntVectorVector(IntVectorVector * vector, bool destroyData);

	/*

		Vector of vertex data

	*/

	typedef struct DataVectorStructure{

		DATA * data;

		int size;

		int capacity;

	} DataVector;

	int initDataVector(DataVector ** vector);

	int appendDataVector(DataVector *vector, DATA * value);

	int increaseCapacityIfNeededDataVector(DataVector *vector);

	void destroyDataVector(DataVector * vector, bool destroyData);
#ifdef __cplusplus
	}
#endif
#endif