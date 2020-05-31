#include "p5_list.h"
#include <iostream>
#include "operations.h"
#include <tuple>
#include <algorithm>
#include <math.h>
using namespace std;

p5_list::p5_list(const int width, const int height)
{
	if (width <= 0 || height <= 0)
		throw exception("invalid height/width value");

	this->height = height;
	this->width = width;

	pixels = vector<vector<unsigned char>>(height, vector<unsigned char>(width));
}

p5_list::p5_list(vector<vector<unsigned char>> pixels)
{
	this->pixels = pixels;
	this->height = pixels.size();
	this->width = pixels.front().size();
}

p5_list::~p5_list()
{
	pixels.clear();
}

void p5_list::ez_line(double x1, double y1, double x2, double y2)
{
	bool is_reverse = abs(y2 - y1) > abs(x2 - x1);

	if (is_reverse)
	{
		swap(x1, y1);
		swap(x2, y2);
	}

	if (x1 > x2)
	{
		swap(x1, x2);
		swap(y1, y2);
	}

	operations<unsigned char>::set_pixel(pixels, x1, y1, 0, is_reverse);
	operations<unsigned char>::set_pixel(pixels, x2, y2, 0, is_reverse);

	float dx = x2 - x1;
	float dy = y2 - y1;
	float gradient = dy / dx;

	float y = y1 + gradient;

	for (auto x = x1 + 1; x <= x2 - 1; x++)
	{
		operations<unsigned char>::set_pixel(pixels, x, (int)y, 255 * (y - (int)y), is_reverse);
		operations<unsigned char>::set_pixel(pixels, x, (int)y + 1, 255 * (1 - (y - (int)y)), is_reverse);
		y += gradient;
	}
}

void p5_list::ez_line_width(double x0, double y0, double x1, double y1, double width)
{
	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = dx - dy, e2, x2, y2;                          /* error value e_xy */
	float ed = dx + dy == 0 ? 1 : sqrt((float)dx * dx + (float)dy * dy);

	for (width = (width + 1) / 2; ; ) {                                   /* pixel loop */
		operations<unsigned char>::set_pixel(pixels, x0, y0, max((double)0, 255 * (abs(err - dx + dy) / ed - width + 1)), false);
		e2 = err; x2 = x0;
		if (2 * e2 >= -dx) {                                           /* x step */
			for (e2 += dy, y2 = y0; e2 < ed * width && (y1 != y2 || dx > dy); e2 += dx)
				operations<unsigned char>::set_pixel(pixels, x0, y2 += sy, max((double)0, 255 * (abs(e2) / ed - width + 1)), false);
			if (x0 == x1) break;
			e2 = err; err -= dy; x0 += sx;
		}
		if (2 * e2 <= dy) {                                            /* y step */
			for (e2 = dx - e2; e2 < ed * width && (x1 != x2 || dx < dy); e2 += dy)
				operations<unsigned char>::set_pixel(pixels, x2 += sx, y0, max((double)0, 255 * (abs(e2) / ed - width + 1)), false);
			if (y0 == y1) break;
			err += dx; y0 += sy;
		}
	}
}

void p5_list::draw_horizontal_gradient()
{
	int height = pixels.size();
	int width = pixels.front().size();
	double s = 255 / (double)width;
	for (int i = 0; i < width; ++i)
		pixels[0][i] = s * i;
	for (int i = 1; i < height; ++i)
		for (int j = 0; j < width; ++j)
			pixels[i][j] = pixels[0][j];
}

void p5_list::base_dithering(vector<vector2d> offsets, float div_value, double bit)
{
	int height = pixels.size();
	int width = pixels.front().size();

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			int np = round(bit * (double)pixels[i][j] / 255.0) * (255.0 / bit);
			float err = (double)pixels[i][j] - (double)np;
			operations<unsigned char>::set_pixel(pixels, j, i, np, false);

			for (int k = 0; k < offsets.size(); ++k)
			{
				auto pixel = operations<unsigned char>::get_pixel(pixels, j + offsets[k].x, i + offsets[k].y, false);
				double new_value = (double)pixel + err * ((double)offsets[k].dith) / div_value;
				operations<unsigned char>::set_pixel(pixels, j + offsets[k].x, i + offsets[k].y, (int)new_value, false);
			}
		}
	}
}

void p5_list::floid_dithering(float bit)
{
	vector<vector2d> offsets;
	offsets.push_back(vector2d(1, 0, 7));
	offsets.push_back(vector2d(-1, 1, 3));
	offsets.push_back(vector2d(0, 1, 5));
	offsets.push_back(vector2d(1, 1, 1));
	base_dithering(offsets, 16, bit);
}

void p5_list::jjn_dithering(float bit)
{
	vector<vector2d> offsets;
	offsets.push_back(vector2d(1, 0, 7));
	offsets.push_back(vector2d(2, 0, 5));
	offsets.push_back(vector2d(-2, 1, 3));
	offsets.push_back(vector2d(-1, 1, 5));
	offsets.push_back(vector2d(0, 1, 7));
	offsets.push_back(vector2d(1, 1, 5));
	offsets.push_back(vector2d(2, 1, 3));
	offsets.push_back(vector2d(-2, 2, 1));
	offsets.push_back(vector2d(-1, 2, 3));
	offsets.push_back(vector2d(0, 2, 5));
	offsets.push_back(vector2d(1, 2, 3));
	offsets.push_back(vector2d(2, 2, 1));
	base_dithering(offsets, 48, bit);
}

void p5_list::atkinson_dithering(float bit)
{
	vector<vector2d> offsets;
	offsets.push_back(vector2d(1, 0, 1));
	offsets.push_back(vector2d(2, 0, 1));
	offsets.push_back(vector2d(-1, 1, 1));
	offsets.push_back(vector2d(0, 1, 1));
	offsets.push_back(vector2d(1, 1, 1));
	offsets.push_back(vector2d(0, 2, 1));
	base_dithering(offsets, 8, bit);
}

void p5_list::sierra_dithering(float bit)
{
	vector<vector2d> offsets;
	offsets.push_back(vector2d(1, 0, 5));
	offsets.push_back(vector2d(2, 0, 3));
	offsets.push_back(vector2d(-2, 1, 2));
	offsets.push_back(vector2d(-1, 1, 4));
	offsets.push_back(vector2d(0, 1, 5));
	offsets.push_back(vector2d(1, 1, 4));
	offsets.push_back(vector2d(2, 1, 2));
	offsets.push_back(vector2d(-1, 2, 2));
	offsets.push_back(vector2d(0, 2, 3));
	offsets.push_back(vector2d(1, 2, 2));
	base_dithering(offsets, 32, bit);
}

void p5_list::inverse_pixel()
{
	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			pixels[i][j] = UCHAR_MAX - pixels[i][j];
}

ostream& p5_list::operator<<(ostream& os)
{
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
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

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
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
