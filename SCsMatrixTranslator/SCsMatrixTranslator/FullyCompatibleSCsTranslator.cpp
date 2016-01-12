#include "FullyCompatibleSCsTranslator.h"

//! Set of created elements
tScAddrSet mScAddrs;
//! Map of elements description
tElementIdtfMap mElementIdtf;
//! Set of elements
tElementSet mElementSet;

//! Map to store assignments
tAssignMap mAssignments;

//! Map that contains global identifiers
tStringAddrMap msGlobalIdtfAddrs;
//! Map that contains system identifiers
tStringAddrMap mSysIdtfAddrs;
//! Map that contains local identifiers
tStringAddrMap mLocalIdtfAddrs;

long long msAutoIdtfCount = 0;

long long elementIdCount = 0;

String trimContentData(String const & path) {
	if (path.size() < 2)
		return String();

	return path.substr(2, path.size() - 3);
}

typedef enum
{
	SentenceUnknown = 0,
	SentenceLevel1 = 1,
	SentenceLevel2_7 = 2,
	SentenceAssign = 3,
	SentenceEOF = 4
} eSentenceType;


/*

	IMPLEMENTATION

*/

IntVector* convertScTypeToMatrixTypes(sc_type types) {
	IntVector* matrixTypes;
	IntVector* correspondingTypes;
	initIntVector(&matrixTypes);
	if (types & sc_type_node) {
		correspondingTypes = getCorrespondingTypesForScsType("sc_node");
		appendIntVectorToIntVector(matrixTypes, correspondingTypes);
		destroyIntVector(correspondingTypes, true);
	}
	if (types & sc_type_link) {
		correspondingTypes = getCorrespondingTypesForScsType("sc_link");
		appendIntVectorToIntVector(matrixTypes, correspondingTypes);
		destroyIntVector(correspondingTypes, true);
	}
	if (types & sc_type_edge_common) {
		correspondingTypes = getCorrespondingTypesForScsType("sc_edge_common");
		appendIntVectorToIntVector(matrixTypes, correspondingTypes);
		destroyIntVector(correspondingTypes, true);
	}
	if (types & sc_type_arc_common) {
		correspondingTypes = getCorrespondingTypesForScsType("sc_arc_common");
		appendIntVectorToIntVector(matrixTypes, correspondingTypes);
		destroyIntVector(correspondingTypes, true);
	}
	if (types & sc_type_arc_access) {
		correspondingTypes = getCorrespondingTypesForScsType("sc_arc_access");
		appendIntVectorToIntVector(matrixTypes, correspondingTypes);
		destroyIntVector(correspondingTypes, true);
	}
	if (types & sc_type_const) {
		correspondingTypes = getCorrespondingTypesForScsType("sc_const");
		appendIntVectorToIntVector(matrixTypes, correspondingTypes);
		destroyIntVector(correspondingTypes, true);
	}
	if (types & sc_type_var) {
		correspondingTypes = getCorrespondingTypesForScsType("sc_var");
		appendIntVectorToIntVector(matrixTypes, correspondingTypes);
		destroyIntVector(correspondingTypes, true);
	}
	if (types & sc_type_arc_pos) {
		correspondingTypes = getCorrespondingTypesForScsType("sc_arc_pos");
		appendIntVectorToIntVector(matrixTypes, correspondingTypes);
		destroyIntVector(correspondingTypes, true);
	}
	if (types & sc_type_arc_neg) {
		correspondingTypes = getCorrespondingTypesForScsType("sc_arc_neg");
		appendIntVectorToIntVector(matrixTypes, correspondingTypes);
		destroyIntVector(correspondingTypes, true);
	}
	if (types & sc_type_arc_fuz) {
		correspondingTypes = getCorrespondingTypesForScsType("sc_arc_fuz");
		appendIntVectorToIntVector(matrixTypes, correspondingTypes);
		destroyIntVector(correspondingTypes, true);
	}
	if (types & sc_type_arc_temp) {
		correspondingTypes = getCorrespondingTypesForScsType("sc_arc_temp");
		appendIntVectorToIntVector(matrixTypes, correspondingTypes);
		destroyIntVector(correspondingTypes, true);
	}
	if (types & sc_type_arc_perm) {
		correspondingTypes = getCorrespondingTypesForScsType("sc_arc_perm");
		appendIntVectorToIntVector(matrixTypes, correspondingTypes);
		destroyIntVector(correspondingTypes, true);
	}
	if (types & sc_type_node_tuple) {
		correspondingTypes = getCorrespondingTypesForScsType("sc_node_tuple");
		appendIntVectorToIntVector(matrixTypes, correspondingTypes);
		destroyIntVector(correspondingTypes, true);
	}
	if (types & sc_type_node_struct) {
		correspondingTypes = getCorrespondingTypesForScsType("sc_node_struct");
		appendIntVectorToIntVector(matrixTypes, correspondingTypes);
		destroyIntVector(correspondingTypes, true);
	}
	if (types & sc_type_node_role) {
		correspondingTypes = getCorrespondingTypesForScsType("sc_node_role");
		appendIntVectorToIntVector(matrixTypes, correspondingTypes);
		destroyIntVector(correspondingTypes, true);
	}
	if (types & sc_type_node_norole) {
		correspondingTypes = getCorrespondingTypesForScsType("sc_node_norole");
		appendIntVectorToIntVector(matrixTypes, correspondingTypes);
		destroyIntVector(correspondingTypes, true);
	}
	if (types & sc_type_node_class) {
		correspondingTypes = getCorrespondingTypesForScsType("sc_node_class");
		appendIntVectorToIntVector(matrixTypes, correspondingTypes);
		destroyIntVector(correspondingTypes, true);
	}
	if (types & sc_type_node_abstract) {
		correspondingTypes = getCorrespondingTypesForScsType("sc_node_abstract");
		appendIntVectorToIntVector(matrixTypes, correspondingTypes);
		destroyIntVector(correspondingTypes, true);
	}
	if (types & sc_type_node_material) {
		correspondingTypes = getCorrespondingTypesForScsType("sc_node_material");
		appendIntVectorToIntVector(matrixTypes, correspondingTypes);
		destroyIntVector(correspondingTypes, true);
	}
	return matrixTypes;
}

