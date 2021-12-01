#include "SuperBlock.h"

SuperBlock::SuperBlock()
{
	for (int i = 0; i < 10; i++) {
		name[i] = 0;
	}
	for (int i = 0; i < 10; i++) {
		password[i] = 0;
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

SuperBlock::SuperBlock(string name, short bytesPerBlock, char blocksPerCluster, char numBlocksOfSuperBlock, short numBlocksOfFAT, int numCluster, unsigned int sizeOfVolume, int numOfEntry, int indexEntryCluster, string password)
{
	int i;
	for (i = 0; i < name.size(); i++) {
		this->name[i] = name[i];
	}

	while (i < 10) {
		this->name[i] = 0;
		i++;
	}

	i = 0;
	while (i < password.size()) {
		this->password[i] = password[i];
		i++;
	}

	while (i < 10) {
		this->password[i] = 0;
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

char* SuperBlock::getNameVolume()
{
	return name;
}

void SuperBlock::setNameVolume(char* name)
{
	for (int i = 0; i < 10; i++) {
		this->name[i] = name[i];
	}
}

short SuperBlock::getBytesPerBlock()
{
	return bytesPerBlock;
}

void SuperBlock::setBytesPerBlock(short bytesPerBlock)
{
	this->bytesPerBlock = bytesPerBlock;
}

char SuperBlock::getBlocksPerCluster()
{
	return blocksPerCluster;
}

void SuperBlock::setBlocksPerCluster(char blocksPerCluster)
{
	this->blocksPerCluster = blocksPerCluster;
}

char SuperBlock::getNumBlocksOfSuperBlock()
{
	return numBlocksOfSuperBlock;
}

void SuperBlock::setNumBlocksOfSuperBlock(char numBlocksOfSuperBlock)
{
	this->numBlocksOfSuperBlock = numBlocksOfSuperBlock;
}

short SuperBlock::getNumBlocksOfFAT()
{
	return numBlocksOfFAT;
}

void SuperBlock::setNumBlocksOfFAT(short numBlocksOfFAT)
{
	this->numBlocksOfFAT = numBlocksOfFAT;
}

int SuperBlock::getNumCluster()
{
	return numCluster;
}

void SuperBlock::setNumCluster(int numCluster)
{
	this->numCluster = numCluster;
}

unsigned int SuperBlock::getSizeOfVolume()
{
	return sizeOfVolume;
}

void SuperBlock::setSizeOfVolume(unsigned int sizeOfVolume)
{
	this->sizeOfVolume = sizeOfVolume;
}

int SuperBlock::getNumOfEntry()
{
	return this->numOfEntry;
}

void SuperBlock::setNumOfEntry(int value)
{
	this->numOfEntry = value;
}

int SuperBlock::getIndexEntryCluster()
{
	return indexEntryCluster;
}

bool SuperBlock::setIndexEntryCluster(int value)
{
	indexEntryCluster = value;
	return true;
}

char* SuperBlock::getPassword()
{
	return password;
}

bool SuperBlock::setPassword(string password)
{
	int i = 0;
	while (i < password.size()) {
		this->password[i] = password[i];
		i++;
	}

	while (i < 10) {
		this->password[i] = 0;
		i++;
	}
	return true;
}

void SuperBlock::setPassword(char* password)
{
	for (int i = 0; i < 10; i++) {
		this->password[i] = password[i];
	}
}
