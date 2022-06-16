#include "homework.h"
#include "account.h"
#include "log.h"
#include "data_io.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#define HOMEWORK_PATH "./data/homework.csv"
HomeworkList homeworkList(HOMEWORK_PATH);
HomeworkList::HomeworkList(string path) {
	DataReader reader(path.c_str());
	for (int i = 0; !reader.isEOF(); i++) {
		Homework newHomework;
		string tmp;
		reader.nextItem(newHomework.course);
		reader.nextItem(newHomework.homework);
		reader.nextItem(tmp);
		newHomework.isFinished = stoi(tmp);
		reader.nextItem(newHomework.ID);
		list.push_back(newHomework);
	}
	logger.write("homework.csv loaded.", Log::SYS);
}
void HomeworkList::print(int mode) {
	int cnt = 0;
	for (auto v : list) {
		cout << ++cnt << ":" << endl;
		v.print(mode);
	}
}
void Homework::print(int mode) {
	if (mode != COURSE) {
		cout << "课程名称：" << course << endl;
	}
	cout << "作业内容：" << homework << endl;
	if (mode != UNDONE && mode != DONE) {
		cout << "完成状态：" << (isFinished ? "已完成" : "未完成") << endl;
	}
}
HomeworkList* HomeworkList::getList(string course) {
	HomeworkList*ret = new HomeworkList;
	for (auto v = list.begin();v < list.end();v++) {
		if (v->course == course && v->ID == currentAccount->profile.id) {
			ret->list.push_back(*v);
		}
	}
	return ret;
}
HomeworkList* HomeworkList::getList(int mode) {
	HomeworkList*ret = new HomeworkList;
	HomeworkList*currentHomework = new HomeworkList;
	for (auto v = list.begin();v < list.end();v++) {
		if (v->ID == currentAccount->profile.id) {
			currentHomework->list.push_back(*v);
		}
	}
	switch (mode) {
		case ALL: {
			ret = currentHomework;
			break;
		}
		case DONE:
		case UNDONE: {
			for (auto v = currentHomework->list.begin();v < currentHomework->list.end();v++) {
				if ((mode == DONE) ? v->isFinished : !v->isFinished) {
					ret->list.push_back(*v);
				}
			}
			break;
		}
	}
	return ret;
}
void HomeworkList::doneHomework(Homework&homework) {
	for (auto v : list) {
		if (v.course == homework.course && v.homework == homework.homework && v.ID == homework.ID) {
			v.isFinished == true;
			write();
			return;
		}
	}
}
void HomeworkList::write() {
	ofstream fout(HOMEWORK_PATH);
	char delim = ',';
	fout << "#course,homework,isFinished,ID,\n";
	for (auto v = list.begin();v < list.end();v++) {
		fout << v->course << delim
			<< v->homework << delim
			<< v->isFinished << delim
			<< v->ID << delim
			<< endl;
	}
	logger.write("Write to homework.csv", Log::SYS);
	fout.close();
}
