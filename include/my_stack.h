#ifndef MYSTACK_H
#define MYSTACK_H
#include <cstdlib>
class myStack {
	// private:
public:
	int size; //最大容量
	int topIdx;//top==-1时栈空
	int* data;
public:
	myStack() :
		size(100),
		topIdx(-1),
		data((int*)malloc(size * sizeof(int))) {}
	void push(int num) {
		if (topIdx < size - 1) {
			data[++topIdx] = num;
		} else {
			realloc(data, size * 2 * sizeof(int));
			size *= 2;
			data[++topIdx] = num;
		}
	}
	int pop() {
		if (topIdx >= 0) {
			return data[topIdx--];
		} else {
			return -1;
		}
	}
	int& top() {
		return data[topIdx];
	}
	bool empty() {
		return topIdx < 0;
	}
};
#endif
