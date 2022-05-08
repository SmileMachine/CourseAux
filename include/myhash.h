#ifndef MYHASH_H
#define MYHASH_H
#include <fstream>
namespace myhash {
	typedef unsigned int HashVal;
	extern unsigned int seed;
	HashVal stringHash(const char str[]);
	HashVal stringHash(std::string str);
	HashVal fileHash(std::fstream&file);
	HashVal fileHash(std::string path);
}
#endif
