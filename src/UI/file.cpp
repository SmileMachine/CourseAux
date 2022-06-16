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
void UI::submitHomework(Homework&homework) {
	cout << "请输入文件路径\n";
	string filePath;
	getline(cin, filePath);
	string fileName = FileManage::getFileName(filePath);
	VersionControl versionFile;
	// int version;
	VersionControl toSubmit(
		currentAccount->profile.id,
		homework.course,
		homework.homework,
		fileName
	);
	string newName = toSubmit.getFileName();
	cout << newName << endl;
	FileManage::submitFile(filePath, newName);
	VersionControlList ls;
	VersionControl*dup = ls.duplicateChecking(toSubmit);
	if (dup) {
		cout << "你的作业与" << dup->getFileName() << "重复！" << endl;
		logger.write("Homework duplicate:" + newName);
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
			FileManage::downloadFile("./upload/" + fileName);
			cout << "下载完成，文件保存在 \"./download/\" 目录下\n";
			logger.write("Download " + fileName);
		}
	}
}

