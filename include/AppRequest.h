#ifndef _APPREQUEST_H_
#define _APPREQUEST_H_

#include <sstream>
#include "HttpRequest.h"

// Base class for Application requests
// provides standardized handling for sending/receiving requests
template <typename Req>
struct AppRequest:public Req, public HttpRequest
{
	typedef AppRequest <Req> base;

	AppRequest(const std::string& url) 
    {
    	setRequestInfo(*this);//set up the http header
        urlStr = url;
	}

    virtual bool OnRequestResult(){
		return false;
	};

    virtual void OnRequestFailed(FAIL_REASON reason) = 0;

    //Called when the search request is done. Successful or not.
    void onRequestDone()
    {
        FAIL_REASON reason = NONE;
        if(status == REQ_OK) 
        {
            printf("%s\n", reinterpret_cast<char*>(&(rxBuffer[0])));
            //if(xml/json decode succfully)
            //{
            //    OnRequestResult();
            //}
        }
        else if(status == REQ_HTTP_ERR)
        {
            reason = SERVER_ERROR;
        }
        else
        {
            reason = NETWORK_ERROR;
        }
        
        failReason = reason;
        
        if(reason != NONE)
        {
            OnRequestFailed(reason);
        }
    }
        
};

#endif
