#include "MessageIdentifiers.h"
#include "RakPeerInterface.h"
#include "RakNetStatistics.h"
#include "RakNetTypes.h"
#include "BitStream.h"
#include "RakSleep.h"
#include "PacketLogger.h"
#include <assert.h>
#include <cstdio>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


bool StartRakNetServer(int port, int broadcastInterval, const char* broadcastMsg, int broadcastMsgLen)
{
	puts("Start RakNet Server ...");

	// Pointers to the interfaces of our server and client.
	// Note we can easily have both in the same program
	RakNet::RakPeerInterface *server = RakNet::RakPeerInterface::GetInstance();
	RakNet::RakNetStatistics *rss;
	server->SetIncomingPassword("Rumpelstiltskin", (int)strlen("Rumpelstiltskin"));
	server->SetTimeoutTime(30000, RakNet::UNASSIGNED_SYSTEM_ADDRESS);
	//	RakNet::PacketLogger packetLogger;
	//	server->AttachPlugin(&packetLogger);

	// Holds packets
	RakNet::Packet* p;

	// Starting the server is very simple.  2 players allowed.
	// 0 means we don't care about a connectionValidationInteger, and false
	// for low priority threads
	// I am creating two socketDesciptors, to create two sockets. One using IPV6 and the other IPV4
	RakNet::SocketDescriptor socketDescriptor(port, 0);
	socketDescriptor.socketFamily = AF_INET;
	bool b = server->Startup(4, &socketDescriptor, 1) == RakNet::RAKNET_STARTED;
	server->SetMaximumIncomingConnections(4);
	if (!b)
	{
		puts("Server failed to start.");
		return false;
	}
	server->SetOccasionalPing(true);
	server->SetUnreliableTimeout(1000);

	DataStructures::List< RakNet::RakNetSocket2* > sockets;
	server->GetSockets(sockets);
	printf("Socket addresses used by RakNet:\n");
	for (unsigned int i = 0; i < sockets.Size(); i++)
	{
		printf("%i. %s\n", i + 1, sockets[i]->GetBoundAddress().ToString(true));
	}

	printf("\nMy IP addresses:\n");
	for (unsigned int i = 0; i < server->GetNumberOfAddresses(); i++)
	{
		RakNet::SystemAddress sa = server->GetInternalID(RakNet::UNASSIGNED_SYSTEM_ADDRESS, i);
		printf("%i. %s (LAN=%i)\n", i + 1, sa.ToString(false), sa.IsLANAddress());
	}

	printf("\nMy GUID is %s\n", server->GetGuidFromSystemAddress(RakNet::UNASSIGNED_SYSTEM_ADDRESS).ToString());

	// Loop for input
	while (1)
	{
		// This sleep keeps RakNet responsive
		RakSleep(broadcastInterval);
		server->Send(broadcastMsg, broadcastMsgLen, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	}

	server->Shutdown(300);
	// We're done with the network
	RakNet::RakPeerInterface::DestroyInstance(server);

	return true;
}
