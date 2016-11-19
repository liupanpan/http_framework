#include <stddef.h>
#include <vector>
#include <string>
#include <reuse.h>
#include <map>

enum REQ_Status
{
   REQ_OK         = 0,        ///< Everything is OK.
   REQ_LE_REJECTED,           ///< Conection was rejected by the server.
   REQ_LE_CLOSED,             ///< Server closed the connection.
   REQ_LE_NO_HOST,            ///< The host address could not be resolved.
   REQ_LE_TIMEOUT,            ///< Client side timeout.
   REQ_LE_SSL_CONNECTION,     ///< There was a problem with the secure connection.
   REQ_LE_UNKNOWN,            ///< All other errors that might ocurrr
   REQ_HTTP_ERR,              ///< We got a http response code not equal to 200 (status ok).
   REQ_CONTROLLED_DISABLE,    ///< Set when the connection was disconnected client side.
   REQ_LE_OLS_CERT_ERR,       ///< There was a problem with the client certificate
   REQ_LE_BACKEND_CERT_ERR,   ///< Peer could not be verified or a problem with the CA certificate.
   REQ_LE_WRITE_ERROR         ///< Could not store the received data.
};

enum FAIL_REASON
{
   NONE = 0,
   DATA_ERROR = 1,
   SERVER_ERROR = 2,
   NETWORK_ERROR = 3,
};

class HttpRequest
{
	public:
		enum Method
		{
			INVALID,
			HEAD,
			GET,
			POST,
			PUT,
			DEL
		};
		enum HeaderType
		{
			ACCEPT,
			ACCEPT_CHARSET,
			ACCEPT_ENCODING,
			ACCEPT_LANGUAGE,
			ACCEPT_DATETIME,
			AUTHORIZATION,
			CACHE_CONTROL,
			CHARSET,
			CONNECTION,
			COOKIE,
			CONTENT_LENGTH,
			CONTENT_MD5,
			CONTENT_TYPE,
			DATE,
			EXPECT,
			FROM,
			HOST,
			IF_MATCH,
			IF_MODIFIED_SINCE,
			IF_NONE_MATCH,
			IF_RANGE,
			IF_UNMODIFIED_SINCE,
			MAX_FORWARDS,
			ORIGIN,
			PRAGMA,
			PROXY_AUTHORIZATION,
			RANGE,
			REFERER,
			TE,
			UNLESS_MODIFIED_SINCE,
			USER_AGENT,
			UPGRADE,
			VIA,
			WARNING
		};
		HttpRequest();
		virtual ~HttpRequest();
		
        virtual void onRequestDone() = 0;

        virtual unsigned int onReceiveHeader(void *pData, unsigned int size, unsigned int nmemb);

        virtual unsigned int onReceiveData(void *pData, unsigned int size, unsigned int nmemb);

        virtual unsigned int onTransmitData(void *pData, unsigned int size, unsigned int nmemb);
        
	//Buffers
	unsigned int txDataTransmitted;//How many bytes has currently been sent.
	std::vector<unsigned char> txBuffer; //Buffer containing send data. Typically xml data serialized from internal structures.
	std::vector<unsigned char> rxBuffer;//Buffer in which to receive uncompleted data
		
	// Connection config
	std::string urlStr;

	typedef std::map<HeaderType, std::string> HeaderMap;
	HeaderMap headers;            //Request headers that will be added to the default set of headers.

	REQ_Status      status;
	int             http_status_code;
	FAIL_REASON failReason;
}


















