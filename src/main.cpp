#include <cstdio>
#include <iostream>
#include "UI.h"
int main(int argc, char const* argv[]) {
	while (!UI::exit) {
		UI::login();
		UI::mainMenu();
	}
	return 0;
}
