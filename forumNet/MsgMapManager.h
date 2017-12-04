#pragma once
#include <iostream>
#include <map>
#include "MsgMap.h"
#include "Message.h"

class MsgMapManager
{
public:
	//获取单个实例
	static MsgMapManager& GetInstance();
	MsgMapManager(const MsgMapManager&) = delete;
	const MsgMapManager& operator=(const MsgMapManager&) = delete;
	virtual ~MsgMapManager();
	void Register(string name, MsgMap* msgMap);
	void InsertMsg(string name, int fd, Message message);
	void GetMsg(string name, int fd, Message& message);
	void DeleteMsg(string name, int fd);
	int GetMapSize(string name);

private:
	std::map <string, std::shared_ptr<MsgMap>> _MsgMaps;
	MsgMapManager();
};
