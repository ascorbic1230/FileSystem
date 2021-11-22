#pragma once
#include <iostream>
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
	Block operator=(Block b);
};

