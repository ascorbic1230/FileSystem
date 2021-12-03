#include "Volume.h"

Volume::Volume()
{
	superBlock = nullptr;
}

Volume::Volume(string name, string password, int opt)
{
	// Các số liệu volume tính toán sẵn
	short numBlocksOfFAT[4] = {96, 477, 954, 1907};
	int numCluster[4] = {12200, 61005, 122010, 244021};
	int sizeOfVolume[4] = {100, 500, 1000, 2000};

	short bytesPerBlock = 512;
	char blocksPerCluster = 16;
	char numBlocksOfSuperBlock = 1;
	int numOfEntry = 0;
	int indexEntryCluster = 1;

	superBlock = new SuperBlock(name, bytesPerBlock, blocksPerCluster, numBlocksOfSuperBlock, numBlocksOfFAT[opt - 1], numCluster[opt - 1], sizeOfVolume[opt - 1], numOfEntry, indexEntryCluster, password);
}

Volume::~Volume()
{
	if (superBlock != nullptr)
		delete superBlock;
}

SuperBlock *Volume::getSuperBlock()
{
	return superBlock;
}

bool Volume::createVolume()
{
	// Lấy tên volume
	string fileName = "";
	int i = 0;
	while (i < 10 && superBlock->getNameVolume()[i] != 0)
	{
		fileName += superBlock->getNameVolume()[i];
		i++;
	}
	string volumePath = "../Volume/" + fileName + ".dat";

	// Tạo volume là 1 tập tin volume_name.dat
	fstream volume(volumePath, ios::binary | ios::out);

	if (volume)
	{

		volume.seekp(0);
		int n = superBlock->getSizeOfVolume() * 100;
		char data[10000];
		for (int i = 0; i < 10000; i++)
		{
			data[i] = 0;
		}
		for (int i = 0; i < n; i++)
		{
			volume.write(data, 10000);
		}

		writeSuperBlock();
		writeFAT();

		volume.close();
		return true;
	}
	else
	{
		cout << "Tao volume khong thanh cong";
		return false;
	}
}

// Ghi Super Block vào volume
void Volume::writeSuperBlock()
{
	Block block;
	int offset = 0;
	while (offset < 10)
	{
		block.setDataAt(superBlock->getNameVolume()[offset], offset);
		offset++;
	}
	short bytesPerBlock = superBlock->getBytesPerBlock();
	while (offset < 12)
	{
		block.setDataAt((bytesPerBlock >> (8 * (11 - offset))) & 0xFF, offset);
		offset++;
	}
	while (offset < 13)
	{
		block.setDataAt(superBlock->getBlocksPerCluster(), offset);
		offset++;
	}
	while (offset < 14)
	{
		block.setDataAt(superBlock->getNumBlocksOfSuperBlock(), offset);
		offset++;
	}
	short numBlocksOfFAT = superBlock->getNumBlocksOfFAT();
	while (offset < 16)
	{
		block.setDataAt((numBlocksOfFAT >> (8 * (15 - offset))) & 0xFF, offset);
		offset++;
	}
	int numCluster = superBlock->getNumCluster();
	while (offset < 20)
	{
		block.setDataAt((numCluster >> (8 * (19 - offset))) & 0xFF, offset);
		offset++;
	}
	unsigned int sizeOfVolume = superBlock->getSizeOfVolume();
	while (offset < 24)
	{
		block.setDataAt((sizeOfVolume >> (8 * (23 - offset))) & 0xFF, offset);
		offset++;
	}
	int numOfEntry = superBlock->getNumOfEntry();
	while (offset < 28)
	{
		block.setDataAt((numOfEntry >> (8 * (27 - offset))) & 0xFF, offset);
		offset++;
	}
	int indexEntryCluster = superBlock->getIndexEntryCluster();
	while (offset < 32)
	{
		block.setDataAt((indexEntryCluster >> (8 * (31 - offset))) & 0xFF, offset);
		offset++;
	}
	while (offset < 42)
	{
		block.setDataAt(superBlock->getPassword()[offset - 32], offset);
		offset++;
	}
	writeBlock(block, 0);
} // writeSuperBlock

