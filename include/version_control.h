#ifndef VERSIONCONTROL_H
#define VERSIONCONTROL_H
#include "myhash.h"
#include "huffman_codec.h"
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include "file_manage.h"
using std::string;
class VersionControl {
	// "id-CourseName-HomeworkName-01-fileName.md"
public:
	string id;
	string course;
	string homework;
	int version;
	string fileName;
	char delim = '-';
	VersionControl() :
		version(1) {}
	VersionControl(string versionFile) {
		dissolve(versionFile);
	}
	VersionControl(
		string id,
		string course,
		string homework,
		string fileName
	) :
		id(id),
		course(course),
		homework(homework),
		fileName(fileName),
		version(getVersion()) {}
	int getVersion();
	void dissolve(string versionFile);
	void print(int hasID);
	std::string getFileName() {
		return id + delim
			+ course + delim
			+ homework + delim
			+ std::to_string(version) + delim
			+ fileName;
	}
};
class VersionControlList {
public:
	std::vector<VersionControl>list;
	VersionControlList(string id);
	VersionControlList();
	VersionControl*duplicateChecking(VersionControl src) {
		for (auto v = list.begin();v != list.end();v++) {
			if (FileManage::duplicateChecking(UPLOAD_PATH + v->getFileName(), UPLOAD_PATH + src.getFileName())) {
				if (v->id != src.id) {
					return &*v;
				}
			}
		}
		return NULL;
	}
	void print(int hasID);
};
#endif