eSentenceType determineSentenceType(pANTLR3_BASE_TREE node)
{
	pANTLR3_COMMON_TOKEN tok = node->getToken(node);
	assert(tok);

	if (tok->type == SEP_SIMPLE)
		return SentenceLevel1;

	if (tok->type == CONNECTORS)
		return SentenceLevel2_7;

	if (tok->type == SEP_ASSIGN)
		return SentenceAssign;

	if (tok->type == EOF)
		return SentenceEOF;

	return SentenceUnknown;
}

void destroy() {
	tElementSet::iterator it, itEnd = mElementSet.end();
	for (it = mElementSet.begin(); it != itEnd; ++it)
		delete *it;
	mElementSet.clear();
	mScAddrs.clear();
	mElementIdtf.clear();
	mAssignments.clear();
	msGlobalIdtfAddrs.clear();
	mSysIdtfAddrs.clear();
	mLocalIdtfAddrs.clear();
	msAutoIdtfCount = 0;
	elementIdCount = 0;
}

 int translateWithFullCompatibility() {
	int result = OK;
	unsigned int index;
	pANTLR3_BASE_TREE sentence;
	// Parse file.
	scsParser_syntax_return scsFile = parseFile();
	pANTLR3_BASE_TREE tree = scsFile.tree;
	int nodesCount = tree->getChildCount(tree);
	for (int i = 0; i < nodesCount; ++i)
	{
		pANTLR3_BASE_TREE sentenceNode = (pANTLR3_BASE_TREE)tree->getChild(tree, i);
		eSentenceType sentenceType = determineSentenceType(sentenceNode);
		switch (sentenceType)
		{
		case SentenceLevel1:
			processSentenceLevel1(sentenceNode);
			break;
		case SentenceLevel2_7:
			processSentenceLevel2_7(sentenceNode);
			break;
		case SentenceAssign:
			processSentenceAssign(sentenceNode);
			break;
		case SentenceEOF:
			break;
		default:
			printf("[WARNING] Undefined type of sentence. %d", sentenceType);
			break;
		}
	}

	// now generate sc-text in memory
	tElementSet::iterator it, itEnd = mElementSet.end();
	for (it = mElementSet.begin(); it != itEnd; ++it)
	{
		sElement *el = *it;
		if (el->type == sc_type_arc_pos_const_perm)
		{
			sc_type type = _getTypeBySetIdtf(el->arc_src->idtf);
			if (type != 0)
			{
				el->ignore = true;
				sc_type newType = el->arc_trg->type | type;
				// TODO check conflicts in sc-type
				// TODO check conflicts in sc-type
				if ((type & sc_type_constancy_mask) != 0)
					newType = (type & sc_type_constancy_mask) | (newType & ~sc_type_constancy_mask);
				el->arc_trg->type = newType;
			}
		}
		// arcs already have types
		if (!(el->type & sc_type_arc_mask))
			determineElementType(el);
	}

	tElementSet arcs;
	for (it = mElementSet.begin(); it != itEnd; ++it)
	{
		sElement *el = *it;

		// skip processed triples
		if (el->ignore) continue;

		sc_addr addr = resolveScAddr(el);

		if (SC_ADDR_IS_EMPTY(addr))
		{
			arcs.insert(el);
		}
	}

	bool created = true;
	while (!arcs.empty() && created)
	{
		created = false;

		tElementSet createdSet;
		itEnd = arcs.end();
		for (it = arcs.begin(); it != itEnd; ++it)
		{
			sElement *arc_el = *it;
			sc_addr addr = resolveScAddr(arc_el);

			if (SC_ADDR_IS_EMPTY(addr)) continue;

			createdSet.insert(arc_el);
		}

		created = !createdSet.empty();
		itEnd = createdSet.end();
		for (it = createdSet.begin(); it != itEnd; ++it)
			arcs.erase(*it);
	}
	destroy();
	return result;
}

