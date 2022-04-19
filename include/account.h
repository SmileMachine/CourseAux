#ifndef ACCOUNT_H
#define ACCOUNT_H
struct UserInfo {
	enum Status {
		STUDENT,
		TEACHER,
		ADMIN,
		GUEST,
	};
	char name[20] = {};	 //姓名
	char id[15] = {};	 //学工号
	char group[15] = {}; //用户所在的群组，如班级
	Status status;		 //该用户的身份
};
struct Password { //用户口令相关
public:
	bool input();		  //输入密码,并检查是否匹配
	bool set(char str[]); //设置密码
	typedef unsigned int DataType;
	static DataType hash(const char str[]);
	//将字符串映射为32位无符号整数,此处使用BKDR_Hash
	bool isInputValid(const char str[]);
	//检查密码是否合规，如8-16位密码，仅包含数字和字母等
	DataType passwordHash; //密码的哈希值
private:
};

class Account {
public:
	UserInfo profile;  //用户信息
	Password password; //用户密码
	void printInfo();
};

class AccountList {
public:
	AccountList(const char path[]); // Todo:采用更佳的数据结构，方便查找
	bool init(const char path[]);
	Account *findByID(char id[]); //通过ID查找
#define AL_SIZE 100				  //初步定为100
	Account list[AL_SIZE];

private:
	int size = 0;
};

extern AccountList accountList;
extern Account *currentAccount;
#endif
