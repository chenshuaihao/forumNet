#pragma once
#include <map>
#include <string>
#include <vector>
#include <queue>
#include "Task.h"
#include "TaskExcutor.h"
//#include "UrlCodeSolution.h"
#include "AuthenticationServer.h"

using namespace std;

class TaskManager
{
public:
	TaskManager() : _taskQueue(), _scheduleResult() {}
	virtual ~TaskManager();
	void CreateTask(string httpBody);
	void TaskScheduling();
	string GetScheduleResult();

private:
	queue<Task> _taskQueue;
	queue<string> _scheduleResult;
	//vector<Task> _taskVector;

};