sElement* _createElement(const String &idtf, sc_type types) {
	String newIdtf = idtf;
	if (!idtf.empty())
	{
		if (idtf == "...")
		{
			StringStream ss;
			ss << "..." << msAutoIdtfCount++ << "...auto...";
			newIdtf = ss.str();
		}
		else {
			tElementIdtfMap::iterator it = mElementIdtf.find(idtf);
			if (it != mElementIdtf.end())
			{
				it->second->type = it->second->type | types;
				return it->second;
			}
		}
	}

	sElement *el = new sElement();

	el->idtf = newIdtf;
	el->type = types;
	if (!newIdtf.empty())
		mElementIdtf[newIdtf] = el;
	mElementSet.insert(el);

	return el;
}

sElement* _addNode(const String & idtf, sc_type additionalTypes) {
	return _createElement(idtf, sc_type_node | additionalTypes);
}

sElement* _addEdge(sElement* source, sElement* target, sc_type additionalTypes, bool is_reversed, const String & idtf) {
	sElement *el = _createElement(idtf, additionalTypes);
	if (is_reversed)
	{
		el->arc_src = target;
		el->arc_trg = source;
	}
	else
	{
		el->arc_src = source;
		el->arc_trg = target;
	}
	return el;
}

sElement* _addLink(const String &idtf, const String & data) {
	sElement *el = _createElement(idtf, sc_type_link);
	el->link_is_file = false;
	el->link_data = data;
	return el;
}

sElement* _addLinkFile(const String & idtf, const String & filePath) {
	sElement *el = _createElement(idtf, sc_type_link);
	el->link_is_file = true;
	el->file_path = filePath;
	return el;
}

sElement* _addLinkString(const String & idtf, const String & str) {
	sElement *el = _createElement(idtf, sc_type_link);
	el->link_is_file = false;
	el->link_data = str;
	return el;
}

#define GENERATE_ATTRS(idx) \
tElementSet::iterator itSubj, itSubjEnd = subjects.end(); \
for (itSubj = subjects.begin(); itSubj != itSubjEnd; ++itSubj) \
{ \
    sElement *el_subj = *itSubj; \
    sElement *el_arc = _addEdge(el_obj, el_subj, type_connector, _isConnectorReversed(connector), ""); \
    tElementSet::iterator itAttrs, itAttrsEnd = var_attrs.end(); \
    for (itAttrs = var_attrs.begin(); itAttrs != itAttrsEnd; ++itAttrs) \
        _addEdge(*itAttrs, el_arc, sc_type_arc_access | sc_type_var | sc_type_arc_pos | sc_type_arc_perm, false, ""); \
    itAttrsEnd = const_attrs.end(); \
    for (itAttrs = const_attrs.begin(); itAttrs != itAttrsEnd; ++itAttrs) \
        _addEdge(*itAttrs, el_arc, sc_type_arc_pos_const_perm, false, ""); \
    if (generate_order) \
	    { \
        StringStream ss; \
        ss << "rrel_" << (idx++); \
        _addEdge(_addNode(ss.str(), sc_type_node_role), el_arc, sc_type_arc_pos_const_perm, false, ""); \
	    } \
}

