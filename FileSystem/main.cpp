#include "Volume.h"
#include <iostream>
#include <string>
using namespace std;

int home()
{
	cout << "\n============== Quan ly Volume ==============\n\n";
	cout << "1. Tao volume moi\n";
	cout << "2. Mo volume da ton tai\n";
	cout << "0. Thoat\n\n";
	return 2;
}

int mainMenu(string volumeName)
{
	cout << "\n============== Quan ly Volume ==============\n\n";
	cout << "Volume: " << volumeName << ".dat\n\n";
	cout << "1. Hien thi thong tin volume\n";
	cout << "2. Dinh dang nhanh volume\n";
	cout << "3. Doi mat khau volume\n";
	cout << "4. Liet ke danh sach tap tin\n";
	cout << "5. Dat/doi mat khau cho tap tin\n";
	cout << "6. Chep tap tin vao volume\n";
	cout << "7. Chep tap tin ra khoi volume\n";
	cout << "8. Xoa tap tin trong volume\n";
	cout << "0. Thoat\n\n";
	return 8;
}

int selectMenu(int maxChoice)
{
	int choice;
	cout << "Lua chon cua ban la: ";
	while (1)
	{
		cin >> choice;
		if (choice < 0 || choice > maxChoice)
		{
			cin.clear();
			cin.ignore();
			continue;
		}
		break;
	}
	return choice;
}

bool isExist(string volumeName)
{
	string volumePath = "../Volume/" + volumeName + ".dat";
	fstream volume(volumePath, ios::in);
	if (volume)
	{
		volume.close();
		return true;
	}
	return false;
}

bool checkPassword(string volumeName, string password)
{
	string pw = "";
	if (isExist(volumeName))
	{
		Block superblock;
		readBlock(volumeName, superblock, 0);
		int offset = 32;
		while (superblock.getDataAt(offset) != 0)
		{
			pw += superblock.getDataAt(offset);
			offset++;
		}
		return pw == password;
	}
	return false;
}

