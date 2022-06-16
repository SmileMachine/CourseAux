#include <iostream>
#include <io.h>
#include <string>
#include <vector>
#include <fstream>
#include "file_manage.h"
#include "version_control.h"
using namespace std;
vector<string>* FileManage::getFileList(string path) {
	vector<string>& files = *new vector<string>;
	// 文件句柄
	long hFile = 0;
	// 文件信息
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {
		do {
			// 跳过当前目录和父目录
			if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
				// 保存文件的全路径
				files.push_back(p.assign(path).append("/").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);  //寻找下一个，成功返回0，否则-1

		_findclose(hFile);
	}
	return &files;
}
