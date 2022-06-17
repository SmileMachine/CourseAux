#include <iostream>
#include <io.h>
#include <string>
#include <vector>
#include <fstream>
#include "log.h"
#include "UI.h"
#include "file_manage.h"
#include "version_control.h"
#include "account.h"

using namespace std;
void UI::uploadMaterial() {
	cout << "请输入文件路径\n";
	string filePath;
	getline(cin, filePath);
	string fileName = FileManage::getFileName(filePath);
	vector<string>*ls = FileManage::getFileList(MATERIAL_PATH);
	FileManage::submitFile(filePath, MATERIAL_PATH + fileName);
	bool hasDuplicate = false;
	int cnt = 0;
	for (auto v : *ls) {
		if (FileManage::duplicateChecking(MATERIAL_PATH + fileName, v)) {
			hasDuplicate = true;
			if (fileName != FileManage::getFileName(v)) {
				FileManage::deleteFile(MATERIAL_PATH + fileName);
			}
			break;
		}
	}
	if (hasDuplicate) {
		cout << "该资料你已经上传过\n";
		logger.write("Material repeat.");
	} else {
		cout << "上传到:" << MATERIAL_PATH << endl;
		logger.write("Material uploaded: " + filePath);
	}
}
void UI::viewMaterial() {
	vector<string>*ls = FileManage::getFileList(MATERIAL_PATH);
	int cnt = 0;
	for (auto v : *ls) {
		cout << cnt++ << ": " << v << endl;
	}
	if (ls->size()) {
		cout << "是否要下载文件？(1/0)\n";
		bool download = getNumber(0, 1);
		if (download) {
			cout << "请选择要下载的文件：";
			int downloadID = getNumber(1, ls->size());
			string fileName = ls->at(downloadID);
			FileManage::downloadFile(fileName);
			cout << "下载完成，文件保存在 \"./download/\" 目录下\n";
			logger.write("Download " + fileName);
		}
	} else {
		cout << "没有资料\n";
	}
}
void UI::submitHomework(Homework&homework) {
	cout << "请输入文件路径\n";
	string filePath;
	getline(cin, filePath);
	string fileName = FileManage::getFileName(filePath);
	VersionControl toSubmit(
		currentAccount->profile.id,
		homework.course,
		homework.homework,
		fileName
	);
	string newName = toSubmit.getFileName();
	cout << newName << endl;
	FileManage::submitFile(filePath, UPLOAD_PATH + newName);
	VersionControlList ls;
	VersionControl*dup = ls.duplicateChecking(toSubmit);
	if (dup) {
		cout << "你的作业与" << dup->getFileName() << "重复！" << endl;
		logger.write("Homework duplicate:" + dup->getFileName());
	} else {
		homeworkList.doneHomework(homework);
		logger.write("Upload homework:" + newName);
		cout << "上传完成\n";
	}
}

void UI::viewSubmittedHomework() {
	VersionControlList ls(currentAccount->profile.id);
	ls.print(0);
	if (ls.list.size()) {
		cout << "是否要下载文件？(1/0)\n";
		bool download = getNumber(0, 1);
		if (download) {
			cout << "请选择要下载的文件：";
			int downloadID = getNumber(1, ls.list.size());
			string fileName = ls.list[downloadID - 1].getFileName();
			FileManage::downloadFile(UPLOAD_PATH + fileName);
			cout << "下载完成，文件保存在 \"./download/\" 目录下\n";
			logger.write("Download " + fileName);
		}
	}
}

