#include "decode.h"
#include "HttpDataDecoder.h"
#include "XmlDataDecoder.h"

static vector<HttpDataDecoder*> g_handle;

static bool checkHandleValid(HANDLE *handle)
{
	HttpDataDecoder * decoder = NULL;
	decoder = (HttpDataDecoder *)handle;
	vector<HttpDataDecoder*>::iterator it;
							   
	for(it = g_handle.begin(); it != g_handle.end(); ++it)
	{
		if ((*it) ==  decoder)
		{
			return true;
		}
	}

	return false;
}

static void earseHandle(HANDLE * handle)
{
    HttpDataDecoder *decoder = (HttpDataDecoder*)handle;
    vector<HttpDataDecoder*>::iterator it;
					   
    for(it = g_handle.begin(); it != g_handle.end();)
	{
		if ((*it) ==  decoder)
		{
			it = g_handle.erase(it);
		}
		else
		{
		    it++;
		}
	}
}

static bool encodingTypeToString(EncodingType encodingType,string & encodingTypeString )
{
	switch (encodingType)
	{
		case EncodingType_UTF8:
		{
			encodingTypeString = "UTF-8";
		}
		break;
		default:
			return false;
	}
		return true;
}

HANDLE * Decode_Create(const char * dataAddr,DataType dataType,EncodingType encodingType)
{
	HttpDataDecoder * decoder = NULL;

	string encodingTypeString;
	if((NULL == dataAddr) || (dataType >= DataType_Invalid) 
					 || (encodingType >= EncodingType_Invalid))
	{
		return NULL;
	}

	encodingTypeToString(encodingType, encodingTypeString);

	switch(dataType)
	{
		case DataType_Xml:
		{
			decoder = new XmlDataDecoder(dataAddr,encodingTypeString);
		}	
		default:
		{
			decoder = NULL;
			break;
		}
	}
	if(decoder)
	{

	}
	else
	{
		return NULL;
	}

	return (HANDLE*)decoder;
}

/*
   *discription:init the decoder
   *@param[in] handle: the decode handle.
   *return: true mean success, false mean fail.
   */
bool Decode_Destroy(HANDLE * handle)
{
	HttpDataDecoder * decoder = NULL;
	checkHandleValid(handle);

	earseHandle(handle);
					
	decoder = (HttpDataDecoder *)handle;
	delete decoder;
							 
	return true;
}

/*
   *discription:get all node address which name is nodename
   *@param[in] handle: the handle of decoder.
   *@param[in] nodename: the name of node.
   *@param[out] node. the address of node which name is nodename
   *return: true mean success, false mean fail.
   */
bool Decode_getNodeByNodeName(HANDLE * handle,const string nodeName, VECTORVOIDPTR &node)
{
	HttpDataDecoder * decoder = NULL;
	checkHandleValid(handle);

	decoder = (HttpDataDecoder *)handle;
    Decode_Error error=decoder->getNodeByNodeName(nodeName,node);
	if (DECODER_OK != error)
    {
		return false;
	}
	
	return true;									
}

/*
   *discription:get node address
   *@param[in] handle: the handle of decoder.
   *@param[in] pparentNode: the address of parent node address.
   *@param[in] nodename: the name of node.
   *@param[out] node. the address of node which name is nodename
   *return: true mean success, false mean fail.
   */
bool Decode_getNodeByNodeName(HANDLE * handle,const void* parentNode, const string nodeName,VECTORVOIDPTR &node)
{
	HttpDataDecoder * decoder = NULL;
	checkHandleValid(handle);

	decoder = (HttpDataDecoder *)handle;
	if (DECODER_OK != decoder->getNodeByNodeName(parentNode,nodeName,node))
	{
		return false;
	}

	return true;							
}

/*
   *discription:get the value of node
   *@param[in] handle: the handle of decoder.
   *@param[in] node: the address of node.
   *@param[out] value: the value of node.
   *return: true mean success, false mean fail.
   */
bool Decode_getNodeValue(HANDLE * handle,const void * node, string &value)
{
	HttpDataDecoder * decoder = NULL;
	checkHandleValid(handle);

	decoder = (HttpDataDecoder *)handle;
	if (DECODER_OK != decoder->getNodeValue(node,value))
    {
		return false;
	}

		return true;
}

/*
   *discription:get the value of node set
   *@param[in] handle: the handle of decoder.
   *@param[in] pnode: the address of parent node.
   *@param[in] nodename: the set of node name.
   *@param[out] values: the value of nodes.
   *return: true mean success, false mean fail.
   */
bool Decode_getNodeValuesByNodeNames(HANDLE * handle,const void *node, VECTORSTRING nodeNames, VECTORSTRING & values)
{
	HttpDataDecoder * decoder = NULL;
	checkHandleValid(handle);

	decoder = (HttpDataDecoder *)handle;
	if (DECODER_OK != decoder->getNodeValuesByNodeNames(node,nodeNames,values))
    {
		return false;
	}

		return true;							
}

