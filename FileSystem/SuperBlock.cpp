#include "SuperBlock.h"

SuperBlock::SuperBlock()
{
	for (int i = 0; i < 10; i++) {
		name[i] = ' ';
	}
	bytesPerBlock = 0;
	blocksPerCluster = 0;
	numBlocksOfSuperBlock = 0;
	numBlocksOfFAT = 0;
	numCluster = 0;
	sizeOfVolume = 0;
	numOfEntry = 0;
	indexEntryCluster = 0;
}

SuperBlock::SuperBlock(string name, short bytesPerBlock, char blocksPerCluster, char numBlocksOfSuperBlock, short numBlocksOfFAT, int numCluster, unsigned int sizeOfVolume, int numOfEntry, int indexEntryCluster)
{
	int i;
	for (i = 0; i < name.size(); i++) {
		this->name[i] = name[i];
	}

	while (i < 10) {
		this->name[i] = ' ';
		i++;
	}

	this->bytesPerBlock = bytesPerBlock;
	this->blocksPerCluster = blocksPerCluster;
	this->numBlocksOfSuperBlock = numBlocksOfSuperBlock;
	this->numBlocksOfFAT = numBlocksOfFAT;
	this->numCluster = numCluster;
	this->sizeOfVolume = sizeOfVolume;
	this->numOfEntry = numOfEntry;
	this->indexEntryCluster = indexEntryCluster;
}

// Lấy tên của volume
char* SuperBlock::getNameVolume()
{
	return name;
}

short SuperBlock::getBytesPerBlock()
{
	return bytesPerBlock;
}

// Trả về số block của một cluster
char SuperBlock::getBlocksPerCluster()
{
	return blocksPerCluster;
}

char SuperBlock::getNumBlocksOfSuperBlock()
{
	return numBlocksOfSuperBlock;
}

short SuperBlock::getNumBlocksOfFAT()
{
	return numBlocksOfFAT;
}

int SuperBlock::getNumCluster()
{
	return numCluster;
}

// Trả về kích thước lưu trữ của volume (bytes)
unsigned int SuperBlock::getSizeOfVolume()
{
	return sizeOfVolume;
}

int SuperBlock::getNumOfEntry()
{
	return this->numOfEntry;
}

// Trả vể chỉ số cluster đầu tiên lưu các entry
int SuperBlock::getIndexEntryCluster()
{
	return indexEntryCluster;
}

// Gán chỉ số cluster đầu tiên lưu các entry
bool SuperBlock::setIndexEntryCluster(int value)
{
	indexEntryCluster = value;
	return true;
}
