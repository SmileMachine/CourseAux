#include "UI.h"
#include "log.h"
#include "sim_time.h"
#include "activity.h"
#include "account.h"
#include <iostream>
#include <cstdlib>
using namespace std;
/**
 * @brief 活动管理
 */
void UI::activityMenu() {
	CLEAR&&system("cls");
	while (true) {
		if (currentAccount->isStu()) {
			cout << "1. 活动查询\n";
			cout << "2. 创建新的活动\n";
			cout << "0. 返回上层\n";
			int select = getNumber();
			switch (select) {
				case 1:
					// 查看当前用户所有活动
					activityQuery();
					logger.write("Activity query.");
					break;
				case 2:
					// 创建新的活动
					createActivity();
					break;
				case 0:
					return;
				default:
					break;
			}
		} else {
			cout << "1. 查看所有活动\n";
			cout << "2. 学生活动查询\n";
			cout << "0. 返回上层\n";
			int select = getNumber();
			switch (select) {
				case 1:
					activityList.print(true);
					break;
				case 2:
					activityQuery();
					break;
				case 0:
					return;
				default:
					break;
			}
		}
	}
}
ActTime getActTime() {
	ActTime ret;
	cout << "年: ";
	while (!ret.setYear(UI::getNumber())) {
		cout << "输入错误，请重试: ";
	}
	cout << "月: ";
	while (!ret.setMonth(UI::getNumber())) {
		cout << "输入错误，请重试: ";
	}
	cout << "日: ";
	while (!ret.setMday(UI::getNumber())) {
		cout << "输入错误，请重试: ";
	}
	cout << "时: ";
	while (!ret.setHour(UI::getNumber())) {
		cout << "输入错误，请重试: ";
	}
	cout << "分: ";
	while (!ret.setMinute(UI::getNumber())) {
		cout << "输入错误，请重试: ";
	}
	return ret;
}
void UI::createActivity() {
	Activity newAct;
	cout << "请选择活动类型： 1.个人活动 2.集体活动\n";
	int select = getNumber(1, 2);
	newAct.type = select - 1;
	newAct.ID = currentAccount->profile.id;
	switch (select) {
		case 1: {
			cout << "请选择活动：\n";
			cout << "1. 自习\n";
			cout << "2. 锻炼\n";
			cout << "3. 外出\n";
			cout << "0. 其他\n";
			int actSelect = getNumber(0, 3);
			switch (actSelect) {
				case 1:
					newAct.content = Activity::SELFSTUDY;
					break;
				case 2:
					newAct.content = Activity::WORKOUT;
					break;
				case 3:
					newAct.content = Activity::GOOUT;
					break;
				case 0:
					newAct.content = Activity::OTHER;
					break;
			}
			break;
		}
		case 2: {
			cout << "请选择活动：\n";
			cout << "1. 班会\n";
			cout << "2. 小组作业\n";
			cout << "3. 创新创业\n";
			cout << "4. 聚餐\n";
			cout << "0. 其他\n";
			int actSelect = getNumber(0, 3);
			switch (actSelect) {
				case 1:
					newAct.content = Activity::CLASSMEETING;
					break;
				case 2:
					newAct.content = Activity::GROUPWORK;
					break;
				case 3:
					newAct.content = Activity::ENTREPRENEUR;
					break;
				case 4:
					newAct.content = Activity::GATHERING;
					break;
				case 0:
					newAct.content = Activity::OTHER;
					break;
			}
			break;
		}
	}
	if (newAct.content == Activity::OTHER) {
		cout << "请输入活动描述\n";
		getline(cin, newAct.description);
	}
	cout << "请输入活动地点：" << endl;
	getline(cin, newAct.location);
	cout << "当前时间: " << currentTime.toString() << endl;
	while (true) {
		cout << "请输入活动开始时间:\n";
		ActTime startTime = getActTime();
		cout << "请输入活动结束时间:\n";
		ActTime endTime = getActTime();
		newAct.startTime = startTime;
		newAct.endTime = startTime;
		Course* courseConf = newAct.detectCourseConflict();
		if (courseConf) {
			cout << "你的活动与以下课程有冲突：";
			courseConf->print(Course::BRIEF);
		}
		Activity* actConf = newAct.detectActivityConflict();
		if (actConf) {
			cout << "你的活动与以下活动有冲突：";
			actConf->print();
		}
		if (courseConf || actConf) {
			cout << "是否要重新填写时间？(1/0)\n";
			if (!getNumber()) {
				break;
			}
		} else {
			break;
		}
	}
	cout << "请输入提醒类型:\n";
	cout << "1. 仅提醒一次\n";
	cout << "2. 每天提醒一次\n";
	cout << "3. 每周提醒一次\n";
	cout << "0. 不提醒\n";
	select = getNumber(0, 3);
	switch (select) {
		case 0:
			newAct.remind = Activity::NOREMIND;
			break;
		case 1:
			newAct.remind = Activity::ONCE;
			break;
		case 2:
			newAct.remind = Activity::DAILY;
			break;
		case 3:
			newAct.remind = Activity::WEEKLY;
			break;
	}
	activityList.add(newAct);
	if (newAct.type == Activity::GROUP) {
		cout << "要为多少个成员添加此活动？(输入0表示不添加)\n";
		int memberCount = getNumber();
		for (int i = 0;i < memberCount;i++) {
			cout << "请输入成员" << i + 1 << "的学号\n";
			string ID;
			getline(cin, ID);
			newAct.ID = ID;
			activityList.add(newAct);
		}
	}
	logger.write("Created an activity:" + newAct.contentName[newAct.content]);
}
void UI::activityQuery() {
	if (currentAccount->isStu()) {
		cout << "1. 查看所有活动\n";
		cout << "2. 查看个人活动\n";
		cout << "3. 查看集体活动\n";
		cout << "4. 按照活动内容查询\n";
		cout << "5. 按照时间查询\n";
		cout << "0. 返回上层\n";
		int select = getNumber(0, 5);
		ActList*query;
		switch (select) {
			case 1: {
				query = activityList.search(currentAccount->profile.id);
				logger.write("View all activities.");
				break;
			}
			case 2: {
				query = activityList.getList(ActList::INDIVIDUAL);
				logger.write("View individual activities.");
				break;
			}
			case 3: {
				query = activityList.getList(ActList::GROUP);
				break;
			}
			case 4: {
				for (int i = 0;i < sizeof(Activity::contentName) / sizeof(string);i++) {
					cout << i + 1 << ". " << Activity::contentName[i] << endl;
				}
				int contentSelect = getNumber(1, 8);
				query = activityList.getList(ActList::CONTENT, contentSelect - 1);
				logger.write("Searched activities:" + Activity::contentName[contentSelect]);
				break;
			}
			case 5: {
				cout << "请输入年月日(如20220630)\n";
				long long int date = getNumber(20220000, 20700000);
				logger.write("Searched activities with date " + to_string(date));
				query = activityList.getList(ActTime(date * 10000));
				break;
			}
			case 0: {
				break;
			}
		}
		if (query) {
			query->print();
			delete query;
		} else {
			cout << "无信息\n";
		}
		cout << endl;
	} else {
		cout << "请输入要查询的学号：";
		string ID;
		getline(cin, ID);
		logger.write("Searched activities of " + ID);
		ActList*actQuery = activityList.search(currentAccount->profile.id);
		if (actQuery) {
			actQuery->print();
			delete actQuery;
		} else {
			cout << "无信息\n";
		}
		cout << endl;
	}
}
