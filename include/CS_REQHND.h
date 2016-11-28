#ifndef _CS_REQHND_H_
#define _CS_REQHND_H_

#include "HttpRequest.h"

/**
 * Valid states for the request handler module
 */
enum REQHND_State
{
   REQHND_INIT,
   REQHND_IDLE,
   REQHND_WORKING
};


void REQHND_Init ();

/**
 * Schedules a given HttpRequest for transmission. By this, the RequestHandler takes ownership of the 
 * pointer and will delete it when done (After the call to HttpRequest::onRequestDone).
 *
 * @param[in] pRequest                    Pre allocated and initialized object to transmit.
 * @return                                True if the request were scheduled
 *                                        False if an error occurred
 */
bool REQHND_Send (HttpRequest* pRequest );

#endif
