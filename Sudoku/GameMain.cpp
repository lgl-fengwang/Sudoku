//"数独"V1.0
//李国良于2016年11月11日编写完成

#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

using namespace std;

const int ArSize = 9;

string loadFile(int arr[ArSize][ArSize]);//读取文件，返回文件名
void printMap(int arr[ArSize][ArSize]);//显示数独
void solve(int arr[ArSize][ArSize], int enumer[ArSize], int i, int j);//计算当前单元格的候选数
bool solveV(int arr[ArSize][ArSize], int i, int j);//判断当前单元格是否可填
bool lopMap(int arr[ArSize][ArSize]);//循环遍历未解单元格，调用solveV求解
bool loopMap(int arr[ArSize][ArSize]);//暴力求解！！！
void saveFile(int arr[ArSize][ArSize], string str);//保存文件

int main()
{
	SetConsoleTitle("数独");
	int map[ArSize][ArSize];
	for (auto &row : map)
		for (auto &ival : row)
			ival = -1;
	string name = loadFile(map);
	printMap(map);
	bool surplus = lopMap(map);
	cout << "lopMap()解答完毕" << endl;
	printMap(map);
	if (!surplus)
	{
		loopMap(map);
		cout << "loopMap()解答完毕" << endl;
		printMap(map);
	}
	saveFile(map, name);
	cin.get();
	cin.get();
	return 0;
}

string loadFile(int arr[ArSize][ArSize])
{
	string fileName;
	ifstream inFile;
	cout << "请输入文件名：" << endl;
	while (true)
	{
		cin >> fileName;
		inFile.open(fileName + ".txt");
		if (!inFile.is_open())
		{
			cout << "文件未能成功打开，请重新输入文件名：" << endl;
			continue;
		}
		bool judg = true;
		for (int i = 0; i < ArSize; ++i)
		{
			for (int j = 0; j < ArSize; ++j)
			{
				inFile >> arr[i][j];
				if (arr[i][j] < 0 || arr[i][j] > 9)
					judg = false;
			}
		}
		if (judg)
		{
			cout << "文件\"" << fileName << ".txt" << "\"载入成功！" << endl;
			inFile.close();
			break;
		}
		else
		{
			cout << "文件内容有误，请重新输入文件名：" << endl;
			inFile.close();
			continue;
		}
	}
	return fileName;
}

void printMap(int arr[ArSize][ArSize])
{
	cout << endl;
	int num = 0;
	for (int i = 0; i < ArSize; ++i)
	{
		for (int j = 0; j < ArSize; ++j)
		{
			cout << arr[i][j];
			if (j != 8)
				cout << " ";
			if (!arr[i][j])
				num += 1;
		}
		cout << endl;
	}
	cout << num << "剩余单元格！" << endl << endl;
}

void solve(int arr[ArSize][ArSize], int enumer[ArSize], int i, int j)
{
	for (int num = 0; num < ArSize; ++num)
		enumer[num] = num + 1;
	for (int m = 0; m < ArSize; ++m)
	{
		if (arr[m][j])
			enumer[arr[m][j] - 1] = 0;
	}
	for (int n = 0; n < ArSize; ++n)
	{
		if (arr[i][n])
			enumer[arr[i][n] - 1] = 0;
	}
	for (int m = i / 3 * 3; m < i / 3 * 3 + 3; ++m)
	{
		for (int n = j / 3 * 3; n < j / 3 * 3 + 3; ++n)
		{
			if (arr[m][n])
				enumer[arr[m][n] - 1] = 0;
		}
	}
}

