#include <iostream>
#include <cstdio>
#include <cstring>
// #ifdef _WIN32
// #include <conio.h>
// #endif
#include <cstdlib>
#include "account.h"
#include "UI.h"
#include "log.h"
#include <io.h>
#include "sim_time.h"
using namespace std;

bool UI::exit = 0;
bool UI::isLoggedIn = false;
bool UI::CLEAR = false;
/**
 * @brief 系统的入口。
 * 使用admin登录之后可以开启调试模式
 */
void UI::entry() {
	while (!exit) {
		CLEAR&&system("cls");
		cout << "欢迎来到线下课程辅助系统!\n";
		cout << "当前时间: " << currentTime.toString() << endl;
		if (!isLoggedIn) {
			login();
			continue;
		}
		cout << "1. 信息查询\n";
		cout << "2. 活动管理\n";
		cout << "3. 课程导航\n";
		cout << "4. 账户管理\n";
		// cout << "27.功能测试\n";
		cout << "0. 退出程序\n";
		int select = getSelection();
		switch (select) {
			case 1://信息查询
				infoQuery();
				break;
			case 2://活动管理
				activityMenu();
				break;
			case 3://课程导航
				courseNavigate();
				break;
			case 4://账户管理
				accountMenu();
				break;
			case 0://退出
				exit = true;
				break;
			case 27://功能测试
				debugMenu();
				break;
			default:
				printf("%d: ", select);
				cout << "请输入正确的选项\n";
				break;
		}
	}
	logger.write("exit.", Log::SYS);
}

/**
 * @brief 从stdin中读取一行字符串，不含末尾回车。
 * @param len 字符串的长度上限
 * @param input 用于接收输入的字符串
 * @return char* 返回字符串的指针。
 */
char* UI::getString(char input[], int len) {
	char* ret = fgets(input, len, stdin);
	if (input[strlen(input) - 1] == '\n') {
		input[strlen(input) - 1] = '\0';
	}
	return ret;
}

/**
 * @brief 读取用户的输入的一个整数，消除末尾回车，处理输入的错误
 */
int UI::getSelection() {
	int ret;
	bool input = false;
	while (!input) {
		std::cin >> ret;
		if (cin.fail()) {
			cout << "输入有误，请重新输入\n";
			cin.clear();//流标志复位
			cin.sync(); //同步缓冲区
		} else {
			input = true;
		}
	}
	// cin.get(); //读取回车
	cin.ignore(10, '\n');//读取回车
	return ret;
}

/**
 * @brief 判断文件或路径是否存在
 * @param path 路径
 */
bool UI::isFileExist(string path) {
	return !_access(path.c_str(), F_OK);
}

/**
 * @brief 读取用户输入的一个文件路径，文件不存在时提示用户重新输入。
 * @return 文件的路径
 */
string UI::getPath() {
	string path;
	getline(cin, path);
	while (!isFileExist(path)) {
		cout << "文件不存在，请重新输入\n";
		cin >> path;
	}
	return path;
}
