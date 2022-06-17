#ifndef BUS_H
#define BUS_H
#include<iostream>
#include<string>
using namespace std;

class Bus {
public:
	int id;//标号,表示是哪辆车
	string id1;//表示车牌号
	bool isBus;//是否是校车，1表示公交车，0表示校车
	int startTime;//起始时间,单位秒
	int length;//车行驶的距离
	int time;// 经过的时间，单位秒
	int way;//1表示沙河到海淀，0表示海淀到沙河
};
#endif
