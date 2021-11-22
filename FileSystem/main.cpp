#include "Volume.h"
#include <iostream>
#include <string>
using namespace std;

void readBlock(string volumeName, Block& block, int index)
{
	// Mở volume
	fstream volume(volumeName, ios::in | ios::out | ios::binary);

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

int main() {
	string name = "MyFS";
	string volumeName = "MyFS.dat";
	Volume volume(name);
	/*volume.createVolume();*/

	Block block;
	readBlock(volumeName, block, 96);
	block.showData();


	return 0;
}