#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <functional>
#include <pthread.h>
#include "cJSON.h"
#include "Task.h"
#include "Message.h"
#include "MsgQueueManager.h"
#include "MsgMapManager.h"
#include "TaskExcutor.h"
#include "TaskManager.h"



typedef struct _Http_Header {
	string method;
	string url;
	string version;
	map<string, string> header;
	string body;
}Http_Header;

using namespace std;
class HttpServer
{
public:
	HttpServer() : _HttpServerState(stop), _HttpCount(0) {}
	virtual ~HttpServer() {}
	//从TcpData队列获取数据Message
	Message RecvRequestData();
	//Http报文解析，解析出请求报头和用户数据
	void HttpParser(string data, Http_Header* pthttp_Header);
	//用户数据处理，交给Json解析器，得到解析结果，然后加入任务队列或者直接交给数据库操作
	//void DataHandler(string httpMessage);
	void DataHandler(Http_Header* pthttp_Header);
	//生成Http响应报头
	void CreateResponseHeader();
	//获取响应数据
	void GetResponseBody();
	//返回数据，插入MsgMap
	void SendResonseData();

	void StartHttpServer();
	void StopHttpServer();
	static void HttpServerCallback(HttpServer* ptHttpServer);

private:
	long long _HttpCount;
	int _clientSocket;
	string _RequestHeader;
	string _RequestData;
	string _ResponseHeader;
	string _ResponseData;
	string _HttpServerName;

	enum HttpServerState
	{
		stop, running
	}_HttpServerState;


};

