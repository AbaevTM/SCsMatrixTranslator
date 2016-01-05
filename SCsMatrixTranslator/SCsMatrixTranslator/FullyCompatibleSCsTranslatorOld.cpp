#include "FullyCompatibleSCsTranslator.h"
#include "AntlrParserUtil.h"

/*
Processes scs sentence of level 1.
*/
void processSentenceLevel1Old(pANTLR3_BASE_TREE sentenceNode);

/*
Processes node elements at sentence of level 1.
returnes : id of discovered element.
*/
int processSentenceLevel1NodeOld(pANTLR3_BASE_TREE node);

/*
Processes connector elements at sentence of level 1.
returnes : id of discovered element.
*/
int processSentenceLevel1ConnectorOld(pANTLR3_BASE_TREE connector, int sourceElementId, int targetElementId);

/*
int translateWithFullCompatibilityOld(IntVectorVector * assignments, DataVector * vertexData) {
		int result = OK;
		unsigned int index;
		pANTLR3_BASE_TREE sentence;
		// Parse file.
		scsParser_syntax_return scsFile = parseFile();
		pANTLR3_BASE_TREE rootNode = scsFile.tree;
		for (index = 0; index < rootNode->getChildCount(rootNode); ++index) {
			sentence = rootNode->getChild(rootNode, index);
			switch (getSentenceLevel(sentence)) {
			case SENTENCE_LEVEL_1:
				processSentenceLevel1(sentence);
				break;
			case SENTENCE_LEVEL_2_OR_HIGHER:
				break;
			case SENTENCE_ASSIGNMENT:
				processAssignmentSentence(sentence, assignments, vertexData);
				break;
			}
		}
		return result;
	}

void processSentenceLevel1Old(pANTLR3_BASE_TREE sentenceNode) {
	int sourceElementId;
	int arcElementId;
	int targetElementId;
	pANTLR3_BASE_TREE sourceElement = (pANTLR3_BASE_TREE)sentenceNode->getChild(sentenceNode, 0);
	pANTLR3_BASE_TREE arcElement = (pANTLR3_BASE_TREE)sentenceNode->getChild(sentenceNode, 1);
	pANTLR3_BASE_TREE targetElement = (pANTLR3_BASE_TREE)sentenceNode->getChild(sentenceNode, 2);
	sourceElementId = processSentenceLevel1Node(sourceElement);
	targetElementId = processSentenceLevel1Node(targetElement);
	processSentenceLevel1Connector(arcElement, sourceElementId, targetElementId);
}

int processSentenceLevel1NodeOld(pANTLR3_BASE_TREE node) {
	IntVector * correspondingTypes;
	char * elementIdentifier;
	char * linkData;
	pANTLR3_COMMON_TOKEN token = node->getToken(node);
	if (token->type == ID_SYSTEM) {
		elementIdentifier = getText(node);
		correspondingTypes = getTypeIdsOfNodesAtSentenceLevel1();
		return elementDiscoveredDefault(
			elementIdentifier,
			strlen(elementIdentifier),
			correspondingTypes->data,
			correspondingTypes->size);
	}
	if (token->type == LINK) {
		linkData = getText(node);
		linkData[strlen(linkData) - 1] = '\0';
		linkData = linkData + 1;
		return elementDiscoveredDefaultLink(
			NULL,
			0,
			linkData,
			strlen(linkData),
			ARG_DATA
			);
	}
}

int processSentenceLevel1ConnectorOld(pANTLR3_BASE_TREE connector, int sourceElementId, int targetElementId) {
	char * typeSeparator;
	char * type;
	char * clearedIdentifier;
	int typeSeparatorIndex;
	IntVector * correspondingTypes;
	char * connectorIdentifier;
	int connectorId;
	connectorIdentifier = getText(connector);
	// Check whether element has explicit type declaration.
	typeSeparator = strchr(connectorIdentifier, ARC_IDENTIFIER_TYPE_SEPARATOR_SYMBOL);
	if (typeSeparator) {
		typeSeparatorIndex = typeSeparator - connectorIdentifier;
		type = calloc(typeSeparatorIndex + 1, sizeof(char));
		strncpy(type, connectorIdentifier, typeSeparatorIndex);
		correspondingTypes = getTypeIdsOfArcsAtSentenceLevel1(type);
		clearedIdentifier = calloc(strlen(connectorIdentifier) - typeSeparatorIndex + 1, sizeof(char));
		strncpy(clearedIdentifier, typeSeparator + 1, strlen(connectorIdentifier) - typeSeparatorIndex);
		connectorId = elementDiscoveredDefault(
			clearedIdentifier,
			strlen(clearedIdentifier),
			correspondingTypes->data,
			correspondingTypes->size);
		free(type);
		free(clearedIdentifier);
	}
	else {
		correspondingTypes = getCorrespondingTypesForScsType("sc_edge_common");
		connectorId = elementDiscoveredDefault(
			connectorIdentifier,
			strlen(connectorIdentifier),
			correspondingTypes->data,
			correspondingTypes->size);
	}
	arcDiscovered(sourceElementId, targetElementId, connectorId);
	return connectorId;
}

int processAssignmentSentenceOld(pANTLR3_BASE_TREE sentenceNode, IntVectorVector * assignments, DataVector * vertexData) {
	int result = OK;
	char * linkData;
	char * identifier;
	pANTLR3_BASE_TREE node_left = (pANTLR3_BASE_TREE)sentenceNode->getChild(sentenceNode, 0);
	pANTLR3_BASE_TREE node_right = (pANTLR3_BASE_TREE)sentenceNode->getChild(sentenceNode, 1);
	pANTLR3_COMMON_TOKEN tok_left = node_left->getToken(node_left);
	pANTLR3_COMMON_TOKEN tok_right = node_left->getToken(node_right);
	if (tok_left->type != ID_SYSTEM) {
		// Assuming logic that is used in sc-machine 
		// left_nodes should always be system identifiers.
	}
	if (tok_right->type == ID_SYSTEM) {
		// TODO:
		// Handle assignment
	}
	else {
		// TODO :
		// Currently only link can be found here
		// So parse right element as link having identifer
		identifier = getText(node_left);
		linkData = getText(node_right);
		linkData[strlen(linkData) - 1] = '\0';
		linkData = linkData + 1;
		elementDiscoveredDefaultLink(
			identifier,
			strlen(identifier),
			linkData,
			strlen(linkData),
			ARG_DATA
			);
	}
	return result;
}
*/