int main()
{

	int choice, opt;
	bool flag = false;
	string volumeName;
	string password;
	do
	{
		system("cls");
		choice = selectMenu(home());
		switch (choice)
		{
		case 1:
		{ // 1. Tao volume moi
			system("cls");
			cout << "\n=============== Nhap thong tin ===============\n\n";
			cout << "Luu y: \n+ Ten toi da 10 ky tu khong bao gom phan mo rong\n";
			cout << "+ Mat khau toi da 10 ky tu, bo trong neu khong co\n\n";
			cout << "Nhap ten volume: ";
			cin.ignore();
			getline(cin, volumeName);
			if (volumeName.size() < 1 || volumeName.size() > 10)
			{
				cout << "\n=> Ten nhap vao khong hop le!!!\n\n";
				system("pause");
				break;
			}
			cout << "Nhap mat khau (neu co): ";
			getline(cin, password);
			if (password.size() > 10)
			{
				cout << "\n=> Mat khau khong hop le!!!\n\n";
				system("pause");
				break;
			}

			cout << "Kich thuoc volume:\n";
			cout << "\t1. 100MB\n";
			cout << "\t2. 500MB\n";
			cout << "\t3. 1GB\n";
			cout << "\t4. 2GB\n";
			cout << "Lua chon cua ban la: ";
			while (1)
			{
				cin >> opt;
				if (opt < 1 || opt > 4)
				{
					cin.clear();
					cin.ignore();
					continue;
				}
				break;
			}
			Volume volume(volumeName, password, opt);
			if (volume.createVolume())
			{
				cout << "\n=> Khoi tao volume thanh cong!\n\n";
				flag = true;
			}
			else
				cout << "\n=> Khoi tao volume khong thanh cong!\n\n";

			system("pause");
			break;
		}
		case 2:
		{ // 2. Mo volume co san
			system("cls");
			cout << "\n=============== Nhap thong tin ===============\n";
			cout << "\nNhap ten volume: ";
			cin.ignore();
			getline(cin, volumeName);
			if (isExist(volumeName))
			{
				cout << "Nhap password (neu co): ";
				getline(cin, password);
				if (checkPassword(volumeName, password))
				{
					flag = true;
					break;
				}
				else
					cout << "\n=> Mat khau khong dung\n\n";
			}
			else
				cout << "\n=> Volume khong ton tai\n\n";
			system("pause");
			break;
		}
		default:
			break;
		}

		/* Xử lý giao diện chính*/
		if (flag)
		{
			flag = false;
			int subChoice;
			Volume volume;
			volume.readSuperBlock(volumeName);
			do
			{
				system("cls");
				subChoice = selectMenu(mainMenu(volumeName));
				switch (subChoice)
				{
				case 1:
				{ // Hien thi thong tin volume
					system("cls");
					cout << "\n=============== Thong tin volume ===============\n\n";
					volume.showVolumeInfo();
					cout << endl;
					system("pause");
					break;
				}
				case 2:
				{ // Dinh dang nhanh volume
					system("cls");
					volume.quickFormat();
					cout << "\n=> Format nhanh thanh cong\n\n";
					system("pause");
					break;
				}
				case 3:
				{ // Doi mat khau volume
					system("cls");
					cout << "\n=============== Doi mat khau volume ===============\n\n";
					cout << "Luu y: Mat khau toi da 10 ky tu, bo trong neu khong co\n\n";
					cout << "Mat khau moi la: ";
					cin.ignore();
					getline(cin, password);
					if (volume.changePassword(password))
						cout << "\n=> Doi mat khau thanh cong\n\n";
					else
						cout << "\n=> Doi mat khau khong thanh cong\n\n";
					system("pause");
					break;
				}
				case 4:
				{ // Liet ke danh sach tap tin
					system("cls");
					cout << "\n==================== Danh sach tap tin ====================\n\n";
					volume.showListFiles();
					cout << endl;
					system("pause");
					break;
				}
				case 5:
				{ // Dat/doi mat khau cho tap tin
					system("cls");
					string fileName;
					string filePassword;
					cout << "\n=============== Dat/doi mat khau tap tin ===============n\n";
					volume.showListFiles();
					if (volume.getSuperBlock()->getNumOfEntry() != 0)
					{
						cout << "\nLuu y: Mat khau toi da 8 ky tu, bo trong neu khong co\n\n";
						cout << "Ten tap tin: ";
						cin.ignore();
						getline(cin, fileName);
						if (fileName.size() < 1 || fileName.size() > 15)
						{
							cout << "\n=> Ten file khong hop le\n";
							system("pause");
							break;
						}
						cout << "Mat khau: ";
						getline(cin, filePassword);
						if (filePassword.size() > 8)
						{
							cout << "\n=> Password khong hop le\n";
							system("pause");
							break;
						}
						if (volume.changePasswordFile(fileName, filePassword))
						{
							cout << "\n=> Dat/doi mat khau file thanh cong\n\n";
						}
						else
							cout << "\n=> Dat/doi mat khau file khong thanh cong\n\n";
					}
					else
					{
						cout << "\n\n=> Khong co tap tin de thao tac\n\n";
					}
					system("pause");
					break;
				}
				case 6:
				{ // Chep tap tin vao volume
					system("cls");
					string fileName;
					string filePassword;
					cout << "\n=============== Them tap tin ===============n\n";
					cout << "Luu y: \n+ Ten tap tin toi da 8 ky tu, phan extension toi da 4 ky tu\n";
					cout << "+ Mat khau toi da 8 ky tu, bo trong neu khong co\n\n";
					cout << "Ten tap tin: ";
					cin.ignore();
					getline(cin, fileName);
					if (fileName.size() < 1 || fileName.size() > 13)
					{
						cout << "\n=> Ten file khong hop le\n";
						system("pause");
						break;
					}
					cout << "Mat khau (neu co): ";
					getline(cin, filePassword);
					if (filePassword.size() > 8)
					{
						cout << "\n=> Password khong hop le\n";
						system("pause");
						break;
					}
					if (volume.addFile(fileName, filePassword))
						cout << "\n=> Them file thanh cong\n\n";
					else
						cout << "\n=> Them file khong thanh cong\n\n";
					system("pause");
					break;
				}
				case 7:
				{ // Chep tap tin ra khoi volume
					system("cls");
					string fileName;
					string filePassword;
					cout << "\n";
					volume.showListFiles();
					if (volume.getSuperBlock()->getNumOfEntry() != 0)
					{
						cout << "\n\nThong tin file muon chep ra ngoai:\n\n";
						cout << "Ten file day du: ";
						cin.ignore();
						getline(cin, fileName);
						if (fileName.size() < 1 || fileName.size() > 13)
						{
							cout << "\n=> Ten file khong hop le\n";
							system("pause");
							break;
						}
						cout << "Mat khau (neu co): ";
						getline(cin, filePassword);
						if (filePassword.size() > 8)
						{
							cout << "\n=> Password khong hop le\n";
							system("pause");
							break;
						}
						if (volume.copyFileOut(fileName, filePassword))
							cout << "\n=> Copy file ra ngoai thanh cong\n\n";
						else
							cout << "\n=> Copy file ra ngoai khong thanh cong\n\n";
					}
					else
					{
						cout << "\n\n=> Khong co tap tin de thao tac\n\n";
					}
					system("pause");
					break;
				}
				case 8:
				{ // Xoa tap tin trong volume
					system("cls");
					string fileName;
					string filePassword;
					cout << "\n";
					volume.showListFiles();
					if (volume.getSuperBlock()->getNumOfEntry() != 0)
					{
						cout << "\n\nThong tin file muon xoa:\n\n";
						cout << "Ten file day du: ";
						cin.ignore();
						getline(cin, fileName);
						if (fileName.size() < 1 || fileName.size() > 13)
						{
							cout << "\n=> Ten file khong hop le\n";
							system("pause");
							break;
						}
						cout << "Mat khau (neu co): ";
						getline(cin, filePassword);
						if (filePassword.size() > 8)
						{
							cout << "\n=> Password khong hop le\n";
							system("pause");
							break;
						}
						if (volume.deleteFile(fileName, filePassword))
							cout << "\n=> Xoa file thanh cong\n\n";
						else
							cout << "\n=> Xoa file khong thanh cong\n\n";
					}
					else
					{
						cout << "\n\n=> Khong co tap tin de thao tac\n\n";
					}
					system("pause");
					break;
				}
				default:
					break;
				}
			} while (subChoice != 0);
		}
	} while (choice != 0);

	return 0;
} // main