// Ghi bảng FAT mặc định vào volume
void Volume::writeFAT()
{
	short numBlocksOfFAT = superBlock->getNumBlocksOfFAT();
	int numCluster = superBlock->getNumCluster();
	int i = 0;
	int k = 0;
	while (i < numBlocksOfFAT)
	{
		Block block;
		for (int j = 0; j < 128; j++)
		{
			int value = 0;
			block.setDataAt((value >> 24) & 0xFF, j * 4);
			block.setDataAt((value >> 16) & 0xFF, j * 4 + 1);
			block.setDataAt((value >> 8) & 0xFF, j * 4 + 2);
			block.setDataAt((value)&0xFF, j * 4 + 3);
			k++;
			if (k >= numCluster)
				break;
		}
		int index = 1 + i;
		writeBlock(block, index);
		i++;
	}
	writeFatAt(-1, 1);
} // writeFAT

// Thay đổi giá trị bảng FAT tại vị trí index
void Volume::writeFatAt(int value, int index)
{
	// Lấy tên volume
	string volumeName = "";
	int i = 0;
	while (i < 10 && superBlock->getNameVolume()[i] != 0)
	{
		volumeName += superBlock->getNameVolume()[i];
		i++;
	}
	string volumePath = "../Volume/" + volumeName + ".dat";

	fstream volume(volumePath, ios::binary | ios::out | ios::in);
	if (volume)
	{
		int blockNum = index / 128 + 1;
		int indexInBlock = (index % 128) * 4;
		Block block;
		readBlock(block, blockNum);
		block.setDataAt((value >> 24) & 0xFF, indexInBlock);
		block.setDataAt((value >> 16) & 0xFF, indexInBlock + 1);
		block.setDataAt((value >> 8) & 0xFF, indexInBlock + 2);
		block.setDataAt((value)&0xFF, indexInBlock + 3);
		writeBlock(block, blockNum);

		volume.close();
	}
} // writeFatAt

// Đọc giá trị tại vị trí ô index trong bảng FAT
int Volume::readFatAt(int index)
{
	int value = 0;

	// Lấy tên volume
	string volumeName = "";
	int i = 0;
	while (i < 10 && superBlock->getNameVolume()[i] != 0)
	{
		volumeName += superBlock->getNameVolume()[i];
		i++;
	}
	string volumePath = "../Volume/" + volumeName + ".dat";

	fstream volume(volumePath, ios::binary | ios::in);
	if (volume)
	{
		int blockNum = index / 128 + 1;
		int indexInBlock = (index % 128) * 4;
		Block block;
		readBlock(block, blockNum);
		value |= (unsigned int)(unsigned char)block.getDataAt(indexInBlock) << 24;
		value |= (unsigned int)(unsigned char)block.getDataAt(indexInBlock + 1) << 16;
		value |= (unsigned int)(unsigned char)block.getDataAt(indexInBlock + 2) << 8;
		value |= (unsigned int)(unsigned char)block.getDataAt(indexInBlock + 3);

		volume.close();
	}
	return value;
} // readFatAt

// Tìm cluster trống đầu tiên
int Volume::findFirstEmptyCluster()
{
	// Bắt đầu tìm từ cluster thứ 2
	int i = 2, value;

	while (i < superBlock->getNumCluster())
	{
		value = readFatAt(i);
		if (value == 0)
			return i;
		i++;
	}

	return -1; // Không tìm thấy
} // findFirstEmptyCluster

