#include "Block.h"

Block::Block()
{
	for (int i = 0; i < BLOCKSIZE; i++) {
		data[i] = 0;
	}
}

char Block::getDataAt(int index)
{
	return data[index];
}

void Block::setDataAt(char value, int index)
{
	if (index > -1 && index < BLOCKSIZE)
		this->data[index] = value;
}

char* Block::getData()
{
	return data;
}

int Block::getBlockSize()
{
	return BLOCKSIZE;
}

void Block::showData()
{
	for (int i = 0; i < BLOCKSIZE; i++) {
		cout << i << ": " << (int)data[i] << endl;
	}
}

void Block::clear()
{
	for (int i = 0; i < BLOCKSIZE; i++) {
		data[i] = 0;
	}
}


void readBlock(string volumeName, Block& block, int index)
{
	volumeName += ".dat";
	// Mở volume
	fstream volume(volumeName, ios::in | ios::binary);

	if (volume) {
		int pos = 512 * index;
		volume.seekg(pos, volume.beg);
		char c;
		for (int i = 0; i < 512; i++) {
			volume >> c;
			block.data[i] = c;
		}

		volume.close();
	}
	else {
		cout << "Khong mo dc volume";
	}
}

void writeBlock(string volumeName, Block& block, int index)
{
	volumeName += ".dat";
	// Mở volume
	fstream volume(volumeName, ios::out | ios::binary);
	if (volume) {
		int pos = 512 * index;
		volume.seekp(pos, volume.beg);
		char c;
		for (int i = 0; i < 512; i++) {
			c = block.data[i];
			volume << c;
		}

		volume.close();
	}
	else {
		cout << "Khong mo dc volume";
	}
}

