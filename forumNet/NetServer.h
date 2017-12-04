#pragma once
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <string.h>
#include <fcntl.h>
#include <string>
#include <arpa/inet.h>
#include <functional>
#include <thread>
#include <pthread.h>
#include "Message.h"
#include "MsgQueueManager.h"
#include "MsgMapManager.h"


#define MAXSIZE 1024
using namespace std;

class NetServer
{
public:
	NetServer() :
		_ServerRequestCount(0), _ServerName("Server0.1"), _ServerIP("127.0.0.1"), _ServerPort(80), _ServerState(stop), _mutex()
	{}
	NetServer(string &serverName, string &serverIP, int &serverPort) :
		_ServerName(serverName), _ServerIP(serverIP), _ServerPort(serverPort), _ServerState(stop)
	{}
	virtual ~NetServer() {}

	void SetServerName(string &serverName);
	void SetServerIp(string serverIP);
	void SetServerPort(int serverPort);
	void InitServer();
	void StartServer();
	void StopServer();
	static void NetServerCallback(NetServer* ptNetServer);

private:
	void Accept(int serverSocket, int epfd);
	bool ReadHandle(int clientSocket);
	void WriteHandle(int clientSocket);

	int _success_cnt;
	long long _ServerRequestCount;
	int _serverSocket;
	string _ServerName;
	string _ServerIP;
	int _ServerPort;

	mutable std::mutex _mutex;

	enum ServerState
	{
		stop,running		
	}_ServerState;
};

