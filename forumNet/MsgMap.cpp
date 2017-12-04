#include "MsgMap.h"



void MsgMap::MsgInsert(int index, Message msg)
{
	std::unique_lock <std::mutex> lock(_mutex);
	_Map.insert({ index, msg });
	_condition.notify_one();
}


bool MsgMap::MsgGet(int& index, Message& msg, bool isBlocked)
{
	//cout << "in the function: 'MsgMap::MsgGet'" << endl;
	typename std::map<int, Message>::iterator it;
	if (isBlocked) {
		std::unique_lock <std::mutex> lock(_mutex);
		while ( _Map.end() == (it = _Map.find(index)) ) {
			cout << "_clientSocket: " << index << endl;
			cout << "MsgMap blocking: waiting new msg fill in..." << endl;
			_condition.wait(lock);
		}
	}
	else
	{
		std::lock_guard <std::mutex> lock(_mutex);
		if ( _Map.end() == (it = _Map.find(index)) ) {
			return false;
		}
	}
	
	msg = it->second;
	return true;
}


int MsgMap::Size()
{
	std::unique_lock <std::mutex> lock(_mutex);
	return _Map.size();
}


void MsgMap::Erase(int& index)
{
	std::unique_lock <std::mutex> lock(_mutex);
	//cout << "_Map.erase(index)" << endl;
	_Map.erase(index);
}


bool MsgMap::IsEmpty()
{
	return false;
}
