#ifndef SCsMatrixTranslatorConstants
#define SCsMatrixTranslatorConstants

#define ARG_UNDEF  -1	
#define ARG_DATA  3
#define DATA_TYPE_LINK 4
#define DATA_TYPE_LINK_FILE 5
#define DATA_TYPE_LINK_STRING 6

/*
Default value of vertex data's data_type.
*/
#define DEFAULT_VERTEX_DATA_TYPE  ARG_UNDEF

/*
Default value of link data's data_type.
*/
#define DEFAULT_LINK_DATA_TYPE  ARG_DATA

/*
Enables parsing compatible to scs translator currently implemented in sc-machine.
Assumes reproduction of wrong and strange behaviour. To avoid mistakes use other
compatibility modes.
*/
#define COMPATIBILITY_MODE_FULL 0 

/*
Enables parsing partially compatible to scs translator currently implemented in sc-machine.
Avoids wrong and strange behaviour. Aimed at the most correct translation.
*/
#define COMPATIBILITY_MODE_PARTIAL 1 

/*
	Enables verification of the generated matrix. Verification includes checking for
	elements having incompatible types (i.e. one element at the same time is POS and NEG).
	List of incompatible types is supplied whithin types config file.
*/
#define VERIFICATION_ENABLED 1 

/*
	Disables verification of the generated matrix. Verification includes checking for
	elements having incompatible types (i.e. one element at the same time is POS and NEG).
	List of incompatible types should be skiped whithin types config file at corresponding section.
*/
#define VERIFICATION_DISABLED 0 

/*
Generates tuple of relation "symmetry" for left and right elements.

a=b

=

a o<-o->o b
^
|
o
symmetry
*/
#define ASSIGNMENT_PROCESSING_TYPE_GENERATE_SYMMETRY 0 

/*
Merges left and right elements processing input and output arcs.

TODO: not implemented yet
*/
#define ASSIGNMENT_PROCESSING_TYPE_MERGE_ELEMENTS 1

#endif
