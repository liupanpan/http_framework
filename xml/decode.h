#ifndef __decode_h__
#define __decode_h__

#include <vector>
#include <string>

using namespace std;

enum DataType
{
		DataType_Xml,
			DataType_Invalid,
};

enum EncodingType
{
		EncodingType_UTF8,
			EncodingType_Invalid,
};

typedef void HANDLE;
typedef vector<const void*>VECTORVOIDPTR;
typedef vector<string> VECTORSTRING;

/*
   *discription:init the decoder
   *@param[in] dataaddr: the address of data.
   *@param[in] dataType: data type(xml/json).
   *@param[in] encodingType: the encoding type.
   *return: NULL mean create fail.
   */

HANDLE* Decode_Create(const char * dataAddr,DataType dataType,EncodingType encodingType);

/*
   *discription:init the decoder
   *@param[in] handle: the decode handle.
   *return: true mean success, false mean fail.
   */
bool Decode_Destroy(HANDLE * handle);

/*
   *discription:get all node address which name is nodename
   *@param[in] handle: the handle of decoder.
   *@param[in] nodename: the name of node.
   *@param[out] node. the address of node which name is nodename
   *return: true mean success, false mean fail.
   */
bool Decode_getNodeByNodeName(HANDLE * handle,const string nodeName, VECTORVOIDPTR &node);

/*
   *discription:get node address
   *@param[in] handle: the handle of decoder.
   *@param[in] pparentNode: the address of parent node address.
   *@param[in] nodename: the name of node.
   *@param[out] node. the address of node which name is nodename
   *return: true mean success, false mean fail.
   */
bool Decode_getNodeByNodeName(HANDLE * handle,const void* parentNode, const string nodeName,VECTORVOIDPTR &node);

/*
   *discription:get the attribute value of node.
   *@param[in] handle: the handle of decoder.
   *@param[in] pnode: the address of node.
   *@param[in] attname: the name of attribute.
   *@param[out] value: the value of attribute.
   *return: true mean success, false mean fail.
   */
bool Decode_getAttrValueByAttrName(HANDLE * handle,const void* node, const string attName,string & value);

/*
   *discription:get the value of node
   *@param[in] handle: the handle of decoder.
   *@param[in] node: the address of node.
   *@param[out] value: the value of node.
   *return: true mean success, false mean fail.
   */
bool Decode_getNodeValue(HANDLE * handle,const void * node, string &value);

/*
   *discription:get the value of node set
   *@param[in] handle: the handle of decoder.
   *@param[in] pnode: the address of parent node.
   *@param[in] nodename: the set of node name.
   *@param[out] values: the value of nodes.
   *return: true mean success, false mean fail.
   */
bool Decode_getNodeValuesByNodeNames(HANDLE * handle,const void *node, VECTORSTRING nodeNames, VECTORSTRING & values);

#endif

