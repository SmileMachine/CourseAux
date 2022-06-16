#include <iostream>
#include <cstdlib>
using namespace std;
#include "file_manage.h"
#include "version_control.h"
#include "UI.h"
#include "myhash.h"
#include "account.h"
#include "huffman_codec.h"
void UI::debugMenu() {
	CLEAR&&system("cls");
	while (true) {
		cout << "1. 文件查重\n";
		cout << "2. 文件压缩\n";
		cout << "3. 文件解压\n";
		cout << "4. 打印所有账户信息\n";
		cout << "0. 返回上层\n";
		int select = getNumber();
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
			case 2: {
				cout << "请输入文件的路径。\n";
				string filePath = getPath();
				encode(filePath, UPLOAD_PATH + FileManage::getFileName(filePath) + ".huf");
				// fileCompress(filePath);
				break;
			}
			case 3: {
				cout << "请输入文件的路径。\n";
				string filePath = getPath();
				decode(filePath, DOWNLOAD_PATH + FileManage::getFileName(filePath) + ".dehuf");
				// fileDeCompress(filePath);
				break;
			}
			case 4: {
				Account*tmp = currentAccount;
				cout << "size = " << accountList.size << endl;
				for (int i = 0;i < accountList.size;i++) {
					currentAccount = &accountList.list[i];
					printAccountInfo();
					cout << '\n';
				}
				currentAccount = tmp;
				system("pause");
				break;
			}
			case 0:
				return;
			default:
				break;
		}
	}
}
