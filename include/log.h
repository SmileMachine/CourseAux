#ifndef LOG_H
#define LOG_H
#include <string>
#include <fstream>
class Log {
public:
	Log();
	enum TAG {
		USER,
		SYS,
	};
	void write(std::string info, TAG = USER);
private:
	std::string getTimeStamp();
	std::ofstream logFile; //日志文件流
};
extern Log logger;
#endif