bool solveV(int arr[ArSize][ArSize], int i, int j)
{
	int enumeration[ArSize];
	int ation[ArSize];
	solve(arr, enumeration, i, j);
	int x = 0;
	int y;
	for (int i = 0; i < ArSize; ++i)
	{
		if (enumeration[i])
		{
			y = i;
			x += 1;
		}
	}
	if (x == 1)
	{
		arr[i][j] = enumeration[y];
		return true;
	}
	else
	{
		for (y = 0; y < ArSize; ++y)
		{
			if (enumeration[y])
			{
				for (int m = 0; m < ArSize; ++m)
				{
					if (arr[m][j] == 0 && m != i)
					{
						solve(arr, ation, m, j);
						if (ation[y])
						{
							break;
						}
					}
				}
				if (!ation[y])
				{
					arr[i][j] = enumeration[y];
					return true;
				}
				for (int n = 0; n < ArSize; ++n)
				{
					if (arr[i][n] == 0 && n != j)
					{
						solve(arr, ation, i, n);
						if (ation[y])
						{
							break;
						}
					}
				}
				if (!ation[y])
				{
					arr[i][j] = enumeration[y];
					return true;
				}
				bool judge = false;
				for (int m = i / 3 * 3; m < i / 3 * 3 + 3; ++m)
				{
					for (int n = j / 3 * 3; n < j / 3 * 3 + 3; ++n)
					{
						if (arr[m][n] == 0 && (m != i || n != j))
						{
							solve(arr, ation, m, n);
							if (ation[y])
							{
								goto label;
							}
						}
					}
				}
			label:
				if (!ation[y])
				{
					arr[i][j] = enumeration[y];
					return true;
				}
			}
		}
	}
	return false;
}

bool lopMap(int arr[ArSize][ArSize])
{
	int num = 0;
	while (true)
	{
		int number = 0;
		for (int i = 0; i < ArSize; ++i)
		{
			for (int j = 0; j < ArSize; ++j)
			{
				if (!arr[i][j])
				{
					if (!solveV(arr, i, j))
					{
						number += 1;
					}
				}
			}
		}
		if (!number || num == number)
		{
			num = number;
			break;
		}
		num = number;
	}
	return num == 0 ? true : false;
}

bool loopMap(int arr[ArSize][ArSize])
{
	for (int i = 0; i < ArSize; ++i)
	{
		for (int j = 0; j < ArSize; ++j)
		{
			if (!arr[i][j])
			{
				int enumer[ArSize];
				solve(arr, enumer, i, j);
				for (int n = 0; n < ArSize; ++n)
				{
					if (enumer[n])
					{
						int maps[ArSize][ArSize];
						for (int x = 0; x < ArSize; ++x)
						{
							for (int y = 0; y < ArSize; ++y)
							{
								maps[x][y] = arr[x][y];
							}
						}
						maps[i][j] = enumer[n];
						if (lopMap(maps))
						{
							for (int x = 0; x < ArSize; ++x)
							{
								for (int y = 0; y < ArSize; ++y)
								{
									arr[x][y] = maps[x][y];
								}
							}
							return true;
						}
						else
						{
							bool judge = true;
							for (int i = 0; i < ArSize; ++i)
							{
								for (int j = 0; j < ArSize; ++j)
								{
									if (!maps[i][j])
									{
										int num = 0;
										int enumerat[ArSize];
										solve(maps, enumerat, i, j);
										for (auto n : enumerat)
										{
											num += n;
										}
										if (!num)
										{
											judge = false;
										}
									}
								}
							}
							if (judge)
							{
								if (loopMap(maps))
								{
									for (int x = 0; x < ArSize; ++x)
									{
										for (int y = 0; y < ArSize; ++y)
										{
											arr[x][y] = maps[x][y];
										}
									}
									return true;
								}
							}
						}
					}
				}
				return false;
			}
		}
	}
}

void saveFile(int arr[ArSize][ArSize], string str)
{
	ofstream outFile;
	outFile.open(str + "answer.txt");
	if (!outFile.is_open())
	{
		cout << "文件保存失败！" << endl;
		return;
	}
	for (int i = 0; i < ArSize; ++i)
	{
		for (int j = 0; j < ArSize; ++j)
		{
			outFile << arr[i][j];
			if (j != 8)
				outFile << " ";
		}
		outFile << endl;
	}
	cout << "文件\"" << str << "answer.txt" << "\"保存成功！" << endl;
	outFile.close();
}