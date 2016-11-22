#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "include/AppRequest.h"
#include "include/SynchronousRequest.h"
#include "include/CS_REQHND.h"

static std::string city;
static std::string language;

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
		printf("Requesting miniweather in city %s and language %s", city.c_str(), language.c_str());
	}

	void OnRequestFailed(FAIL_REASON reason)
	{
		printf("GetMiniWeatherRequest OnRequestFailed() reason = %d\n", reason);
	}

	bool OnRequestResult()
	{
		printf("GetMiniWeatherRequest OnRequestResult()");

		return true;
	}
};

void refreshMiniweather(int signo)
{
	printf(">>>>\n");
	GetMiniWeatherRequest* req = new GetMiniWeatherRequest(city, language);
	if(REQHND_Send(req))
	{
		printf("successfully\n");
	}
	else
	{
		printf("failed\n");
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
	while ( 1 )
	{
		sleep(2); /*睡眠2秒*/
		/*向主进程发送信号，实际上是自己给自己发信号*/
		sigqueue(getpid(), 50, tsval);
	}
	return 0;
}
