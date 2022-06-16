#ifndef HOMEWORK_H
#define HOMEWORK_H
#include <string>
#include <vector>
using std::string;
// course,homework,isFinished,ID,
class Homework {
public:
	string course;
	string homework;
	bool isFinished;
	string ID;
	enum PrintMode {
		FULL,  //查看所有
		COURSE, //按课程查询
		UNDONE,
		DONE,
	};
	void print(int mode);
};
class HomeworkList {
public:
	std::vector<Homework>list;
	HomeworkList() {}
	HomeworkList(string path);
	void print(int mode = Homework::FULL);
	enum QueryMode {
		ALL,
		DONE,
		UNDONE,
	};
	void write();
	HomeworkList* getList(int mode);
	HomeworkList* getList(string course);
	void doneHomework(Homework&homework);
};
extern HomeworkList homeworkList;
#endif
