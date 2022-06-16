#include "UI.h"
#include "log.h"
#include "course.h"
#include "homework.h"
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
				examQuery();
				logger.write("View exam info.");
				break;
			case 3:
				viewMaterial();
				break;
			case 4:
				homeworkQuery();
				break;
			case 0:
				return;
			default:
				break;
		}
	}
}
void UI::courseQuery() {
	cout << "1. 查看所有课程信息\n";
	cout << "2. 查看今天未上的课\n";
	cout << "3. 按照课程名称查询\n";
	cout << "4. 查看本周课程表\n";
	// cout << "5. ";
	cout << "0. 返回\n";
	int select = getNumber(0, 4);
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
			cout << "请输入课程名称：";
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
		case 4: {
			showCourseSchedule();
			break;
		}
		case 0: {
			break;
		}
	}
}
#include <cstdio>
#include <sstream>
#include <iomanip>
void UI::showCourseSchedule() {
	const string week[5] = { "一","二","三","四","五" };
	for (int i = 1;i <= 5;i++) {
		CourseList*query = courseList.queryWeekday(i);
		//输出课程名称
		cout << setiosflags(ios::left) << setw(4) << " " << "|";
		for (auto v : query->list) {
			cout << "" << setw(20) << v.courseName << "|";
		}
		cout << endl;
		//输出上课时间
		cout << "" << setw(4) << week[i - 1] << "|";
		for (auto v : query->list) {
			char time[30];
			sprintf(time, "%02d:%02d ~ %02d:%02d ",
				v.courseTime.hour, v.courseTime.minute, v.courseEndTime.hour, v.courseEndTime.minute
			);
			cout << "" << setw(20) << time << "|";
		}
		cout << endl;
		//输出上课地点
		cout << "" << setw(4) << " " << "|";
		for (auto v : query->list) {
			stringstream ss(v.courseAddress);
			string address;
			ss >> address >> address;
			cout << "" << setw(20) << address << "|";
		}
		cout << endl;
		cout << endl;
	}
}
void UI::examQuery() {
	cout << "1. 查看所有考试信息\n";
	cout << "2. 按照课程名称查询\n";
	cout << "0. 返回\n";
	int select = getNumber(0, 2);
	switch (select) {
		case 1: {
			courseList.print(Course::EXAM);
			logger.write("View all exam info.");
			break;
		}
		case 2: {
			cout << "请输入课程名称：";
			string name;
			getline(cin, name);
			logger.write("Searched course name:" + name + ".");
			CourseList* query = courseList.query(name);
			if (query) {
				query->list[0].print(Course::EXAM);
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
void UI::homeworkQuery() {
	while (true) {
		cout << "1. 查看所有作业\n";
		cout << "2. 查看未交作业\n";
		cout << "3. 查看已交作业\n";
		cout << "4. 根据课程查询\n";
		cout << "5. 查看已上传作业\n";
		cout << "0. 返回上层\n";
		int select = getNumber(0, 5);
		if (select == 5) {
			viewSubmittedHomework();
			return;
		}
		HomeworkList*query = nullptr;
		switch (select) {
			case 1: {
				query = homeworkList.getList(HomeworkList::ALL);
				if (query) {
					query->print(Homework::FULL);
				} else {
					cout << "没有作业\n";
				}
				break;
			}
			case 2: {
				query = homeworkList.getList(HomeworkList::UNDONE);
				if (query) {
					query->print(Homework::UNDONE);
				} else {
					cout << "作业已经全部完成\n";
				}
				break;
			}
			case 3: {
				query = homeworkList.getList(HomeworkList::DONE);
				if (query) {
					query->print(Homework::DONE);
				} else {
					cout << "没有已经完成的作业\n";
				}
				break;
			}
			case 4: {
				string course;
				cout << "请输入课程名称\n";
				getline(cin, course);
				query = homeworkList.getList(course);
				if (query) {
					query->print(Homework::COURSE);
				} else {
					cout << "没有找到该课程。\n";
				}
				break;
			}
			case 0: {
				return;
			}
		}
		if (query) {
			cout << "1. 提交/更新作业\n";
			cout << "2. 下载已上传作业\n";
			cout << "0. 返回\n";
			select = getNumber(0, 2);
			switch (select) {
				case 1: {
					cout << "请输入编号：";
					int courseNumber = getNumber(1, query->list.size() + 1);
					submitHomework(query->list[courseNumber - 1]);
					break;
				}
				case 2:
					viewSubmittedHomework();
					break;
				case 0:
					return;
			}
			delete query;
		}
	}
}