// Đọc thông tin phần bootsector của volume rồi lưu vào đối tượng volume
bool Volume::readSuperBlock(string volumeName)
{

	string volumePath = "../Volume/" + volumeName + ".dat";
	fstream volume(volumePath, ios::binary | ios::in);
	if (volume)
	{
		Block block;
		volume.seekg(0, ios::beg);

		volume.read(block.getData(), 512);

		superBlock = new SuperBlock;

		char name[10];
		for (int i = 0; i < 10; i++)
		{
			name[i] = block.getDataAt(i);
		}
		superBlock->setNameVolume(name);

		short bytesPerBlock = 0;
		bytesPerBlock |= (unsigned short)((unsigned char)block.getDataAt(10) << 8);
		bytesPerBlock |= (unsigned short)((unsigned char)block.getDataAt(11));
		superBlock->setBytesPerBlock(bytesPerBlock);

		char blocksPerCluster = block.getDataAt(12);
		superBlock->setBlocksPerCluster(blocksPerCluster);

		char numBlocksOfSuperBlock = block.getDataAt(13);
		superBlock->setNumBlocksOfSuperBlock(numBlocksOfSuperBlock);

		short numBlocksOfFAT = 0;
		numBlocksOfFAT |= (unsigned short)((unsigned char)block.getDataAt(14) << 8);
		numBlocksOfFAT |= (unsigned short)((unsigned char)block.getDataAt(15));
		superBlock->setNumBlocksOfFAT(numBlocksOfFAT);

		int numCluster = 0;
		numCluster |= (unsigned short)((unsigned char)block.getDataAt(16) << 24);
		numCluster |= (unsigned short)((unsigned char)block.getDataAt(17) << 16);
		numCluster |= (unsigned short)((unsigned char)block.getDataAt(18) << 8);
		numCluster |= (unsigned short)((unsigned char)block.getDataAt(19));
		superBlock->setNumCluster(numCluster);

		unsigned int sizeOfVolume = 0;
		sizeOfVolume |= (unsigned short)((unsigned char)block.getDataAt(20) << 24);
		sizeOfVolume |= (unsigned short)((unsigned char)block.getDataAt(21) << 16);
		sizeOfVolume |= (unsigned short)((unsigned char)block.getDataAt(22) << 8);
		sizeOfVolume |= (unsigned short)((unsigned char)block.getDataAt(23));
		superBlock->setSizeOfVolume(sizeOfVolume);

		int numOfEntry = 0;
		numOfEntry |= (unsigned short)((unsigned char)block.getDataAt(24) << 24);
		numOfEntry |= (unsigned short)((unsigned char)block.getDataAt(25) << 16);
		numOfEntry |= (unsigned short)((unsigned char)block.getDataAt(26) << 8);
		numOfEntry |= (unsigned short)((unsigned char)block.getDataAt(27));
		superBlock->setNumOfEntry(numOfEntry);

		int indexEntryCluster = 0;
		indexEntryCluster |= (unsigned short)((unsigned char)block.getDataAt(28) << 24);
		indexEntryCluster |= (unsigned short)((unsigned char)block.getDataAt(29) << 16);
		indexEntryCluster |= (unsigned short)((unsigned char)block.getDataAt(30) << 8);
		indexEntryCluster |= (unsigned short)((unsigned char)block.getDataAt(31));
		superBlock->setIndexEntryCluster(indexEntryCluster);

		char password[10];
		for (int i = 0; i < 10; i++)
		{
			password[i] = block.getDataAt(i + 32);
		}
		superBlock->setPassword(password);

		volume.close();
		return true;
	}
	return false;
} // readSuperBlock

// Định dạng nhanh volume (Xoá hết dữ liệu nhưng vẫn giữ thông tin volume)
void Volume::quickFormat()
{
	// Ghi lại bảng FAT
	writeFAT();

	// Xoá hết dữ liệu trên cluster 1 (cluster chứa thông tin file đầu tiên)
	Block block;
	int index = superBlock->getNumBlocksOfSuperBlock() + superBlock->getNumBlocksOfFAT();
	for (int i = 0; i < superBlock->getBlocksPerCluster(); i++)
	{
		writeBlock(block, index + i);
	}

	// Sửa thông tin số file trong volume trên super block
	superBlock->setNumOfEntry(0);
	writeSuperBlock();

} // quickFormat

// Thay đổi mật khẩu volume
bool Volume::changePassword(string password)
{
	superBlock->setPassword(password);
	writeSuperBlock();
	return true;
} // changePassword

