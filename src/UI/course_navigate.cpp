#include "UI.h"
#include <iostream>
#include <cstdlib>
using namespace std;

void UI::courseNavigate() {
	system("cls");
	cout << "课程导航\n";
	cout << "0.返回\n";
	int select = getSelection();

}
