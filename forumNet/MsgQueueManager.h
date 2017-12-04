#pragma once
#include <map>
#include "MsgQueue.h"
#include "Message.h"

using namespace std;

/*管理多个消息队列*/
class MsgQueueManager
{
public:
	//获取单个实例
	static MsgQueueManager& GetInstance();
	MsgQueueManager(const MsgQueueManager&) = delete;
	const MsgQueueManager& operator=(const MsgQueueManager&) = delete;
	virtual ~MsgQueueManager();
	void Register(string name, MsgQueue* msgQueue);
	void PostMsg(string name, Message message);
	void PopMsg(string name, Message& message);
	int GetQueueSize(string name);

private:
	std::map <string, std::shared_ptr<MsgQueue>> _MsgQueues;
	//单例化
	MsgQueueManager();
};

