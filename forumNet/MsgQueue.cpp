#include "MsgQueue.h"



void MsgQueue::MsgPush(Message msg)
{	
	std::lock_guard <std::mutex> lock(_mutex);
	cout << "push msg..." << endl;
	_queue.push(msg);
	_condition.notify_one();
}


bool MsgQueue::MsgPop(Message& msg, bool isBlocked)
{
	if (isBlocked) {
		std::unique_lock <std::mutex> lock(_mutex);
		while (_queue.empty()) {
			cout << "MsgQueuePop is Blocking,waiting..." << endl;
			_condition.wait(lock);
		}
	}
	else   
	{
		std::lock_guard <std::mutex> lock(_mutex);
		if (_queue.empty()) {
			return false;
		}
	}

	msg = std::move(_queue.front());
	//cout << "_queue.pop()" << endl;
	_queue.pop();
	return true;
}


int MsgQueue::Size()
{
	std::lock_guard <std::mutex> lock(_mutex);
	return _queue.size();
}


bool MsgQueue::IsEmpty()
{
	std::lock_guard <std::mutex> lock(_mutex);
	return _queue.empty();
}
