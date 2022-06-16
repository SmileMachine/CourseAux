#ifndef EDGE_H
#define EDGE_H
#include<iostream>
using namespace std;

class Edge {
public:
	int start;//开始标号
	int end;//结束标号
	int length;//长度,单位m
	int bike;//是否可以骑车 1可以 0不可以
	double crowd;//拥挤程度,每条道路不一样,是一个0-1的数字
	double time;//用于方案2的计算 单位秒
	double ridingTime;//用于方案3的计算 单位秒
};
#endif
//理想步行速度与理想骑车速度固定，
// 最短路径就是长度而言。
// 最短时间就是将长度度量换成时间度量。时间如何计算，就是长度÷（速度*拥挤）
//交通工具最短时间就是根据是否可以骑车，以及骑车的速度。
