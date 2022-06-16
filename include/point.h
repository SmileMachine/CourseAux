#ifndef POINT_H
#define POINT_H
#include<iostream>
using namespace std;
#include<string>

class Point {
public:
	int id;//标号
	string info;//建筑物名称
	int type;//是否是节点还是建筑物和服务设施，来决定是否输出
};
#endif
