#include <cstdio>
#include <csignal>
#include "NetServer.h"
#include "Message.h"
#include "MsgQueue.h"
#include "MsgMap.h"
#include "MsgQueueManager.h"
#include "MsgMapManager.h"
#include "HttpServer.h"

void signalHandler( int signum)
{
	cout << "send signal exception" << signum << endl;
}
/*void signalHandler2(int signum)
{
	cout << "send signal exception" << signum << endl;
	kill(pid, SIG_KILL);
}*/
int main()
{
    printf("hello from forumNet!\n");
	signal(SIGPIPE, signalHandler);
	//signal(SIGINT, signalHandler2);

	MsgQueue TCPDataRecv;
	MsgMap TcpDataSend;
	/*int test;
	Message mmsg;
	TcpDataSend.MsgInsert(test, mmsg);*/
	string MsgName("TCPDataSend");
	MsgQueueManager::GetInstance().Register("TCPDataRecv", &TCPDataRecv);
	MsgMapManager::GetInstance().Register(MsgName, &TcpDataSend);
	NetServer netServer;
	//netServer.StartServer();
	HttpServer httpServer1;
	HttpServer httpServer2;
	//httpServer.StartHttpServer();bind(&Foo::foo, &f, i)
	//netServer.StartThread();
	//httpServer.StartThread();
	//std::thread th1(&NetServer::NetServerCallback,&netServer);
	netServer.InitServer();
	std::thread th1(&NetServer::NetServerCallback, &netServer);
	std::thread th2(&NetServer::NetServerCallback, &netServer);
	//std::thread th3(&NetServer::NetServerCallback, &netServer);
	//std::thread th5(&NetServer::NetServerCallback, &netServer);
	std::thread th4(&HttpServer::HttpServerCallback,&httpServer1);
	//std::thread th5(&HttpServer::HttpServerCallback, &httpServer2);
	th1.join();
	th2.join();
	//th3.join();
	th4.join();
	//th5.join();
	//httpServer1.StartHttpServer();
	//netServer.StartServer();
	
    return 0;
}