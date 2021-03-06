﻿#pragma once
#include <vector>
#include "p6_list.h"
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
	static void set_p6_pixel(vector<vector<p6_data>>& obj, int x, int y, p6_data pixel);
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
		float linear = pixel * 255.0f;
		float srgb;
		if (linear <= 0.0031308f) {
			srgb = linear * 12.92f;
		}
		else {
			srgb = 1.055f * std::powf(linear, 1.0f / 2.4f) - 0.055f;
		}
		obj[y][x] = srgb * 255.0f;
	}

	T new_pixel = pow((double)pixel / 255, (double)gamma) * 255;

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

template<class T>
inline void operations<T>::set_p6_pixel(vector<vector<p6_data>>& obj, int x, int y, p6_data pixel)
{
	int height = obj.size();
	int width = obj.front().size();

	if (x < 0 || y < 0 || y>=height || x>=width)
		return;

	obj[y][x] = pixel;
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
		auto linear = obj[y][x] / 255.0f;
		if (linear <= 0.04045f) {
			linear = linear / 12.92f;
		}
		else {
			linear = std::powf((linear + 0.055f) / 1.055f, 2.4f);
		}
		return linear * 255.0f;
	}

	if (reverse)
		return powf(obj[x][y] / 255.0, 1.0 / gamma) * 255.0;
	else
		return powf(obj[y][x] / 255.0, 1.0 / gamma) * 255.0;
}
