#include <fstream>
#include "log.h"
#include <thread>
#include <cstdlib>
#include "activity.h"
#include "data_io.h"
#include "account.h"
#include "course.h"
using namespace std;
#define ACTIVITY_PATH "./data/activity.csv"
ActList activityList(ACTIVITY_PATH);
ActList::ActList(string path) {
	DataReader reader(path.c_str());
	for (int i = 0; !reader.isEOF(); i++) {
		Activity newAct;
		string tmp;
		reader.nextItem(tmp);
		newAct.startTime = ActTime(stoll(tmp));
		reader.nextItem(tmp);
		newAct.endTime = ActTime(stoll(tmp));
		reader.nextItem(newAct.location);
		reader.nextItem(newAct.ID);
		reader.nextItem(tmp);
		newAct.type = stoi(tmp);
		reader.nextItem(tmp);
		newAct.remind = stoi(tmp);
		reader.nextItem(tmp);
		newAct.content = stoi(tmp);
		reader.nextItem(newAct.description);
		list.push_back(newAct);
	}
	sortByTime();
	write();
	isListenerOn = false;
	logger.write("activity.csv loaded.", Log::SYS);
	std::thread*reminder = new std::thread(&ActList::remindListener, this);
}
bool Activity::isConflict(Activity b) {
	return  (b.startTime <= startTime && startTime <= b.endTime
		|| b.startTime <= endTime && endTime <= b.endTime
		|| startTime <= b.startTime && b.endTime <= endTime);
}
bool Activity::isConflict(Course b) {
	CourseTime start(this->startTime);
	CourseTime end(this->endTime);
	return  (b.courseTime <= start && start <= b.courseEndTime
		|| b.courseTime <= end && end <= b.courseEndTime
		|| start <= b.courseTime && b.courseEndTime <= end);
}
Course*Activity::detectCourseConflict() {
	for (auto v = courseList.list.begin();v != courseList.list.end();v++) {
		if (isConflict(*v)) {
			return &*v;
		}
	}
	return nullptr;
}
Activity*Activity::detectActivityConflict() {
	for (auto v = activityList.list.begin();v != activityList.list.end();v++) {
		if (isConflict(*v)) {
			return &*v;
		}
	}
	return nullptr;
}
void ActList::add(Activity act) {
	list.push_back(act);
	sortByTime();
	isListenerOn = true;
	if (this == &activityList) {
		write();
	}
}
void ActList::write() {
	ofstream fout(ACTIVITY_PATH);
	char delim = ',';
	fout << "#startTime,endTime,location,ID,type,remind,content,description,\n";
	for (auto v = list.begin();v < list.end();v++) {
		fout << v->startTime.convertToPureNumber() << delim
			<< v->endTime.convertToPureNumber() << delim
			<< v->location << delim
			<< v->ID << delim
			<< v->type << delim
			<< v->remind << delim
			<< v->content << delim
			<< v->description << delim
			<< endl;
	}
	fout.close();
}
ActList*ActList::getList(string ID) {
	ActList* ret = new ActList;
	for (auto v = list.begin();v < list.end();v++) {
		if (v->ID == ID) {
			ret->add(*v);
		}
	}
	return ret;
}

const string Activity::contentName[8] = {
	"??????",
	"??????",
	"??????",
	"??????",
	"??????",
	"????????????",
	"????????????",
	"??????",
};
const string remindName[4]{
	"?????????",
	"????????????",
	"????????????",
	"????????????",
};
void Activity::print() {
	cout << "???????????????" << startTime.toString() << endl;
	cout << "???????????????" << endTime.toString() << endl;
	cout << "?????????" << location << endl;
	cout << "?????????" << (content ? Activity::contentName[content] : description) << endl;
}
void ActList::print(bool withID) {
	int cnt = 0;
	for (auto v : list) {
		cout << ++cnt << ":" << endl;
		if (withID) {
			cout << "?????????" << v.ID << endl;
		}
		v.print();
		cout << "?????????";
		if (v.type == Activity::SOLO) {
			cout << "????????????\n";
		} else {
			cout << "????????????\n";
		}
		cout << "?????????" << remindName[v.remind] << endl;
	}
}
/**
 * @brief ??????????????????????????????
 * @return ActList*
 */
