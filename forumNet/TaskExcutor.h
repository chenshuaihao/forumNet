#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "Task.h"
#include "CommentStruc.h"



class TaskExcutor
{
public:

	TaskExcutor();
	virtual ~TaskExcutor();
	vector<HomePageContentStr> GetDataFromDB(const Task& task);
	string Run(Task task);
	
private:
	string CreateHomePage(const Task& task);
	string CreateDynamicPage(const Task& task);

};

