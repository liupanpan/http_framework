#include <sstream>
#include "HttpRequest.h"

// Base class for Application requests
// provides standardized handling for sending/receiving requests
template <typename Req, typename Rsp>
struct AppRequest:public Req, public Rsp, public HttpRequest
{
	typedef AppRequest <Req, Rsp> base;
	typedef Rsp R;

	AppRequest(const std::string& url) 
    {
        urlStr = url;
	}

    virtual void OnRequestResult ( const Rsp& rsp ) = 0;
    virtual void OnRequestFailed ( FAIL_REASON reason ) = 0;

    //Called when the search request is done. Successful or not.
    void onRequestDone()
    {
        FAIL_REASON reason = NONE;
        if(status == REQ_OK) 
        {
            Rsp rsp; Rsp* prsp = &rsp;
            printf(">>>>>>>%s>>>>>>\n", reinterpret_cast<char*>(&(rxBuffer[0])));
            //if(xml/json decode succfully)
            //{
            //    OnRequestResult(rsp);
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