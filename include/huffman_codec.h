#ifndef HUFFMANCODEC_H
#define HUFFMANCODEC_H
// #include <cstdio>
#include <string>
struct HufNode {
	int w;
	int l, r;
	int p;
	HufNode() :w(0), l(0), r(0), p(0) {}
};
class HufTree {
public:
	HufNode*tree;
	const int leafNum;
	HufTree(int n) :tree(new HufNode[n * 2]), leafNum(n) {
		// memset(tree,0,sizeof(HufNode)*leafNum*2);//没有必要，因为自动调用了HufNode的构造函数。
	}
	~HufTree() {
		// delete tree;//不能delete，因为HT可能作为参数传入。
	}
	HufNode&operator[](int n) {//方便其他地方使用此处数据。
		return tree[n];
	}
	//输入weight数组以及weight的大小即可构造此树，编解码皆可用。
	void build(int weight[], int size);
	void printWeight();
	void printTree();
private:
	void select(int range, int &idx1, int&idx2);
};
class HufCode {
public:
	char**code;
	int size;
	HufCode(HufTree&HT);//根据树来形成码表。	
	void printCode();
	char*operator[](int idx) {
		return code[idx];
	}
};
void printBar(int n, int len);
void encode(std::string infile, std::string outfile);
void decode(std::string infile, std::string outfile);
#endif
