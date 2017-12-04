#pragma once
#include <iostream>
#include <queue>
#include <mutex>  
#include <condition_variable> 
#include "Message.h"

class MsgQueue
{
	MsgQueue& operator=(const MsgQueue&) = delete;
	MsgQueue(const MsgQueue& other) = delete;

public:
	MsgQueue() : _queue(), _mutex(), _condition() 
	{
		cout << "MsgQueue Construction" << endl;
	}
	virtual ~MsgQueue() {}
	void MsgPush(Message msg);
	bool MsgPop(Message& msg, bool isBlocked);
	int Size();
	bool IsEmpty();


private:
	std::queue <Message> _queue;
	mutable std::mutex _mutex;
	std::condition_variable _condition;
};