sc_addr createScAddr(sElement *el)
{
	sc_addr addr;
	SC_ADDR_MAKE_EMPTY(addr);
	IntVector* matrixTypes = NULL;
	if (el->type & sc_type_node) {
		matrixTypes = convertScTypeToMatrixTypes(el->type);
		int elementId = elementDiscoveredDefault(
			el->idtf.c_str(),
			el->idtf.size(),
			matrixTypes->data,
			matrixTypes->size);
		addr.matrixId = elementId;
	}
	else if (el->type & sc_type_link)
	{

		// setup link content
		if (el->link_is_file)
		{
			String file_path;
			if (_getAbsFilePath(el->file_path))
			{
				int elementId = elementDiscoveredDefaultLink(
					el->idtf.c_str(),
					el->idtf.size(),
					el->file_path.c_str(),
					el->file_path.size(),
					DATA_TYPE_LINK_FILE
					);
				addr.matrixId = elementId;

				/*
				sc_stream *stream = sc_stream_file_new(file_path.c_str(), SC_STREAM_FLAG_READ);
				if (stream)
				{
				sc_memory_set_link_content(mContext, addr, stream);
				sc_stream_free(stream);
				}
				else
				{
				printf("Can't open file %s", el->file_path);
				}
				*/
			}
			else
			{
				printf("Unsupported link type : %s", el->file_path);
			}

		}
		else
		{
			int elementId = elementDiscoveredDefaultLink(
				el->idtf.c_str(),
				el->idtf.size(),
				el->link_data.c_str(),
				el->link_data.size(),
				DATA_TYPE_LINK_STRING
				);
			addr.matrixId = elementId;
			/*sc_stream *stream = sc_stream_memory_new(el->link_data.data.data(), (UINT32)el->link_data.data.size(), SC_STREAM_FLAG_READ, SC_FALSE);
			sc_memory_set_link_content(mContext, addr, stream);
			sc_stream_free(stream);*/
		}

		// TODO skipping this part too
		// generate format information
		/*if (mParams.autoFormatInfo)
		{
		if (el->link_is_file)
		{
		size_t n = el->file_path.find_last_of(".");
		if (n != String::npos)
		generateFormatInfo(addr, el->file_path.substr(n + 1));
		}
		}*/
	}
	else
	{
		assert(el->arc_src && el->arc_trg);
		if (SC_ADDR_IS_EMPTY(el->arc_src->addr) || SC_ADDR_IS_EMPTY(el->arc_trg->addr))
			return addr;
		matrixTypes = convertScTypeToMatrixTypes(el->type);
		int elementId = elementDiscoveredDefault(
			el->idtf.c_str(),
			el->idtf.size(),
			matrixTypes->data,
			matrixTypes->size);
		addr.matrixId = elementId;
		arcDiscovered(el->arc_src->addr.matrixId, el->arc_trg->addr.matrixId, addr.matrixId);
		//addr = sc_memory_arc_new(mContext, el->type, el->arc_src->addr, el->arc_trg->addr);
	}
	if (matrixTypes) {
		destroyIntVector(matrixTypes, true);
	}
	el->addr = addr;
	return addr;
}

void determineElementType(sElement *el)
{
	//assert(el);
	sc_type oldType = el->type;
	sc_type newType = oldType;

	if ((newType & sc_type_element_mask) == 0)
		newType = newType | sc_type_node;

	newType = (newType & (~sc_type_constancy_mask));
	sc_type const_type = _isIdentifierVar(el->idtf) ? sc_type_var : sc_type_const;
	newType = newType | (const_type);

	el->type = newType;
}

void processAttrsIdtfList(bool ignore_first, pANTLR3_BASE_TREE node, sElement * el_obj, const String &connector, bool generate_order)
{
	sc_type type_connector = _getTypeByConnector(connector);

	tElementSet var_attrs, const_attrs;
	tElementSet subjects;
	UINT32 n = node->getChildCount(node);
	UINT32 idx = 1;
	for (UINT32 i = ignore_first ? 1 : 0; i < n; ++i)
	{
		pANTLR3_BASE_TREE child = (pANTLR3_BASE_TREE)node->getChild(node, i);
		pANTLR3_COMMON_TOKEN tok = child->getToken(child);
		//assert(tok);
		// skip internal sentences
		if (tok->type == SEP_LINT) continue;

		if (tok->type == SEP_ATTR_CONST || tok->type == SEP_ATTR_VAR)
		{
			if (!subjects.empty())
			{
				GENERATE_ATTRS(idx);
					subjects.clear();
				const_attrs.clear();
				var_attrs.clear();
			}
			pANTLR3_BASE_TREE nd = (pANTLR3_BASE_TREE)child->getChild(child, 0);
			sElement *el = _addNode(GET_NODE_TEXT(nd), NULL);
			if (tok->type == SEP_ATTR_CONST)
				const_attrs.insert(el);
			else
				var_attrs.insert(el);
		}
		else
		{
			subjects.insert(parseElementTree(child, NULL));
		}
	}
	GENERATE_ATTRS(idx);
}

