#include <cstdio>
#include <iostream>
#include <fstream>
#include <ctime> //获取系统时间
#include "log.h"
#include "mkdir.h"
#include "account.h" //获取当前用户的信息
using namespace std;

Log logger;
Log::Log() {
	createDirectory("log"); //创建log目录
	time_t rawtime;
	time(&rawtime);
	char logPath[30]; //日志文件的路径和文件名，通过当前时间来唯一生成
	strftime(logPath, 30, "./log/%y%m%d_%H%M%S.log", localtime(&rawtime));
	logFile.open(logPath, ios::out);
	write("Program launched.", SYS);
}

string Log::getTimeStamp() { //获取当前时间戳
	time_t rawtime;
	time(&rawtime);
	char buffer[30];
	strftime(buffer, 30, "[%Y-%m-%d %H:%M:%S]", localtime(&rawtime));
	std::string ret(buffer);
	return ret;
}

void Log::write(string info, TAG tag) {
	//向文件内写日志，例："[1970-01-01 00:00:00] <header>: <something>"
	logFile << getTimeStamp();
	string header;
	switch (tag) { //根据不同的tag来确定header
		case USER:
			header = currentAccount->profile.id;
			break;
		case SYS:
			header = "System";
			break;
		default:
			header = "";
			break;
	}
	logFile << ' ' << header << ": ";
	logFile << info << endl;
	/* 以下是三个有用的宏，必要时也可以用于日志的输出。
	 * __FUNCTION__ 或 __func__ 当前函数名
	 * __LINE__ 当前代码行
	 * __FILE__ 当前源文件名
	 */
}
