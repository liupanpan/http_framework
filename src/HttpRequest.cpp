#include <stdio.h>
#include <string.h>
#include "HttpRequest.h"
#include "Http_status_codes.h"
#include "SynchronousRequest.h"

HttpRequest::HttpRequest():
	status(REQ_OK),
	http_status_code(HTTP_STATUS_OK),
	failReason(NONE)
{
		
}

HttpRequest::~HttpRequest()
{
}

void HttpRequest::setLinkError ( const REQ_Status req_status )
{
   status = req_status;
}

void HttpRequest::setHttpError ( int http_code )
{
   status            = REQ_HTTP_ERR;
   http_status_code  = http_code;
}

unsigned int HttpRequest::onReceiveHeader(void *pData, unsigned int size, unsigned int nmemb)
{
   return size*nmemb;
}

unsigned int HttpRequest::onTransmitData(void *pData, unsigned int size, unsigned int nmemb)
{
   // Calc how much we should copy to the send buffer
   unsigned int sizeDataLeft = txBuffer.size() - txDataTransmitted;
   unsigned int sendSize = sizeDataLeft < (size*nmemb) ? sizeDataLeft : size*nmemb;

   printf("Curl accepts %d bytes, %d provided...\n",  size*nmemb, sendSize);

   if (sendSize > 0)
   {
      // Copy data tocurl send buffer
      memcpy(pData, &txBuffer[0] + txDataTransmitted, sendSize);
      txDataTransmitted += sendSize;
   }

   return sendSize;
}

unsigned int HttpRequest::onReceiveData(void *pData, unsigned int size, unsigned int nmemb)
{
   printf("HttpRequest (%p) received %d bytes...\n", this, size*nmemb);

   // Append data to rx buffer
   rxBuffer.insert(rxBuffer.end(), (unsigned char*)(pData), (unsigned char*)(pData) + size*nmemb);

   return size*nmemb;
}

void HttpRequest::setRequestInfo(const SynchronousRequest &synchronousRequest)
{
	this->headers[CONTENT_TYPE] = "Content-Type: " + synchronousRequest.HTTPContentType;
	this->headers[CHARSET] = "charset: utf-8";
}