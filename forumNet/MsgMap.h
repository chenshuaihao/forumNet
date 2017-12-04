#pragma once

#include <iostream>
#include <map>  
#include <stdint.h>
#include <mutex>  
#include <condition_variable> 
#include "Message.h"


using namespace std;


class MsgMap
{
	MsgMap& operator=(const MsgMap&) = delete;
	MsgMap(const MsgMap& other) = delete;

public:
	MsgMap() : _Map(), _mutex(), _condition() 
	{
		cout << "MsgMap Construction" << endl;
	}
	virtual ~MsgMap() {}

	void MsgInsert(int index, Message msg);
	bool MsgGet(int& index, Message& msg, bool isBlocked);
	int Size();
	void Erase(int& index);
	bool IsEmpty();

private:
	std::map <int, Message> _Map;
	mutable std::mutex _mutex;
	std::condition_variable _condition;
};


