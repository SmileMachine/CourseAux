#ifndef ACCOUNT_H
#define ACCOUNT_H
#include "myhash.h"
#include <string>
struct UserInfo {
	enum Status {
		STUDENT,
		TEACHER,
		ADMIN,
		GUEST,
	};
	std::string name = {};	 //姓名
	std::string id = {};	 //学工号
	std::string group = {}; //用户所在的群组，如班级
	Status status;		 //该用户的身份
};
class AccountList;
class Password { //用户口令相关
	friend AccountList;
public:
	bool check(const char str[]); //输入密码,并检查是否匹配
	bool set(char str[]); //设置密码
	typedef unsigned int HashVal;
	bool isInputValid(const char str[]); //检查密码是否合规，如8-16位密码，仅包含数字和字母等
private:
	HashVal passwordHash; //密码的哈希值
};

class Account {
private:
public:
	UserInfo profile;  //用户信息
	Password password; //用户密码
};

class AccountList {
public:
	AccountList(const char path[]); // Todo:采用更佳的数据结构，方便查找
	void init(const char path[]);
	Account *findByID(std::string id); //通过ID查找
#define AL_SIZE 100				  //初步定为100
	Account list[AL_SIZE];
	int size = 0;
private:
};

extern AccountList accountList;
extern Account *currentAccount;
#endif
