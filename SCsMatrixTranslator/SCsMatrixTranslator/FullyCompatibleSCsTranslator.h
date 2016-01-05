#ifndef FullyCompatibleSCsTranslator_h
#define FullyCompatibleSCsTranslator_h

#include "AntlrParserUtil.h"
#include "utils.h"
#include "TypeUtil.h"
#include "types.h"
#include <string>
#include <vector>
#include <iostream>
#include <assert.h>
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

	int translateWithFullCompatibility();

#define GET_NODE_TEXT(node) String((const char*)node->getText(node)->chars)	

	struct sc_addr
	{
		int matrixId;
	};

	struct sc_addr_compare
	{
		bool operator() (const sc_addr & addr1, const sc_addr & addr2) const
		{
			if (addr1.matrixId < addr2.matrixId) return true;
			return false;
		}
	};

	struct sBuffer
	{
		std::vector<char> data;

		sBuffer()
		{
		}

		sBuffer(char const * in_data, UINT32 in_len)
		{
			set(in_data, in_len);
		}

		void set(char const * in_data, UINT32 in_len)
		{
			data.assign(in_data, in_data + in_len);
		}


	};

#define SC_ADDR_MAKE_EMPTY(addr) { addr.matrixId = -1; }

#define SC_ADDR_IS_EMPTY(addr) (addr.matrixId == -1)

#define SC_ADDR_IS_NOT_EMPTY(addr) (!SC_ADDR_IS_EMPTY(addr))

	extern long long elementIdCount;

	struct sElement
	{
		long id;
		IntVector* type;
		sc_addr addr;
		String idtf;

		// arc info
		sElement *arc_src;
		sElement *arc_trg;

		// link info
		bool link_is_file;
		String file_path;
		String link_data;
		//sBuffer link_data;

		bool ignore;

		sElement()
			: type(0)
			, id(elementIdCount++)
			, arc_src(0)
			, arc_trg(0)
			, link_is_file(false)
			, ignore(false)
		{
			SC_ADDR_MAKE_EMPTY(addr);
		}

		sElement* clone()
		{
			sElement *cl = new sElement();

			cl->type = type;
			cl->addr = addr;
			cl->idtf = idtf;
			cl->arc_src = arc_src;
			cl->arc_trg = arc_trg;
			cl->link_is_file = link_is_file;
			cl->link_data = link_data;
			cl->ignore = ignore;
			cl->id = elementIdCount++;
			return cl;
		}
	};

	struct sc_element_compare
	{
		bool operator() (const sElement*  element1, const sElement*  element2) const
		{
			if (element1->id < element2->id) return true;
			return false;
		}
	};

	typedef std::stringstream StringStream;
	typedef std::string String;
	typedef std::set<sc_addr, sc_addr_compare> tScAddrSet;
	typedef std::map<String, sElement*> tElementIdtfMap;
	typedef std::set<sElement*, sc_element_compare> tElementSet;
	typedef std::map<String, String> tAssignMap;
	typedef std::map<String, IntVector*> tScTypesMap;
	typedef std::map<String, sc_addr> tStringAddrMap;

	//! Set of created elements
	extern tScAddrSet mScAddrs;
	//! Map of elements description
	extern tElementIdtfMap mElementIdtf;
	//! Set of elements
	extern tElementSet mElementSet;

	//! Map to store assignments
	extern tAssignMap mAssignments;

	//! Map that contains global identifiers
	extern tStringAddrMap msGlobalIdtfAddrs;
	//! Map that contains system identifiers
	extern tStringAddrMap mSysIdtfAddrs;
	//! Map that contains local identifiers
	extern tStringAddrMap mLocalIdtfAddrs;
	/*

	HEADERS

	*/

	void destroy();

	bool _isConnectorReversed(const String & connector);

	sElement* _createElement(const String & idtf, IntVector* types);

	sElement* _addNode(const String & idtf, IntVector* additionalTypes);

	sElement* _addEdge(sElement* source, sElement* target, IntVector* additionalTypes, bool is_reversed, const String & idtf);

	sElement* _addLink(const String &idtf, const String & data);

	sElement* _addLinkFile(const String & idtf, const String & filePath);

	sElement* _addLinkString(const String & idtf, const String & str);

	IntVector* _getArcPreffixType(const String &preffix);

	IntVector* _getTypeBySetIdtf(const String &setIdtf);

	IntVector* _getTypeByConnector(const String &connector);

	sElement* parseElementTree(pANTLR3_BASE_TREE tree, const String *assignIdtf);

	sc_addr resolveScAddr(sElement *el);

	bool _isIdentifierVar(const String & idtf);

	bool _isConnectorReversed(const String &connector);

	bool _getAbsFilePath(const String &url);

	void determineElementType(sElement *el);

	sc_addr createScAddr(sElement *el);

	void processAttrsIdtfList(bool ignore_first, pANTLR3_BASE_TREE node, sElement * el_obj, const String &connector, bool generate_order);

	void processSentenceLevel1(pANTLR3_BASE_TREE node);

	void processSentenceLevel2_7(pANTLR3_BASE_TREE node);

	void processSentenceAssign(pANTLR3_BASE_TREE node);

	bool parseContentBinaryData(String const & data, sBuffer & outBuffer);

#ifdef __cplusplus
}
#endif

#endif 

