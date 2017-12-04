#include "NetServer.h"

using namespace std;

void setnonblocking(int socked)
{
	//cout << "setnonblocking" << endl;
	int opts = fcntl(socked, F_GETFL);
	if (opts<0)
	{
		perror("fcntl(socked,GETFL)");
		exit(1);
	}
	if (fcntl(socked, F_SETFL, opts | O_NONBLOCK)<0)
	{
		perror("fcntl(sock,SETFL,opts)");
		exit(1);
	}
}


void NetServer::SetServerName(string & serverName)
{
	_ServerName = serverName;
}

void NetServer::SetServerIp(string serverIP)
{
	_ServerIP = serverIP;
}

void NetServer::SetServerPort(int serverPort)
{
	_ServerPort = serverPort;
}

void NetServer::Accept(int serverSocket,int epfd)
{
	socklen_t lengthofsockaddr;
	int clientSocket;
	struct epoll_event ev;
	struct sockaddr_in clientaddr;

	lengthofsockaddr = sizeof(clientaddr);
	std::lock_guard <std::mutex> lock(_mutex);
	//2017.12.01 新增for循环，接收更多socket
	//int i = 1;
	//while(i--)
	for (;;)
	{
		clientSocket = accept(serverSocket, (struct sockaddr*)&clientaddr, &lengthofsockaddr);
		if (clientSocket < 0) {
			perror("clientSocket<0");
			break;
		}
		++_ServerRequestCount;
		//cout << "client clientSocket:" << clientSocket << endl;
		cout << "client IP:" << inet_ntoa(*(struct in_addr *)&(clientaddr.sin_addr.s_addr)) << endl;
		setnonblocking(clientSocket);
		ev.data.fd = clientSocket;
		ev.events = EPOLLIN | EPOLLET;
		epoll_ctl(epfd, EPOLL_CTL_ADD, clientSocket, &ev);
	}

}

/*返回true时，表示接收正常。*/
bool NetServer::ReadHandle(int clientSocket)
{
	//cout << "in the function: 'NetServer::ReadHandle' " << endl;
	ssize_t nByte;
	Message msg;
	char buffer[MAXSIZE];
	memset(buffer, '\0', 1024);
	//cout << buffer << endl;
	msg.Setfd(clientSocket);
	for (;;)
	{	
		/*思考：数据的完整性接收*/
		/*问题定位，本来应该read客户端发送来的数据，结果现在read到部分之前发送给客户端的数据*/
		//cout << "server is recving data..." << endl;
		nByte = recv(clientSocket, buffer, sizeof(buffer), 0);
		//cout << "+++++ nByte: " << nByte << endl;
		if (nByte > 0)
		{
			/*目前假设一次性可以接收完全客户端的数据
			* 按道理应该又应用层决定是否接收完
			*/
			msg.AppendData(buffer);
			//string sss = buffer;
			//sss.substr()
			//sss.erase(nByte,sss.size()-nByte);
			//cout << sss << endl;

			//cout << "log_string recv buffer: \n" << buffer << endl;
			memset(buffer, '\0', 1024);
			break;
		}
		else if (nByte < 0)//异常
		{
			if (errno == EAGAIN)//系统缓冲区未有数据，非阻塞返回
			{
				cout << "errno == EAGAIN" << endl;
				break;
			}
			else if (errno == EINTR)
			{
				cout << "errno == EINTR" << endl;
				continue;
			}
			else
			{
				cout << "recv error" << endl;
				close(clientSocket);
				return false;
			}
		}
		else//返回0，客户端关闭socket
		{
			cout << "client close the Socket" << endl;
			close(clientSocket);
			return false;
		}
	}
	//cout << msg.GetData() << endl;
	MsgQueueManager::GetInstance().PostMsg("TCPDataRecv", msg);
	return true;
}

