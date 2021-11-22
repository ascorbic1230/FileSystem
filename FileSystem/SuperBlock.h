#pragma once
#include <iostream>
using namespace std;

class SuperBlock {
private:
	char name[10];					// Tên của volume
	short bytesPerBlock;			// Số bytes của một block
	char blocksPerCluster;			// Số blocks thuộc về một cluster
	char numBlocksOfSuperBlock;		// Số blocks thuộc về phần SuperBlock
	short numBlocksOfFAT;			// Số blocks thuộc về phần FAT
	int numCluster;					// Số blocks thuộc về phần Data
	unsigned int sizeOfVolume;		// Kích thước của volume (bytes)
	int numOfEntry;					// Số file có trong volume
	int indexEntryCluster;			// Chỉ số cluster đầu tiên lưu các entry
public:
	SuperBlock();
	SuperBlock(string name, short bytesPerBlock, char blocksPerCluster, char numBlocksOfSuperBlock,
		short numBlocksOfFAT, int numCluster, unsigned int sizeOfVolume, int numOfEntry, int indexEntryCluster);
	char* getNameVolume();							// Lấy tên của volume
	//bool setNameVolume(char* name, int size);
	short getBytesPerBlock();
	//bool setBytesPerBlock(short value);

	char getBlocksPerCluster();			// Trả về số block của một cluster
	/*bool setBlocksPerCluster(char value);*/
	char getNumBlocksOfSuperBlock();
	/*bool setNumBlocksOfSuperBlock(char value);*/
	short getNumBlocksOfFAT();
	//bool setNumBlocksOfFAT(short value);
	int getNumCluster();
	unsigned int getSizeOfVolume();		// Trả về kích thước lưu trữ của volume (bytes)
	//bool setSizeOfVolume(unsigned int value);

	int getNumOfEntry();

	int getIndexEntryCluster();					// Trả vể chỉ số cluster đầu tiên lưu các entry
	bool setIndexEntryCluster(int value);		// Gán chỉ số cluster đầu tiên lưu các entry

};