// Thêm tập tin vào volume
bool Volume::addFile(string fileName, string filePassword)
{
	// Kiểm tra file có tồn tại không
	string filePath = "../Input/" + fileName;
	fstream file(filePath, ios::in);
	if (!file.is_open())
		return false;
	file.close();

	// Tìm cluster lưu file
	int clusterIndex = findFirstEmptyCluster();
	if (clusterIndex == -1)
		return false;
	// Tạo đối tượng entry
	Entry entry;
	entry.create(fileName, filePassword, clusterIndex);

	// Kiểm tra kích thước file
	if (entry.getSize() == 0)
		return false;

	// Ghi thông tin file vào RDET
	writeNewFileInfo(entry);

	// Ghi dữ liệu file vào Data Area
	writeNewFileData(entry);

	// Ghi lại giá trị vào super block trên volume
	superBlock->setNumOfEntry(superBlock->getNumOfEntry() + 1);
	writeSuperBlock();

	return true;
} // addFile

// Đọc thông tin tất cả files có trong volume
vector<Entry> Volume::readAllFilesInfo()
{
	vector<Entry> entrys;
	int numFiles = superBlock->getNumOfEntry();
	if (numFiles != 0)
	{
		vector<int> clusters;
		int clusterIndex = superBlock->getIndexEntryCluster();
		clusters.push_back(clusterIndex);
		while (readFatAt(clusterIndex) != -1)
		{
			clusters.push_back(readFatAt(clusterIndex));
			clusterIndex = readFatAt(clusterIndex);
		}

		for (int i = 0; i < clusters.size(); i++)
		{
			int clusterIndex = clusters[i];
			for (int j = 0; j < superBlock->getBlocksPerCluster(); j++)
			{
				Block block;
				int blockIndex = superBlock->getNumBlocksOfSuperBlock() + superBlock->getNumBlocksOfFAT() + (clusterIndex - 1) * superBlock->getBlocksPerCluster() + j;
				readBlock(block, blockIndex);
				int offset;
				for (int k = 0; k < 512 / 32; k++)
				{
					offset = k * 32;
					if (block.getDataAt(offset) != 0)
					{
						char temp[32];
						for (int m = 0; m < 32; m++)
						{
							temp[m] = block.getDataAt(offset + m);
						}
						Entry entry;
						toEntry(temp, entry);
						entrys.push_back(entry);
						if (entrys.size() == numFiles)
							return entrys;
					}
				}
			}
		}
	}

	return entrys;
} // readAllFilesInfo

// Ghi thông tin file vào RDET hoặc SDET
void Volume::writeNewFileInfo(Entry &newEntry)
{
	// Tìm cluster cuối cùng lưu file info
	int clusterIndex = 1;
	while (readFatAt(clusterIndex) != -1)
	{
		clusterIndex = readFatAt(clusterIndex);
	}

	// Tìm vị trí ô trống lưu thông tin file mới
	for (int i = 0; i < superBlock->getBlocksPerCluster(); i++)
	{
		Block block;
		int offset = 0;
		int blockIndex = superBlock->getNumBlocksOfSuperBlock() + superBlock->getNumBlocksOfFAT() + (clusterIndex - 1) * superBlock->getBlocksPerCluster() + i;
		readBlock(block, blockIndex);
		while (offset < 512 && block.getData()[offset] != 0)
		{
			offset += 32;
		}

		// Tìm ra được vị trí trống
		if (block.getData()[offset] == 0)
		{
			char temp[32];
			newEntry.toCharArray(temp);
			for (int j = 0; j < 32; j++)
			{
				block.setDataAt(temp[j], offset + j);
			}
			writeBlock(block, blockIndex); // Ghi block xuống volume

			return;
		}
	}
	// Cluster đang xét ko còn chỗ trống
	int newClusterIndex = findFirstEmptyCluster();
	writeFatAt(newClusterIndex, clusterIndex);
	writeFatAt(-1, newClusterIndex);
	clusterIndex = newClusterIndex;
	int blockIndex = superBlock->getNumBlocksOfSuperBlock() + superBlock->getNumBlocksOfFAT() + (clusterIndex - 1) * superBlock->getBlocksPerCluster();
	Block block;
	char temp[32];
	newEntry.toCharArray(temp);
	for (int j = 0; j < 32; j++)
	{
		block.setDataAt(temp[j], j);
	}
	writeBlock(block, blockIndex); // Ghi block xuống volume

} // writeNewFilesInfo

