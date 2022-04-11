#include <cstdio>
#include <cstring>
#include "account.h"
#include "data_io.h"
AccountList accountList("./data/account.csv"); //读取账户列表
Account* currentAccount; //在程序运行中的当前账户。
bool Password::input() { //输入密码,并检查是否匹配
	const int MAX_SIZE = 20;
	char str[MAX_SIZE];
	fgets(str, MAX_SIZE, stdin);
	if (str[strlen(str) - 1] == '\n') {
		str[strlen(str) - 1] = '\0';
	}
	return hash(str) == passwordHash;
}
bool Password::set(char str[]) { //设置密码
	if (isInputValid(str)) {
		passwordHash = hash(str);
		return true;
	} else {
		return false;
	}
}
Password::DataType Password::hash(const char str[]) { //将字符串映射为32位无符号整数,此处使用BKDR_Hash
	unsigned int seed = 31;
	DataType ret = 0;
	for (int i = 0; str[i]; i++) {
		ret = ret * seed + str[i];
	}
	return ret;
}
bool Password::isInputValid(const char str[]) { //检查密码是否合规，如8-16位密码，仅包含数字和字母等
	return true;
}

void Account::printInfo() {
	printf("name:\t%s\n", profile.name);
	printf("id:\t%s\n", profile.id);
	printf("group:\t%s\n", profile.group);
	printf("status:\t%d\n", profile.status);
	// status的打印可更改为字符串，如显示"student"。当前效果：显示'0'。
}

AccountList::AccountList(const char path[]) {
	/* Todo:
	 * 1. 采用更佳的数据结构存储，方便查找。
	 * 2. 修改数据在文件的保存格式，方便读取。
	 */
	DataReader reader(path);
	for (int i = 0; !reader.isEOF(); i++) {
		reader.nextTerm(list[i].profile.name, 20);	//姓名
		reader.nextTerm(list[i].profile.id, 15);	//学号
		reader.nextTerm(list[i].profile.group, 15); //群组
		char tmp[30];
		reader.nextTerm(tmp, 30);
		sscanf(tmp, "%d", &list[i].profile.status); //权限
		reader.nextTerm(tmp, 30);
		if (tmp[0]) {	//检查是否存有密码
			sscanf(tmp, "%d", &list[i].password.passwordHash); //读入密码
		} else { //如果不存在，则默认密码为学号
			list[i].password.passwordHash =
				Password::hash(list[i].profile.id);
		}
		size++;
	}
}
Account* AccountList::findByID(char id[]) { // Todo:顺序查找，有待优化
	for (int i = 0; i < size; i++) {
		if (strcmp(id, list[i].profile.id) == 0) { // strcmp或许需要自己写一个?
			return &list[i];
		}
	}
	return NULL; //  Todo:存在空引用问题，需要解决。
}
