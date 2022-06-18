#include <iostream>
#include "log.h"
#include<fstream>
#include <string>
#include <stack>
#include <windows.h>
#include<iomanip>
#include <sstream>
#include "edge.h"
#include "point.h"
#include"bus.h"
#include"sim_time.h"
#include "course.h"
#include "UI.h"
#include "my_stack.h"
using namespace std;

#define MAXNUM 167//建筑物＋节点数量
#define INF 1e4 //此路不通
#define busNumber 20//车总数

double speedFoot = 1.5;//步行速度m/s
double speedBike = 5.6;//骑车速度m/s
int shaHePoint = 44;//沙河校区总建筑物＋中间节点数
int haiDianPoint = 200;//海淀校区总建筑物+中间节点数
enum Method {
	DIST,
	TIME,
	RIDINGTIME,
};
Point ve[MAXNUM];//点类
Edge we[MAXNUM][MAXNUM];//边类
Bus bus[20];//车类，包括公交汽车和班车,最多十辆车
void outputRoute(int path[], int start, int end);
void readPoint()//读建筑物的文件到ve数组中，记录建筑物和中间节点的信息
{
	int i = 0;
	int j;
	ifstream ifs;
	ifs.open("./data/point.txt", ios::in);
	if (!ifs.is_open()) {
		cout << "打开point.txt文件失败" << endl;
		return;
	}
	for (int i = 0; i < MAXNUM; i++) {
		ifs >> j;
		ifs >> ve[j].info;
		ifs >> ve[j].type;
		ve[j].id = j;
	}
	ifs.close();
}
void readBus()//读建筑物的文件到bus数组中，记录车辆信息
{
	int i = 0;
	ifstream ifs;
	ifs.open("./data/bus.txt", ios::in);
	if (!ifs.is_open()) {
		cout << "打开bus.txt文件失败" << endl;
		return;
	}
	while (!ifs.eof()) {
		ifs >> bus[i].id &&ifs >> bus[i].id1&& ifs >> bus[i].isBus&& ifs >> bus[i].startTime&&
			ifs >> bus[i].length&& ifs >> bus[i].time&&
			ifs >> bus[i].way;//沙河到海淀1 海淀到沙河0
		i++;
	}
	ifs.close();
}
void readEdge()//读边的文件到we数组中，记录每条边的信息
{
	int i;
	int j;
	ifstream ifs;
	ifs.open("./data/edge.txt", ios::in);
	if (!ifs.is_open()) {
		cout << "打开edge.txt文件失败" << endl;
		return;
	}
	while (!ifs.eof()) {
		ifs >> i;
		ifs >> j;
		ifs >> we[i][j].length;
		ifs >> we[i][j].bike;
		ifs >> we[i][j].crowd;
		ifs >> we[i][j].time;
		we[j][i] = we[i][j];
		we[j][i].start = we[i][j].start = i;
		we[j][i].start = we[i][j].end = j;
	}
	ifs.close();
}
void init() {
	//初始化时间
	for (int i = 0; i < MAXNUM; i++) {
		for (int j = 0; j < MAXNUM; j++) {
			if (we[i][j].length) {
				we[i][j].time = we[i][j].length / (we[i][j].crowd * speedFoot);
			} else {
				we[i][j].time = 0;
			}
			if (we[i][j].bike) {
				we[i][j].ridingTime = we[i][j].length / (we[i][j].crowd * speedBike);
			} else {
				we[i][j].ridingTime = 0;
			}
		}
	}
}
//建筑物信息界面输出
void choose() {
	cout << "===============================================================================================================================" << endl;
	cout << "||***************************************************北京邮电大学建筑物信息***************************************************||" << endl;
	cout << "||                                                                                                                            ||";
	int count = 0;
	for (int i = 0; i < MAXNUM; i++) {
		if (ve[i].type) {
			if (count == 0) {
				cout << endl << "||";
			}
			printf("%3d、", ve[i].id);
			cout << setiosflags(ios::left) << setw(26) << ve[i].info;
			count++;
			if (count == 4) {
				cout << "||";//末尾边框
				count = 0;
			}
		}
	}
	cout << endl << " ==============================================================================================================================" << endl;
	cout << "序号0-43为沙河校区建筑物" << endl;
	cout << "序号44以后为本部校区建筑物" << endl;
	cout << "1. 实现两点间导航(包括根据上课地点)" << endl;
	cout << "2. 根据上课时间导航上课地点" << endl;
	cout << "3. 根据课程名称导航上课地点" << endl;
	cout << "0. 返回上层" << endl;
}
void outputProject(int method, int path[], double shortest, int start, int end) {
	if (shortest == INF) {
		cout << "对不起" << endl
			<< ve[start].info << "到" << ve[end].info << "没有您想要方案的路径" << endl;
	} else {
		switch (method) {
			case DIST: {
				cout << "最短距离路径查找如下" << endl
					<< ve[start].info << "->" << ve[end].info
					<< "的最短距离为:" << shortest << "m" << endl
					<< "路径为：" << endl;
				logger.write("Find shortest distance route from " + ve[start].info + " to " + ve[end].info + ".");
				break;
			}
			case TIME: {
				cout << "最短时间方案的路径查找如下" << endl
					<< ve[start].info << "->" << ve[end].info
					<< "的最短时间为：" << shortest / 60 << "min" << endl
					<< "路径为：" << endl;
				logger.write("Find shortest time route from " + ve[start].info + " to " + ve[end].info + ".");
				break;
			}
			case RIDINGTIME: {
				cout << "骑车最短时间方案的路径查找如下" << endl
					<< ve[start].info << "->" << ve[end].info
					<< "的最短时间为:" << shortest / 60 << "min" << endl;
				logger.write("Find shortest riding time route from " + ve[start].info + " to " + ve[end].info + ".");
				break;
			}
		}
		outputRoute(path, start, end);
	}
}
/**
 * @brief 根据所选方案和起止点，找到合适的路径，并返回最短长度或最短时间
 * @param method 方案类型
 * @param path 储存路径的数组
 * @param start 起点
 * @param end 终点
 * @return double 最短长度或最短时间
 */
