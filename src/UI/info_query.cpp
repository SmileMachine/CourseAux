#include "UI.h"
#include "log.h"
#include "course.h"
#include <string>
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
		int select = getNumber();
		switch (select) {
			case 1:
				courseQuery();
				logger.write("Course query.");
				break;
			case 2:
				courseList.print(Course::EXAM);
				logger.write("View exam info.");
				break;
			case 0:
				return;
			default:
				break;
		}
	}
}
void UI::courseQuery() {
	cout << "1. 查看本周课程信息\n";
	cout << "2. 查看今天未上的课\n";
	cout << "3. 按照课程名称查询\n";
	cout << "0. 返回\n";
	int select = getNumber(0, 3);
	switch (select) {
		case 1: {
			courseList.print();
			logger.write("View course schedule of week.");
			break;
		}
		case 2: {
			logger.write("View course schedule of the day.");
			CourseList* query = courseList.queryToday();
			if (query) {
				query->print();
				delete query;
			} else {
				cout << "今天没有课了。\n";
			}
			break;
		}
		case 3: {
			string name;
			getline(cin, name);
			logger.write("Searched course name:" + name + ".");
			CourseList* query = courseList.query(name);
			if (query) {
				query->print();
				delete query;
			} else {
				cout << "没有找到该课程。\n";
			}
			break;
		}
		case 0: {
			break;
		}
	}
}

