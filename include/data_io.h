#ifndef DATA_IO_H
#define DATA_IO_H
#include <cstdio>
#include <string>
#include <fstream>
// Data_io 用于读写一个csv文件。
// 自定义csv行首注释: '#'，即如果在读到'#'则跳过此行。方便在csv中添加表头，或者注释某个条目
class DataReader {
public:
	DataReader(const char path[]); //参数为文件路径
	~DataReader();
	bool nextTerm(char dst[], int len); //读取下一项
	bool nextItem(std::string&dst); //读取下一项
	bool isEOF();
	void ignore();
private:
	FILE* file;
	std::ifstream fin;
	// 也可以读取一行到缓冲区
	// const int BUF_SIZE=100;
	// char*buf=new char[BUF_SIZE];
};
#endif