double getPath(int method, int path[], int start, int end) {
	double dis[MAXNUM];//dist数组中保存的是起始点到数组下标对应顶点的路径长度（累加的结果）
	// int path[MAXNUM]; //path数组中保存的是对应path数组下标顶点的前驱顶点（前一个顶点）
	int visited[MAXNUM];//代表是否访问过点
	//初始化
	for (int i = 0; i < MAXNUM; i++) {
		//是开始的点
		if (i == start) {
			dis[i] = 0;
		} else {
			double data;
			switch (method) {
				case DIST: {
					data = we[start][i].length;
					break;
				}
				case TIME: {
					data = we[start][i].time;
					break;
				}
				case RIDINGTIME: {
					data = we[start][i].ridingTime;
					break;
				}
			}
			if (data == 0) {
				dis[i] = INF;//不存在边、时间
			} else {
				dis[i] = data;//存在边、时间
			}
		}
		path[i] = -1;//由于顶点标号都是从0开始，所以在这里把他们都初始化为-1
		visited[i] = 0;
	}
	visited[start] = 1;//把所有顶点分成两个集合
	while (1) {
		int k = 0;//标志是否访问过
		int next;
		for (next = 0; next < MAXNUM; next++)//找到一个没被访问的点
		{
			if (visited[next] == 0) {
				k = 1;
				break;
			}
		}
		int min = next;//min临时设定这个点
		if (k == 0)
			break;
		for (int i = 0; i < MAXNUM; i++)//遍历这个dis，找最短距离，找下一个节点
		{
			if (visited[i] == 0)
				if (dis[min] > dis[i])
					min = i;
		}
		visited[min] = 1; //把这个点变成已经访问过
		for (int i = 0; i < MAXNUM; i++)//更新dis数组
		{
			double data;
			switch (method) {
				case DIST: {
					data = we[min][i].length;
					break;
				}
				case TIME: {
					data = we[min][i].time;
					break;
				}
				case RIDINGTIME: {
					data = we[min][i].ridingTime;
					break;
				}
			}
			if (data != 0 && visited[i] == 0 //i是min的邻接点
				&& dis[i] > dis[min] + data) {
				dis[i] = dis[min] + data;//记录最短长度，更新
				path[i] = min; //记录前面的那个点，便于最后逆推
			}
		}
	}
	return dis[end];
}
//方案1，最短距离
void shortestDistance(int start, int end) {
	int path[MAXNUM]; //path数组中保存的是对应path数组下标顶点的前驱顶点（前一个顶点）
	double shortest = getPath(DIST, path, start, end);
	outputProject(DIST, path, shortest, start, end);
}
//方案2 最短时间
void shortestTime(int start, int end) {
	int path[MAXNUM]; //path数组中保存的是对应path数组下标顶点的前驱顶点（前一个顶点）
	double shortest = getPath(TIME, path, start, end);
	outputProject(TIME, path, shortest, start, end);
}
//骑车的最短时间
void ridingShortestTime(int start, int end) {
	int path[MAXNUM]; //path数组中保存的是对应path数组下标顶点的前驱顶点（前一个顶点）
	double shortest = getPath(RIDINGTIME, path, start, end);
	outputProject(RIDINGTIME, path, shortest, start, end);
}

