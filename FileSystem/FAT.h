#pragma once
#include <vector>
using namespace std;

class FAT
{
private:
	int* clusterMap;
	int size;
public:
	FAT();
	~FAT();
	FAT(int numClusters);						// Khởi tạo bảng quản lý cluster
	bool initClusterMap(int numClusters);		
	int* getClusterMap();				// Lấy bảng quản lý cluster
	int getSize();								// Lấy kích thước của bảng quản lý cluster
	int getValueAt(int index);					// Lấy giá trị tại ô index
	bool changeValueAt(int index, int value);	// Thay đổi giá trị tại ô index trong bảng quản lý
};