ActList*ActList::getList(ActTime time) {
	ActList*ret = new ActList;
	for (auto v = list.begin();v < list.end();v++) {
		if (v->startTime.sameDay(time)) {
			ret->add(*v);
		}
	}
	return ret;
}
/**
 * @brief ????????????????????????????????????????????????????????????
 * @param mode 0-???????????????1-???????????????2-????????????
 * @param content ????????????
 * @return ActList*
 */
ActList*ActList::getList(int mode, int content) {
	ActList*ret = new ActList;
	for (auto v = list.begin();v < list.end();v++) {
		switch (mode) {
			case INDIVIDUAL: {
				if (v->type == Activity::SOLO) {
					ret->add(*v);
				}
				break;
			}
			case GROUP: {
				if (v->type == Activity::GROUP) {
					ret->add(*v);
				}
				break;
			}
			case CONTENT: {
				if (v->content == content) {
					ret->add(*v);
				}
				break;
			}
		}
	}
	return ret;
}

void ActList::remind(Activity*act) {
	cout << "????????????: " << currentTime.toString() << endl;
	cout << "\n!!--------ATTENTION--------!!\n";
	cout << "?????????????????????????????????????????????\n";
	cout << "?????????" << act->startTime.toString() << endl;
	cout << "?????????" << act->location << endl;
	string content = (act->content ? Activity::contentName[act->content] : act->description);
	cout << "?????????" << content << endl;
	cout << "?????????";
	if (act->type == Activity::SOLO) {
		cout << "????????????\n";
	} else {
		cout << "????????????\n";
	}
	cout << "!!-------------------------!!\n\n";
	logger.write("Activity attention:" + content + ".");
	system("pause");
}
void ActList::remindListener() {
	Activity*listen = nullptr;
	Account*judge = currentAccount;
	while (true) {
		if (judge != currentAccount) {
			judge = currentAccount;
			listen = nullptr;
		}
		if (isListenerOn == true) {
			isListenerOn = false;
			listen = nullptr;
			for (auto v = list.begin();v != list.end();v++) {
				// cout << v->remind << "\t" << v->ID << "\t" << v->time.toString() << endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				if (currentAccount && v->remind != Activity::NOREMIND && v->ID == currentAccount->profile.id) {
					listen = &*v;
					break;
				}
			}
		}
		if (listen != nullptr) {
			//??????????????????????????????????????????????????????????????????
			// cout << listen->time.toString() << '\t' << listen->time.convertToPureNumber() << endl;
			// cout << currentTime.getActTime().toString() << '\t' << currentTime.getActTime().convertToPureNumber() << endl;
			// cout << "????????????: " << currentTime.toString() << endl;
			if (listen->startTime < currentTime.getActTime()) {
				remind(listen);
				switch (listen->remind) {
					case Activity::ONCE: {
						listen->remind = Activity::NOREMIND;
						break;
					}
					case Activity::DAILY: {
						while (listen->startTime < currentTime.getActTime()) {
							listen->startTime.toNextDay();
							listen->endTime.toNextDay();
						}
						break;
					}
					case Activity::WEEKLY: {
						while (listen->startTime < currentTime.getActTime()) {
							listen->startTime.toNextWeek();
							listen->endTime.toNextWeek();
						}
						break;
					}
				}
				listen = nullptr;
				isListenerOn = true;
				sortByTime();
				write();
			}
		}
		// if (listen) {
		// 	cout << "listening\n";
		// } else {
		// 	cout << "waiting\n";
		// }
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}