void processSentenceLevel1(pANTLR3_BASE_TREE node)
{
	unsigned int nodesCount = node->getChildCount(node);
	assert(nodesCount == 3);

	pANTLR3_BASE_TREE node_obj = (pANTLR3_BASE_TREE)node->getChild(node, 0);
	pANTLR3_BASE_TREE node_pred = (pANTLR3_BASE_TREE)node->getChild(node, 1);
	pANTLR3_BASE_TREE node_subj = (pANTLR3_BASE_TREE)node->getChild(node, 2);

	pANTLR3_COMMON_TOKEN tok_pred = node_pred->getToken(node_pred);

	if (tok_pred->type != ID_SYSTEM) {
		printf("Invalid predicate in simple sentence");
	}
	sElement * el_obj = parseElementTree(node_obj, NULL);
	sElement * el_subj = parseElementTree(node_subj, NULL);
	// determine arc type
	sc_type type = sc_type_edge_common;
	String pred = GET_NODE_TEXT(node_pred);
	size_t n = pred.find_first_of("#");
	if (n != pred.npos)
		type = _getArcPreffixType(pred.substr(0, n));
	_addEdge(el_obj, el_subj, type, false, pred);
}

void processSentenceLevel2_7(pANTLR3_BASE_TREE node) {
	String connector = GET_NODE_TEXT(node);
	// determine object
	pANTLR3_BASE_TREE node_obj = (pANTLR3_BASE_TREE)node->getChild(node, 0);
	sElement * el_obj = _createElement(GET_NODE_TEXT(node_obj), 0);
	// no we need to parse attributes and predicates
	processAttrsIdtfList(true, node, el_obj, connector, false);
}

void processSentenceAssign(pANTLR3_BASE_TREE node)
{
	unsigned int nodesCount = node->getChildCount(node);
	assert(nodesCount == 2);

	pANTLR3_BASE_TREE node_left = (pANTLR3_BASE_TREE)node->getChild(node, 0);
	pANTLR3_BASE_TREE node_right = (pANTLR3_BASE_TREE)node->getChild(node, 1);

	pANTLR3_COMMON_TOKEN tok_left = node_left->getToken(node_left);
	pANTLR3_COMMON_TOKEN tok_right = node_left->getToken(node_right);

	assert(tok_left && tok_right);

	if (tok_left->type != ID_SYSTEM) {
		printf("Unsupported type of tokens at the left side of assignment sentence : %s", tok_left->getLine(tok_left));
	}

	if (tok_right->type == ID_SYSTEM)
	{
		// TODO : handle assignments
		//mAssignments[GET_NODE_TEXT(node_left)] = GET_NODE_TEXT(node_right);
	}
	else
	{
		String left_idtf = (GET_NODE_TEXT(node_left));
		parseElementTree(node_right, &left_idtf);
	}
}

typedef enum
{
	IdtfSystem = 0,
	IdtfLocal = 1,
	IdtfGlobal = 2

} eIdtfVisibility;

eIdtfVisibility _getIdentifierVisibility(const String &idtf) {
	if (startsWith(idtf, "..", false))
		return IdtfLocal;
	else if (startsWith(idtf, ".", false))
		return IdtfGlobal;

	return IdtfSystem;
}

sc_addr resolveScAddr(sElement *el)
{
	//assert(SC_ADDR_IS_EMPTY(el->addr));

	sc_addr addr;
	SC_ADDR_MAKE_EMPTY(addr);
	if (!el->idtf.empty())
	{
		// try to find in system identifiers
		tStringAddrMap::iterator it = mSysIdtfAddrs.find(el->idtf);
		if (it != mSysIdtfAddrs.end())
		{
			addr = it->second;
		}
		else
		{
			// try to find in global identifiers
			it = msGlobalIdtfAddrs.find(el->idtf);
			if (it != msGlobalIdtfAddrs.end())
				addr = it->second;
			else
			{
				// try to find in local identifiers
				it = mLocalIdtfAddrs.find(el->idtf);
				if (it != mLocalIdtfAddrs.end())
					addr = it->second;
				else
				{
					// As we cannot ask SC memory helper here just skip this.
					// In case if address cannot be resolverd from caches then create it


					// resolve system identifier
					/*sc_result res = sc_helper_find_element_by_system_identifier(mContext, el->idtf.c_str(), (UINT3232)el->idtf.size(), &addr);
					if (res == SC_RESULT_OK)
						mSysIdtfAddrs[el->idtf] = addr;
						*/
				}
			}
		}
	}

	if (SC_ADDR_IS_NOT_EMPTY(addr))
	{
		// TODO Not sure about how this should be handled.
		/*
		sc_type t = 0;
		if (sc_memory_get_element_type(mContext, addr, &t) == SC_RESULT_OK)
			sc_memory_change_element_subtype(mContext, addr, ~sc_type_element_mask & (el->type | t));
		*/
		el->addr = addr;
		return addr;
	}

	// generate addr
	addr = createScAddr(el);


	// store in addrs map
	if (!el->idtf.empty())
	{
		switch (_getIdentifierVisibility(el->idtf))
		{
		case IdtfSystem:
			//sc_helper_set_system_identifier(mContext, addr, el->idtf.c_str(), (UINT32)el->idtf.size());
			mSysIdtfAddrs[el->idtf] = addr;
			break;
		case IdtfLocal:
			mLocalIdtfAddrs[el->idtf] = addr;
			break;
		case IdtfGlobal:
			msGlobalIdtfAddrs[el->idtf] = addr;
			break;
		}

	}

	return addr;
}


