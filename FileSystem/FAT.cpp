#include "FAT.h"

FAT::FAT()
{
	clusterMap = nullptr;
	size = 0;
}

FAT::~FAT()
{
	if (clusterMap != nullptr)
		delete[]clusterMap;
}

FAT::FAT(int numClusters)
{
	size = numClusters + 1;
	clusterMap = new int[size];
	for (int i = 0; i < size; i++) {
		clusterMap[i] = 1234567;
	}
}

// Khởi tạo bảng quản lý cluster
bool FAT::initClusterMap(int numClusters)
{
	size = numClusters + 1;
	clusterMap = new int[size];
	for (int i = 0; i < size; i++) {
		clusterMap[i] = 1;
	}
	return true;
}

// Lấy bảng quản lý cluster
int* FAT::getClusterMap()
{
	return clusterMap;
}

// Lấy kích thước của bảng quản lý cluster
int FAT::getSize()
{
	return size;
}

// Lấy giá trị tại ô index
int FAT::getValueAt(int index)
{
	return clusterMap[index];
}

// Thay đổi giá trị tại ô index trong bảng quản lý
bool FAT::changeValueAt(int index, int value)
{
	if (index > 0 && index < size) {
		if (value >= -1 && value < size) {
			clusterMap[index] = value;
			return true;
		}
	}
	return false;
}
