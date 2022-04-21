#include <cstdio>
#include <iostream>
#include <fstream>
#include <ctime> //获取系统时间
#include "log.h"
#include "mkdir.h"
#include "account.h" //获取当前用户的信息
using namespace std;

Log logger;
/**
 * @brief 在当前工作目录下创建"./log"目录，并且在目录下用当前系统时间作为文件名创建文件，如"20220101_114514.log"
 */
Log::Log() {
	createDirectory("./log"); //创建log目录
	time_t rawtime;
	time(&rawtime);
	char logPath[30]; //日志文件的路径和文件名，通过当前时间来唯一生成
	strftime(logPath, 30, "./log/%y%m%d_%H%M%S.log", localtime(&rawtime));
	logFile.open(logPath, ios::out);
	write("Program launched.", SYS);
}
/**
 * @brief 获取当前时间戳
 * @return 形如"[1970-01-01 00:00:00]"的字符串
 */
string Log::getTimeStamp() {
	time_t rawtime;
	time(&rawtime);
	char buffer[30];
	strftime(buffer, 30, "[%Y-%m-%d %H:%M:%S]", localtime(&rawtime));
	std::string ret(buffer);
	return ret;
}
/**
 * @brief 向文件内写日志，例："[1970-01-01 00:00:00] <header>: <something>"
 *
 * @param info 要向日志写的内容。
 * @param tag 用于控制<header>
 */
void Log::write(string info, TAG tag) {
	logFile << getTimeStamp();
	string header;
	switch (tag) { //根据不同的tag来确定header
		case USER:
			header = currentAccount->profile.id;
			break;
		case SYS:
			header = "SYSTEM";
			break;
		default:
			header = "";
			break;
	}
	logFile << ' ' << header << ": " << info << endl;
	/**
	 * 以下是三个有用的宏，必要时也可以用于日志的输出。
	 * __FUNCTION__ 或 __func__ 当前函数名
	 * __LINE__ 当前代码行
	 * __FILE__ 当前源文件名
	 */
}
