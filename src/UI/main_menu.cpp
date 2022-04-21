#include <iostream>
#include <cstdio>
#include <cstring>
#ifdef _WIN32
#include <conio.h>
#endif
#include "account.h"
#include "UI.h"
#include "log.h"
using namespace std;
bool UI::exit = 0;
bool UI::isLoggedIn = false;

/**
 * @brief 系统的入口。
 * 使用admin登录之后可以开启调试模式
 */
void UI::entry() {
	while (!exit) {
		cout << "欢迎来到线下课程辅助系统!\n";
		if (!isLoggedIn) {
			login();
			continue;
		}
		cout << "1. 账户管理\n";
		cout << "0. 退出程序\n";
		int select = getSelection();
		switch (select) {
			case 1:
				accountMenu();
				break;
			case 0:
				exit = true;
				break;
			case 27:
				// debugMenu();
			default:
				printf("%d\n", select);
				cout << "输入错误，请重试\n";
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
