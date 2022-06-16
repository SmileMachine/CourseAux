#include <iostream>
#include <io.h>
#include <string>
#include <vector>
#include <fstream>
#include "file_manage.h"
#include "version_control.h"
using namespace std;
void VersionControl::dissolve(string versionFile) {
	stringstream ss(versionFile);
	getline(ss, id, delim);
	getline(ss, course, delim);
	getline(ss, homework, delim);
	string tmp;
	getline(ss, tmp, delim);
	version = stoi(tmp);
	getline(ss, fileName);
}
int VersionControl::getVersion() {
	VersionControlList ls(id);
	int maxVersion = 0;
	for (auto v : ls.list) {
		if (v.id == id && v.course == course && v.homework == homework) {
			maxVersion = v.version > maxVersion ? v.version : maxVersion;
		}
	}
	return maxVersion + 1;
}
VersionControlList::VersionControlList(string id) {
	vector<string>*fileList = FileManage::getFileList("./upload");
	for (auto v : *fileList) {
		VersionControl item(FileManage::getFileName(v));
		if (item.id == id) {
			list.push_back(item);
		}
	}
}
VersionControlList::VersionControlList() {
	vector<string>*fileList = FileManage::getFileList("./upload");
	for (auto v : *fileList) {
		list.push_back(FileManage::getFileName(v));
	}
}
void VersionControl::print(int hasID) {
	if (hasID) {
		cout << "学号:" << id << endl;
	}
	cout << "课程:" << course << endl;
	cout << "作业:" << homework << endl;
	cout << "文件名:" << getFileName() << endl;
	cout << "版本:" << version << endl;
}
void VersionControlList::print(int hasID) {
	int cnt = 1;
	if (list.size() == 0) {
		cout << "你还没有上传过作业\n";
	}
	for (auto v : list) {
		cout << cnt++ << ":\n";
		v.print(hasID);
	}
	cout << endl;
}
