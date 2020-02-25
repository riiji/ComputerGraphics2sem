#pragma once
#include <vector>
using namespace std;

template <class T>
class operations final
{
public:
	static void turn_left(vector<vector<T>>& obj);
	static void turn_right(vector<vector<T>>& obj);
	static void horizontal_reflect(vector<vector<T>>& obj);
	static void vertical_reflect(vector<vector<T>>& obj);
};

template <class T>
void operations<T>::turn_left(vector<vector<T>>& obj)
{
	int height = obj.size();
	int width = obj.front().size();

	vector<vector<T>> result = vector<vector<T>>(width, vector<T>(height));

	for (int i = 0;i < height;++i)
		for (int j = 0;j < width;++j)
			result[j][i] = obj[i][j];

	obj = result;
}

template <class T>
void operations<T>::turn_right(vector<vector<T>>& obj)
{
	int height = obj.size();
	int width = obj.front().size();

	vector<vector<T>> result = vector<vector<T>>(width, vector<T>(height));

	for (int i = 0;i < height;++i)
		for (int j = 0;j < width;++j)
			result[j][height-1-i] = obj[i][j];

	obj = result;
}

template <class T>
void operations<T>::horizontal_reflect(vector<vector<T>>& obj)
{
	for (int i = 0;i < obj.size();++i)
	{
		for(int j = 0;j<obj.front().size()/2;++j)
		{
			swap(obj[i][j], obj[i][obj.front().size() - j - 1]);
		}
	}
}

template <class T>
void operations<T>::vertical_reflect(vector<vector<T>>& obj)
{
	turn_right(obj);
	turn_right(obj);
	horizontal_reflect(obj);
}

