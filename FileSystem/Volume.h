#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Block.h"
#include "SuperBlock.h"
#include "Entry.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

using namespace std;

class Volume
{
private:
	SuperBlock *superBlock;

public:
	Volume();
	Volume(string name, string password, int opt);
	~Volume();
	SuperBlock *getSuperBlock();
	bool createVolume(); // Tạo ra volume trên ổ đĩa

	void writeSuperBlock();				   // Ghi Super Block vào volume
	void writeFAT();					   // Ghi bảng FAT mặc định vào volume
	void writeFatAt(int value, int index); // Thay đổi giá trị bảng FAT tại vị trí index
	int readFatAt(int index);			   // Đọc giá trị tại vị trí ô index trong bảng FAT

	int findFirstEmptyCluster(); // Tìm cluster trống đầu tiên, không tìm thấy trả về -1

	bool readSuperBlock(string volumeName); // Đọc thông tin phần bootsector của volume rồi lưu vào đối tượng volume

	void quickFormat();					  // Định dạng nhanh volume (Xoá hết dữ liệu nhưng vẫn giữ thông tin volume)
	bool changePassword(string password); // Thay đổi mật khẩu volume

	bool addFile(string fileName, string filePassword);			   // Thêm tập tin vào volume
	vector<Entry> readAllFilesInfo();							   // Đọc thông tin tất cả files có trong volume
	void writeNewFileInfo(Entry &newEntry);						   // Ghi thông tin file vào RDET hoặc SDET
	void writeNewFileData(Entry newEntry);						   // Ghi dữ liệu file vào volume
	void showListFiles();										   // Hiển thị thông tin các file trong volume dạng danh sách
	bool changePasswordFile(string fileName, string filePassword); // Đặt/đổi password cho tập tin

	bool copyFileOut(string fileName, string password); // Copy file ra khỏi volume

	bool deleteFile(string fileName, string filePassword); // Xoá file bình thường ra khỏi volume

	bool writeBlock(Block &block, int index); // Đọc dữ liệu tại block có chỉ số index
	bool readBlock(Block &block, int index);  // Ghi dữ liệu tại block có chỉ số index

	void showVolumeInfo(); // Hiển thị thông tin về volume
	void showFATInfo();
};
