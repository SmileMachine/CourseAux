#ifndef UI_H
#define UI_H
#include <iostream>
class UI {
public:
	//工具类函数及状态变量
	static bool isLoggedIn;
	static bool exit;
	static char*getString(char input[], int len); //从stdio读取字符串
	static int getSelection(); //获取用户输入的选择
	static bool isFileExist(std::string path); //判断文件是否存在
	static std::string getPath();
	//主菜单
	static void entry(); //整个系统的入口
	static void login(); //登录界面
	//用户账户相关
	static void accountMenu();
	static void printAccountInfo();
	static void changePassword();
	//学生菜单
	static void activityMenu(); //活动管理
	static void infoQuery(); //信息查询
	static void courseNavigate(); //课程导航
	//管理员菜单

	//调试用
	static void debugMenu();
};
#endif
