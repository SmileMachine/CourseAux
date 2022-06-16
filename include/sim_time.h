#ifndef SIM_TIME_H
#define SIM_TIME_H
#include <ctime>
#include <windows.h>
#include <iostream>
#include <string>
#include <thread>
#include <cstdio>
class CourseTime;
class ActTime {
	friend CourseTime;
private:
	int year, month, mday;
	int hour, minute;
	long long pureNumber;
	static const int monthday[13];// = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
	/**
	 * @brief 对于超出范围的时间，进行修正。
	 */
	void trim() {
		if (!setMinute(minute)) {
			hour += minute / 60;
			minute %= 60;
		}
		if (!setHour(hour)) {
			mday += hour / 24;
			hour %= 24;
		}
		if (!setMday(mday)) {
			month += (mday - 1) / (monthday[month] + isFebLeap());
			mday = (mday - 1) % (monthday[month] + isFebLeap()) + 1;
		}
		if (!setMonth(month)) {
			year += (month - 1) / 12;
			month = (month - 1) % 12 + 1;
		}
		convertToPureNumber();
	}
	bool isFebLeap() {
		return (month == 2 && (!(year % 4) && (year % 100 || !(year % 400))));
	}
public:
	ActTime() {};
	ActTime(int year, int month, int mday, int hour, int minute) :
		year(year),
		month(month),
		mday(mday),
		hour(hour),
		minute(minute) {
		convertToPureNumber();
	}
	ActTime(long long pureNumber) :pureNumber(pureNumber) {
		dissolve();
	}
	std::string toString() {
		char ret[40];
		char week[7][10] = { "日","一","二","三","四","五","六" };
		sprintf(ret, "%d年%d月%d日 %02d:%02d 周%s",
			// year, month, mday, hour, minute, weekday()
			year, month, mday, hour, minute, week[weekday()]
		);
		return ret;
	}
	bool sameDay(ActTime b) {
		return (year == b.year
			&& month == b.month
			&& mday == b.mday);
	}
	bool setYear(int y) {
		year = y;
		convertToPureNumber();
		return y > 1970;
	}
	bool setMonth(int m) {
		month = m;
		convertToPureNumber();
		return 1 <= m && m <= 12;
	}
	bool setMday(int d) {
		mday = d;
		convertToPureNumber();
		return 1 <= d && d <= (monthday[month] + isFebLeap());
	}
	bool setHour(int h) {
		hour = h;
		convertToPureNumber();
		return 0 <= h && h <= 23;
	}
	bool setMinute(int min) {
		minute = min;
		convertToPureNumber();
		return 0 <= min && min <= 59;
	}
	/**
	 * @brief 将活动时间延后一天
	 */
	void toNextDay() {
		mday++;
		trim();
	}
	/**
	 * @brief 将活动时间延后一周
	 */
	void toNextWeek() {
		mday += 7;
		trim();
	}
	/**
	 * @brief 返回星期几，0代表星期日，然后依次表示星期一、二……六。
	 */
	int weekday() {
		int y = year;
		int m = month;
		int d = mday;
		if (m <= 2) {
			y--;
			m += 12;
		}
		return  (d + 2 * m + 3 * (m + 1) / 5 + y + y / 4 - y / 100 + y / 400 + 1) % 7;
	}
	/**
	 * @brief 转换为形如197001010000的长整数
	 */
	long long convertToPureNumber() {
		pureNumber = (long long)year * 100000000
			+ month * 1000000
			+ mday * 10000
			+ hour * 100
			+ minute;
		return pureNumber;
	}
	/**
	 * @brief 返回一天中该时间的秒数。
	 */
	int nowTime() {
		return hour * 3600 + minute * 60;
	}
	void dissolve() {
		year = pureNumber / 100000000;
		month = pureNumber / 1000000 % 100;
		mday = pureNumber / 10000 % 100;
		hour = pureNumber / 100 % 100;
		minute = pureNumber % 100;
	}
	void write(std::ostream out) {
		out << pureNumber;
	}
	bool operator>(ActTime b) {
		return pureNumber > b.pureNumber;
	}
	bool operator<(ActTime b) {
		return pureNumber < b.pureNumber;
	}
	bool operator<=(ActTime b) {
		return pureNumber <= b.pureNumber;
	}
};
class CourseTime {
public:
	int weekday;
	int hour;
	int minute;
	CourseTime() {};
	CourseTime(int num) {
		weekday = num / 10000;
		hour = num / 100 % 100;
		minute = num % 100;
	}
	CourseTime(ActTime actTime) :
		weekday(actTime.weekday()),
		hour(actTime.hour),
		minute(actTime.minute) {}
	int convertToNumber() {
		return weekday * 10000 + hour * 100 + minute;
	}
	std::string toString(CourseTime endTime) {
		char ret[40];
		char week[7][10] = { "日","一","二","三","四","五","六" };
		sprintf(ret, "周%s %02d:%02d 到 %02d:%02d ",
			week[weekday], hour, minute, endTime.hour, endTime.minute
		);
		return ret;
	}
	bool operator<(CourseTime b) {
		return convertToNumber() < b.convertToNumber();
	}
	bool operator<=(CourseTime b) {
		return convertToNumber() <= b.convertToNumber();
	}
};
class SimTime {
public:
	SimTime() :speed(360), isPause(false) {
		time(&simulateTime);
		timeStruct = localtime(&simulateTime);
		std::thread*timeRun = new std::thread(&SimTime::run, this);
	}
	void pause() {
		isPause = true;
	}
	void unpause() {
		isPause = false;
	}
	/**
	 * @brief 返回形如"1970-01-01 00:00:00"的字符串
	 */
	std::string toString() {
		char ret[30];
		strftime(ret, 30, "%Y-%m-%d %H:%M:%S", localtime(&simulateTime));
		return ret;
	}
	/**
	 * @brief 获得当前时间（模拟）的小时
	 */
	ActTime getActTime() {
		return ActTime(
			timeStruct->tm_year + 1900,
			timeStruct->tm_mon + 1,
			timeStruct->tm_mday,
			timeStruct->tm_hour,
			timeStruct->tm_min
		);
	}
	/**
	 * @brief 设置时间流逝的速度。
	 * @param speed 时间流逝的倍数(每秒)
	 */
	void setSpeed(int speed) {
		this->speed = speed;
	}
	void write(std::ostream out) {
		out << simulateTime;
	}
	void read(std::istream in) {
		in >> simulateTime;
	}
	int speed;
private:
	time_t simulateTime;
	tm*timeStruct;
	bool isPause;
	void run() {
		while (true) {
			tick();
		}
	}
	void tick() {
		// _sleep(100);
		// Sleep(100);
		const int step = 100; //millisecond
		std::this_thread::sleep_for(std::chrono::milliseconds(step));
		// std::cout << toString() << std::endl;
		if (!isPause) {
			simulateTime += speed / (1000 / step);
			timeStruct = localtime(&simulateTime);
		}
	}
};
extern SimTime currentTime;
#endif
