#pragma once
#include <string>

using namespace std;
class Message
{
public:
	Message() : _fd(0) {}
	virtual ~Message() {}
	void Setfd(int fd)
	{
		_fd = fd;
	}
	void SetData(string data)
	{
		_Data = data;
	}
	void AppendData(string s)
	{
		_Data += s;
	}
	int Getfd()
	{
		return _fd;
	}
	string GetData()
	{
		return _Data;
	}
private:
	//¶Ô¶ËsocketÌ×½Ó×Ö
	int _fd;
	string _Data;
};

