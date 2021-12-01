#include "DateTime.h"

DateTime::DateTime()
{
	day = month = year = hour = minute = second = 0;
}

DateTime::DateTime(char day, char month, char year, char hour, char minute, char second)
{
	this->day = day;
	this->month = month;
	this->year = year;
	this->hour = hour;
	this->minute = minute;
	this->second = second;
}

// Chuyển kiểu dữ liệu DateTime sang số nguyên 4 bytes
int DateTime::toInt()
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
	char second = this->second / 2;
	while (offset < 32) {
		int mask = 1 << (31 - offset);
		int bit = (second >> (31 - offset)) & 1;
		res = ((res & ~mask) | (bit << (31 - offset)));
		offset++;
	}
	return res;
} // toInt

// Lấy giá trị thời gian hiện tại
void DateTime::getNow()
{
	time_t t = time(0);
	tm* now = localtime(&t);
	day = (char) now->tm_mday;
	month = (char)(now->tm_mon + 1);
	year = (char)((now->tm_year + 1900) - 1980);
	hour = (char)now->tm_hour;
	minute = (char)now->tm_min;
	second = (char)(now->tm_sec / 2);
} // getNow

// Quá tải toán tử xuất
ostream& operator<<(ostream& os, DateTime& dt)
{
	os << (int)dt.day << "/" << (int)dt.month << "/" << (int)dt.year + 1980 << " " << (int)dt.hour << ":" << (int)dt.minute << ":" << (int)dt.second;
	return os;
}

// Chuyển kiểu dữ liệu số nguyên 4 bytes sang DateTime
void toDateTime(DateTime& dt, int s)
{
	for (int i = 0; i < 5; i++) {
		char mask = (char)1 << (4 - i);
		char bit = (s >> (31 - i)) & 1;
		dt.day = ((dt.day & ~mask) | (bit << (4 - i)));
	}
	for (int i = 0; i < 4; i++) {
		char mask = (char)1 << (3 - i);
		char bit = (s >> (26 - i)) & 1;
		dt.month = ((dt.month & ~mask) | (bit << (3 - i)));
	}
	for (int i = 0; i < 7; i++) {
		char mask = (char)1 << (6 - i);
		char bit = (s >> (22 - i)) & 1;
		dt.year = ((dt.year & ~mask) | (bit << (6 - i)));
	}
	for (int i = 0; i < 5; i++) {
		char mask = (char)1 << (4 - i);
		char bit = (s >> (15 - i)) & 1;
		dt.hour = ((dt.hour & ~mask) | (bit << (4 - i)));
	}
	for (int i = 0; i < 6; i++) {
		char mask = (char)1 << (5 - i);
		int bit = (s >> (10 - i)) & 1;
		dt.minute = ((dt.minute & ~mask) | (bit << (5 - i)));
	}
	for (int i = 0; i < 5; i++) {
		char mask = (char)1 << (4 - i);
		char bit = (s >> (4 - i)) & 1;
		dt.second = ((dt.second & ~mask) | (bit << (4 - i)));
	}
	dt.second *= 2;
} // toDateTime
