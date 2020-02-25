#include "p5_list.h"
#include <iostream>
#include "operations.h"
#include <tuple>

using namespace std;

p5_list::p5_list(const int width, const int height)
{
	if (width <= 0 || height <= 0)
		throw exception("invalid height/width value");
	
	this->height = height;
	this->width = width;

	pixels = vector<vector<unsigned char>>(height, vector<unsigned char>(width));
}

p5_list::~p5_list()
{
	pixels.clear();
}

void p5_list::inverse_pixel()
{
	for (int i = 0;i < height;++i)
		for (int j = 0;j < width;++j)
			pixels[i][j] = UCHAR_MAX - pixels[i][j];
}

ostream& p5_list::operator<<(ostream& os)
{
	for (int i = 0;i < height;++i)
	{
		for (int j = 0;j < width;++j)
		{
			os.put(pixels[i][j]);
		}
	}

	return os;
}

istream& p5_list::operator>>(istream& is)
{
	// skip endl symbol
	is.get();

	//check valid
	const int pos = is.tellg();
	is.seekg(0, SEEK_END);
	const int end_pos = is.tellg();
	const int size = end_pos - pos;
	is.seekg(pos, SEEK_SET);
	
	if (size != height * width)
		throw exception("invalid file data");

	for (int i = 0;i < height;++i)
	{
		for (int j = 0;j < width;++j)
		{
			char amount;
			is.get(amount);

			pixels[i][j] = amount;
		}
	}

	return is;

}

void p5_list::turn_left()
{
	operations<unsigned char>::turn_left(pixels);
}

void p5_list::turn_right()
{
	operations<unsigned char>::turn_right(pixels);
}

void p5_list::horizontal_reflect()
{
	operations<unsigned char>::horizontal_reflect(pixels);
}

void p5_list::vertical_reflect()
{
	operations<unsigned char>::vertical_reflect(pixels);
}
