#include <thread>
#include <string.h>
#include<cstring>
#include<cstdio>
#include <sys/timeb.h>
#include <cstdlib>

using namespace std;

bool StartRakNetServer(int port, int broadcastInterval, const char* broadcastMsg, int broadcastMsgLen);

int main(int argn, char* argv[])
{
	int dataSzie = 400;			// 数据包大小
	int broadcastInterval = 1000;			// 广播间隔
	const int raknetPort = 5001;			// raknet server 端口号

	if (argn > 1)
	{
		broadcastInterval = atoi(argv[1]);
	}

	char msg[100];

	struct timeb tb;
    ftime(&tb);
    snprintf(msg, sizeof(msg), "%d", (int)tb.time);
	dataSzie = strlen(msg);

	std::thread thrd1(StartRakNetServer, raknetPort, broadcastInterval, msg, dataSzie);

	for (;;)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1000));
	}
}
