#include "Volume.h"
#include <iostream>
#include <string>
using namespace std;

int main() {
	string name = "MyFS";
	//Volume volume(name);
	//volume.createVolume();

	
	fstream file("MyFS.dat", ios::in | ios::binary);
	char temp[4];
	if (file) {
		file.seekg(3000);
		int i = 0;
		while (i < 4) {
			file.read(&temp[i], 1);
			i++;
		}
		cout << (int) temp[0] << endl;
		cout << (int) temp[1] << endl;
		cout << (int) temp[2] << endl;
		cout << (int) temp[3] << endl;
	}

	//Block block = volume.readBlock(0);
	//for (int i = 0; i < block.getBlockSize(); i++) {
	//	cout << block.getDataAt(i) << " ";
	//}

	return 0;
}