#define CHECK_LINK_DATA(__d) if (__d.empty()) { printf("[WARNING] Empty link content");}

sElement* parseElementTree(pANTLR3_BASE_TREE tree, const String *assignIdtf) {
	pANTLR3_COMMON_TOKEN tok = tree->getToken(tree);
	assert(tok);

	sElement *res = 0;
	if (tok->type == ID_SYSTEM)
		res = _addNode(GET_NODE_TEXT(tree), NULL);

	if (tok->type == SEP_LPAR)
	{
		assert(tree->getChildCount(tree) >= 3);
		pANTLR3_BASE_TREE node_obj = (pANTLR3_BASE_TREE)tree->getChild(tree, 0);
		pANTLR3_BASE_TREE node_pred = (pANTLR3_BASE_TREE)tree->getChild(tree, 1);
		pANTLR3_BASE_TREE node_subj = (pANTLR3_BASE_TREE)tree->getChild(tree, 2);

		String pred = GET_NODE_TEXT(node_pred);
		sElement *src = parseElementTree(node_obj, NULL);
		sElement *trg = parseElementTree(node_subj, NULL);

		assert(src && trg);

		res = _addEdge(src, trg, _getTypeByConnector(pred), _isConnectorReversed(pred), "");
	}

	if (tok->type == LINK)
	{
		String data = GET_NODE_TEXT(tree);
		CHECK_LINK_DATA(data);
		res = _addLinkFile(assignIdtf ? *assignIdtf : "", data.substr(1, data.size() - 2));
	}

	if (tok->type == CONTENT)
	{
		res = _addNode(assignIdtf ? *assignIdtf : "", sc_type_node_struct);

		String content = GET_NODE_TEXT(tree);
		content = content.substr(1, content.size() - 2);

		if (startsWith(content, "*", false) && endsWith(content, "*", false))
		{
			// parse contour data
			String data = content.substr(1, content.size() - 2);
			//bool autoFormatInfo = mParams.autoFormatInfo;
			String fileName = "";//mParams.fileName;

			// check if link to file
			if (startsWith(data, "^\"", false))
			{
				// File name should be passed here
				bool result = false;
				if (_getAbsFilePath(trimContentData(data)))
				{
					// TODO absolute file path handling removed here
					/*String name;
					fileName = name;
					std::ifstream ifs(name.c_str());
					if (ifs.is_open())
					{
						data = String((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
						ifs.close();
						result = true;
					}
					else {
						printf("[WARNING] Can't open file %s", name.c_str());
					}*/
				}
			}

			// parse data
			if (!data.empty())
			{
				//TODO hardcore parsing of linkscontaining other SCs happens here - proceed with this  stuff later.
				/*
				SCsTranslator translator(mContext);
				translator.mParams.autoFormatInfo = autoFormatInfo;
				translator.mParams.fileName = fileName;
				translator.processString(data);

				// now we need to get all created elements and create arcs to them
				tElementSet::iterator it, itEnd = translator.mElementSet.end();
				for (it = translator.mElementSet.begin(); it != itEnd; ++it)
				{
					if ((*it)->ignore) continue;

					sElement *el = new sElement();
					el->ignore = true;
					el->addr = (*it)->addr;

					mElementSet.insert(el);
					_addEdge(res, el, sc_type_arc_pos_const_perm, false, "");
				}

				// merge identifiers map
				mSysIdtfAddrs.insert(translator.mSysIdtfAddrs.begin(), translator.mSysIdtfAddrs.end());
				mLocalIdtfAddrs.insert(translator.mLocalIdtfAddrs.begin(), translator.mLocalIdtfAddrs.end());
				*/
			}
		}
		else
		{
			if (startsWith(content, "^\"", false))
			{
				String data = trimContentData(content);
				CHECK_LINK_DATA(data);
				sBuffer buffer;

				// TODO removed buffer from here
				if (parseContentBinaryData(data, buffer))
				{
					res = _addLink("", data);
				}
				else
				{
					res = _addLinkFile("", data);
				}
			}
			else
			{
				content = replaceAll(content, "\\[", "[");
				content = replaceAll(content, "\\]", "]");
				CHECK_LINK_DATA(content);
				res = _addLinkString("", content);
			}
		}
	}
	if (tok->type == SEP_LTUPLE || tok->type == SEP_LSET)
	{
		res = _addNode("", sc_type_node_tuple);
		processAttrsIdtfList(false, tree, res, "->", tok->type == SEP_LTUPLE);
	}
	// now process internal sentences
	UINT32 n = tree->getChildCount(tree);
	for (UINT32 i = 0; i < n; ++i)	{
		pANTLR3_BASE_TREE internal = (pANTLR3_BASE_TREE)tree->getChild(tree, i);
		pANTLR3_COMMON_TOKEN tok = internal->getToken(internal);

		if (tok->type != SEP_LINT) continue;

		// process internal sentences
		UINT32 nc = internal->getChildCount(internal);
		for (UINT32 j = 0; j < nc; ++j)
		{
			pANTLR3_BASE_TREE node_pred = (pANTLR3_BASE_TREE)internal->getChild(internal, j);
			String connector = GET_NODE_TEXT(node_pred);
			processAttrsIdtfList(false, node_pred, res, connector, false);
		}
	}
	return res;
}