// Ghi dữ liệu file vào volume
void Volume::writeNewFileData(Entry newEntry)
{
	// Ghép tên file và extension
	string fileName = "";
	int i = 0;
	while (i < 8 && newEntry.getName()[i] != 0)
	{
		fileName += newEntry.getName()[i];
		i++;
	}
	fileName += ".";
	i = 0;
	while (i < 4 && newEntry.getExtension()[i] != 0)
	{
		fileName += newEntry.getExtension()[i];
		i++;
	}

	string filePath = "../Input/" + fileName;
	// Mở file
	fstream file(filePath, ios::binary | ios::in);
	if (file)
	{
		int clusterIndex = newEntry.getClusterIndex();
		int numCluster = (int)ceil((float)newEntry.getSize() / (512 * superBlock->getBlocksPerCluster()));
		vector<int> clusters;
		for (int i = 0; i < numCluster; i++)
		{
			clusters.push_back(clusterIndex);
			writeFatAt(-1, clusterIndex);
			clusterIndex = findFirstEmptyCluster();
		}
		// Sửa thông tin bảng FAT
		int l = 0;
		while (l < numCluster - 1)
		{
			writeFatAt(clusters[l + 1], clusters[l]);
			l++;
		}
		// Ghi dữ liệu vào các cluster đầy đủ
		for (int i = 0; i < clusters.size() - 1; i++)
		{
			clusterIndex = clusters[i];
			for (int j = 0; j < superBlock->getBlocksPerCluster(); j++)
			{
				Block block;
				int blockIndex = superBlock->getNumBlocksOfSuperBlock() + superBlock->getNumBlocksOfFAT() + (clusterIndex - 1) * superBlock->getBlocksPerCluster() + j;

				file.read(block.getData(), 512);

				writeBlock(block, blockIndex);
			}
		}
		// Ghi dữ liệu vào các block đầy đủ của cluster cuối cùng
		int numBlocksInLastCluster = (int)ceil((float)(newEntry.getSize() - (numCluster - 1) * superBlock->getBlocksPerCluster() * 512) / 512);
		clusterIndex = clusters[clusters.size() - 1];
		for (int i = 0; i < numBlocksInLastCluster - 1; i++)
		{
			Block block;
			int blockIndex = superBlock->getNumBlocksOfSuperBlock() + superBlock->getNumBlocksOfFAT() + (clusterIndex - 1) * superBlock->getBlocksPerCluster() + i;

			file.read(block.getData(), 512);

			writeBlock(block, blockIndex);
		}
		// Ghi dữ liệu vào các block cuối cùng của cluster cuối cùng (có thể ko ghi hết block)
		int lastBlockIndex = superBlock->getNumBlocksOfSuperBlock() + superBlock->getNumBlocksOfFAT() + (clusterIndex - 1) * superBlock->getBlocksPerCluster() + numBlocksInLastCluster - 1;
		Block block;
		char c;
		int j = 0;
		while (j < 512 && !file.eof())
		{
			file.read(&c, 1);
			block.setDataAt(c, j);
			j++;
		}
		writeBlock(block, lastBlockIndex);

		file.close();
	}
} // writeNewFileData

