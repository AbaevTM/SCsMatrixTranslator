#ifndef Element_H
#define Element_H

	#include "Vector.h"

	typedef struct ElementStruct
	{
		
		/*
			Element's index in matrix.
		*/
		int index;
		
		/*
			Vector containing indexes of all adjacent elements (only for input arcs of this element).
		*/
		IntVector * adjacentElements;

	} Element;

#endif