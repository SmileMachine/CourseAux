#include <iostream>
#include <cstdlib>
using namespace std;

#include "UI.h"
#include "myhash.h"
void UI::debugMenu() {
	system("cls");
	while (true) {
		cout << "1. 文件查重\n";
		cout << "2. 文件压缩\n";
		cout << "3. 文件解压\n";
		cout << "0. 返回上层\n";
		int select = getSelection();
		switch (select) {
			case 1: {
				cout << "请输入文件1的路径。\n";
				string file1 = getPath();
				cout << "请输入文件2的路径。\n";
				string file2 = getPath();
				if (myhash::fileHash(file1) == myhash::fileHash(file2)) {
					cout << "文件相同\n";
				} else {
					cout << "文件不同\n";
				}
				break;
			}
			case 2: { //未实现
				cout << "请输入文件的路径。\n";
				string filePath = getPath();
				// fileCompress(filePath);
				break;
			}
			case 3: { //未实现
				cout << "请输入文件的路径。\n";
				string filePath = getPath();
				// fileDeCompress(filePath);
				break;
			}
			case 0:
				return;
			default:
				break;
		}
	}
}