// Hiển thị thông tin các file trong volume dạng danh sách
void Volume::showListFiles()
{
	int numFiles = superBlock->getNumOfEntry();
	if (numFiles != 0)
	{
		// Đọc thông tin vào entrys
		vector<Entry> entrys;
		vector<int> clusters;
		int clusterIndex = superBlock->getIndexEntryCluster();
		clusters.push_back(clusterIndex);
		while (readFatAt(clusterIndex) != -1)
		{
			clusters.push_back(readFatAt(clusterIndex));
			clusterIndex = readFatAt(clusterIndex);
		}

		for (int i = 0; i < clusters.size(); i++)
		{
			int clusterIndex = clusters[i];
			for (int j = 0; j < superBlock->getBlocksPerCluster(); j++)
			{
				Block block;
				int blockIndex = superBlock->getNumBlocksOfSuperBlock() + superBlock->getNumBlocksOfFAT() + (clusterIndex - 1) * superBlock->getBlocksPerCluster() + j;
				readBlock(block, blockIndex);
				int offset;
				for (int k = 0; k < 512 / 32; k++)
				{
					offset = k * 32;
					if (block.getDataAt(offset) != 0)
					{
						char temp[32];
						for (int m = 0; m < 32; m++)
						{
							temp[m] = block.getDataAt(offset + m);
						}
						Entry entry;
						toEntry(temp, entry);
						entrys.push_back(entry);
						if (entrys.size() == numFiles)
							goto L;
					}
				}
			}
		}
	L:
		cout << "STT\tTen file\t\t\tNgay them\t\t\tKich thuoc\n";
		for (int i = 0; i < numFiles; i++)
		{
			string fileName = "";
			int j = 0;
			while (j < 8 && entrys[i].getName()[j] != 0)
			{
				fileName += entrys[i].getName()[j];
				j++;
			}
			fileName += ".";
			j = 0;
			while (i < 4 && entrys[i].getExtension()[j] != 0)
			{
				fileName += entrys[i].getExtension()[j];
				j++;
			}
			DateTime time;
			toDateTime(time, entrys[i].getTime());
			cout << i + 1 << "\t" << fileName << "\t\t\t" << time << "\t\t" << entrys[i].getSize() << " bytes" << endl;
		}
		cout << endl;
	}
	else
	{
		cout << "\n=> Volume rong, khong co tap tin de hien thi.\n";
	}
} // showListFiles

// Đặt/đổi password cho tập tin
bool Volume::changePasswordFile(string fileName, string filePassword)
{
	int numFiles = superBlock->getNumOfEntry();
	int q = 0; // Biến chạy số file đã duyệt
	if (numFiles != 0)
	{
		vector<int> clusters;
		int clusterIndex = superBlock->getIndexEntryCluster();
		clusters.push_back(clusterIndex);
		while (readFatAt(clusterIndex) != -1)
		{
			clusters.push_back(readFatAt(clusterIndex));
			clusterIndex = readFatAt(clusterIndex);
		}

		for (int i = 0; i < clusters.size(); i++)
		{
			int clusterIndex = clusters[i];
			for (int j = 0; j < superBlock->getBlocksPerCluster(); j++)
			{
				Block block;
				int blockIndex = superBlock->getNumBlocksOfSuperBlock() + superBlock->getNumBlocksOfFAT() + (clusterIndex - 1) * superBlock->getBlocksPerCluster() + j;
				readBlock(block, blockIndex);
				int offset;
				for (int k = 0; k < 512 / 32; k++)
				{
					offset = k * 32;
					if (block.getDataAt(offset) != 0)
					{
						char temp[32];
						for (int m = 0; m < 32; m++)
						{
							temp[m] = block.getDataAt(offset + m);
						}
						Entry entry;
						toEntry(temp, entry);

						if (entry.isCorrectName(fileName))
						{
							entry.setPassword(filePassword);
							entry.toCharArray(temp);
							for (int m = 0; m < 32; m++)
							{
								block.setDataAt(temp[m], offset + m);
							}
							writeBlock(block, blockIndex);
							return true;
						}

						q++;
						// Duyệt qua tất cả các file nhưng ko tìm ra
						if (q == numFiles)
						{
							return false;
						}
					}
				}
			}
		}
	}
	return false;
} // changePasswordFile

