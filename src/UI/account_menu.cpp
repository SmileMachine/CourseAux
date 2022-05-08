#include <cstdio>
#include <iostream>
#include <cstdlib>
#include "account.h"
#include "UI.h"
#include "log.h"
using namespace std;
// 登录界面
void UI::login() {
	CLEAR&&system("cls");
	cout << "1. 登录\n";
	cout << "2. 退出\n";
	while (true) {
		int select = getSelection();
		if (select == 1) {
			break;
		} else if (select == 2) {
			exit = true;
			return;
		} else {
			cout << "输入错误，请重试\n";
		}
	}
	while (!isLoggedIn) {
		cout << "学号: ";
		char idInput[20];
		getString(idInput, 20);
		currentAccount = accountList.findByID(idInput);
		if (!currentAccount) {
			cout << "学号不存在。\n";
			continue;
		}
		for (int i = 0; i < 3; i++) { //密码输入可以尝试3次
			cout << "密码: ";
			char pwInput[20];
			getString(pwInput, 20);
			isLoggedIn = currentAccount->password.check(pwInput);
			if (!isLoggedIn) {
				cout << "密码错误，请重试。\n";
				logger.write("login failed: wrong password.");
			} else {
				break;
			}
		}
	}
	logger.write("logged in.");
}

// 账户管理菜单
void UI::accountMenu() {
	while (true) {
		cout << "1. 个人信息\n";
		cout << "2. 修改密码\n";
		cout << "3. 退出账号\n";
		cout << "0. 返回上层\n";
		int select = getSelection();
		switch (select) {
			case 1:
				printAccountInfo();
				break;
			case 2:
				changePassword();
				logger.write("Changed Password.");
				break;
			case 3:
				isLoggedIn = false;
				logger.write("logged out.");
				currentAccount = NULL;
				return;
			case 0:
				return;
			default:
				break;
		}
	}
}

// 打印个人信息
void UI::printAccountInfo() {
	cout << "name:\t" << currentAccount->profile.name << '\n';
	cout << "id:\t" << currentAccount->profile.id << '\n';
	cout << "group:\t" << currentAccount->profile.group << '\n';
	string status[] = { "student","teacher","admin","guest" };
	cout << "status:\t" << status[currentAccount->profile.status] << '\n';
}

// 修改密码
void UI::changePassword() {
	cout << "旧密码: \n";
	char oldPassword[20];
	getString(oldPassword, 20);
	bool isRight = currentAccount->password.check(oldPassword);
	if (!isRight) {
		cout << "密码错误。\n";
		return;
	}
	cout << "新密码:\n";
	char newPassword[20];
	getString(newPassword, 20);
	// 如果新密码需要确认输入，则补充以下内容
	// cout<<"确认新密码：\n";
	// char confirm[20];
	// mygets(confirm, 20);
	// if (strcmp(newPassword, confirm) == 0) {
	currentAccount->password.set(newPassword);
	// } else {
	// 	cout<<"两次输入不匹配。\n";
	// }
}
