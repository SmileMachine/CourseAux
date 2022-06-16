#include "UI.h"
#include "log.h"
#include "course.h"
#include "sim_time.h"
#include <iostream>
using namespace std;
ActTime getActTime();
void UI::courseArrange() {
	cout << "1. 课程时间修改\n";
	cout << "2. 课程地点修改\n";
	cout << "3. 更改考试时间\n";
	cout << "4. 更改考试地点\n";
	cout << "5. 查看课程表\n";
	cout << "0. 返回\n";
	int select = getNumber(0, 4);
	if (select == 0) {
		return;
	}
	if (select == 1 || select == 2) {
		courseList.print(Course::BRIEF);
	} else {
		courseList.print(Course::EXAM);
	}
	cout << "请选择要修改的课程：" << endl;
	int courseId = getNumber(0, courseList.list.size());
	Course& toChange = courseList.list[courseId - 1];
	switch (select) {
		case 1: {
			CourseTime newTime;
			cout << "请输入周几：(0-周日，1-周一，...，6-周六)\n";
			newTime.weekday = getNumber(0, 7);
			cout << "请输入开始时间：\n";
			cout << "时：";
			newTime.hour = getNumber(0, 23);
			cout << "分：";
			newTime.minute = getNumber(0, 59);
			CourseTime newEndTime(newTime);
			cout << "请输入结束时间：\n";
			cout << "时：";
			newEndTime.hour = getNumber(0, 23);
			cout << "分：";
			newEndTime.minute = getNumber(0, 59);
			toChange.courseTime = newTime;
			toChange.courseEndTime = newEndTime;
			logger.write("Modify time of course:" + toChange.courseName + " to " + toChange.courseTimeString());
			break;
		}
		case 2: {
			cout << "请输入新的地点：";
			getline(cin, toChange.courseAddress);
			logger.write("Modify address of course:" + toChange.courseName + " to " + toChange.courseAddress);
			break;
		}
		case 3: {
			cout << "请输入开始时间\n";
			toChange.examTime = getActTime();
			cout << "请输入结束时间\n";
			toChange.examEndTime = getActTime();
			logger.write("Modify time of exam:" + toChange.courseName + " to " + toChange.examTimeString());
			break;
		}
		case 4: {
			cout << "请输入新的地点：";
			getline(cin, toChange.examAddress);
			logger.write("Modify address of exam:" + toChange.courseName + " to " + toChange.examAddress);
			break;
		}
		case 5: {
			logger.write("View course schedule.");
			courseList.print(Course::FULL);
			break;
		}
	}
	courseList.write();
}
