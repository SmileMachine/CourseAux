#include <iostream>
#include <cstdio>
#include <cstring>
#ifdef WINDOWS
#include <conio.h>
#endif
// #include "global_variable.cpp"
#include "account.h"
#include "menu.h"
char* mygets(char input[], int len) {
	char* ret = fgets(input, len, stdin);
	if (input[strlen(input) - 1] == '\n') {
		input[strlen(input) - 1] = '\0';
	}
	return ret;
}
void UI::login() {
	char input[20];
	bool isLoggedIn = false;
	while (!isLoggedIn) {
		puts("请先登录。");
		printf("学号: ");
		mygets(input, 20);
		currentAccount = accountList.findByID(input);
		for (int i = 0; i < 3; i++) {
			printf("密码: ");
			isLoggedIn = currentAccount->password.input();
			if (!isLoggedIn) {
				printf("密码错误，请重试。\n");
				// Log::write();
			} else {
				break;
			}
		}
	}
	// Log::write(); //Todo:写入日志，待实现
}
void UI::mainMenu() {
	bool isLoggedIn = true;
	while (isLoggedIn) {
		printf("欢迎来到线下课程辅助系统!\n");
		printf("1. 个人信息\n");
		printf("2. 修改密码\n");
		printf("0. 退出账号\n");
		int select;
		std::cin >> select;
		getchar();
		switch (select) {
			case 1:
				currentAccount->printInfo();
				break;
			case 2: {
				printf("旧密码: \n");
				bool isRight = currentAccount->password.input();
				if (!isRight) {
					printf("密码错误。\n");
					break;
				}
				printf("新密码:\n");
				char newPassword[20];
				mygets(newPassword, 20);
				// 如果新密码需要确认输入，则补充一下内容
				// printf("确认新密码：\n");
				// char confirm[20];
				// mygets(confirm,20);
				// if(strcmp(newPassword,confirm)==0){
				currentAccount->password.set(newPassword);
				// }
				break;
			}
			case 0:
				isLoggedIn = false;
				break;
			default:
				break;
		}
	}
}
