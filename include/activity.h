#ifndef ACTIVITY_H
#define ACTIVITY_H
#include "sim_time.h"
#include "course.h"
#include <string>
#include <vector>
#include <algorithm>
using std::string;
using std::vector;
class Activity {
public:
	Activity() {};
	ActTime startTime;	//起始时间
	ActTime endTime;	//结束时间
	string location;	//活动地点
	string ID;			//所属账户的学号
	int type;			//个人/集体活动
	int remind;			//提醒类型
	int content;		//内容标号
	string description;	//活动描述
	enum Type {
		SOLO,
		GROUP,
	};
	enum Content {
		OTHER,
		SELFSTUDY,
		WORKOUT,
		GOOUT,
		CLASSMEETING,
		GROUPWORK,
		ENTREPRENEUR,
		GATHERING,
	};
	static const string contentName[8];
	enum Remind {
		NOREMIND,
		ONCE,
		DAILY,
		WEEKLY,
	};
	bool operator<(Activity b) {
		return startTime < b.startTime;
		// return true;
	}
	bool isConflict(Activity);			//检测与某个活动的冲突
	bool isConflict(Course);			//检测与某个课程的冲突
	Course* detectCourseConflict();		//检测与所有课程的冲突
	Activity* detectActivityConflict(); //检测与所有活动冲突
	void print();
};
class ActList {
public:
	vector<Activity> list;
	ActList(string path);
	ActList() {}
	void sortByTime() {
		std::sort(list.begin(), list.end());
	}
	void sortByID() {
		std::sort(list.begin(), list.end(), [](Activity a, Activity b) {
			return a.ID < b.ID;
			});
	}
	bool isListenerOn = false;
	enum QueryMode {
		INDIVIDUAL,
		GROUP,
		CONTENT,
	};
	ActList*getList(string ID);//根据学号查询
	ActList*getList(int mode, int content = 0);//按照指定模式查询
	ActList*getList(ActTime time);//根据时间查询
	void write();				//写入文件
	void add(Activity act);		//添加活动
	void print(bool withID = false);//打印信息
	void remind(Activity*act);	//活动提醒
	void remindListener();		//活动提醒监测
};
extern ActList activityList;
#endif
