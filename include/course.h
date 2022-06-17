#ifndef COURSE_H
#define COURSE_H
#include "sim_time.h"
#include <vector>
#include <algorithm>
class Course {
public:
	Course() {}
	std::string courseName; 	//课程名称
	std::string teacherName;	//老师姓名
	CourseTime courseTime;		//上课时间
	CourseTime courseEndTime;	//下课时间
	std::string courseAddress;	//上课地点
	std::string contact;		//联系方式
	std::string currentProgress;//课程进度
	ActTime examTime;			//考试时间
	ActTime examEndTime;		//考试结束时间
	std::string examAddress;	//考试地点
	std::string courseTimeString() {
		return courseTime.toString(courseEndTime);
	}
	std::string examTimeString() {
		return examTime.toString() + " 到 " + examEndTime.toString();
		// return examTime.toString(examEndTime);
	}
	enum PrintMode {
		FULL,
		BRIEF,
		EXAM,
	};
	void print(int mode = FULL);
	// int GetHomeworkNum();
	// void SaveHomework();
	//void SubmitWork();
};
class CourseList {
public:
	CourseList() {}
	CourseList(std::string path);
	void remind();
	void remindContext();
	std::vector<Course>list;
	void write();
	void sortByTime() {
		std::sort(list.begin(), list.end(), [](Course a, Course b) {
			return a.courseTime < b.courseTime;
			// return true;
			});
	}
	void sortByContact() {
		std::sort(list.begin(), list.end(), [](Course a, Course b) {
			return a.contact < b.contact;
			});
	}
	CourseList*query(std::string name);
	CourseList*queryWeekday(int weekday);
	CourseList*queryToday();
	void print(int mode = 0);//课表
};
extern CourseList courseList;
#endif
