#include <iostream>  
#include <list> 
#include <string> 
#include <libxml/xmlmemory.h>
#include "HttpDataDecoder.h"

using namespace std;

class XmlDataDecoder:public HttpDataDecoder
{
	public:
		XmlDataDecoder(const char* pxmldataaddress, const string encodingtype);		
		~XmlDataDecoder();

		Decode_Error parseData();

		/*
		*discription:get all node address which name is nodename
		*@param[in] nodename: the name of node.
		*@param[out] node. the address of node which name is nodename
		*return: true mean success, false mean fail.
		*/
		Decode_Error  getNodeByNodeName(const string nodeName, VECTORVOIDPTR &node);

		/*
		*discription:get node address
		*@param[in] pparentNode: the address of parent node address.
		*@param[in] nodename: the name of node.
		*@param[out] node. the address of node which name is nodename
		*return: true mean success, false mean fail.
		*/
		Decode_Error getNodeByNodeName(const void* parentNode, const string nodeName,VECTORVOIDPTR &node);

		/*
		*discription:get the attribute value of node.
		*@param[in] pnode: the address of node.
		*@param[in] attname: the name of attribute.
		*@param[out] value: the value of attribute.
		*return: true mean success, false mean fail.
		*/
		Decode_Error getAttrValueByAttrName(const void* node, const string attName,string & value);

		/*
		*discription:get the value of node
		*@param[in] node: the address of node.
		*@param[out] value: the value of node.
		*return: true mean success, false mean fail.
		*/
        Decode_Error getNodeValue(const void * node, string &value);

		/*
		*discription:get the value of node set
		*@param[in] pnode: the address of parent node.
		*@param[in] nodename: the set of node name.
		*@param[out] values: the value of nodes.
		*return: true mean success, false mean fail.
		*/
		Decode_Error getNodeValuesByNodeNames(const void *node, VECTORSTRING nodeNames, VECTORSTRING & values);


	private:
		void getNodeByNode(xmlNodePtr parentNode, const string nodeName,VECTORVOIDPTR &node);
		void getNodeByNodePtr(xmlNodePtr parentNode, xmlNodePtr nodePtr,bool & isValid);
		bool checkNodeValid(const void * node);
		void* xmlReader;
};
