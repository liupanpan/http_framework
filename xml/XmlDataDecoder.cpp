#include <iostream>  
#include "string.h"  
#include "XmlDataDecoder.h"  
#include <libxml/xmlmemory.h>  
#include <libxml/parser.h>  
#include <libxml/tree.h>  

using namespace std;

XmlDataDecoder::XmlDataDecoder(const char * xmlDataAddr,const string encodingType):HttpDataDecoder(xmlDataAddr,encodingType)
{  
	xmlReader = NULL;
} 

XmlDataDecoder::~XmlDataDecoder()
{
	if(xmlReader)
	{
		xmlDocPtr reader = (xmlDocPtr)xmlReader;
		xmlFreeDoc(reader);
		xmlReader = NULL;
	}
	else
	{

	}
}

void XmlDataDecoder::getNodeByNode(xmlNodePtr parentNode, const string nodeName,VECTORVOIDPTR& node)
{
	string value;
	xmlNodePtr curr = NULL;
					
	if (parentNode == NULL)
		return;

	if (0 == xmlStrcmp(parentNode->name,BAD_CAST(nodeName.c_str())))
	{
		node.push_back((const void *)parentNode);
	}

	curr = parentNode->children;
			
	getNodeByNode(curr, nodeName, node);

	getNodeByNode(parentNode->next,nodeName,node);
								
	return;
}

void XmlDataDecoder::getNodeByNodePtr(xmlNodePtr parentNode, xmlNodePtr nodePtr,bool & isValid)
{
	if(parentNode == NULL || isValid == true)
		return;
	if (parentNode == nodePtr)
	{
		isValid = true;
		return;
	}

	getNodeByNodePtr(parentNode->children, nodePtr, isValid);

	getNodeByNodePtr(parentNode->next,nodePtr,isValid);
				
	return;
}

bool XmlDataDecoder::checkNodeValid(const void * node)
{
	xmlNodePtr nodePtr = NULL;
	xmlDocPtr reader = NULL;
	bool isValid = false;
	xmlNodePtr parNode = NULL;
	void * voidNode=NULL;
	voidNode  = const_cast<void *>(node);
	parNode = (xmlNodePtr)voidNode;
							
	reader = (xmlDocPtr)xmlReader;		
	nodePtr = xmlDocGetRootElement(reader);
										
	getNodeByNodePtr(nodePtr,parNode,isValid);
									  
	return isValid;
}

Decode_Error XmlDataDecoder::parseData()
{
	if(NULL != buffAddr)
	{
		int xmlReaderOptions = XML_PARSE_NOENT | XML_PARSE_NOERROR
		| XML_PARSE_NOWARNING | XML_PARSE_NONET | XML_PARSE_NSCLEAN;

		xmlReaderOptions |= XML_PARSE_NOCDATA;
					
		xmlDocPtr reader = NULL;

		reader = xmlReadMemory(buffAddr,strlen(buffAddr),NULL,encodingType.c_str(),xmlReaderOptions);
		if(NULL == reader)
		{
			//to do print the warning info of not create the tree unsuccessful.  
			return DATA_ERROR;  
		}
		else
		{
			xmlReader = (void *)reader;
			xmlKeepBlanksDefault(0);
			return DECODER_OK;
			//to do notify the parse tree created successful. 
		}
	}
	else
	{
		//to do print the error message;
		return ADDR_ERROR;
	}
}

/*
*discription:get all node address which name is nodename
*@param[in] nodename: the name of node.
*@param[out] node. the address of node which name is nodename
*return: true mean success, false mean fail.
*/
Decode_Error  XmlDataDecoder::getNodeByNodeName(const string nodeName, VECTORVOIDPTR& node)
{
	xmlNodePtr nodePtr = NULL;
	xmlDocPtr reader = NULL;
					
	if (nodeName.empty())
	{
		return PARAM_ERROR;
	}

	const char * elementName = nodeName.c_str();

	// to do print nodeName
	if(xmlReader && elementName)
	{
		reader = (xmlDocPtr)xmlReader;		
		nodePtr = xmlDocGetRootElement(reader);
		
		if(NULL == nodePtr)
		{
			//to do print empty xml
			return PARSE_ERROR;
		}
		else
		{
			getNodeByNode(nodePtr,nodeName,node);
		}
	}
	else
	{
		return DATA_ERROR;
	}

	return DECODER_OK;
}

/*
*discription:get node address
*@param[in] pparentNode: the address of parent node address.
*@param[in] nodename: the name of node.
*@param[out] node. the address of node which name is nodename
*return: true mean success, false mean fail.
*/
Decode_Error XmlDataDecoder::getNodeByNodeName(const void* parentNode, const string nodeName,VECTORVOIDPTR &node)
{
	xmlNodePtr parNode = NULL;
	xmlNodePtr currNode = NULL;
	void * nodePtr = NULL;

	if (NULL == parentNode || !checkNodeValid(parentNode))
	{
		return NODE_INVALID;
	}
	else
	{
		nodePtr  = const_cast<void *>(parentNode);
		parNode = (xmlNodePtr)nodePtr;
	}

	if(NULL != xmlReader)
	{
		currNode = parNode->xmlChildrenNode;
		while(NULL != currNode)
		{
			if(0 == xmlStrcmp(currNode->name, BAD_CAST(nodeName.c_str())))
			{
				node.push_back((const char *)currNode);
			}														
			currNode = currNode->next;
		}
	}
	else
	{
		return DATA_ERROR;
	}

	return DECODER_OK;
}


/*
*discription:get the value of node
*@param[in] node: the address of node.
*@param[out] value: the value of node.
*return: true mean success, false mean fail.
*/
Decode_Error XmlDataDecoder::getNodeValue(const void * node, string &value)
{
	if (NULL == node || !checkNodeValid(node))
	{
		//to do print param node is not valid
		return NODE_INVALID;
	}

	if(NULL != xmlReader)
	{	
		xmlChar *szKey = xmlNodeGetContent((xmlNodePtr)node);
		if(NULL == szKey)
		{
			return PARSE_ERROR;
		}
		else
		{
			value = (char *)szKey;
			free(szKey);
		}
	}
	else
	{
		return DATA_ERROR;
	}

	return DECODER_OK;
}



























