#include "huffman_codec.h"
#include <string.h>
size_t decode_filesize;
void decode(std::string src, std::string dst) {
	FILE*infile = fopen(src.c_str(), "rb");
	FILE*outfile = fopen(dst.c_str(), "wb");
	HufTree HT(256);
	int weight[256];
	fread(&decode_filesize, sizeof(size_t), 1, infile);
	fread(weight, sizeof(int), 256, infile);
	HT.build(weight, 256);//建立哈夫曼树。
	// HT.printTree();
	unsigned char readChar;
	size_t cnt = 0;
	int troot = 256 * 2 - 1;
	int tp = troot;
	int percent = 0;
	while (cnt < decode_filesize) {
		fread(&readChar, sizeof(unsigned char), 1, infile);
		for (int i = 7;i >= 0;i--) {
			bool bit = readChar & (1 << i);
			// printf("%d",bit);
			tp = bit ? HT[tp].r : HT[tp].l;
			bool isLeaf = !HT[tp].r && !HT[tp].l;
			if (isLeaf) {
				unsigned char result = tp - 1;
				// printf("%c",result);
				fwrite(&result, sizeof(unsigned char), 1, outfile);
				cnt++;
				tp = troot;
				if (cnt * 100 / decode_filesize != percent) {//打印进度条
					percent = cnt * 100 / decode_filesize;
					printBar(percent, 60);
				}
				if (cnt >= decode_filesize) {
					printf("\n");
					return;
				}
			}
		}
		// printf("\n");
	}
	fclose(infile);
	fclose(outfile);
}
void printBar(int n, int len) {
	putchar('[');
	for (int i = 0;i < n * len / 100;i++) {
		putchar('#');
	}
	for (int i = 0;i < len - n * len / 100;i++) {
		putchar(' ');
	}
	putchar(']');
	printf("[%d%%]\r", n);
}
