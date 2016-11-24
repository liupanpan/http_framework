#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "include/AppRequest.h"
#include "include/SynchronousRequest.h"
#include "include/CS_REQHND.h"

static std::string city = "suzhou";
static std::string language = "ZH";

class MiniWeatherRequest: public SynchronousRequest{
public:
	MiniWeatherRequest(){
		HTTPMethod = "GET";				
	}
	virtual ~MiniWeatherRequest() {

	}
};

struct GetMiniWeatherRequest:public AppRequest<MiniWeatherRequest>
{
	GetMiniWeatherRequest(const std::string& city, const std::string& language):base("http://192.168.1.2/OLS/MINIWeather_zh_CN.xml")
	{
		printf("Requesting miniweather in city %s and language %s\n", city.c_str(), language.c_str());
	}

	void OnRequestFailed(FAIL_REASON reason)
	{
		printf("GetMiniWeatherRequest OnRequestFailed() reason = %d\n", reason);
	}

	bool OnRequestResult()
	{
		printf("GetMiniWeatherRequest OnRequestResult()\n");

		return true;
	}
};

void refreshMiniweather(int signo)
{
	GetMiniWeatherRequest* req = new GetMiniWeatherRequest(city, language);
	if(REQHND_Send(req))
	{
		printf("REQHAND_Send successfully\n");
	}
	else
	{
		printf("REQHAND_Send failed\n");
		delete req;
	}
}

int main()
{
	struct sigaction act;
	union sigval tsval;
	act.sa_handler = refreshMiniweather;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	sigaction(50, &act, NULL);
	REQHND_Init();
	while ( 1 )
	{
		/*向主进程发送信号，实际上是自己给自己发信号*/
		sigqueue(getpid(), 50, tsval);
		sleep(5); /*睡眠2秒*/
	}
	return 0;
}