// Copy file ra khỏi volume
// Tên file bao gồm cả extension
bool Volume::copyFileOut(string fileName, string password)
{
	// Kiểm tra file có tồn tại trong volume không
	vector<Entry> entrys;
	entrys = readAllFilesInfo();
	if (entrys.size() == 0)
		return false;
	for (int i = 0; i < entrys.size(); i++)
	{
		// Tìm file
		if (entrys[i].isCorrectName(fileName))
		{

			// Kiểm tra mật khẩu có đúng không
			if (entrys[i].isCorrectPassword(password))
			{

				// Copy file ra ngoài
				string filePath = "../Output/" + fileName;
				fstream file(filePath, ios::binary | ios::out | ios::trunc);
				if (file)
				{
					file.seekp(0, ios::beg);
					int clusterIndex = entrys[i].getClusterIndex();
					int fileSize = entrys[i].getSize();
					vector<int> clusters;
					clusters.push_back(clusterIndex);
					while (readFatAt(clusterIndex) != -1)
					{
						clusters.push_back(readFatAt(clusterIndex));
						clusterIndex = readFatAt(clusterIndex);
					}

					// Đọc dữ liệu của các cluster đầy đủ ghi ra file
					for (int i = 0; i < clusters.size() - 1; i++)
					{
						clusterIndex = clusters[i];
						for (int j = 0; j < superBlock->getBlocksPerCluster(); j++)
						{
							int blockIndex = superBlock->getNumBlocksOfSuperBlock() + superBlock->getNumBlocksOfFAT() + (clusterIndex - 1) * superBlock->getBlocksPerCluster() + j;
							Block block;
							readBlock(block, blockIndex);
							file.write(block.getData(), 512);
						}
					}
					// Đọc dữ liệu của các blocks đầy đủ của cluster cuối cùng ghi ra file
					int numBlocksInLastCluster = (int)ceil((float)(entrys[i].getSize() - (clusters.size() - 1) * superBlock->getBlocksPerCluster() * 512) / 512);
					clusterIndex = clusters[clusters.size() - 1];
					for (int i = 0; i < numBlocksInLastCluster - 1; i++)
					{
						Block block;
						int blockIndex = superBlock->getNumBlocksOfSuperBlock() + superBlock->getNumBlocksOfFAT() + (clusterIndex - 1) * superBlock->getBlocksPerCluster() + i;
						readBlock(block, blockIndex);
						file.write(block.getData(), 512);
					}
					// Đọc dữ liệu của các blocks cuối cùng của cluster cuối cùng ghi ra file (có thể không đầy đủ)
					int bytesInlastBlock = fileSize % 512;
					int blockIndex = superBlock->getNumBlocksOfSuperBlock() + superBlock->getNumBlocksOfFAT() + (clusterIndex - 1) * superBlock->getBlocksPerCluster() + numBlocksInLastCluster - 1;
					Block block;
					readBlock(block, blockIndex);
					char c;
					for (int i = 0; i < bytesInlastBlock; i++)
					{
						c = block.getDataAt(i);
						file.write(&c, 1);
					}
					file.close();
					return true;
				}
			}
		}
	}
	return false;
} // copyFileOut

// Xoá file bình thường ra khỏi volumes
bool Volume::deleteFile(string fileName, string filePassword)
{
	int numFiles = superBlock->getNumOfEntry();
	int q = 0; // Biến chạy chỉ số file đã duyệt
	if (numFiles != 0)
	{
		vector<int> clusters;
		int clusterIndex = superBlock->getIndexEntryCluster();
		clusters.push_back(clusterIndex);
		while (readFatAt(clusterIndex) != -1)
		{
			clusters.push_back(readFatAt(clusterIndex));
			clusterIndex = readFatAt(clusterIndex);
		}
		for (int i = 0; i < clusters.size(); i++)
		{
			int clusterIndex = clusters[i];
			for (int j = 0; j < superBlock->getBlocksPerCluster(); j++)
			{
				Block block;
				int blockIndex = superBlock->getNumBlocksOfSuperBlock() + superBlock->getNumBlocksOfFAT() + (clusterIndex - 1) * superBlock->getBlocksPerCluster() + j;
				readBlock(block, blockIndex);
				int offset;
				for (int k = 0; k < 512 / 32; k++)
				{
					offset = k * 32;
					if (block.getDataAt(offset) != 0)
					{
						char temp[32];
						for (int m = 0; m < 32; m++)
						{
							temp[m] = block.getDataAt(offset + m);
						}

						Entry entry;
						toEntry(temp, entry);
						if (entry.isCorrectName(fileName) && entry.isCorrectPassword(filePassword))
						{
							// Giải phóng clusters trên bảng FAT
							vector<int> fileClusters;
							int fileClusterIndex = entry.getClusterIndex();
							fileClusters.push_back(fileClusterIndex);
							while (readFatAt(fileClusterIndex) != -1)
							{
								fileClusters.push_back(readFatAt(fileClusterIndex));
								fileClusterIndex = readFatAt(fileClusterIndex);
							}
							for (int l = 0; l < fileClusters.size(); l++)
							{
								writeFatAt(0, fileClusters[l]);
							}
							// Xoá entry bằng cách ghi ký tự Null vào đầu entry
							block.setDataAt(0, offset);
							writeBlock(block, blockIndex);
							// Sửa thông tin super block
							superBlock->setNumOfEntry(superBlock->getNumOfEntry() - 1);
							writeSuperBlock();

							return true;
						}

						q++;
						if (q == numFiles)
							return false;
					}
				}
			}
		}
	}
	return false;
} // deleteFile

