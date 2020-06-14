#include "p6_list.h"
#include <tuple>
#include <iostream>
#include "operations.h"
#include <algorithm>
#include <map>
#include <stack>
typedef tuple<unsigned char, unsigned char, unsigned char> p6_data;
using namespace std;

p6_list::p6_list(vector<vector<p6_data>> pixels)
{
	height = pixels.size();
	width = pixels.front().size();

	this->pixels = pixels;
}

p6_list::p6_list(const int width, const int height)
{
	this->height = height;
	this->width = width;
	pixels = vector<vector<p6_data>>(height, vector<p6_data>(width));
}

p6_list::~p6_list()
{
	pixels.clear();
}

void p6_list::inverse_pixel()
{
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			char r = get<0>(pixels[i][j]);
			char g = get<1>(pixels[i][j]);
			char b = get<2>(pixels[i][j]);

			r = UCHAR_MAX - r;
			g = UCHAR_MAX - g;
			b = UCHAR_MAX - b;

			pixels[i][j] = make_tuple(r, g, b);
		}
	}
}


vector<vector<p6_data>> p6_list::combine(vector<vector<vector<p5_data>>>& xyz)
{
	return combine(xyz[0], xyz[1], xyz[2]);
}

vector<vector<p6_data>> p6_list::combine(vector<vector<p5_data>>& x, vector<vector<p5_data>>& y, vector<vector<p5_data>>& z)
{
	int height = x.size();
	int width = x.front().size();

	vector<vector<p6_data>> res(height, vector<p6_data>(width));

	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			res[i][j] = p6_data(x[i][j], y[i][j], z[i][j]);

	return res;
}

vector<vector<vector<p5_data>>> p6_list::split(vector<vector<p6_data>>& x)
{
	int height = x.size();
	int width = x.front().size();

	vector<vector<vector<p5_data>>> res(3, vector<vector<p5_data>>(height, vector<p5_data>(width)));

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			res[0][i][j] = get<0>(x[i][j]);
			res[1][i][j] = get<1>(x[i][j]);
			res[2][i][j] = get<2>(x[i][j]);
		}
	}
	return res;
}

ostream& p6_list::operator<<(ostream& os)
{
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
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

	//check valid
	const int pos = is.tellg();
	is.seekg(0, SEEK_END);
	const int end_pos = is.tellg();
	const int size = end_pos - pos;
	is.seekg(pos, SEEK_SET);

	if (size != (height * width * 3) && height > 0 && width > 0)
		throw exception("invalid file data");

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			char r, g, b;
			is >> noskipws >> r >> g >> b;

			pixels[i][j] = make_tuple(r, g, b);
		}
	}

	return is;
}

void p6_list::turn_left()
{
	operations<p6_data>::turn_left(pixels);
}

void p6_list::turn_right()
{
	operations<p6_data>::turn_right(pixels);
}

void p6_list::horizontal_reflect()
{
	operations<p6_data>::horizontal_reflect(pixels);
}

void p6_list::vertical_reflect()
{
	operations<p6_data>::vertical_reflect(pixels);
}
