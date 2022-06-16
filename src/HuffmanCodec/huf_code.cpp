#include "huffman_codec.h"
#include <cstdio>
#include <cstring>
HufCode::HufCode(HufTree&HT) ://根据树来形成码表。	
	code(new char*[HT.leafNum]),
	size(HT.leafNum) {
	char*cd = new char[size];
	cd[size - 1] = '\0';
	for (int i = 1;i <= size;i++) {
		int start = size - 1;
		for (int c = i, f = HT[i].p;f;c = f, f = HT[f].p) {
			cd[--start] = '0' + (HT[f].r == c);
		}
		code[i - 1] = new char[size - start];
		strcpy(code[i - 1], &cd[start]);
	}
	delete cd;
}
void HufCode::printCode() {//用于调试
	for (int i = 0;i < size;i++) {
		printf("%02x:%20s", i, code[i]);
		printf((i + 1) % 4 ? "\t" : "\n");
	}
}
