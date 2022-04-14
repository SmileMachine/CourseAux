#include<iostream>
using namespace std;

class homework//作业类 
{
	string homeworkName;//这个作业的名字 
	bool flag;//这个作业是否完成  	
};

class classes//课程类 
{
	private:
		string className;//课程名字 “数据结构” 
		int classPlace;//课程地点 建筑物以标号命名 
		string classTime//课程时间 课程时间命名 24小时 8:00-9:35 13:00-14:35 如此 
		//string classpdf;//当前课程资料 是不是得弄一个string的数组//上传下载 
		string classQq;//课程群联系方式。就随便导入qq号
		//homework * //链表实现作业	 
		
};

class building//建筑类 
{
	string buildingName;
	int buildingPlace;//建筑标号  
	int buildingX;//建筑坐标 
	int buildingY;
	 
}; 

class test//考试类 
{
	string testName;//考试时间 命名  2022-07-01-8：00-10：00
	int testPlace;//考试地点 以建筑物命名，如果需要输出，可以写一个转换函数 
};

class play//活动类 
{
	string personPlayName;//活动名称 
	int personPlayPlace;//活动地点
	int startPersonPlay;//起始周
	int endPersonPlay;//终止周 
	string personPlayTime;//活动时间，命名方式 8：00-10：00
};

int main()
{
	
	system("pause");
	return 0;
} 
