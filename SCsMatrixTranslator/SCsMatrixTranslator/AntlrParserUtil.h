#ifndef AntlrParserUtil_H
#define AnlrParserUtil_H

	#define ARC_IDENTIFIER_TYPE_SEPARATOR_SYMBOL '#'

	#include "antlr3.h"
	#include "scsParser.h"
	#include "SCsMatrixTranslator.h"

	void destroyAntlrParser();

	/**
		Initializes parser creating inputStream.
	*/
	void initializeAntlrParser(const char * fileName);

	/**
		Parses previously given file.
		Creates necessary structures and allocates memory.
		Prints description and exits the program if errors occur while parsing scs files.

		returns:	scsParser_syntax_return object containing pointer to tree node.
	*/
	scsParser_syntax_return parseFile();

	/**
		Destroys parser and cleans allocated memory.
	*/
	void destroyAntlrParser();

	/**
		returns: char* - pointer to string representation of tree node. 
	*/
	char* getText(pANTLR3_BASE_TREE tree);

	#define SENTENCE_LEVEL_1 1

	#define SENTENCE_LEVEL_2_OR_HIGHER 2

	# define SENTENCE_ASSIGNMENT 3

	/*
	Checks type of given node representing scs sentence.

	sentenceTreeNode : tree node

	returns: one of SENTENCE_LEVEL_1, SENTENCE_LEVEL_2_or_higher constants
	*/
	int getSentenceLevel(pANTLR3_BASE_TREE sentenceTreeNode);


#endif
