#include <iostream>
#include <fstream>
using namespace std;

struct Block {
    char data[512];
};

void showBlock(Block block) {
    for (int i = 0; i < 512; i++) {
        cout << i << ": " << (int) block.data[i] << endl;
    }
}

void readBlock(string volumeName, Block& block, int index) {
    
	// Mở volume
	fstream volume(volumeName, ios::in | ios::out | ios::binary);

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

int toInt(char day, char month, char year, char hour, char minute, char second)
{
	int res = 0;
	int offset = 0;
	while (offset < 5) {
		int mask = 1 << (31 - offset);
		int bit = (day >> (4 - offset)) & 1;
		res = ((res & ~mask) | (bit << (31 - offset)));
		offset++;
	}
	while (offset < 9) {
		int mask = 1 << (31 - offset);
		int bit = (month >> (8 - offset)) & 1;
		res = ((res & ~mask) | (bit << (31 - offset)));
		offset++;
	}
	while (offset < 16) {
		int mask = 1 << (31 - offset);
		int bit = (year >> (15 - offset)) & 1;
		res = ((res & ~mask) | (bit << (31 - offset)));
		offset++;
	}
	while (offset < 21) {
		int mask = 1 << (31 - offset);
		int bit = (hour >> (20 - offset)) & 1;
		res = ((res & ~mask) | (bit << (31 - offset)));
		offset++;
	}
	while (offset < 27) {
		int mask = 1 << (31 - offset);
		int bit = (minute >> (26 - offset)) & 1;
		res = ((res & ~mask) | (bit << (31 - offset)));
		offset++;
	}
	char second1 = second / 2;
	while (offset < 32) {
		int mask = 1 << (31 - offset);
		int bit = (second1 >> (31 - offset)) & 1;
		res = ((res & ~mask) | (bit << (31 - offset)));
		offset++;
	}
	return res;
}

int main() {
    // string volumeName = "abc.dat";

    // Block block;
    // readBlock(volumeName, block, 0);
    // showBlock(block);

	int a = toInt(25, 11, 41, 10, 4, 50);
	cout << a << endl;

    return 0;
}

