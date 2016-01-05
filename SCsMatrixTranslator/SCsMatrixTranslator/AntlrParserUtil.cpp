#include "AntlrParserUtil.h"
#include "antlr3.h"
#include "scsLexer.h"
#include "scsParser.h"
#include "TranslatorLogger.h"

	const char * fileName;

	pANTLR3_UINT8 file;

	pANTLR3_INPUT_STREAM inputStream;

	pscsLexer lexer;

	pANTLR3_COMMON_TOKEN_STREAM tokenStream;

	pscsParser parser;

	scsParser_syntax_return rootNode;

	void initializeAntlrParser(const char * fileNameArg) {
		printMessage("Initializing ANTLR parser");
		fileName = fileNameArg;
		file = (pANTLR3_UINT8) fileName;
		inputStream = antlr3FileStreamNew(file, ANTLR3_ENC_UTF8);
	};

	scsParser_syntax_return parseFile() {
		unsigned int errorNumber;
		printMessage("Parsing file : %s", fileName);
		if (inputStream == NULL)
		{
			printMessage("Unable to open file %s due to malloc() failure1", fileName);
			exit(ANTLR3_ERR_NOMEM);
		}
		lexer = scsLexerNew(inputStream);
		if (lexer == NULL)
		{
			printMessage("Unable to create the lexer due to malloc() failure1");
			exit(ANTLR3_ERR_NOMEM);
		}
		tokenStream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lexer));
		if (tokenStream == NULL)
		{
			printMessage("Out of memory trying to allocate token stream");
			exit(ANTLR3_ERR_NOMEM);
		}
		parser = scsParserNew(tokenStream);
		if (parser == NULL)
		{
			printMessage("Out of memory trying to allocate parser");
			exit(ANTLR3_ERR_NOMEM);
		}
		errorNumber = parser->pParser->rec->getNumberOfSyntaxErrors(parser->pParser->rec);
		if (errorNumber > 0) {
			printMessage("The parser returned %d errors, tree walking aborted", errorNumber);
			exit(ANTLR3_ERR_BASE + 4);
		}
		return parser->syntax(parser);
	}

	void destroyAntlrParser() {
		parser->free(parser);
		parser = NULL;
		tokenStream->free(tokenStream);
		tokenStream = NULL;
		lexer->free(lexer);
		lexer = NULL;
		inputStream->close(inputStream);
		inputStream = NULL;
	}

	char* getText(pANTLR3_BASE_TREE tree) {
		return (char*) tree->getText(tree)->chars;
	}

	int getSentenceLevel(pANTLR3_BASE_TREE sentenceTreeNode){
		switch (sentenceTreeNode->getToken(sentenceTreeNode)->type) {
			case SEP_SIMPLE :
				return SENTENCE_LEVEL_1;
			case CONNECTORS :
				return SENTENCE_LEVEL_2_OR_HIGHER;
			case SEP_ASSIGN :
				return SENTENCE_ASSIGNMENT;
			default:
				return -1;
		}
	}
