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
	static void set_pixel(vector<vector<T>>& obj, int x, int y, T pixel, bool reverse);
	static T get_pixel(vector<vector<T>>& obj, int x, int y, bool reverse);
};

template <class T>
void operations<T>::turn_left(vector<vector<T>>& obj)
{
	turn_right(obj);
	turn_right(obj);
	turn_right(obj);
}

template <class T>
void operations<T>::turn_right(vector<vector<T>>& obj)
{
	int height = obj.size();
	int width = obj.front().size();

	vector<vector<T>> result = vector<vector<T>>(width, vector<T>(height));

	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			result[j][height - 1 - i] = obj[i][j];

	obj = result;
}

template <class T>
void operations<T>::horizontal_reflect(vector<vector<T>>& obj)
{
	for (int i = 0; i < obj.size(); ++i)
	{
		for (int j = 0; j < obj.front().size() / 2; ++j)
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

template <class T>
void operations<T>::set_pixel(vector<vector<T>>& obj, int x, int y, T pixel, bool reverse)
{
	if (x < 0 || y < 0)
		return;

	int height = obj.size();
	int width = obj.front().size();

	if (y >= height || x >= width)
		return;

	if (reverse)
	{
		if (y >= width || x >= height)
			return;

		obj[x][y] = pixel;
	}
	else
	{
		if (y >= height || x >= width)
			return;

		obj[y][x] = pixel;
	}
}

template <class T>
T operations<T>::get_pixel(vector<vector<T>>& obj, int x, int y, bool reverse)
{
	if (x < 0 || y < 0)
		return T();

	int height = obj.size();
	int width = obj.front().size();

	if (y >= height || x >= width)
		return T();

	if (reverse)
		return obj[x][y];
	else
		return obj[y][x];
}
