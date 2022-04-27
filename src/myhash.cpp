#include "myhash.h"
#include <fstream>
unsigned int myhash::seed = 331;

/**
 * @brief 计算一个字符串的BKDR_Hash
 *
 * @param str 要计算的字符串
 * @return 无符号32位整数，该文件的哈希值
 */
myhash::HashVal myhash::stringHash(const char str[]) {
	HashVal ret = 0;
	for (int i = 0; str[i]; i++) {
		ret = ret * seed + str[i];
	}
	return ret;
}

/**
 * @brief 计算一个文件的BKDR_Hash
 *
 * @param path 该文件的路径
 * @return 无符号32位整数，该文件的哈希值
 */
myhash::HashVal myhash::fileHash(std::string path) {
	std::fstream fin(path);
	return fileHash(fin);
}

/**
 * @brief 计算一个文件的BKDH_Rash
 *
 * @param file 文件流
 * @return 无符号32位整数，该文件的哈希值
 */
#include <iostream>
myhash::HashVal myhash::fileHash(std::fstream&file) {
	HashVal ret = 0;
	file >> std::noskipws; //不跳过空白字符
	while (true) {
		char tmp;
		file >> tmp;
		if (file.eof()) {
			break;
		}
		ret = ret * seed + tmp;
	}
	return ret;
}
