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
	char *getName();
	char *getExtension();
	char *getPassword();
	void setPassword(string password);
	int getTime();
	int getClusterIndex();
	int getSize();

	bool create(string fileName, string filePassword, int clusterIndex); // Khởi tạo thông tin cho file

	bool isCorrectName(string fileName);		 // Kiểm tra xem tên file có giống với fileName không
	bool isCorrectPassword(string filePassword); // Kiểm tra filePassword có đúng không

	void toCharArray(char *arr);				  // Chuyển thông tin file thành mảng char 32 bytes
	friend void toEntry(char *arr, Entry &entry); // Chuyển mảng char 32 bytes thành entry
};
