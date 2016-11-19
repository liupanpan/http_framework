#include <curl/curl.h>
#include <curl/easy.h>
#include <openssl/ssl.h>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

#include "CS_REQHND.h"
#include "HttpRequest.h"

static REQHND_State state = { REQHND_INIT };
static CURLM multiInstance = 0;

static std::list<HttpRequest *> g_ongoingRequests;

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

static void processCurlError(CURLMsg* msg, HttpRequest* req, bool &requestRescheduled)
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
}

static void processCurlOk(CURLMsg* msg, HttpRequest* req, const long http_code, bool &requestRescheduled)
{

}
	
static void processCurl (void* dummy)
{
	int curlMultiPerform = CURLM_CALL_MULTI_PERFORM;
	int still_running = 0;

	// read/write for all easy handles 
   	while (curlMultiPerform == CURLM_CALL_MULTI_PERFORM) 
   	{
    	curlMultiPerform = curl_multi_perform(multiInstance, &still_running);
   	}

	if (!still_running)
   	{
      	state = REQHND_IDLE;
   	}

	// read any messages
   	CURLMsg* msg;
   	int msgs_left;
   	while ((msg = curl_multi_info_read (multiInstance, &msgs_left))) 
   	{
   		if (msg->msg == CURLMSG_DONE) 
   		{
   			HttpRequest* req;
   			bool requestRescheduled = false;

			curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &req);
			
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

			req->onRequestDone();
   		}
   	}
}

void REQHND_Init ()
{
	if (NULL != multiInstance)
	{
    	REQHND_TerminateRequests();
      	curl_multi_cleanup(multiInstance);
      	multiInstance = NULL;
   	}
	
	// multi instance used for pumping requests
	multiInstance = curl_multi_init();

	state = REQHND_IDLE;
}

bool REQHND_Send (HttpRequest* pRequest)
{
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
				return false;
			}
			else
			{
				printf("Send HTTP Request[Addr: 0x%p]: (%s) '%s\n", pRequest, pRequest->urlStr.c_str());
				g_ongoingRequests.push_back(pRequest);
				
				printf("Adding handle (%p) to multihandle container.", h);
            			curl_multi_add_handle(multiInstance, h);

				if(state == REQHND_IDLE)
            			{
                			//Start request scheduling
            				Timer t(0, online::core::TimerCbData(processCurl));
            				state = REQHND_WORKING;
            			}
				return true;	
			}
		}
	}
}

static void terminateRequest(HttpRequest *pRequest)
{
   printf("Terminating request %p\n", pRequest);
   
   // Finalize and delete request object
   pRequest->status = REQ_CONTROLLED_DISABLE;
   pRequest->onRequestDone();
   delete pRequest;
}

void REQHND_TerminateRequests(void)
{
   // If we add a new request in an OnRequestDone call we will end up
   // in an endless loop if we don't copy the list first.

   std::list<HttpRequest *> ongoingRequests( g_ongoingRequests );
   g_ongoingRequests.clear();

   printf("Terminate all ongoing requests: %d\n", ongoingRequests.size());
   std::for_each(ongoingRequests.begin(), ongoingRequests.end(), terminateRequest);
}






