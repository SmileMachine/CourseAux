#include "UI.h"
#include <iostream>
#include <cstdlib>
using namespace std;
void UI::infoQuery() {
	CLEAR&&system("cls");
	while (true) {
		cout << "信息查询\n";
		cout << "1. 课程信息\n";
		cout << "2. 考试安排\n";
		cout << "3. 课程资料\n";
		cout << "4. 作业查询\n";
		cout << "0. 返回上层\n";
		int select = getSelection();
		switch (select) {
			case 1:

				break;
			case 0:
				return;
			default:
				break;
		}
	}
}
