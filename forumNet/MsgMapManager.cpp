#include "MsgMapManager.h"



MsgMapManager::MsgMapManager()
{
}


MsgMapManager & MsgMapManager::GetInstance()
{
	static MsgMapManager mapManager;
	return mapManager;
}

MsgMapManager::~MsgMapManager()
{
}

void MsgMapManager::Register(string name, MsgMap* msgMap)
{
	cout << "new map register" << endl;
	_MsgMaps.insert({ name, std::shared_ptr<MsgMap>(msgMap) });
}

void MsgMapManager::InsertMsg(string name, int fd, Message message)
{
	//cout << "MsgMapManager::InsertMsg" << endl;
	auto MsgMapPair = _MsgMaps.find(name);
	if (_MsgMaps.end() != MsgMapPair)
	{
		//cout << "InsertMsg..." << endl;
		//cout << "MapSize: " << MsgMapManager::GetInstance().GetMapSize(name) << endl;
		//BUG日志：insert前先判断是否已经存在，存在则删除
		MsgMapPair->second->Erase(fd);
		//cout << "MapSize: " << MsgMapManager::GetInstance().GetMapSize(name) << endl;
		MsgMapPair->second->MsgInsert(fd, message);
	}
}

void MsgMapManager::GetMsg(string name, int fd, Message& message)
{
	//cout << "MsgMapManager::GetMsg" << endl;
	auto MsgMapPair = _MsgMaps.find(name);
	if (_MsgMaps.end() != MsgMapPair)
	{
		//有待改进，改为非阻塞模式
		MsgMapPair->second->MsgGet(fd, message, true);
	}
}

void MsgMapManager::DeleteMsg(string name, int fd)
{
	//cout << "MsgMapManager::DeleteMsg" << endl;
	auto MsgMapPair = _MsgMaps.find(name);
	if (_MsgMaps.end() != MsgMapPair)
	{
		//cout << "Erase Msg" << endl;
		MsgMapPair->second->Erase(fd);
	}
	else
	{
		cout << "not find" << name << endl;
	}
}

int MsgMapManager::GetMapSize(string name)
{
	//cout << "MsgMapManager::GetMapSize" << endl;
	auto MsgMapPair = _MsgMaps.find(name);
	if (_MsgMaps.end() != MsgMapPair)
	{
		//cout << "wtf??" << endl;
		return MsgMapPair->second->Size();
	}
	cout << "there is no map" << endl;
	return -1;
}