sc_type _getArcPreffixType(const String &preffix) {
	// do not use map, to prevent it initialization on class creation
	if (preffix == "sc_arc_common")
		return sc_type_arc_common;
	if (preffix == "sc_arc_main")
		return sc_type_arc_pos_const_perm;
	if (preffix == "sc_arc_access")
		return sc_type_arc_access;
	return sc_type_edge_common;
}

sc_type _getTypeBySetIdtf(const String &setIdtf) {
	if (setIdtf == "sc_edge_const")
		return sc_type_edge_common | sc_type_const;
	if (setIdtf == "sc_edge_var")
		return sc_type_edge_common | sc_type_var;
	if (setIdtf == "sc_arc_common_const")
		return sc_type_arc_common | sc_type_const;
	if (setIdtf == "sc_arc_common_var")
		return sc_type_arc_common | sc_type_var;

	if (setIdtf == "sc_arc_access_var_pos_perm")
		return sc_type_arc_access | sc_type_var | sc_type_arc_pos | sc_type_arc_perm;
	if (setIdtf == "sc_arc_access_const_neg_perm")
		return sc_type_arc_access | sc_type_const | sc_type_arc_neg | sc_type_arc_perm;
	if (setIdtf == "sc_arc_access_var_neg_perm")
		return sc_type_arc_access | sc_type_var | sc_type_arc_neg | sc_type_arc_perm;
	if (setIdtf == "sc_arc_access_const_fuz_perm")
		return sc_type_arc_access | sc_type_const | sc_type_arc_fuz | sc_type_arc_perm;
	if (setIdtf == "sc_arc_access_var_fuz_perm")
		return sc_type_arc_access | sc_type_var | sc_type_arc_fuz | sc_type_arc_perm;

	if (setIdtf == "sc_arc_access_const_pos_temp")
		return sc_type_arc_access | sc_type_const | sc_type_arc_pos | sc_type_arc_temp;
	if (setIdtf == "sc_arc_access_var_pos_temp")
		return sc_type_arc_access | sc_type_var | sc_type_arc_pos | sc_type_arc_temp;
	if (setIdtf == "sc_arc_access_const_neg_temp")
		return sc_type_arc_access | sc_type_const | sc_type_arc_neg | sc_type_arc_temp;
	if (setIdtf == "sc_arc_access_var_neg_temp")
		return sc_type_arc_access | sc_type_var | sc_type_arc_neg | sc_type_arc_temp;
	if (setIdtf == "sc_arc_access_const_fuz_temp")
		return sc_type_arc_access | sc_type_const | sc_type_arc_fuz | sc_type_arc_temp;
	if (setIdtf == "sc_arc_access_var_fuz_temp")
		return sc_type_arc_access | sc_type_var | sc_type_arc_fuz | sc_type_arc_temp;

	if (setIdtf == "sc_const")
		return sc_type_const;
	if (setIdtf == "sc_var")
		return sc_type_var;
	if (setIdtf == "sc_node_not_binary_tuple")
		return sc_type_node_tuple;
	if (setIdtf == "sc_node_struct")
		return sc_type_node_struct;
	if (setIdtf == "sc_node_role_relation")
		return sc_type_node_role;
	if (setIdtf == "sc_node_norole_relation")
		return sc_type_node_norole;
	if (setIdtf == "sc_node_not_relation")
		return sc_type_node_class;
	if (setIdtf == "sc_node_abstract")
		return sc_type_node_abstract;
	if (setIdtf == "sc_node_material")
		return sc_type_node_material;
	return 0;
}

