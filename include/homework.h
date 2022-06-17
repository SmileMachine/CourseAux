#ifndef HOMEWORK_H
#define HOMEWORK_H
#include <string>
#include <vector>
using std::string;
// course,homework,isFinished,ID,
class Homework {
public:
	string course;	//课程名称
	string homework;//作业内容
	bool isFinished;//是否完成
	string ID;		//相关学号
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
	void write();//写入文件
	HomeworkList* getList(int mode);		//根据特定模式查询
	HomeworkList* getList(string course);	//根据课程名称查询
	void doneHomework(Homework&homework);	//将某个作业的状态设置为已完成
};
extern HomeworkList homeworkList;
#endif
