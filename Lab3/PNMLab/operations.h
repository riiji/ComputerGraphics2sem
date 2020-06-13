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
	static void set_pixel(vector<vector<T>>& obj, int x, int y, T pixel, bool reverse, float gamma);
	static T get_pixel(vector<vector<T>>& obj, int x, int y, bool reverse, float gamma);
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
void operations<T>::set_pixel(vector<vector<T>>& obj, int x, int y, T pixel, bool reverse, float gamma)
{
	if (x < 0 || y < 0)
		return;

	int height = obj.size();
	int width = obj.front().size();

	if (y >= height || x >= width)
		return;

	if (gamma == 0)
	{
		auto linear = obj[y][x] / 255.0f;
		if (linear <= 0.04045f) {
			linear = linear / 12.92f;
		}
		else {
			linear = std::powf((linear + 0.055f) / 1.055f, 2.4f);
		}
		obj[y][x] = linear * 255.0f;
		return;
	}

	T new_pixel = pow((double)pixel / 255, (double)gamma) * 255;

	if (new_pixel > 255)
		new_pixel = 255;
	else if (new_pixel < 0)
		new_pixel = 0;

	if (reverse)
	{
		if (y >= width || x >= height)
			return;

		obj[x][y] = new_pixel;
	}
	else
	{
		if (y >= height || x >= width)
			return;

		obj[y][x] = new_pixel;
	}
}

template <class T>
T operations<T>::get_pixel(vector<vector<T>>& obj, int x, int y, bool reverse, float gamma)
{
	if (x < 0 || y < 0)
		return T();

	int height = obj.size();
	int width = obj.front().size();

	if (y >= height || x >= width)
		return T();

	if (gamma == 0)
	{
		float srgb;
		float pixel = obj[y][x];
		pixel /= 255.0f;
		if (pixel <= 0.0031308f) {
			srgb = pixel * 12.92f;
		}
		else {
			srgb = 1.055f * std::powf(pixel, 1.0f / 2.4f) - 0.055f;
		}

		return srgb*255.0f;
	}

	if (reverse)
		return powf(obj[x][y] / 255.0, 1.0 / gamma) * 255.0;
	else
		return powf(obj[y][x] / 255.0, 1.0 / gamma) * 255.0;
}
