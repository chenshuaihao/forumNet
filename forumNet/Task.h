#pragma once
#include <string>
#include "cJSON.h"
using namespace std;

class Task
{
public:
	string taskType;
	string userID;
	string accessToken;
	string resourceId;
	string resourceType;

	Task();
	virtual ~Task();

private:

};

