#include <curl/curl.h>
#include <curl/easy.h>
#include <openssl/ssl.h>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <sstream>
#include <stdlib.h> 
#include <signal.h> 
#include <unistd.h>

#include "CS_REQHND.h"
#include "HttpRequest.h"
#include "Http_status_codes.h"

static REQHND_State state = { REQHND_INIT };
static void* multiInstance = 0;

static std::list<HttpRequest *> g_ongoingRequests;

static REQ_Status CurlConnectionErrorToRequest ( int err )
{
   switch ( err )
   {
   	case CURLE_COULDNT_RESOLVE_PROXY:   
   		return REQ_LE_NO_HOST;
   	case CURLE_COULDNT_RESOLVE_HOST:    
		return REQ_LE_NO_HOST;
   	case CURLE_COULDNT_CONNECT:         
		return REQ_LE_REJECTED;
    case CURLE_SSL_CERTPROBLEM:         
		return REQ_LE_OLS_CERT_ERR;
   	case CURLE_SSL_CACERT:              /** fall through */
   	case CURLE_SSL_CACERT_BADFILE:      
		return REQ_LE_BACKEND_CERT_ERR;
   	case CURLE_SSL_CIPHER:              /** fall through */
   	case CURLE_SSL_CONNECT_ERROR:      
		return REQ_LE_SSL_CONNECTION;
   	case CURLE_OPERATION_TIMEDOUT:      
		return REQ_LE_TIMEOUT;
   	case CURLE_WRITE_ERROR:             
		return REQ_LE_WRITE_ERROR;
   	default:                          
      	printf("Unknown CURL error code: %d", err);
			
      	return REQ_LE_UNKNOWN;
   }
}

static unsigned int onReceiveHeader(void *pData, unsigned int size, unsigned int nmemb, HttpRequest *req)
{
   return req->onReceiveHeader(pData, size, nmemb);
}

static unsigned int onReceiveData(void *pData, unsigned int size, unsigned int nmemb, HttpRequest *req)
{
   return req->onReceiveData(pData, size, nmemb);
}

static unsigned int onTransmitData(void *pData, unsigned int size, unsigned int nmemb, HttpRequest *req)
{
   return req->onTransmitData(pData, size, nmemb);
}

static bool configureSession ( void* handle, HttpRequest* req )
{
	curl_easy_setopt(handle, CURLOPT_PROXY,"");
    curl_easy_setopt(handle, CURLOPT_PROXYUSERNAME, "");
    curl_easy_setopt(handle, CURLOPT_PROXYPASSWORD, "");

	curl_easy_setopt(handle, CURLOPT_DNS_SERVERS, "");

	curl_easy_setopt(handle, CURLOPT_URL, req->urlStr.c_str());

	curl_easy_setopt(handle, CURLOPT_HEADERDATA, req);
   	curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, onReceiveHeader);

   	curl_easy_setopt(handle, CURLOPT_WRITEDATA, req);
   	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, onReceiveData);

   	curl_easy_setopt(handle, CURLOPT_READDATA, req);
   	curl_easy_setopt(handle, CURLOPT_READFUNCTION, onTransmitData);

	// Setup redirection and its limit...
	curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1);
    	curl_easy_setopt(handle, CURLOPT_MAXREDIRS, 2);

	curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, true);
    	curl_easy_setopt(handle, CURLOPT_SSL_SESSIONID_CACHE, true);

	// Set timeouts
   	curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, 20);
   	curl_easy_setopt(handle, CURLOPT_TIMEOUT, 20);

	return true;
}

static void processCurlError(CURLMsg* msg, HttpRequest* req)
{
	switch(msg->data.result)
	{
		case CURLE_COULDNT_RESOLVE_HOST:
      		case CURLE_SSL_CONNECT_ERROR:
	      	case CURLE_SSL_CIPHER:
	      	case CURLE_SSL_CERTPROBLEM:
	      	case CURLE_SSL_CACERT:
	      	case CURLE_SSL_CACERT_BADFILE:
	      	case CURLE_SSL_SHUTDOWN_FAILED:
	      	case CURLE_SSL_CRL_BADFILE:
	      	case CURLE_SSL_ISSUER_ERROR:

		default:
         	/* Do nothing */
      		break;
	}

	req->setLinkError ( CurlConnectionErrorToRequest ( msg->data.result ) );
}

static void processCurlOk(CURLMsg* msg, HttpRequest* req, const long http_code)
{
	if(http_code != HTTP_STATUS_OK)
	{
		req->setHttpError(http_code);
		
		if (http_code >= HTTP_STATUS_BAD_REQUEST)
		{

		}
	}
}