void outputRoute(int path[], int start, int end) {
	//输出
	int n = end;
	myStack output;
	// stack<int> output;
	//持续入栈
	output.push(end);
	while (path[n] != -1) {
		output.push(path[n]);
		n = path[n];//根据记录的前一个顶点决定入栈的下一个节点是谁
	}
	output.push(start);
	//出栈加输出
	cout << ve[output.top()].info;
	output.pop();
	while (!output.empty()) {
		// cout << output.topIdx << '\t' << output.size << endl;
		cout << "->" << ve[output.top()].info;
		output.pop();
	}
	cout << endl;
}
void busShahe2Haidian(int select)//沙河到海淀方案
{
	ActTime now = currentTime.getActTime();//用于获取当前静态时间
	int min[busNumber];//最小车辆信息的标号，可以是多辆
	int j = 0;
	for (int i = 0; i < busNumber; i++)//初始化最小车辆信息的标号
	{
		min[i] = -1;
	}
	min[0] = 0;
	switch (select) {
		case 1://最短距离的车辆信息
		{
			for (int i = 1; i < busNumber; i++) {
				if (bus[i].startTime > now.nowTime() && bus[i].way == 1)//符合要求的车辆
				{
					if (bus[i].length < bus[min[0]].length)//更新最小距离车辆
					{
						min[0] = i;
						min[1] = -1;
					}
					if (bus[i].length == bus[min[0]].length) {
						min[j + 1] = i;
						j++;
					}
				}
			}
			cout << "您可以从沙河西门乘坐的车辆信息如下" << endl;
			for (int i = 0; min[i] != -1; i++) {
				//i就是沙河到海淀最短距离的班车车牌号
				cout << "这是" << ((bus[min[i]].isBus == 1) ? "公交车" : "校车") << endl;
				cout << "车牌号是:" << bus[min[i]].id1 << " "
					<< "乘车时间是:" << bus[min[i]].startTime / 3600 << ":00" << endl
					<< "大约行驶距离是:" << bus[min[i]].length / 1000 << "km" << " "
					<< "大约需要:" << bus[min[i]].time / 60 << "min" << endl;
				logger.write("Find shortest distance route for bus from Shahe to Haidian.");
			}
		}
		break;
		case 2:
		case 3://最短时间
		{
			for (int i = 1; i < busNumber; i++) {
				if (bus[i].startTime > now.nowTime() && bus[i].way == 1)//符合要求的车辆
				{
					if (bus[i].time < bus[min[0]].time)//更新最小距离车辆
					{
						min[0] = i;
						min[1] = -1;
					}
					if (bus[i].time == bus[min[0]].time) {
						min[j + 1] = i;
						j++;
					}
				}
			}
			cout << "您可以从沙河西门乘坐的车辆信息如下" << endl;
			for (int i = 0; min[i] != -1; i++) {
				//i就是沙河到海淀最短距离的车辆车牌号
				cout << "这是" << ((bus[min[i]].isBus == 1) ? "公交车" : "校车") << endl;
				cout << "车牌号是:" << bus[min[i]].id1 << " "
					<< "乘车时间是:" << bus[min[i]].startTime / 3600 << ":00" << endl
					<< "大约行驶距离是:" << bus[min[i]].length / 1000 << "km" << " "
					<< "大约需要:" << bus[min[i]].time / 60 << "min" << endl;

				logger.write("Find shortest time route for bus from Shahe to Haidian.");
			}
			break;
		}
	}
}
void busHaidian2Shahe(int select)//海淀到沙河
{
	ActTime now = currentTime.getActTime();
	int min[busNumber] = { -1 };//最小车辆信息的标号，可以是多辆
	int j = 0;
	for (int i = 0; i < busNumber; i++) {//初始化最小车辆信息的标号
		min[i] = -1;
	}
	min[0] = 0;
	switch (select) {
		case 1://最短距离的车辆信息
		{
			for (int i = 1; i < busNumber; i++) {
				if (bus[i].startTime > now.nowTime() && bus[i].way == 0)//符合要求的车辆
				{
					if (bus[i].length < bus[min[0]].length)//更新最小距离车辆
					{
						min[0] = i;
						min[1] = -1;
					}
					if (bus[i].length == bus[min[0]].length) {
						min[j + 1] = i;
						j++;
					}
				}
			}
			cout << "您可以从海淀校门乘坐的车辆信息如下" << endl;
			for (int i = 0; min[i] != -1; i++) {
				//i就是沙河到海淀最短距离的路径
				cout << "这是" << ((bus[min[i]].isBus == 1) ? "公交车" : "校车") << endl;
				cout << "车牌号是:" << bus[min[i]].id1 << " "
					<< "乘车时间是:" << bus[min[i]].startTime / 3600 << ":00" << endl
					<< "大约行驶距离是:" << bus[min[i]].length / 1000 << "km" << " "
					<< "大约需要:" << bus[min[i]].time / 60 << "分" << endl;

				logger.write("Find shortest distance route for bus from Haidian to Shahe.");
			}
		}
		break;
		case 2:
		case 3://最短时间
		{
			for (int i = 1; i < busNumber; i++) {
				if (bus[i].startTime > now.nowTime() && bus[i].way == 0)//符合要求的车辆
				{
					if (bus[i].time < bus[min[0]].time)//更新最小距离车辆
					{
						min[0] = i;
						min[1] = -1;
					}
					if (bus[i].time == bus[min[0]].time) {
						min[j + 1] = i;
						j++;
					}
				}
			}
			cout << "您可以从海淀校门乘坐的车辆信息如下" << endl;
			for (int i = 0; min[i] != -1; i++) {
				//i就是海淀到沙河最短距离的路径
				cout << "这是" << ((bus[min[i]].isBus == 1) ? "公交车" : "校车") << endl;
				cout << "车牌号是:" << bus[min[i]].id1 << " "
					<< "乘车时间是:" << bus[min[i]].startTime / 3600 << ":00" << endl
					<< "大约行驶距离是:" << bus[min[i]].length / 1000 << "km" << " "
					<< "大约需要:" << bus[min[i]].time / 60 << "分" << endl;

				logger.write("Find shortest time route for bus from Haidian to Shahe.");
			}
			break;
		}
	}
}
int findIdByTime();
int findIdByAddress();
int findIdByName();
//大的导航策略
void Route() {
	//读文件操作
	readPoint();
	readEdge();
	readBus();
	init();
	choose();
	int navigateMethod;
	int end;
	int start;
	// while (1) 
	{
		cin >> navigateMethod;
		switch (navigateMethod) {
			case 0: {
				return;
				break;
			}
			case 1: {//两点间导航
				cout << "请输入您的起点" << endl;
				cin >> start;
				cout << endl << "请输入您的终点" << endl;//可以根据课程名称，上课时间等查询地点
				cin >> end;
				cout << endl;
				break;
			}
			case 2: {//根据上课时间导航课程地点
				cout << "请输入您的起点" << endl;
				cin >> start;
				end = findIdByTime();
				break;
			}
			case 3: {//根据课程名称导航课程地点
				cout << "请输入您的起点" << endl;
				cin >> start;
				end = findIdByName();
				break;
			}
			default: {
				cout << "输入错误，请重新输入" << endl;
				break;
			}
		}
	}
	if (end < 0) {
		cout << "找不到终点\n";
		return;
	}
	cout << endl << "请选择您的导航策略" << endl
		<< "1-----最短距离，步行" << endl
		<< "2-----最短时间，步行" << endl
		<< "3-----最短时间，骑车" << endl;
	int select;
	cin >> select;
	cout << endl;
	if ((0 <= start && start < shaHePoint && 0 <= end && end < shaHePoint) || (shaHePoint <= start && start < haiDianPoint && shaHePoint <= end && end < haiDianPoint))//在沙河或海淀的导航
	{
		switch (select) {
			case 0:
			{
				cout << "感谢本次使用，欢迎下次使用" << endl;
				return;
			}
			system("pause");
			break;
			case 1://最短距离，步行
			{
				shortestDistance(start, end);
				cout << "导航成功" << endl;
			}
			system("pause");
			break;
			case 2://最短时间，步行
			{

				shortestTime(start, end);
			}
			system("pause");
			break;
			case 3://最短时间，骑车
			{
				ridingShortestTime(start, end);
			}
			system("pause");
			break;
		}
	} else if (0 <= start && start < shaHePoint && shaHePoint <= end && end < haiDianPoint)//沙河到海淀
	{
		switch (select) {
			case 0:
			{
				cout << "感谢本次使用，欢迎下次使用" << endl;
				return;
			}
			system("pause");
			break;
			case 1://最短距离，步行
			{
				cout << "在沙河校区内" << endl;
				shortestDistance(start, 0);//0代表沙河西门
				cout << endl;
				busShahe2Haidian(select);//最短距离，沙河到海淀	
				cout << endl << "在本部校区内" << endl;
				shortestDistance(shaHePoint, end);//shaHePoint代表海淀的校门标号
				cout << "导航成功" << endl;
			}
			system("pause");
			break;
			case 2://最短时间，步行
			{
				cout << "在沙河校区内" << endl;
				shortestTime(start, 0);//0代表沙河西门
				cout << endl;
				busShahe2Haidian(select);//最短距沙河到海淀
				cout << endl << "在本部校区内" << endl;
				shortestTime(shaHePoint, end);//shaHePoint代表海淀的校门标号
				cout << "导航成功" << endl;
			}
			system("pause");
			break;
			case 3:
			{
				cout << "在沙河校区内" << endl;
				ridingShortestTime(start, 0);//0代表沙河西门
				cout << endl;
				busShahe2Haidian(select);//最短距沙河到海淀
				cout << endl << "在本部校区内" << endl;
				ridingShortestTime(shaHePoint, end);//shaHePoint代表海淀的校门标号
				cout << "导航成功" << endl;
			}
			system("pause");
			break;
		}
	} else//海淀到沙河
	{
		switch (select) {
			case 0:
			{
				cout << "感谢本次使用，欢迎下次使用" << endl;
				return;
			}
			system("pause");
			break;
			case 1://最短距离，步行
			{
				cout << "在本部校区内" << endl;
				shortestDistance(start, shaHePoint);//0代表沙河西门
				cout << endl;
				busHaidian2Shahe(select);//最短距离海淀到沙河
				cout << endl << "在沙河校区内" << endl;
				shortestDistance(0, end);//shaHePoint代表海淀的校门标号
				cout << "导航成功" << endl;
			}
			system("pause");
			break;
			case 2://最短时间，步行
			{
				cout << "在本部校区内" << endl;
				shortestTime(start, shaHePoint);//0代表沙河西门
				cout << endl;
				busHaidian2Shahe(select);//海淀到沙河
				cout << endl << "在沙河校区内" << endl;
				shortestTime(0, end);//shaHePoint代表海淀的校门标号
				cout << "导航成功" << endl;
			}
			system("pause");
			break;
			case 3:
			{
				cout << "在本部校区内" << endl;
				ridingShortestTime(start, shaHePoint);//0代表沙河西门
				cout << endl;
				busHaidian2Shahe(select);//海淀到沙河
				cout << endl << "在沙河校区内" << endl;
				ridingShortestTime(0, end);//shaHePoint代表海淀的校门标号
				cout << "导航成功" << endl;
			}
			system("pause");
			break;
		}
	}


}
int findIdByAddress(string addr) {
	for (int i = 0;i < MAXNUM;i++) {
		if (ve[i].info == addr) {
			return ve[i].id;
		}
	}
	return -1;
}
int findIdByTime() {
	cout << "请输入上课时间：(0-周日，1-周一，...，6-周六)" << endl;
	CourseTime time;
	time.weekday = UI::getNumber(0, 6);
	cout << "请输入上课时间：\n";
	cout << "时：";
	time.hour = UI::getNumber();
	cout << "分：";
	time.minute = UI::getNumber();
	CourseTime timeAfterAnHour = time;
	timeAfterAnHour.hour++;
	courseList.sortByTime();
	for (auto v : courseList.list) {
		if (time < v.courseTime && v.courseTime < timeAfterAnHour) {
			cout << "你要上的课是：\n";
			v.print(1);
			stringstream ss(v.courseAddress);
			string addr;
			ss >> addr >> addr;
			return findIdByAddress(addr);
		}
	}
	return -1;
}
int findIdByName() {
	cout << "请输入课程名称：\n";
	string name;
	getline(cin, name);
	for (auto v : courseList.list) {
		if (v.courseName == name) {
			return findIdByAddress(v.courseAddress);
		}
	}
	return -1;
}
// int main() {
// 	Route();
// 	return 0;
// }