void NetServer::WriteHandle(int clientSocket)
{
	//cout << "in the function: 'NetServer::WriteHandle' " << endl;
	ssize_t nByte;
	Message message;
	MsgMapManager::GetInstance().GetMsg("TCPDataSend", clientSocket, message);
	string msg = message.GetData();
	char buffer[MAXSIZE+1];
	//ssize_t pos = 0;
	//size_t length = msg.copy(buffer, MAXSIZE, pos);
	size_t length = msg.copy(buffer, MAXSIZE, 0);
	buffer[length] = '\0';
	//debug
	//cout << "the msg is: \n" <<msg << endl;
	for (;;)
	{
		//cout << "sending data" << endl;
		//BUG，信号异常捕捉
		/*sending data
		send signal exception13
		+++++ nByte: -1
		errno: 32
		recv: Broken pipe
		write error
		*/
		nByte = send(clientSocket, buffer, length, 0);
		
		//if (nByte < MAXSIZE) sleep(1);
		//cout << "+++++ nByte: " << nByte << endl;
		if (nByte > 0)
		{
			//cout << buffer << endl;
			//pos += nByte;
			msg.erase(0, nByte);
			length = msg.copy(buffer, MAXSIZE, 0);
			buffer[length] = '\0';
			if (length == 0 )
			{
				std::lock_guard <std::mutex> lock(_mutex);
				++_success_cnt;
				cout << "done !! length == 0"  << endl;
				MsgMapManager::GetInstance().DeleteMsg("TCPDataSend", clientSocket);
				break;
			}
			//cout << "!!! sleep 5555555555555"<< endl;
			//sleep(5);
		}
		else if (nByte < 0)//异常,重大错误，之前为if，结果造成与下面的else构成组合，socket被关闭
		{
			cout << errno << endl;
			perror("recv");
			if (errno == EAGAIN)//系统缓冲区满，非阻塞返回
			{
				cout << "write errno == EAGAIN" << endl;
				cout << "not finish!" << endl;
				return;
			}
			else if (errno == EINTR)
			{
				cout << "write errno == EINTR" << endl;
				continue;
			}
			else
			{
				cout << "write error" << endl;
				MsgMapManager::GetInstance().DeleteMsg("TCPDataSend", clientSocket);
				break;
			}
		}
		else//返回0
		{
			cout << "client close the Socket!" << endl;
			MsgMapManager::GetInstance().DeleteMsg("TCPDataSend", clientSocket);
			//close(clientSocket);
			break;
		}
	}
	//MsgMapManager::GetInstance().DeleteMsg("TCPDataSend", clientSocket);
	//cout << "MapSize: " << MsgMapManager::GetInstance().GetMapSize("TCPDataSend") << endl;
	//cout << "close clientSocket: " << clientSocket << endl;
	close(clientSocket);
}

void NetServer::InitServer()
{
	cout << "in the function: 'NetServer::InitServer' " << endl;
	_ServerState = running;
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket == -1)
	{
		perror("error socket");
		exit(-1);
	}
	int on = 1;
	setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);//inet_addr(_ServerIP.c_str());
	serveraddr.sin_port = htons(_ServerPort);
	int resval = bind(_serverSocket, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (resval == -1)
	{
		close(_serverSocket);
		perror("error bind");
		exit(-1);
	}
	resval = listen(_serverSocket, 10);
	if (resval< 0)
	{
		perror("error listen");
		close(_serverSocket);
		exit(-1);
	}
	setnonblocking(_serverSocket);
	//return serverSocket;
}

