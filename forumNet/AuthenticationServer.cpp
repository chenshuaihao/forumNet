#include "AuthenticationServer.h"

using namespace std;

AuthenticationServer::AuthenticationServer()
{
}


AuthenticationServer::~AuthenticationServer()
{
}

bool AuthenticationServer::CheckUser(Task authenticationTask)
{
	//cout << "in the function: 'AuthenticationServer::CheckUser' " << endl;
	//data
	if (authenticationTask.userID == "002426" &&
		authenticationTask.accessToken == "298a7098e037dc")
	{
		cout << "check right" << endl;
		return true;
	}
	else
	{
		return false;
	}
}
