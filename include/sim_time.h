#ifndef SIM_TIME_H
#define SIM_TIME_H
#include <ctime>
#include <windows.h>
#include <iostream>
#include <string>
#include <thread>
class ActTime {
public:
	int year, month, mday;
	int hour, minute;
	long long pureNumber;
	ActTime(int year, int month, int mday, int hour, int minute) :
		year(year),
		month(month),
		mday(mday),
		hour(hour),
		minute(minute) {
		convertToPureNumber();
	}
	ActTime(int pureNumber) :pureNumber(pureNumber) {
		dissolve();
	}
	/**
	 * @brief 转换为形如197001010000的长整数
	 */
	void convertToPureNumber() {
		pureNumber = year * 100000000
			+ month * 1000000
			+ mday * 10000
			+ hour * 100
			+ minute;
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
	ActTime() {};
	void write(std::ostream out) {
		out << pureNumber;
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
			timeStruct->tm_year,
			timeStruct->tm_mon,
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
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		// std::cout << toString() << std::endl;
		if (!isPause) {
			simulateTime += speed / 10;
			timeStruct = localtime(&simulateTime);
		}
	}
};
extern SimTime currentTime;
#endif
