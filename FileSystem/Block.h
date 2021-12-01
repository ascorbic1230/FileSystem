#pragma once
#include <iostream>
#include <fstream>
using namespace std;

#define BLOCKSIZE 512

class Block {
protected:
	char data[BLOCKSIZE];
public:
	Block();
	char getDataAt(int index);
	void setDataAt(char value, int index);
	char* getData();
	int getBlockSize();
	void showData();
	void clear();

	friend void readBlock(string volumeName, Block& block, int index);
	friend void writeBlock(string volumeName, Block& block, int index);
};

