#include <thread>
#include <string.h>
#include<cstring>
#include<cstdio>

bool StartRakNetServer(int port, int broadcastInterval, const char* broadcastMsg, int broadcastMsgLen);

int main(int argn, char* argv[])
{
	int dataSzie = 400;			// ���ݰ���С
	int broadcastInterval = 1000;			// �㲥���
	const int raknetPort = 5001;			// raknet server �˿ں�

	if (argn > 1)
	{
		broadcastInterval = atoi(argv[1]);
	}

	// char msg[dataSzie] = { 0 };
	char msg[] = {"daichaofan"};
	// memset(msg, 97, dataSzie - 1);	// msg =  "aaaaa ... aaaaaa"
	// msg[dataSzie - 1] = 0;
	dataSzie = strlen(msg);

	std::thread thrd1(StartRakNetServer, raknetPort, broadcastInterval, msg, dataSzie);

	for (;;)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1000));
	}
}
