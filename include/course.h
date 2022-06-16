#ifndef COURSE_H
#define COURSE_H
#include "sim_time.h"
#include <vector>
#include <algorithm>
class Course {
public:
	Course() {}
	std::string courseName;
	std::string teacherName;
	CourseTime courseTime;
	CourseTime courseEndTime;
	std::string courseAddress;
	std::string contact;
	std::string currentProgress;
	ActTime examTime;
	ActTime examEndTime;
	std::string examAddress;
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
	CourseList*query(std::string name) {
		CourseList*ret = new CourseList;
		for (auto v = list.begin();v != list.end();v++) {
			if (v->courseName == name) {
				ret->list.push_back(*v);
			}
		}
		return ret;
	}
	CourseList*queryToday() {
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
	void print(int mode = 0);//课表
};
extern CourseList courseList;
#endif
