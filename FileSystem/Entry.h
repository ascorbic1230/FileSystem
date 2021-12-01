#pragma once
#include "DateTime.h"
#include <fstream>

class Entry
{
private:
	char name[8];
	char extension[4];
	char password[8];
	int time;
	int clusterIndex;
	int size;
public:
	Entry();
	Entry(string name, string extension, string password, int time, int clusterIndex, int size);
	char* getName();
	char* getExtension();
	char* getPassword();
	int getTime();
	int getClusterIndex();
	int getSize();

	void setPassword(string password);						// Đổi thông tin mật khẩu

	bool create(string fileName, string filePassword, int clusterIndex);		// Khởi tạo thông tin cho file
	void toCharArray(char* arr);				// Chuyển thông tin file thành mảng char 32 bytes
	
	bool isCorrectName(string fileName);
	
	void showInfo();			// Hiển thị thông tin chi tiết của file

	friend void toEntry(char* arr, Entry& entry);			// Chuyển mảng char 32 bytes thành entry
};

