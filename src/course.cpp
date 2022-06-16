#include <fstream>
#include <string>
#include "course.h"
#include "data_io.h"
#include "account.h"
#include "log.h"
#include <thread>
#define COURSE_PATH "./data/course.csv"
using namespace std;
CourseList courseList(COURSE_PATH);
CourseList::CourseList(string path) {
	DataReader reader(path.c_str());
	for (int i = 0; !reader.isEOF(); i++) {
		Course newCourse;
		string tmp;
		reader.nextItem(newCourse.courseName);
		reader.nextItem(newCourse.teacherName);
		reader.nextItem(tmp);
		newCourse.courseTime = CourseTime(stoll(tmp));
		reader.nextItem(tmp);
		newCourse.courseEndTime = CourseTime(stoll(tmp));
		reader.nextItem(newCourse.courseAddress);
		reader.nextItem(newCourse.contact);
		reader.nextItem(newCourse.currentProgress);
		reader.nextItem(tmp);
		newCourse.examTime = ActTime(stoll(tmp));
		reader.nextItem(tmp);
		newCourse.examEndTime = ActTime(stoll(tmp));
		reader.nextItem(newCourse.examAddress);
		list.push_back(newCourse);
	}
	// print();
	logger.write("course.csv loaded.", Log::SYS);
	// thread courseRemind
}
CourseList*CourseList::query(std::string name) {
	CourseList*ret = new CourseList;
	for (auto v = list.begin();v != list.end();v++) {
		if (v->courseName == name) {
			ret->list.push_back(*v);
		}
	}
	return ret;
}
CourseList*CourseList::queryWeekday(int weekday) {
	CourseList*ret = new CourseList;
	for (auto v = list.begin();v != list.end();v++) {
		if (v->courseTime.weekday == weekday) {
			ret->list.push_back(*v);
		}
	}
	return ret;
}
CourseList*CourseList::queryToday() {
	CourseList*ret = new CourseList;
	for (auto v = list.begin();v != list.end();v++) {
		CourseTime now(currentTime.getActTime());
		CourseTime tomorrow = now;
		tomorrow.weekday = (tomorrow.weekday + 1) % 7;
		if (now < v->courseTime && v->courseTime < tomorrow) {
			ret->list.push_back(*v);
		}
	}
	return ret;
}
void CourseList::write() {
	ofstream fout(COURSE_PATH);
	char delim = ',';
	fout << "#course,teacher,courseTime,courseEndTime,courseAddress,contact,currentProgress,examTime,examEndTime,examAddress,\n";
	for (auto v = list.begin();v < list.end();v++) {
		fout << v->courseName << delim
			<< v->teacherName << delim
			<< v->courseTime.convertToNumber() << delim
			<< v->courseEndTime.convertToNumber() << delim
			<< v->courseAddress << delim
			<< v->contact << delim
			<< v->currentProgress << delim
			<< v->examTime.convertToPureNumber() << delim
			<< v->examEndTime.convertToPureNumber() << delim
			<< v->examAddress << delim
			<< endl;
	}
	logger.write("Write to course.csv", Log::SYS);
	fout.close();
}
void Course::print(int mode) {
	cout << "课程名称：" << courseName << endl;
	if (mode == FULL || mode == BRIEF) {
		cout << "上课时间：" << courseTimeString() << endl;
		cout << "上课地点：" << courseAddress << endl;
	}
	if (mode == FULL) {
		cout << "教师名称：" << teacherName << endl;
		cout << "联系方式：" << contact << endl;
		cout << "当前进度：" << currentProgress << endl;
	}
	if (mode == FULL || mode == EXAM) {
		if (examTime.convertToPureNumber()) {
			cout << "考试时间：" << examTimeString() << endl;
			cout << "考试地点：" << examAddress << endl << endl;
		} else {
			cout << "考试：未安排\n";
		}
	}
}
void CourseList::print(int mode) {
	int cnt = 0;
	for (auto v : list) {
		cout << ++cnt << ":" << endl;
		v.print(mode);
	}
}
// void CourseList::remindContext() {
// 	Course*listen = nullptr;
// 	Account*judge = currentAccount;
// 	while (true) {
// 		if (judge != currentAccount) {
// 			judge = currentAccount;
// 			listen = nullptr;
// 		}
// 		if (isListenerOn == true) {
// 			isListenerOn = false;
// 			listen = nullptr;
// 			for (auto v = list.begin();v != list.end();v++) {
// 				// cout << v->remind << "\t" << v->ID << "\t" << v->time.toString() << endl;
// 				std::this_thread::sleep_for(std::chrono::milliseconds(100));
// 				if (currentAccount && v->remind != Activity::NOREMIND && v->ID == currentAccount->profile.id) {
// 					listen = &*v;
// 					break;
// 				}
// 			}
// 		}
// 		if (listen != nullptr) {
// 			//输出当前时间、正在监听的活动时间，用于测试。
// 			// cout << listen->time.toString() << '\t' << listen->time.convertToPureNumber() << endl;
// 			// cout << currentTime.getActTime().toString() << '\t' << currentTime.getActTime().convertToPureNumber() << endl;
// 			// cout << "当前时间: " << currentTime.toString() << endl;
// 			if (listen->startTime < currentTime.getActTime()) {
// 				remind(listen);
// 				switch (listen->remind) {
// 					case Activity::ONCE: {
// 						listen->remind = Activity::NOREMIND;
// 						break;
// 					}
// 					case Activity::DAILY: {
// 						while (listen->startTime < currentTime.getActTime()) {
// 							listen->startTime.toNextDay();
// 						}
// 						break;
// 					}
// 					case Activity::WEEKLY: {
// 						while (listen->startTime < currentTime.getActTime()) {
// 							listen->startTime.toNextWeek();
// 						}
// 						break;
// 					}
// 				}
// 				listen = nullptr;
// 				isListenerOn = true;
// 				sortByTime();
// 				write();
// 			}
// 		}
// 		// if (listen) {
// 		// 	cout << "listening\n";
// 		// } else {
// 		// 	cout << "waiting\n";
// 		// }
// 		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
// 	}
// }
