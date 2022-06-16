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
	ActTime startTime;
	ActTime endTime;
	string location;
	string ID;
	int type;
	int remind;
	int content;
	string description;
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
	bool isConflict(Activity);
	bool isConflict(Course);
	Course* detectCourseConflict();
	Activity* detectActivityConflict();
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
	ActList*search(string ID);
	enum QueryMode {
		INDIVIDUAL,
		GROUP,
		CONTENT,
	};
	// ActList*getList(int mode);
	ActList*getList(int mode, int content = 0);
	ActList*getList(ActTime time);
	void write();
	void add(Activity act);
	void print(bool withID = false);
	void remind(Activity*act);
	void remindListener();
};
extern ActList activityList;
#endif
