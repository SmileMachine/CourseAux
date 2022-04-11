#include "data_io.h"
// Data_io 用于读写一个csv文件。
// 自定义csv行首注释: '#'，即如果在读到'#'则跳过此行。方便在csv中添加表头，或者注释某个条目
DataReader::DataReader(const char path[])
	: file(fopen(path, "r")) {
	if (file == NULL) {
		printf("文件或路径 \"%s\" 不存在，请检查工作目录是否正确。", path);
		exit(-1);
	}
}
DataReader::~DataReader() {
	fclose(file);
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
	return feof(file);
}
