#include <cstdio>
#include <iostream>
#include "UI.h"
#include "mkdir.h"
int main(int argc, char const* argv[]) {
	createDirectory("./upload");
	createDirectory("./download");
	createDirectory("./material");
	UI::entry();
	return 0;
}
/**
 * TODO:
 *  查询、排序算法优化
 */
