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
		if (currentAccount && currentAccount->isStu()) {
			cout << "1. 信息查询\n";
			cout << "2. 活动管理\n";
			cout << "3. 课程导航\n";
			cout << "4. 账户管理\n";
			cout << "5. 系统设置\n";
			// cout << "27.功能测试\n";
			cout << "0. 退出程序\n";
			int select = getNumber();
			switch (select) {
				case 1://信息查询
					logger.write("Info query.\n");
					infoQuery();
					break;
				case 2://活动管理
					logger.write("Activity manage.\n");
					activityMenu();
					break;
				case 3://课程导航
					logger.write("Course navigate.\n");
					courseNavigate();
					break;
				case 4://账户管理
					logger.write("Account manage.");
					accountMenu();
					break;
				case 5://系统设置
					settings();
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
		} else {
			cout << "1. 课程安排\n";
			cout << "2. 活动管理\n";
			cout << "3. 查看日志\n";
			cout << "4. 账户管理\n";
			cout << "5. 系统设置\n";
			// cout << "27.功能测试\n";
			cout << "0. 退出程序\n";
			int select = getNumber();
			switch (select) {
				case 1://课程安排
					logger.write("Course arrange.");
					courseArrange();
					break;
				case 2://活动管理
					logger.write("Activity manage.");
					activityMenu();
					break;
				case 3://查看日志
					logger.write("View log file.");
					logger.launchFile();
					break;
				case 4://账户管理
					logger.write("Account manage.");
					accountMenu();
					break;
				case 5://系统设置
					settings();
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
	}
	logger.write("exit.", Log::SYS);
}
void UI::settings() {
	cout << "调整模拟时间的速度。当前速度:现实每 1 秒模拟系统" << currentTime.speed << "秒\n";
	cout << "请输入新的速度，输入0表示不修改\n";
	int newSpeed = getNumber();
	if (newSpeed > 0) {
		currentTime.setSpeed(newSpeed);
		logger.write("Time speed set to" + to_string(newSpeed) + ".");
	} else {
		cout << "速度未修改。\n";
	}
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
 * @brief 读取用户的输入的一个给定范围内的整数（默认范围为int范围内正整数），消除末尾回车，处理输入的错误
 */
int UI::getNumber(int lb, int hb) {
	int ret;
	bool input = false;
	while (!input) {
		std::cin >> ret;
		if (cin.fail() || ret<lb || ret>hb) {
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
	// logger.write("Input path " + path);
	return path;
}
