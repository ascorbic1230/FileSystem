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
	char password[10];				// Mật khẩu của volume
public:
	SuperBlock();
	SuperBlock(string name, short bytesPerBlock, char blocksPerCluster, char numBlocksOfSuperBlock,
		short numBlocksOfFAT, int numCluster, unsigned int sizeOfVolume, int numOfEntry, int indexEntryCluster, string password);
	char* getNameVolume();
	void setNameVolume(char* name);
	short getBytesPerBlock();
	void setBytesPerBlock(short bytesPerBlock);
	char getBlocksPerCluster();		
	void setBlocksPerCluster(char blocksPerCluster);
	char getNumBlocksOfSuperBlock();
	void setNumBlocksOfSuperBlock(char numBlocksOfSuperBlock);
	short getNumBlocksOfFAT();
	void setNumBlocksOfFAT(short numBlocksOfFAT);
	int getNumCluster();
	void setNumCluster(int numCluster);
	unsigned int getSizeOfVolume();
	void setSizeOfVolume(unsigned int sizeOfVolume);
	int getNumOfEntry();
	void setNumOfEntry(int value);
	int getIndexEntryCluster();				
	bool setIndexEntryCluster(int value);	
	char* getPassword();
	bool setPassword(string password);
	void setPassword(char* password);


};

