#pragma once
#include <iostream>
#include "Task.h"
class AuthenticationServer
{
public:
	AuthenticationServer();
	virtual ~AuthenticationServer();
	bool CheckUser(Task authenticationTask);

};

