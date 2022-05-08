#include <cstdio>
#include <cstring>
#include <string>
#include "account.h"
#include "data_io.h"
#include "myhash.h"
AccountList accountList("./data/account.csv"); //读取账户列表
Account *currentAccount;					   //在程序运行中的当前账户。

/**
 * @brief 检查输入的密码是否匹配
 * @param str 要检查的输入
 */
bool Password::check(const char str[]) {
	return myhash::stringHash(str) == passwordHash;
}
/**
 * @brief 将密码设置成指定的字符串
 * @param str 将要设置的密码
 */
bool Password::set(char str[]) { //设置密码
	if (isInputValid(str)) {
		passwordHash = myhash::stringHash(str);
		return true;
	} else {
		return false;
	}
}
/**
 * @brief [未实现]检查密码是否合规，如8-16位密码，仅包含数字和字母等
 * @param str 要检查的密码
 */
bool Password::isInputValid(const char str[]) {
	return true;
}

AccountList::AccountList(const char path[]) {
	init(path);
}
/**
 * @brief 从文件中读取账户信息
 * @param path 要读取的文件路径
 */
void AccountList::init(const char path[]) {
	/* Todo:
	 * 1. 采用更佳的数据结构存储，方便查找。
	 * 2. 修改数据在文件的保存格式，方便读取。
	 */
	DataReader reader(path);
	for (int i = 0; !reader.isEOF(); i++) {
		reader.nextItem(list[i].profile.name);	//姓名
		reader.nextItem(list[i].profile.id);	//学号
		reader.nextItem(list[i].profile.group); //群组
		std::string tmp;
		reader.nextItem(tmp);
		sscanf(tmp.c_str(), "%d", &list[i].profile.status); //权限
		reader.nextItem(tmp);
		if (tmp.length()) {//检查是否存有密码
			sscanf(tmp.c_str(), "%d", &list[i].password.passwordHash); //读入密码
		} else { //如果不存在，则默认密码为学号
			list[i].password.passwordHash = myhash::stringHash(list[i].profile.id);
		}
		size++;
		// 	reader.nextTerm(list[i].profile.name, 20);	//姓名
		// 	reader.nextTerm(list[i].profile.id, 15);	//学号
		// 	reader.nextTerm(list[i].profile.group, 15); //群组
		// 	char tmp[30];
		// 	reader.nextTerm(tmp, 30);
		// 	sscanf(tmp, "%d", &list[i].profile.status); //权限
		// 	reader.nextTerm(tmp, 30);
		// 	if (tmp[0]) {//检查是否存有密码
		// 		sscanf(tmp, "%d", &list[i].password.passwordHash); //读入密码
		// 	} else { //如果不存在，则默认密码为学号
		// 		list[i].password.passwordHash = myhash::stringHash(list[i].profile.id);
		// 	}
		// 	size++;
	}
}
/**
 * @brief [待优化]通过id查找账户，
 * @param id 要查找的id
 * @return Account* 返回找到的账户指针
 */
Account *AccountList::findByID(std::string id) { // Todo:顺序查找，有待优化
	for (int i = 0; i < size; i++) {
		if (myhash::stringHash(id) == myhash::stringHash(list[i].profile.id)) {
			return &list[i];
		}
	}
	return NULL; //  Todo:存在空引用问题，需要解决。
}
