#include "p5_list.h"
#include <iostream>
#include "operations.h"
#include <tuple>
#include <algorithm>
#include <map>
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

unsigned char get_pixel(vector<vector<unsigned char>>& obj, int y, int x, bool& failed)
{
	failed = false;

	if (x < 0 || y < 0)
	{
		failed = true;
		return 0;
	}

	int height = obj.size();
	int width = obj.front().size();

	if (y >= height || x >= width)
	{
		failed = true;
		return 0;
	}

	return obj[y][x];
}

void p5_list::ez_line_width(double x1, double y1, double x2, double y2, double width, float gamma)
{

}

float foo(float x, float y, float z)
{
	return x * (1 - z) + y * z;
}

vector<vector<unsigned char>> p5_list::fidelity_fx(float sharpness)
{
	vector<vector<unsigned char>> res = vector<vector<unsigned char>>(height, vector<unsigned char>(width));

	bool fake;

	for (int i = 0; i < height; ++i)
	{
		res[i][0] = pixels[i][0];
		res[i][width-1] = pixels[i][width-1];
	}
	
	for (int i = 0; i < width; ++i)
	{
		res[0][i] = pixels[0][i];
		res[height-1][i] = pixels[height-1][i];
	}

	for (int i = 1; i < height-1; ++i)
	{
		for (int j = 1; j < width-1; ++j)
		{
			float soft_mi = soft_min(j, i);
			float soft_ma = soft_max(j, i);

			float diag_mi = mi(j, i);
			float diag_ma = ma(j, i);

			float mi_ = soft_mi + min(soft_mi, diag_mi);
			float ma_ = soft_ma + max(soft_ma, diag_ma);

			mi_ /= 255.0f;
			ma_ /= 255.0f;

			float A = sqrt(min(mi_, 2 - ma_) / ma_);
			float core_weight = -A / (foo(8, 5, sharpness));

			float u = get_pixel(pixels, i - 1, j, fake) / 255.0f;
			float lc = get_pixel(pixels, i, j - 1, fake) / 255.0f;
			float c = get_pixel(pixels, i, j, fake) / 255.0f;
			float rc = get_pixel(pixels, i, j + 1, fake) / 255.0f;
			float d = get_pixel(pixels, i + 1, j, fake) / 255.0f;

			float pixel = (u * core_weight + lc * core_weight + c + rc * core_weight + d * core_weight) / (1.0f + 4 * core_weight) * 255.0f;

			if (pixel > 255)
				pixel = 255;
			else if (pixel < 0)
				pixel = 0;

			res[i][j] = pixel;
		}
	}
	return res;
}

int p5_list::soft_min(int x, int y)
{
	unsigned char soft_min = 255;
	bool failed;

	auto u = get_pixel(pixels, y - 1, x, failed);

	if (!failed)
		soft_min = min(soft_min, u);

	auto lc = get_pixel(pixels, y, x - 1, failed);

	if (!failed)
		soft_min = min(soft_min, lc);

	auto c = get_pixel(pixels, y, x, failed);

	if (!failed)
		soft_min = min(soft_min, c);

	auto rc = get_pixel(pixels, y, x + 1, failed);

	if (!failed)
		soft_min = min(soft_min, rc);

	auto d = get_pixel(pixels, y + 1, x, failed);

	if (!failed)
		soft_min = min(soft_min, d);

	return soft_min;
}

int p5_list::soft_max(int x, int y)
{
	unsigned char soft_max = 0;
	bool failed;

	auto u = get_pixel(pixels, y - 1, x, failed);

	if (!failed)
		soft_max = max(soft_max, u);

	auto lc = get_pixel(pixels, y, x - 1, failed);

	if (!failed)
		soft_max = max(soft_max, lc);

	auto c = get_pixel(pixels, y, x, failed);

	if (!failed)
		soft_max = max(soft_max, c);

	auto rc = get_pixel(pixels, y, x + 1, failed);

	if (!failed)
		soft_max = max(soft_max, rc);

	auto d = get_pixel(pixels, y + 1, x, failed);

	if (!failed)
		soft_max = max(soft_max, d);

	return soft_max;
}

int p5_list::mi(int x, int y)
{
	unsigned char diag_min = 255;
	bool failed;

	auto lu = get_pixel(pixels, y - 1, x - 1, failed);

	if (!failed)
		diag_min = min(diag_min, lu);

	auto ru = get_pixel(pixels, y - 1, x + 1, failed);

	if (!failed)
		diag_min = min(diag_min, ru);

	auto ld = get_pixel(pixels, y + 1, x - 1, failed);

	if (!failed)
		diag_min = min(diag_min, ld);

	auto rd = get_pixel(pixels, y + 1, x + 1, failed);

	if (!failed)
		diag_min = min(diag_min, rd);

	return diag_min;
}

int p5_list::ma(int x, int y)
{
	unsigned char diag_max = 0;
	bool failed;

	auto lu = get_pixel(pixels, y - 1, x - 1, failed);

	if (!failed)
		diag_max = max(diag_max, lu);

	auto ru = get_pixel(pixels, y - 1, x + 1, failed);

	if (!failed)
		diag_max = max(diag_max, ru);

	auto ld = get_pixel(pixels, y + 1, x - 1, failed);

	if (!failed)
		diag_max = max(diag_max, ld);

	auto rd = get_pixel(pixels, y + 1, x + 1, failed);

	if (!failed)
		diag_max = max(diag_max, rd);

	return diag_max;
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

	if (size != height * width && height > 0 && width > 0)
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
