#include "p6_list.h"
#include <tuple>
#include <iostream>
#include "operations.h"
#include <stack>
using namespace std;

p6_list::p6_list(const int width, const int height)
{
	this->height = height;
	this->width = width;
	pixels = vector<vector<tuple<char, char, char>>>(height, vector<tuple<char, char, char>>(width));
}

p6_list::~p6_list()
{
	pixels.clear();
}

void p6_list::inverse_pixel()
{
	for (int i = 0;i < height;++i)
	{
		for (int j = 0;j < width;++j)
		{
			char r = get<0>(pixels[i][j]);
			char g = get<1>(pixels[i][j]);
			char b = get<2>(pixels[i][j]);

			r = CHAR_MAX - r;
			g = CHAR_MAX - g;
			b = CHAR_MAX - b;

			pixels[i][j] = make_tuple(r, g, b);
		}
	}
}

ostream& p6_list::operator<<(ostream& os)
{
	for (int i = 0;i < height;++i)
	{
		for (int j = 0;j < width;++j)
		{
			const char r = get<0>(pixels[i][j]);
			const char g = get<1>(pixels[i][j]);
			const char b = get<2>(pixels[i][j]);

			os << noskipws << r << g << b;
		}
	}

	return os;
}

istream& p6_list::operator>>(istream& is)
{
	//skip endl symbol
	is.get();
	
	for (int i = 0;i < height;++i)
	{
		for (int j = 0;j < width;++j)
		{
			char r,g,b;
			is >> noskipws >> r >> g >> b;

			pixels[i][j] = make_tuple(r,g,b);
		}
	}

	return is;
}

void p6_list::turn_left()
{
	operations<tuple<char, char, char>>::turn_left(pixels);
}

void p6_list::turn_right()
{
	operations<tuple<char, char, char>>::turn_right(pixels);
}

void p6_list::horizontal_reflect()
{
	operations<tuple<char, char, char>>::horizontal_reflect(pixels);
}

void p6_list::vertical_reflect()
{
	operations<tuple<char, char, char>>::vertical_reflect(pixels);
}
