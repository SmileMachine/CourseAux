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
	std::ofstream logFile;
};
extern Log logger;
#endif
