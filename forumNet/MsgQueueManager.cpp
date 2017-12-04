#include "MsgQueueManager.h"

using namespace std;

MsgQueueManager::MsgQueueManager()
{

}


MsgQueueManager & MsgQueueManager::GetInstance()
{
	// µ¥Àý
	static MsgQueueManager Queuemanager;
	return Queuemanager;
}

MsgQueueManager::~MsgQueueManager()
{
}

void MsgQueueManager::Register(string name, MsgQueue * msgQueue)
{
	//cout << "in the function: 'MsgQueueManager::Register'" << endl;
	_MsgQueues.insert({ name, std::shared_ptr<MsgQueue>(msgQueue) });
}

void MsgQueueManager::PostMsg(string name, Message message)
{
	//cout << "in the function: 'MsgQueueManager::PostMsg'"<< endl;
	auto ptrMsgQueuePair = _MsgQueues.find(name);
	if (ptrMsgQueuePair != _MsgQueues.end())
	{
		//cout << "Queue Name:" << ptrMsgQueuePair->first << endl;
		ptrMsgQueuePair->second->MsgPush(message);
	}
}

void MsgQueueManager::PopMsg(string name, Message& message)
{
	cout << "in the function: 'MsgQueueManager::PopMsg'" << endl;
	auto ptrMsgQueuePair = _MsgQueues.find(name);
	if (ptrMsgQueuePair != _MsgQueues.end())
	{
		ptrMsgQueuePair->second->MsgPop(message, true);
	}
}

int MsgQueueManager::GetQueueSize(string name)
{
	auto ptrMsgQueuePair = _MsgQueues.find(name);
	if (ptrMsgQueuePair != _MsgQueues.end())
	{
		return ptrMsgQueuePair->second->Size();
	}
	cout << "there is no queue" << endl;
	return -1;
}

