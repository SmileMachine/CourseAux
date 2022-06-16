#ifndef FILEMANAGE_H
#define FILEMANAGE_H
#include "myhash.h"
#include "huffman_codec.h"
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
using std::string;
#define UPLOAD_PATH "./upload/"
#define DOWNLOAD_PATH "./download/"
class FileManage {
public:
	static bool duplicateChecking(string path1, string path2) {
		return myhash::fileHash(path1) == myhash::fileHash(path2);
	}
	static void submitFile(string filePath, string newName) {
		encode(filePath, "./upload/" + newName);
	}
	static void downloadFile(string filePath) {
		std::cout << filePath << std::endl;
		decode(filePath, "./download/" + getFileName(filePath));
	}
	// void openFile()
	// D:\Documents\GitHub\shujujiegou27\README.md 
	// 0713-CourseName-HomeworkName-01-dasjkdfgajsgd.md
	static string getFileName(string path) {
		int pos = path.find_last_of('\\');
		if (pos == -1) {
			pos = path.find_last_of('/');
		}
		if (pos == -1) {
			return path;
		} else {
			return path.substr(pos + 1);
		}
	}
	static std::vector<string>*getFileList(string dir);
};
#endif
