#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <ctime>
using namespace std;

class DateTime
{
private:
	char day;
	char month;
	char year;
	char hour;
	char minute;
	char second;
public:
	DateTime();
	DateTime(char day, char month, char year, char hour, char minute, char second);

	int toInt();				// Chuyển kiểu dữ liệu DateTime sang số nguyên 4 bytes
	void getNow();				// Lấy giá trị thời gian hiện tại

	friend ostream& operator<<(ostream& os, DateTime& dt);	// Quá tải toán tử xuất
	friend void toDateTime(DateTime& dt, int s);			// Chuyển kiểu dữ liệu số nguyên 4 bytes sang DateTime

};

