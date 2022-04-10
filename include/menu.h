#ifndef MENU_H
#define MENU_H
char* mygets(char input[], int len);
class UI {
public:
	static void login();	//登录界面
	static void mainMenu(); //主菜单
};
#endif
