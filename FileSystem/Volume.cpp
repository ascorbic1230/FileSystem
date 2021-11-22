#include "Volume.h"

Volume::Volume()
{
	superBlock = nullptr;
}

Volume::Volume(string name)
{
	int sizeOfVolume = 512 + 96 * 512 + 12200 * 16 * 512;
	superBlock = new SuperBlock(name, (short) 512, (char) 16, (char) 1, (short) 96, 12200, sizeOfVolume, 0, 1);
}

Volume::~Volume()
{
	if (superBlock != nullptr)
		delete superBlock;
}

bool Volume::createVolume()
{
	// Lấy tên volume
	char fileName[15];
	char* name = superBlock->getNameVolume();
	int i = 0;
	while (i < 10 && name[i] != 0) {
		fileName[i] = name[i];
		i++;
	}
	fileName[i] = '\0';
	strcat(fileName, ".dat");

	// Tạo volume là 1 tập tin volume_name.dat
	fstream volume(fileName, ios::in | ios::out | ios::binary);

	if (volume) {
		// Tạo volume có kích thước 1GB = 1 000 000 000 bytes
		volume.seekp(0);
		char data[10000];
		for (int i = 0; i < 10000; i++) {
			data[i] = 0;
		}
		for (int i = 0; i < 10000; i++) {			// Kích thước test 100 000 000 bytes
			volume.write(data, 10000);
		}

		writeSuperBlock();
		writeFAT();

		volume.close();
		return true;
	}
	else {
		cout << "Tao volume khong thanh cong";
		return false;
	}
}

SuperBlock* Volume::getSuperBlock()
{
	return this->superBlock;
	
}

void Volume::writeSuperBlock()
{
	Block block;
	int offset = 0;
	while (offset < 10) {
		block.setDataAt(superBlock->getNameVolume()[offset], offset);
		offset++;
	}
	short bytesPerBlock = superBlock->getBytesPerBlock();
	while (offset < 12) {	
		block.setDataAt((bytesPerBlock >> (8 * (11 - offset))) & 0xFF, offset);
		offset++;
	}
	while (offset < 13) {
		block.setDataAt(superBlock->getBlocksPerCluster(), offset);
		offset++;
	}
	while (offset < 14) {
		block.setDataAt(superBlock->getNumBlocksOfSuperBlock(), offset);
		offset++;
	}
	short numBlocksOfFAT = superBlock->getNumBlocksOfFAT();
	while (offset < 16) {
		block.setDataAt((numBlocksOfFAT >> (8 * (15 - offset))) & 0xFF, offset);
		offset++;
	}
	int numCluster = superBlock->getNumCluster();
	while (offset < 20) {
		block.setDataAt((numCluster >> (8 * (19 - offset))) & 0xFF, offset);
		offset++;
	}
	unsigned int sizeOfVolume = superBlock->getSizeOfVolume();
	while (offset < 24) {
		block.setDataAt((sizeOfVolume >> (8 * (23 - offset))) & 0xFF, offset);
		offset++;
	}
	int numOfEntry = superBlock->getNumOfEntry();
	while (offset < 28) {
		block.setDataAt((numOfEntry >> (8 * (27 - offset))) & 0xFF, offset);
		offset++;
	}
	int indexEntryCluster = superBlock->getIndexEntryCluster();
	while (offset < 32) {
		block.setDataAt((indexEntryCluster >> (8 * (31 - offset))) & 0xFF, offset);
		offset++;
	}
	writeBlock(block, 0);
}

void Volume::writeFAT()
{
	short numBlocksOfFAT = superBlock->getNumBlocksOfFAT();
	int numCluster = superBlock->getNumCluster();
	int i = 0;
	int k = 0;
	while (i < numBlocksOfFAT) {
		Block block;
		for (int j = 0; j < 128; j++) {
			int value = 0;
			block.setDataAt((value >> 24) & 0xFF, j * 4);
			block.setDataAt((value >> 16) & 0xFF, j * 4 + 1);
			block.setDataAt((value >> 8) & 0xFF, j * 4 + 2);
			block.setDataAt((value) & 0xFF, j * 4 + 3);
			k++;
			if (k >= numCluster)
				break;
		}
		int index = 1 + i;
		writeBlock(block, index);
		i++;
	}
}

bool Volume::writeBlock(Block& block, int index)
{
	// Lấy tên volume
	char fileName[15];
	char* name = superBlock->getNameVolume();
	int i = 0;
	while (i < 10 && name[i] != ' ') {
		fileName[i] = name[i];
		i++;
	}
	fileName[i] = '\0';
	strcat(fileName, ".dat");

	// Mở volume
	fstream volume(fileName, ios::in | ios::out | ios::binary);
	if (volume) {
		int pos = 512 * index;
		volume.seekp(pos, volume.beg);
		char c;
		for (int i = 0; i < 512; i++) {
			c = block.getDataAt(i);
			volume << c;
		}

		volume.close();
		return true;
	}
	else {
		cout << "Khong mo dc volume";
		return false;
	}
}

void Volume::readBlock(Block& block, int index)
{
	// Lấy tên volume
	char fileName[15];
	char* name = superBlock->getNameVolume();
	int i = 0;
	while (i < 10 && name[i] != ' ') {
		fileName[i] = name[i];
		i++;
	}
	fileName[i] = '\0';
	strcat(fileName, ".dat");

	// Mở volume
	fstream volume(fileName, ios::in | ios::out | ios::binary);

	if (volume) {
		int pos = 512 * index;
		volume.seekg(pos, volume.beg);
		char c;
		for (int i = 0; i < 512; i++) {
			volume >> c;
			block.setDataAt(c, i);
		}

		volume.close();
	}
	else {
		cout << "Khong mo dc volume";
	}
}

void Volume::showVolumeInfo()
{
	cout << "Ten volume: " << superBlock->getNameVolume() << endl;
	cout << "So byte trong 1 block: " << superBlock->getBytesPerBlock() << endl;
	cout << "So block trong 1 cluster: " << (int) superBlock->getBlocksPerCluster() << endl;
	cout << "So block trong Super Block: " << (int) superBlock->getNumBlocksOfSuperBlock() << endl;
	cout << "So block trong FAT: " << superBlock->getNumBlocksOfFAT() << endl;
	cout << "So clusters trong Data: " << superBlock->getNumCluster() << endl;
	cout << "Kich thuoc volume (bytes): " << superBlock->getSizeOfVolume() << endl;
	cout << "So file trong volume: " << superBlock->getNumOfEntry() << endl;
	cout << "Block dau tien luu entry: " << superBlock->getIndexEntryCluster() << endl;
}

void Volume::showFATInfo()
{
//	int size = fat->getClusterMap().size();
//	for (int i = 0; i < size; i++) {
//		cout << fat->getClusterMap()[i] << " ";
//	}
}