// Đọc dữ liệu tại block có chỉ số index
bool Volume::writeBlock(Block &block, int index)
{
	// Lấy tên volume
	string volumeName = "";
	int i = 0;
	while (i < 10 && superBlock->getNameVolume()[i] != 0)
	{
		volumeName += superBlock->getNameVolume()[i];
		i++;
	}
	string volumePath = "../Volume/" + volumeName + ".dat";

	// Mở volume
	fstream volume(volumePath, ios::binary | ios::out | ios::in);
	if (volume)
	{
		int pos = 512 * index;
		volume.seekp(pos, volume.beg);
		char c;
		for (int i = 0; i < 512; i++)
		{
			c = block.getDataAt(i);
			volume.write(&c, 1);
		}

		volume.close();
		return true;
	}
	else
	{
		cout << "Khong mo dc volume";
		return false;
	}
} // writeBlock

// Ghi dữ liệu tại block có chỉ số index
bool Volume::readBlock(Block &block, int index)
{
	// Lấy tên volume
	string volumeName = "";
	int i = 0;
	while (i < 10 && superBlock->getNameVolume()[i] != 0)
	{
		volumeName += superBlock->getNameVolume()[i];
		i++;
	}
	string volumePath = "../Volume/" + volumeName + ".dat";

	// Mở volume
	fstream volume(volumePath, ios::binary | ios::in);

	if (volume)
	{
		int pos = 512 * index;
		volume.seekg(pos, volume.beg);
		char c;
		for (int i = 0; i < 512; i++)
		{
			volume.read(&c, 1);
			block.setDataAt(c, i);
		}

		volume.close();
		return true;
	}
	else
	{
		cout << "Khong mo dc volume";
		return false;
	}
} // readBlock

// Hiển thị thông tin về volume
void Volume::showVolumeInfo()
{
	cout << "- Ten volume: " << superBlock->getNameVolume() << endl;
	int i = 0;
	while (superBlock->getPassword()[i] != 0)
	{
		i++;
		continue;
	}
	cout << "- Mat khau volume: ";
	for (int j = 0; j < i; j++)
		cout << "*";
	cout << endl;
	cout << "- So byte trong 1 block: " << superBlock->getBytesPerBlock() << endl;
	cout << "- So block trong 1 cluster: " << (int)superBlock->getBlocksPerCluster() << endl;
	cout << "- So block trong Super Block: " << (int)superBlock->getNumBlocksOfSuperBlock() << endl;
	cout << "- So block trong FAT: " << superBlock->getNumBlocksOfFAT() << endl;
	cout << "- So clusters trong Data: " << superBlock->getNumCluster() << endl;
	cout << "- Kich thuoc volume: " << superBlock->getSizeOfVolume() << " MB" << endl;
	cout << "- So file trong volume: " << superBlock->getNumOfEntry() << endl;
	cout << "- Block dau tien luu entry: " << superBlock->getIndexEntryCluster() << endl;
} // showVolumeInfo

void Volume::showFATInfo()
{
	for (int i = 0; i < 100; i++)
	{
		cout << i << ": " << readFatAt(i) << endl;
	}
}
