#include "Entry.h"

Entry::Entry()
{
	for (int i = 0; i < 8; i++) {
		name[i] = 0;
	}
	for (int i = 0; i < 4; i++) {
		extension[i] = 0;
	}
	for (int i = 0; i < 8; i++) {
		password[i] = 0;
	}
	DateTime now;
	now.getNow();
	time = now.toInt();
	clusterIndex = 0;
	size = 0;
}

Entry::Entry(string name, string extension, string password, int time, int clusterIndex, int size)
{
	int i;
	for (i = 0; i < name.size(); i++) {
		this->name[i] = name[i];
	}

	while (i < 8) {
		this->name[i] = 0;
		i++;
	}

	for (i = 0; i < extension.size(); i++) {
		this->extension[i] = extension[i];
	}

	while (i < 4) {
		this->extension[i] = 0;
		i++;
	}

	i = 0;
	while (i < password.size()) {
		this->password[i] = password[i];
		i++;
	}
	while (i < 8) {
		this->password[i] = password[i];
		i++;
	}

	this->time = time;
	this->clusterIndex = clusterIndex;
	this->size = size;
}

char* Entry::getName()
{
	return name;
}

char* Entry::getExtension()
{
	return extension;
}

char* Entry::getPassword()
{
	return password;
}

int Entry::getTime()
{
	return time;
}

int Entry::getClusterIndex()
{
	return clusterIndex;
}

int Entry::getSize()
{
	return size;
}

// Đổi thông tin mật khẩu
void Entry::setPassword(string password)
{
	int i = 0;
	while (i < password.size()) {
		this->password[i] = password[i];
		i++;
	}
	while (i < 8) {
		this->password[i] = 0;
		i++;
	}
} // setPassword

// Khởi tạo thông tin cho file
bool Entry::create(string fileName, string filePassword, int clusterIndex)
{
	string name;
	string extension;
	int found = fileName.find_last_of(".");
	name = fileName.substr(0, found);
	int i;
	for (i = 0; i < name.size(); i++) {
		this->name[i] = name[i];
	}

	while (i < 8) {
		this->name[i] = 0;
		i++;
	}

	extension = fileName.substr(found + 1);
	for (i = 0; i < extension.size(); i++) {
		this->extension[i] = extension[i];
	}

	while (i < 4) {
		this->extension[i] = 0;
		i++;
	}
	i = 0;
	while (i < filePassword.size()) {
		this->password[i] = filePassword[i];
		i++;
	}
	while (i < 8) {
		this->password[i] = 0;
		i++;
	}

	DateTime time;
	time.getNow();
	this->time = time.toInt();
	this->clusterIndex = clusterIndex;
	fstream file(fileName, ios::binary | ios::in);
	if (file) {
		file.seekg(0, ios::end);
		this->size = file.tellg();

		file.close();
		return true;
	}
	return false;
} // create

// Chuyển thông tin file thành mảng char 32 bytes
void Entry::toCharArray(char* arr)
{
	int offset = 0;
	for (int i = 0; i < 8; i++) {
		arr[offset] = name[i];
		offset++;
	}
	for (int i = 0; i < 4; i++) {
		arr[offset] = extension[i];
		offset++;
	}
	for (int i = 0; i < 8; i++) {
		arr[offset] = password[i];
		offset++;
	}
	arr[20] = (time >> 24) & 0xFF;
	arr[21] = (time >> 16) & 0xFF;
	arr[22] = (time >> 8) & 0xFF;
	arr[23] = time & 0xFF;

	arr[24] = (clusterIndex >> 24) & 0xFF;
	arr[25] = (clusterIndex >> 16) & 0xFF;
	arr[26] = (clusterIndex >> 8) & 0xFF;
	arr[27] = clusterIndex & 0xFF;

	arr[28] = (size >> 24) & 0xFF;
	arr[29] = (size >> 16) & 0xFF;
	arr[30] = (size >> 8) & 0xFF;
	arr[31] = size & 0xFF;
} // toCharArray

// Kiểm tra xem tên file có giống với fileName không
bool Entry::isCorrectName(string fileName)
{
	string name;
	string extension;
	char name1[8];
	char extension1[4];
	int found = fileName.find_last_of(".");
	name = fileName.substr(0, found);
	int i;
	for (i = 0; i < name.size(); i++) {
		name1[i] = name[i];
	}

	while (i < 8) {
		name1[i] = 0;
		i++;
	}

	extension = fileName.substr(found + 1);
	for (i = 0; i < extension.size(); i++) {
		extension1[i] = extension[i];
	}

	while (i < 4) {
		extension1[i] = 0;
		i++;
	}

	for (int i = 0; i < 8; i++) {
		if (this->name[i] != name1[i])
			return false;
	}
	for (int i = 0; i < 4; i++) {
		if (this->extension[i] != extension1[i])
			return false;
	}
	return true;
}

// Hiển thị thông tin file
void Entry::showInfo()
{
	string fileName = "";
	int i = 0;
	while (i < 8 && name[i] != 0) {
		fileName += name[i];
		i++;
	}
	fileName += ".";
	i = 0;
	while (i < 8 && extension[i] != 0) {
		fileName += extension[i];
		i++;
	}
	cout << "Ten file: " << fileName << endl;
	i = 0;
	while (password[i] != 0) {
		i++;
		continue;
	}
	cout << "Mat khau file: ";
	for (int j = 0; j < i; j++)
		cout << "*";
	cout << endl;
	DateTime t;
	toDateTime(t, time);
	cout << "Thoi gian tao file: " << t << endl;
	cout << "Cluster chua file dau tien: " << clusterIndex << endl;
	cout << "Kich thuoc file: " << size << "(bytes)" << endl;
} // showInfo

// Chuyển mảng char 32 bytes thành entry
void toEntry(char* arr, Entry& entry)
{
	int offset = 0;
	for (int i = 0; i < 8; i++) {
		entry.name[i] = arr[offset];
		offset++;
	}
	for (int i = 0; i < 4; i++) {
		entry.extension[i] = arr[offset];
		offset++;
	}
	for (int i = 0; i < 8; i++) {
		entry.password[i] = arr[offset];
		offset++;
	}
	entry.time = 0;
	entry.time |= (unsigned int)((unsigned char)arr[20]) << 24;
	entry.time |= (unsigned int)((unsigned char)arr[21]) << 16;
	entry.time |= (unsigned int)((unsigned char)arr[22]) << 8;
	entry.time |= (unsigned int)((unsigned char)arr[23]);

	entry.clusterIndex = 0;
	entry.clusterIndex |= (unsigned int)((unsigned char)arr[24]) << 24;
	entry.clusterIndex |= (unsigned int)((unsigned char)arr[25]) << 16;
	entry.clusterIndex |= (unsigned int)((unsigned char)arr[26]) << 8;
	entry.clusterIndex |= (unsigned int)((unsigned char)arr[27]);

	entry.size = 0;
	entry.size |= (unsigned int)((unsigned char)arr[28]) << 24;
	entry.size |= (unsigned int)((unsigned char)arr[29]) << 16;
	entry.size |= (unsigned int)((unsigned char)arr[30]) << 8;
	entry.size |= (unsigned int)((unsigned char)arr[31]);
} // toEntry
