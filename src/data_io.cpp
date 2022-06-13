#include "data_io.h"
// Data_io 用于读写一个csv文件。
// 自定义csv行首注释: '#'，即如果在读到'#'则跳过此行。方便在csv中添加表头，或者注释某个条目
#include <cstdlib>
#include <string>
#include <iostream>
using namespace std;
DataReader::DataReader(const char path[])
	:fin(path, ios::in) {
	// : file(fopen(path, "r")) {
	if (fin.fail()) {
		// if (file == NULL) {
		printf("文件或路径 \"%s\" 不存在，请查工作目录是否正确。", path);
		// 注意，可执行文件必须从项目的根目录打开，否则会出现相对路径导致的错误。
		exit(-1);
	}
}
DataReader::~DataReader() {
	// fclose(file);
	fin.close();
}
/**
 * @brief 从文件中读取下一项，存到dst中
 *
 * @param dst 接受字符串的变量
 * @return true 读取成功
 * @return false 读取失败
 */
bool DataReader::nextItem(string&dst) {
	char delimiter = ','; // 每一项的分隔符，可以是除换行和注释符外的任何符号。
	if (fin.eof()) {
		return false;
	}
	ignore();
	getline(fin, dst, delimiter); //读取一项
	// cout << dst << ": iseof=" << isEOF() << '\n';
	return true;
}
/**
 * @brief 忽略换行和注释
 */
void DataReader::ignore() {
	while (fin.peek() == '\n' || fin.peek() == '#') {
		while (fin.peek() == '\n') { //忽略多余的换行
			fin.ignore();
		}
		while (fin.peek() == '#') { //忽略注释
			string tmp;
			getline(fin, tmp, '\n');
		}
	}
}
bool DataReader::nextTerm(char dst[], int len) {
	//从文件中读下一项，存到dst中。
	if (file == NULL) {
		return false;
	}
	char ch;

	dst[len - 1] = '\0'; //提前将最后一位设置为'\0'，防止因为该字符串未初始化而导致bug
	for (int i = 0; i < len; i++) {
		ch = fgetc(file);
		if (ch == ',' || ch == '\n' || ch == EOF) { //读到分隔符就结束
			dst[i] = '\0';
			break;
		} else if (ch == '#') { //读到'#'则跳过本行
			do {
				ch = fgetc(file);
			} while (ch != EOF && ch != '\n');
			i--;
		} else {
			dst[i] = ch;
		}
	}
	if (dst[len - 1]) { //如果最后一位不是'\0',说明条目过长，需要截断。
		dst[len - 1] = '\0';
		do {
			ch = fgetc(file);
		} while (ch != ',' || ch != EOF || ch != '\n');
	}
	return ch;
}
bool DataReader::isEOF() {
	// return feof(file);
	ignore();
	return fin.eof() || fin.fail();
}
