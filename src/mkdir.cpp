#ifdef _WIN32
#include <direct.h> // _mkdir()
#else //除了Windows，剩下的几乎都是类Unix系统
#include <sys/stat.h>
#include <sys/types.h>
#endif

#include "mkdir.h"

void createDirectory(const char path[]) {
	// 根据path创建文件夹
#ifdef _WIN32
	_mkdir(path);
#else
	mkdir(path, 0755); // 0755是文件夹的读写权限，此处为默认设置。
#endif
}
