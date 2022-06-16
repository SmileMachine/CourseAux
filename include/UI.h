#ifndef UI_H
#define UI_H
#include <iostream>
class UI {
public:
	//工具类函数及状态变量
	static bool CLEAR;
	static bool isLoggedIn;
	static bool exit;
	static char*getString(char input[], int len); //从stdio读取字符串
	static int getNumber(int lb = 0, int hb = 2147483647); //获取用户输入的选择
	static bool isFileExist(std::string path); //判断文件是否存在
	static std::string getPath(); //读取用户输入的文件路径;
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
	static void courseQuery();//课程查询
	static void courseNavigate(); //课程导航
	static void activityQuery(); //活动查询
	static void createActivity(); //创建活动
	//管理员菜单
	static void courseArrange();//课程、考试安排
	static void settings();//系统设置
	//调试用
	static void debugMenu();
};
#endif
