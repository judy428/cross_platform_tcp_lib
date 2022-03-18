#ifndef _STRUCT_INFO_
#define _STRUCT_INFO_

enum CMD1
{
	CMD_LOGIN = 1,
	CMD_LOGIN_RESULT,
	CMD_LOGOUT,
	CMD_LOGOUT_RESULT,
	CMD_ERROR,
	CMD_NEWUSR_JOIN
};

// struct Login :public dataHeader
// {
// 	Login()
// 	{
// 		cmd = CMD_LOGIN;
// 		dataLength = sizeof(Login);
// 	}
// 	char usrName[32];
// 	char passWd[32];
// 	char data[932];
// };

// struct LoginResult :public dataHeader
// {
// 	LoginResult()
// 	{
// 		cmd = CMD_LOGIN_RESULT;
// 		dataLength = sizeof(LoginResult);
// 	}
// 	int result;
// 	char data[992];
// };

// struct  Logout :public dataHeader
// {
// 	Logout()
// 	{
// 		cmd = CMD_LOGOUT;
// 		dataLength = sizeof(Logout);
// 	}
// 	char usrName[32];
// };

// struct  LogoutResult :public dataHeader
// {
// 	LogoutResult()
// 	{
// 		cmd = CMD_LOGOUT_RESULT;
// 		dataLength = sizeof(LogoutResult);
// 	}
// 	int result;
// };

// struct  NewUsrJoin :public dataHeader
// {
// 	NewUsrJoin()
// 	{
// 		cmd = CMD_NEWUSR_JOIN;
// 		dataLength = sizeof(NewUsrJoin);
// 	}
// 	int sockid;
// };

struct Login 
{
	char passWd[32];
	char data[932];
	char usrName[32];
	~Login(){};
};

struct LoginResult
{

	int result;
	char data[992];
};

struct  Logout
{
	
	char usrName[32];
};

struct  LogoutResult 
{
	
	int result;
};

struct  NewUsrJoin 
{
	
	int sockid;
};

#endif