sc_type _getTypeByConnector(const String &connector) {
	if (connector == ">" || connector == "<")
		return sc_type_arc_common;
	if (connector == "->" || connector == "<-")
		return sc_type_arc_pos_const_perm;
	if (connector == "<>")
		return sc_type_edge_common;
	if (connector == "..>" || connector == "<..")
		return sc_type_arc_access;
	if (connector == "<=>")
		return sc_type_edge_common | sc_type_const;
	if (connector == "_<=>")
		return sc_type_edge_common | sc_type_var;

	if (connector == "=>" || connector == "<=")
		return sc_type_arc_common | sc_type_const;
	if (connector == "_=>" || connector == "_<=")
		return sc_type_arc_common | sc_type_var;
	if (connector == "_->" || connector == "_<-")
		return sc_type_arc_access | sc_type_arc_pos | sc_type_var | sc_type_arc_perm;
	if (connector == "-|>" || connector == "<|-")
		return sc_type_arc_access | sc_type_arc_neg | sc_type_const | sc_type_arc_perm;
	if (connector == "_-|>" || connector == "_<|-")
		return sc_type_arc_access | sc_type_arc_neg | sc_type_var | sc_type_arc_perm;
	if (connector == "-/>" || connector == "</-")
		return sc_type_arc_access | sc_type_arc_fuz | sc_type_const | sc_type_arc_perm;
	if (connector == "_-/>" || connector == "_</-")
		return sc_type_arc_access | sc_type_arc_fuz | sc_type_var | sc_type_arc_perm;
	if (connector == "~>" || connector == "<~")
		return sc_type_arc_access | sc_type_arc_pos | sc_type_const | sc_type_arc_temp;
	if (connector == "_~>" || connector == "_<~")
		return sc_type_arc_access | sc_type_arc_pos | sc_type_var | sc_type_arc_temp;
	if (connector == "~|>" || connector == "<|~")
		return sc_type_arc_access | sc_type_arc_neg | sc_type_const | sc_type_arc_temp;
	if (connector == "_~|>" || connector == "_<|~")
		return sc_type_arc_access | sc_type_arc_neg | sc_type_var | sc_type_arc_temp;
	if (connector == "~/>" || connector == "</~")
		return sc_type_arc_access | sc_type_arc_fuz | sc_type_const | sc_type_arc_temp;
	if (connector == "_~/>" || connector == "_</~")
		return sc_type_arc_access | sc_type_arc_fuz | sc_type_var | sc_type_arc_temp;
	return 0;
}

bool _isConnectorReversed(const String &connector)
{
	if (connector == "<" || connector == "<-" || connector == "<.." || connector == "<=" ||
		connector == "_<=" || connector == "_<-" || connector == "<|-" || connector == "_<|-" ||
		connector == "</-" || connector == "_</-" || connector == "<~" || connector == "_<~" ||
		connector == "<|~" || connector == "_<|~" || connector == "</~" || connector == "_</~")
		return true;

	return false;
}

bool _getAbsFilePath(const String &url) {
	// split file name
	//String path, name;
	//splitFilename(mParams.fileName, name, path);
	String file_preffix = "file://";
	if (startsWith(url, file_preffix, true)) {
		/*String file_path = url.substr(file_preffix.size());
		if (!startsWith(file_path, "/", false))
		{
			boost::filesystem::path dir(path);
			boost::filesystem::path file(file_path);
			boost::filesystem::path full_path = dir / file;
			abs_path = full_path.string();
		}
		*/
		return true;
	}
	return false;
}

bool _isIdentifierVar(const String & idtf)
{
	// remove visibility points
	String s = idtf;
	int i = 0;
	while ((i < 2) && startsWith(s, ".", false))
	{
		s = s.substr(1);
		++i;
	}

	return startsWith(s, "_", false);
}

bool parseContentBinaryData(String const & data, sBuffer & outBuffer) {
	if (startsWith(data, "float", true))
	{
		float value = (float)::atof(data.substr(6).c_str());
		outBuffer.set((char*)&value, sizeof(value));
		return true;
	}

	if (startsWith(data, "int32", true))
	{
		UINT32 value = (UINT32)::atoi(data.substr(6).c_str());
		outBuffer.set((char*)&value, sizeof(value));
		return true;
	}

	if (startsWith(data, "int16", true))
	{
		UINT16 value = (UINT16)::atoi(data.substr(6).c_str());
		outBuffer.set((char*)&value, sizeof(value));
		return true;
	}

	if (startsWith(data, "int8", true))
	{
		UINT8 value = (UINT8)::atoi(data.substr(6).c_str());
		outBuffer.set((char*)&value, sizeof(value));
		return true;
	}

	return false;
}