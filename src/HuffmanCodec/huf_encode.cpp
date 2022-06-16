#include "huffman_codec.h"
#include <string>
#include <cstring>
size_t encode_filesize = 0;
class WriteMachine {
public:
	int BUF_SIZE;
	unsigned char*buffer;
	int BufferCursor;
	int BitCursor;
	int newfileSize;
	FILE*IO;
	WriteMachine(int size, FILE*fp) :
		BUF_SIZE(size),
		buffer(new unsigned char[size]),
		BufferCursor(0),
		BitCursor(0),
		newfileSize(0),
		IO(fp) {
		memset(buffer, 0, size);
	}
	~WriteMachine() {
		delete buffer;
	}
	void read(char input);
	void end(void);
};
class ReadMachine {
public:
	size_t BUF_SIZE;
	unsigned char*buffer;
	size_t BufferCursor;
	int BitCursor;
	size_t readNum;
	char**code;
	char*byteCode;
	FILE*IO;
	size_t readsize;
	int percent;
	ReadMachine(int size, FILE*fp, char**HC) :
		BUF_SIZE(size),
		buffer(new unsigned char[size]),
		BufferCursor(-1),
		BitCursor(0),
		readNum(BUF_SIZE),
		code(HC),
		IO(fp),
		readsize(0),
		percent(-1) {}
	~ReadMachine() {
		delete buffer;
	}
	char write(void);
};
void buildHufTree(FILE*infile, HufTree&HT, FILE*outfile);
void encode(std::string src, std::string dst) {
	FILE*infile = fopen(src.c_str(), "rb");
	FILE*outfile = fopen(dst.c_str(), "wb");
	HufTree HT(256);
	buildHufTree(infile, HT, outfile);
	printf("File size = %lld Byte\n", encode_filesize);
	// HT.printWeight();
	// HT.printTree();
	HufCode HC(HT);
	// HC.printCode();

	freopen(src.c_str(), "rb", infile);
	WriteMachine WM(1024, outfile);
	ReadMachine RM(1024, infile, HC.code);

	char input;
	while ((input = RM.write())) {
		WM.read(input);
	}
	WM.end();
	encode_filesize = 0;
	// if (0) {//用于输出编码后的文件，便于检查编码的正确性
	// 	fclose(outfile);
	// 	FILE*preview = fopen(OUTFILE, "rb");
	// 	const int BUF_SIZE = 1024;
	// 	unsigned char tmpbuffer[BUF_SIZE];
	// 	int readnum;
	// 	int cnt = 0;
	// 	while ((readnum = fread(tmpbuffer, sizeof(char), BUF_SIZE, preview))) {
	// 		for (int i = 0;i < readnum;i++) {
	// 			if (!(cnt % 16))printf("\n%d\t", cnt / 16 + 1);
	// 			printf("%02x", tmpbuffer[i]);
	// 			if (!((cnt + 1) % 2))printf(" ");
	// 			cnt++;
	// 		}
	// 	}
	// }
	fclose(infile);
	fclose(outfile);
	delete HT.tree;
	delete HC.code;
}
void buildHufTree(FILE*infile, HufTree&HT, FILE*outfile) {
	const int BUF_SIZE = 1024;
	unsigned char buffer[BUF_SIZE];
	int readnum;
	int weight[256] = { 0 };
	while ((readnum = fread(buffer, sizeof(char), BUF_SIZE, infile))) {
		for (int i = 0;i < readnum;i++) {
			encode_filesize++;
			weight[buffer[i]]++;
		}
	}
	fwrite(&encode_filesize, sizeof(size_t), 1, outfile);//写入文件的总大小
	fwrite(weight, sizeof(int), 256, outfile);//写入256个整数，代表权重。
	HT.build(weight, 256);
}

char ReadMachine::write() {
	if (BufferCursor >= readNum) {
		if (readNum == BUF_SIZE) {
			readNum = fread(buffer, sizeof(char), BUF_SIZE, IO);//读入下一个缓冲区
		} else {//全部文件读入完毕。
			printf("\n");
			return 0;
		}
		BufferCursor = 0;
		byteCode = code[buffer[BufferCursor]];//更新字符编码
	}
	char input = byteCode[BitCursor];//向WriteMachine输入的字符
	BitCursor++;
	if (!byteCode[BitCursor]) {//当前字节编码完毕
		BitCursor = 0;//清零
		BufferCursor++;//读下一个字符
		readsize++;
		if (readsize * 100 / encode_filesize != percent) {//打印进度条
			percent = readsize * 100 / encode_filesize;
			printBar(percent, 60);
		}
		if (BufferCursor < readNum)byteCode = code[buffer[BufferCursor]];//更新字符编码
	}
	return input;
}
void WriteMachine::read(char input) {
	if (BufferCursor >= BUF_SIZE) {
		newfileSize += BUF_SIZE;
		fwrite(buffer, sizeof(char), BUF_SIZE, IO);
		memset(buffer, 0, BUF_SIZE);
		BufferCursor = 0;
	}
	buffer[BufferCursor] <<= 1;
	buffer[BufferCursor] |= (input - '0');
	BitCursor++;
	if (BitCursor == 8) {
		BitCursor = 0;
		BufferCursor++;
	}
}
void WriteMachine::end() {
	buffer[BufferCursor] <<= 8 - BitCursor;
	newfileSize += BufferCursor + 1 - !BitCursor;
	printf("New file size = %d Byte\n", newfileSize);
	fwrite(buffer, sizeof(char), BufferCursor + 1 - !BitCursor, IO);
}
