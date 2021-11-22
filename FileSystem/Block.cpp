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
		data[i] = ' ';
	}
}

Block Block::operator=(Block b)
{
	for (int i = 0; i < getBlockSize(); i++) {
		this->data[i] = b.data[i];
	}
	return (*this);
}

