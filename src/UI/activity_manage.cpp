#include "UI.h"
#include <iostream>
#include <cstdlib>
using namespace std;
/**
 * @brief 活动管理
 */
void UI::activityMenu() {
	system("cls");
	while (true) {
		cout << "1. 查看当前活动\n";
		cout << "2. 创建新的活动\n";
		cout << "0. 返回上层\n";
		int select = getSelection();
		switch (select) {
			case 1:
				// 查看当前活动
				break;
			case 2:
				// 创建新的活动
				break;
			case 0:
				return;
			default:
				break;
		}
	}
}
