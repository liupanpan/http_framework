#ifndef _HTTP_STATUS_CODES_H_
#define _HTTP_STATUS_CODES_H_

/*********************************************************************/
/*                           USER DEFINES                            */
/*********************************************************************/

/** Continue */
#define HTTP_STATUS_CONTINUE 100

/* Switching Protocols */
#define HTTP_STATUS_SWITCHING_PROTOCOLS 101

/**/
#define HTTP_STATUS_PROCESSING 102

/* OK */
#define HTTP_STATUS_OK 200

/* Created */
#define HTTP_STATUS_CREATED 201

/* Accepted */
#define HTTP_STATUS_ACCEPTED 202

/* Non-Authoritative Information */
#define HTTP_STATUS_NON_AUTHORITATIVE_INFORMATION 203

/* No Content */
#define HTTP_STATUS_NO_CONTENT 204

/* Reset Content */
#define HTTP_STATUS_RESET_CONTENT 205

/* Partial Content */
#define HTTP_STATUS_PARTIAL_CONTENT 206

/* Multiple Choices */
#define HTTP_STATUS_MULTIPLE_CHOICES 300

/* Moved Permanently */
#define HTTP_STATUS_MOVED_PERMANENTLY 301

/* Moved Temporarily */
#define HTTP_STATUS_MOVED_TEMPORARILY 302

/* See Other */
#define HTTP_STATUS_SEE_OTHER 303

/* Not Modified */
#define HTTP_STATUS_NOT_MODIFIED 304

/* Use Proxy */
#define HTTP_STATUS_USE_PROXY 305

/**/
#define HTTP_STATUS_SWITCH_PROXY 306

/* Temporary Redirect */
#define HTTP_STATUS_TEMPORARY_REDIRECT 307


/* Bad Request */
#define HTTP_STATUS_BAD_REQUEST 400

/* Unauthorized */
#define HTTP_STATUS_UNAUTHORIZED 401

/* Payment Required */
#define HTTP_STATUS_PAYMENT_REQUIRED 402

/* Forbidden */
#define HTTP_STATUS_FORBIDDEN 403

/* Not Found */
#define HTTP_STATUS_NOT_FOUND 404

/* Method Not Allowed */
#define HTTP_STATUS_METHOD_NOT_ALLOWED 405

/* Not Acceptable */
#define HTTP_STATUS_NOT_ACCEPTABLE 406

/* Proxy Authentication Required */
#define HTTP_STATUS_PROXY_AUTHENTICATION_REQUIRED 407

/* Request Time-out */
#define HTTP_STATUS_REQUEST_TIME_OUT 408

/* Conflict */
#define HTTP_STATUS_CONFLICT 409

/* Gone */
#define HTTP_STATUS_GONE 410

/* Length Required */
#define HTTP_STATUS_LENGTH_REQUIRED 411

/* Precondition Failed */
#define HTTP_STATUS_PRECONDITION_FAILED 412

/* Request Entity Too Large */
#define HTTP_STATUS_REQUEST_ENTITY_TOO_LARGE 413

/* Request-URI Too Large */
#define HTTP_STATUS_REQUEST_URI_TOO_LARGE 414

/* Unsupported Media Type */
#define HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE 415

/* Requested Range Not Satisfiable */
#define HTTP_STATUS_REQ_RANGE_NOT_SATISFIABLE 416

/* Internal Server Error */
#define HTTP_STATUS_INTERNAL_SERVER_ERROR 500

/* Not Implemented */
#define HTTP_STATUS_NOT_IMPLEMENTED 501

/* Bad Gateway */
#define HTTP_STATUS_BAD_GATEWAY 502

/* Service Unavailable */
#define HTTP_STATUS_SERVICE_UNAVAILABLE 503

/* Gateway Time-out */
#define HTTP_STATUS_GATEWAY_TIME_OUT 504

/* HTTP Version not supported */
#define HTTP_STATUS_HTTP_VERSION_NOT_SUPPORTED 505

#endif
