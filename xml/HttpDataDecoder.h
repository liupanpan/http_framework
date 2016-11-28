#ifndef __HTTPDATADECODER__
#define __HTTPDATADECODER__
  
#include <string>
#include <vector>

typedef enum DECODE_ERROR_ERROR 
{
	DECODER_OK,
	ADDR_ERROR,
	PARAM_ERROR,
	DATA_ERROR,
	NODE_INVALID,
	PARSE_ERROR,
	RETURN_INVALID,
}Decode_Error;

using namespace std;

#ifndef VECTOR_VOIDPTR_STRING
#define VECTOR_VOIDPTR_STRING
typedef vector<const void*>VECTORVOIDPTR;
typedef vector<string> VECTORSTRING;

#endif

class HttpDataDecoder
{
	public:
		const char* buffAddr;
		string encodingType;
		HttpDataDecoder(const char* bufferAddr, string type):buffAddr(bufferAddr), encodingType(type)
		{

		}
		~HttpDataDecoder()
		{

		}

		virtual Decode_Error parseData()
		{
			return RETURN_INVALID;
		}

		/*
		*discription:get all node address which name is nodename
		*@param[in] nodename: the name of node.
		*@param[out] node. the address of node which name is nodename
		*return: true mean success, false mean fail.
		*/
		virtual Decode_Error  getNodeByNodeName(const string nodeName, VECTORVOIDPTR &node)
		{
			return RETURN_INVALID;
		}

		/*
		*discription:get node address
		*@param[in] pparentNode: the address of parent node address.
		*@param[in] nodename: the name of node.
		*@param[out] node. the address of node which name is nodename
		*return: true mean success, false mean fail.
		*/
		virtual Decode_Error getNodeByNodeName(const void* parentNode, const string nodeName,VECTORVOIDPTR &node)
		{
			return RETURN_INVALID;
		}
		
		/*
		*discription:get the attribute value of node.
		*@param[in] pnode: the address of node.
		*@param[in] attname: the name of attribute.
		*@param[out] value: the value of attribute.
		*return: true mean success, false mean fail.
		*/
		virtual Decode_Error getAttrValueByAttrName(const void* node, const string attName,string & value)
		{
			return RETURN_INVALID;
		}

		/*
		*discription:get the value of node
		*@param[in] node: the address of node.
		*@param[out] value: the value of node.
		*return: true mean success, false mean fail.
		*/
        virtual Decode_Error getNodeValue(const void * node, string &value)
		{
			return RETURN_INVALID;
		}

		/*
		*discription:get the value of node set
		*@param[in] pnode: the address of parent node.
		*@param[in] nodename: the set of node name.
		*@param[out] values: the value of nodes.
		*return: true mean success, false mean fail.
		*/
		virtual Decode_Error getNodeValuesByNodeNames(const void *node, VECTORSTRING nodeNames, VECTORSTRING & values)
		{
			return RETURN_INVALID;
		}
};

#endif

