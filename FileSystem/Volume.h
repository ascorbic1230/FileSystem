#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Block.h"
#include "SuperBlock.h"
#include <iostream>
#include <fstream>

using namespace std;

class Volume {
private:
	SuperBlock* superBlock;
public:
	Volume();
	Volume(string name);
	~Volume();
	bool createVolume();							// Tạo ra volume trên ổ đĩa
	SuperBlock* getSuperBlock();

	void writeSuperBlock();
	void writeFAT();

	bool writeBlock(Block& block, int index);
	void readBlock(Block& block, int index);
	
	void showVolumeInfo();
	void showFATInfo();
};