void NetServer::StartServer()
{
	cout << "in the function: 'NetServer::StartServer' " << endl;
	int cnt_nfds=0;
	int i;
	int epfd;
	int nfds;
	//int serverSocket;
	int clientSocket;
	struct epoll_event ev, events[20];
	//struct sockaddr_in clientaddr;
	_success_cnt = 0;
	//初始化
	//setnonblocking(_serverSocket);
	//InitServer();
	ev.data.fd = _serverSocket;
	ev.events = EPOLLIN | EPOLLET;
	
	epfd = epoll_create(100);
	epoll_ctl(epfd, EPOLL_CTL_ADD, _serverSocket, &ev);

	//NEW test fork
	/*
	for (int ii = 0; ii < 1; ++ii)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			for (;;)
			{
				///cout << "---------------------------------------" << endl;
				///cout << "-----------wait for client-------------" << endl;
				cout << "-----------_success_cnt---==--------=--" << _success_cnt << endl;
				cout << "-----------_success_nfds---==--------=--" << cnt_nfds << endl;
				///cout << "---------------------------------------" << endl;
				nfds = epoll_wait(epfd, events, 20, 20);
				///cout << "+++++ nfds: "<< nfds << endl;
				if (nfds > 1)
				{
					++cnt_nfds;
				}
				cout << "***********ServerRequestCount:" << _ServerRequestCount << "..........." << endl;
				///cout << "+++++ QueueSize: " << MsgQueueManager::GetInstance().GetQueueSize("TCPDataRecv") << endl;
				///cout << "+++++ MapSize: " << MsgMapManager::GetInstance().GetMapSize("TCPDataSend") << endl; 
				for (i = 0; i<nfds; ++i)
				{
					///cout << "**********handel event" << endl;
					///cout << "event type:" << events[i].events << endl;
					if (events[i].data.fd == _serverSocket)
					{
						///cout << "+++++ QueueSize: " << MsgQueueManager::GetInstance().GetQueueSize("TCPDataRecv") << endl;
						///cout << "+++++ MapSize: " << MsgMapManager::GetInstance().GetMapSize("TCPDataSend") << endl;
						//新连接请求
						cout << "@@@@@ Accept" << endl;
						++_ServerRequestCount;

						//cout << "*****************************************" << endl;
						//cout << "*****************************************" << endl;
						//cout << "***********ServerRequestCount:" << _ServerRequestCount << "***********" << endl;
						//cout << "***********ServerRequestCount:" << _ServerRequestCount << "***********" << endl;
						//cout << "***********ServerRequestCount:" << _ServerRequestCount << "***********" << endl;
						//cout << "***********ServerRequestCount:" << _ServerRequestCount << "***********" << endl;

						Accept(_serverSocket, epfd);
					}
					else if (events[i].events == EPOLLRDHUP)//对方关闭socket事件
					{
						cout << "EPOLLRDHUP" << endl;
						cout << "client close the socket!" << endl;
						clientSocket = events[i].data.fd;
						close(clientSocket);
					}
					else if (events[i].events == EPOLLIN)//读事件
					{
						//debug
						//cout << "EPOLLIN" << endl;
						clientSocket = events[i].data.fd;
						if (clientSocket < 0)
						{
							continue;
						}
						bool readResult = ReadHandle(clientSocket);
						if (readResult)
						{
							ev.data.fd = clientSocket;
							ev.events = EPOLLOUT | EPOLLET;
							epoll_ctl(epfd, EPOLL_CTL_MOD, clientSocket, &ev);
						}
						else
						{
							ev.data.fd = clientSocket;
							ev.events = EPOLLIN | EPOLLET;
							epoll_ctl(epfd, EPOLL_CTL_DEL, clientSocket, &ev);
						}
					}
					else if (events[i].events == EPOLLOUT)//写事件
					{
						//cout << "EPOLLOUT" << endl;
						//cout << "send data to client" << endl;
						clientSocket = events[i].data.fd;
						if (clientSocket < 0)
						{
							continue;
						}
						WriteHandle(clientSocket);
						//ev.data.fd = clientSocket;
						//ev.events = EPOLLIN | EPOLLET;
						//epoll_ctl(epfd, EPOLL_CTL_MOD, clientSocket, &ev);
					}
					else
					{
						close(clientSocket);
						cout << "UNKNOWN EVENT" << endl;
					}
				}
				//cout << "sleep for 1s"<< endl;
				//sleep(1);
			}
		}
		else
		{
			cout << "====== fork father" << endl;
			cout << "continue to create process" << endl;
		}
	}*/

	for (;;)
			{
				///cout << "---------------------------------------" << endl;
				///cout << "-----------wait for client-------------" << endl;
				cout << "-----------_success_cnt---==--------=--" << _success_cnt << endl;
				//cout << "-----------_success_nfds---==--------=--" << cnt_nfds << endl;
				cout << "---------------------------------------" << endl;
				
				nfds = epoll_wait(epfd, events, 20, 50);
				cout << "+++++ nfds: "<< nfds << endl;
				if (nfds > 1)
				{
					++cnt_nfds;
				}
				//cout << "----------------- nfds ----------------------------" << nfds << endl;
				//cout << "----------------- nfds ----------------------------" << nfds << endl;
				cout << "**********************ServerRequestCount:	" << _ServerRequestCount << endl;
				///cout << "+++++ QueueSize: " << MsgQueueManager::GetInstance().GetQueueSize("TCPDataRecv") << endl;
				///cout << "+++++ MapSize: " << MsgMapManager::GetInstance().GetMapSize("TCPDataSend") << endl; 
				for (i = 0; i<nfds; ++i)
				{
					///cout << "**********handel event" << endl;
					///cout << "event type:" << events[i].events << endl;
					if (events[i].data.fd == _serverSocket)
					{
						///cout << "+++++ QueueSize: " << MsgQueueManager::GetInstance().GetQueueSize("TCPDataRecv") << endl;
						///cout << "+++++ MapSize: " << MsgMapManager::GetInstance().GetMapSize("TCPDataSend") << endl;
						//新连接请求
						//cout << "@@@@@ Accept" << endl;
												//cout << "*****************************************" << endl;
						//cout << "*****************************************" << endl;
						//cout << "***********ServerRequestCount:" << _ServerRequestCount << "***********" << endl;
						//cout << "***********ServerRequestCount:" << _ServerRequestCount << "***********" << endl;
						//cout << "***********ServerRequestCount:" << _ServerRequestCount << "***********" << endl;
						//cout << "***********ServerRequestCount:" << _ServerRequestCount << "***********" << endl;

						Accept(_serverSocket, epfd);
					}
					else if (events[i].events == EPOLLRDHUP)//对方关闭socket事件
					{
						cout << "EPOLLRDHUP" << endl;
						cout << "client close the socket!" << endl;
						clientSocket = events[i].data.fd;
						close(clientSocket);
					}
					else if (events[i].events == EPOLLIN)//读事件
					{
						//debug
						cout << "EPOLLIN" << endl;
						clientSocket = events[i].data.fd;
						if (clientSocket < 0)
						{
							continue;
						}
						bool readResult = ReadHandle(clientSocket);
						if (readResult)
						{
							ev.data.fd = clientSocket;
							ev.events = EPOLLOUT | EPOLLET;
							epoll_ctl(epfd, EPOLL_CTL_MOD, clientSocket, &ev);
						}
						else
						{
							ev.data.fd = clientSocket;
							ev.events = EPOLLIN | EPOLLET;
							epoll_ctl(epfd, EPOLL_CTL_DEL, clientSocket, &ev);
						}
					}
					else if (events[i].events == EPOLLOUT)//写事件
					{
						cout << "EPOLLOUT" << endl;
						//cout << "send data to client" << endl;
						clientSocket = events[i].data.fd;
						if (clientSocket < 0)
						{
							continue;
						}
						WriteHandle(clientSocket);
						//ev.data.fd = clientSocket;
						//ev.events = EPOLLIN | EPOLLET;
						//epoll_ctl(epfd, EPOLL_CTL_MOD, clientSocket, &ev);
					}
					else
					{
						close(clientSocket);
						cout << "UNKNOWN EVENT" << endl;
					}
				}
				//cout << "sleep for 1s"<< endl;
				//sleep(2);
			}
}

void NetServer::StopServer()
{
	close(_serverSocket);

	cout << "Hello world!" << endl;
}

void NetServer::NetServerCallback(NetServer * ptNetServer)
{
	//cout << "in the function: 'NetServer::StartServer' " << endl;
	ptNetServer->StartServer();
}