static void cleanupRequestWithCurl(HttpRequest* pRequest)
{
	if (NULL != pRequest->pSessionHandle)
	{
		curl_multi_remove_handle(multiInstance, pRequest->pSessionHandle);
		curl_easy_cleanup(pRequest->pSessionHandle);
	}

		     
    	pRequest->pSessionHandle = NULL;
}

	
static void processCurl()
{
	int curlMultiPerform = CURLM_CALL_MULTI_PERFORM;
	int still_running = 0;

	//调用curl_multi_perform函数执行curl请求 
   	while (curlMultiPerform == CURLM_CALL_MULTI_PERFORM) 
   	{
    	curlMultiPerform = curl_multi_perform(multiInstance, &still_running);
   	}

	if (!still_running)
   	{
      	state = REQHND_IDLE;
   	}

	//输出执行结果
   	CURLMsg* msg;
   	int msgs_left;
   	while ((msg = curl_multi_info_read(multiInstance, &msgs_left))) 
   	{
   		if (msg->msg == CURLMSG_DONE) 
   		{
   			HttpRequest* req;

			curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &req);

			// This request is finished w/ REQHND, so remove it from the 'ongoing' list.
         		g_ongoingRequests.remove(req);
			//printf("### HTTP Request response [Addr: 0x%p] '%s'\n",req, req->urlStr.c_str());
			
			if (msg->data.result != CURLE_OK)
			{
				processCurlError(msg, req);
			}
			else
			{
				long http_code;
            			curl_easy_getinfo(msg->easy_handle, CURLINFO_RESPONSE_CODE, &http_code);
				//printf("HTTP resp code: %li\n", http_code);
				processCurlOk(msg, req, http_code);
			}
			// Cleanup curl session
			cleanupRequestWithCurl(req);
			req->onRequestDone();

			delete req;
   		}
   	}
	/**
      	* check if there is more work
      	* update running state if needed
      	*/
   	if(still_running)
   	{
		//Timer t(100, TimerCbData(processCurl));
		processCurl();
	}
}

static void terminateRequest(HttpRequest *pRequest)
{
	// Cleanup curl session
	cleanupRequestWithCurl(pRequest);
	// Finalize and delete request object
	pRequest->status = REQ_CONTROLLED_DISABLE;
	pRequest->onRequestDone();
	delete pRequest;
}

void reqhnd_TerminateRequests(void)
{
   std::list<HttpRequest *> ongoingRequests( g_ongoingRequests );
   g_ongoingRequests.clear();

   std::for_each( ongoingRequests.begin(), ongoingRequests.end(), terminateRequest);
}

void REQHND_Init ()
{
	printf("enter the reqhand_int\n");
	if (NULL != multiInstance)
	{
    	reqhnd_TerminateRequests();
      	curl_multi_cleanup(multiInstance);
      	multiInstance = NULL;
   	}

	// It is ok to call this func multiple times...
   	curl_global_init ( CURL_GLOBAL_DEFAULT );
	
	// multi instance used for pumping requests
	multiInstance = curl_multi_init();

	curl_multi_setopt(multiInstance, CURLMOPT_PIPELINING, 1); //Pipelining
	
	state = REQHND_IDLE;
}

bool REQHND_Send (HttpRequest* pRequest)
{
	printf("enter the reqhand_send\n");
	if(NULL == pRequest)
	{
		printf("Try to send NULL request...\n");
      	return false;
	}

	if(state >= REQHND_IDLE)
	{
		//allocate a session
		void* h = curl_easy_init();
		if(h)
		{
			if(!configureSession (h, pRequest))
			{	
				printf("Failed to configure request session: '%s'", pRequest->urlStr.c_str());
				curl_easy_cleanup(h);
				return false;
			}
			else
			{
				g_ongoingRequests.push_back(pRequest);
				curl_easy_setopt(h, CURLOPT_PRIVATE, pRequest);
				pRequest->pSessionHandle = h;
				
				printf("Send HTTP Request[Addr: 0x%p]: %s\n", pRequest, pRequest->urlStr.c_str());
				printf("Adding handle (%p) to multihandle container.\n", h);
            			curl_multi_add_handle(multiInstance, h);

				if(state == REQHND_IDLE)
            			{
					//Start request scheduling
            				//Timer t(0, TimerCbData(processCurl));
					state = REQHND_WORKING;
					processCurl();
            			}
				
				return true;	
			}
		}
	}

	return false;
}



