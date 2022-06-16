#include "huffman_codec.h"
void HufTree::build(int weight[], int size) {//输入weight数组以及weight的大小即可构造此树，编解码皆可用。
	for (int i = 0;i < size;i++) {
		tree[i + 1].w = weight[i];
	}
	for (int i = size + 1;i < size * 2;i++) {
		int idx1, idx2;
		select(i - 1, idx1, idx2);
		tree[idx1].p = i;	tree[idx2].p = i;
		tree[i].l = idx1; tree[i].r = idx2;
		tree[i].w = tree[idx1].w + tree[idx2].w;
	}
}
void HufTree::printWeight() {//用于调试
	for (int i = 1;i <= leafNum;i++) {
		printf("%02x:%d", i - 1, tree[i].w);
		printf(i % 4 ? "\t\t" : "\n");
	}
}
void HufTree::printTree() {//用于调试
	printf("idx\tw\tl\tr\tp\n");
	for (int i = 1;i < leafNum * 2;i++) {
		printf("%d\t%d\t%d\t%d\t%d\n", i, tree[i].w, tree[i].l, tree[i].r, tree[i].p);
	}
}
void HufTree::select(int range, int &idx1, int&idx2) {//找最小的两个结点。
	int start = 1;
	idx1 = 0;idx2 = 0;
	for (start = 1;start <= range;start++) {
		if (tree[start].p == 0 && !idx1) {
			idx1 = start;
		} else if (tree[start].p == 0) {
			idx2 = start;
			break;
		}
	}
	if (tree[idx2].w < tree[idx1].w) {
		std::swap(idx2, idx1);
	}
	for (int i = start + 1;i <= range;i++) {
		if (tree[i].w < tree[idx2].w && (tree[i].p == 0)) {
			idx2 = i;
			if (tree[idx2].w < tree[idx1].w) {
				std::swap(idx2, idx1);
			}
		}
	}
}
