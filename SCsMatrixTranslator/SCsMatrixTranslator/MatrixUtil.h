#ifndef MatrixUtil_H
#define MatrixUtil_H

	#include "matrix.h"
	#include "type.h"

#ifdef __cplusplus
extern "C" {
#endif
	/*
		Writes to the matrix info about the arc between source and target elements.
		Overwrites existing arc.

		Ids should be provided avoiding separation of type and "ordinary" nodes.
		I.e. if there is an arc having source as the last type and target as the "first" element
		function should receive values TYPES_CNT -1, TYPES_CNT not TYPES_CNT -1, 0.
		*/
	void generateArc(BYTE ** matrix, int sourceElementId, int targetElementId);

#ifdef __cplusplus
}
#endif
#endif