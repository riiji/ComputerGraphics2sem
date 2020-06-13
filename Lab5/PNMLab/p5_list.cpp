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

	operations<unsigned char>::set_pixel(pixels, x1, y1, 0, is_reverse, 2.2);
	operations<unsigned char>::set_pixel(pixels, x2, y2, 0, is_reverse, 2.2);

	float dx = x2 - x1;
	float dy = y2 - y1;
	float gradient = dy / dx;

	float y = y1 + gradient;

	for (auto x = x1 + 1; x <= x2 - 1; x++)
	{
		operations<unsigned char>::set_pixel(pixels, x, (int)y, 255 * (y - (int)y), is_reverse, 2.2);
		operations<unsigned char>::set_pixel(pixels, x, (int)y + 1, 255 * (1 - (y - (int)y)), is_reverse, 2.2);
		y += gradient;
	}
}

void p5_list::ez_line_width(double x1, double y1, double x2, double y2, double width, float gamma)
{

}

void p5_list::draw_horizontal_gradient(float gamma)
{
	int height = pixels.size();
	int width = pixels.front().size();
	double s = 256.0 / (double)width;

	for (int i = 0; i < width; ++i)
	{
		double np = round(s * i);

		if (np < 0) np = 0;
		if (np > 255) np = 255;

		operations<unsigned char>::set_pixel(pixels, i, 0, np, false, gamma);

	}

	for (int i = 1; i < height; ++i)
		for (int j = 0; j < width; ++j)
			pixels[i][j] = pixels[0][j];
}

void p5_list::base_dithering(vector<vector2d> offsets, double bit, float gamma)
{
	int height = pixels.size();
	int width = pixels.front().size();

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			unsigned char old_pixel = operations<unsigned char>::get_pixel(pixels, j, i, false, gamma);

			float ap = ((powf(2.0f, bit)) * (float)old_pixel - 255) / 256.0f;
			float rap = ceil(ap);
			float np = rap * (255 / (powf(2.0f, bit) - 1));
			float err = (float)old_pixel - np;

			if (np > 255)
				np = 255;

			if (np < 0)
				np = 0;

			operations<unsigned char>::set_pixel(pixels, j, i, (unsigned char)np, false, gamma);


			for (int k = 0; k < offsets.size(); ++k)
			{
				auto pixel = operations<unsigned char>::get_pixel(pixels, j + offsets[k].x, i + offsets[k].y, false, gamma);
				float new_value = (float)pixel + err * offsets[k].dith;

				if (new_value < 0.0f)
					new_value = 0;

				if (new_value > 255.0f)
					new_value = 255;


				operations<unsigned char>::set_pixel(pixels, j + offsets[k].x, i + offsets[k].y, (unsigned char)new_value, false, gamma);
			}
		}
	}
}

void p5_list::floid_dithering(float bit, float gamma)
{
	vector<vector2d> offsets;
	offsets.push_back(vector2d(1, 0, 7.0f / 16.0f));
	offsets.push_back(vector2d(-1, 1, 3.0f / 16.0f));
	offsets.push_back(vector2d(0, 1, 5.0f / 16.0f));
	offsets.push_back(vector2d(1, 1, 1.0f / 16.0f));
	base_dithering(offsets, bit, gamma);
}

void p5_list::jjn_dithering(float bit, float gamma)
{
	vector<vector2d> offsets;
	offsets.push_back(vector2d(1, 0, 7.0f / 48.0f));
	offsets.push_back(vector2d(2, 0, 5.0f / 48.0f));
	offsets.push_back(vector2d(-2, 1, 3.0f / 48.0f));
	offsets.push_back(vector2d(-1, 1, 5.0f / 48.0f));
	offsets.push_back(vector2d(0, 1, 7.0f / 48.0f));
	offsets.push_back(vector2d(1, 1, 5.0f / 48.0f));
	offsets.push_back(vector2d(2, 1, 3.0f / 48.0f));
	offsets.push_back(vector2d(-2, 2, 1.0f / 48.0f));
	offsets.push_back(vector2d(-1, 2, 3.0f / 48.0f));
	offsets.push_back(vector2d(0, 2, 5.0f / 48.0f));
	offsets.push_back(vector2d(1, 2, 3.0f / 48.0f));
	offsets.push_back(vector2d(2, 2, 1.0f / 48.0f));
	base_dithering(offsets, bit, gamma);
}

void p5_list::atkinson_dithering(float bit, float gamma)
{
	vector<vector2d> offsets;
	offsets.push_back(vector2d(1, 0, 1.0f / 8.0f));
	offsets.push_back(vector2d(2, 0, 1.0f / 8.0f));
	offsets.push_back(vector2d(-1, 1, 1.0f / 8.0f));
	offsets.push_back(vector2d(0, 1, 1.0f / 8.0f));
	offsets.push_back(vector2d(1, 1, 1.0f / 8.0f));
	offsets.push_back(vector2d(0, 2, 1.0f / 8.0f));
	base_dithering(offsets, bit, gamma);
}

void p5_list::sierra_dithering(float bit, float gamma)
{
	vector<vector2d> offsets;
	offsets.push_back(vector2d(1, 0, 5.0f / 32.0f));
	offsets.push_back(vector2d(2, 0, 3.0f / 32.0f));
	offsets.push_back(vector2d(-2, 1, 2.0f / 32.0f));
	offsets.push_back(vector2d(-1, 1, 4.0f / 32.0f));
	offsets.push_back(vector2d(0, 1, 5.0f / 32.0f));
	offsets.push_back(vector2d(1, 1, 4.0f / 32.0f));
	offsets.push_back(vector2d(2, 1, 2.0f / 32.0f));
	offsets.push_back(vector2d(-1, 2, 2.0f / 32.0f));
	offsets.push_back(vector2d(0, 2, 3.0f / 32.0f));
	offsets.push_back(vector2d(1, 2, 2.0f / 32.0f));
	base_dithering(offsets, bit, gamma);
}

void p5_list::random_dithering(float bit, float gamma)
{
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			auto old_pixel = operations<unsigned char>::get_pixel(pixels, j, i, false, gamma);

			auto new_pixel = old_pixel + (rand() % 255) - 128;

			if (new_pixel > 255)
				new_pixel = 255;

			if (new_pixel < 0)
				new_pixel = 0;

			float ap = ((powf(2.0f, bit)) * (float)new_pixel - 255) / 256.0f;
			float rap = ceil(ap);
			float np = rap * (255 / (powf(2.0f, bit) - 1));

			if (np > 255)
				np = 255;

			if (np < 0)
				np = 0;

			operations<unsigned char>::set_pixel(pixels, j, i, np, false, gamma);
		}
	}
}

void p5_list::base_matrix_dithering(vector<vector<unsigned char>> matrix, float bit, float gamma)
{
	int matrix_height = matrix.size();
	int matrix_width = matrix.front().size();
	int cur_j = 0;
	int cur_i = 0;
	map<float, float> chk;
	for (;;)
	{
		for (int i = 0; i < matrix_height; ++i)
		{
			for (int j = 0; j < matrix_width; ++j)
			{
				auto old_pixel = operations<unsigned char>::get_pixel(pixels, cur_j + j, cur_i + i, false, gamma);
				auto new_pixel = old_pixel + matrix[i][j] - 128;

				if (new_pixel > 255)
					new_pixel = 255;

				if (new_pixel < 0)
					new_pixel = 0;

				float ap = ((powf(2.0f, bit)) * (float)new_pixel - 255) / 256.0f;
				float rap = ceil(ap);
				float np = rap * (255 / (powf(2.0f, bit) - 1));

				if (np > 255)
					np = 255;

				if (np < 0)
					np = 0;

				operations<unsigned char>::set_pixel(pixels, cur_j + j, cur_i + i, np, false, gamma);
			}
		}

		cur_j += matrix_width;

		if (cur_j > width)
		{
			cur_j = 0;
			cur_i += matrix_height;

			if (cur_i > height)
				break;
		}
	}
}

void p5_list::ordered_dithering(float bit, float gamma)
{
	vector<vector<unsigned char>> matrix =
	{
		{0, 48,12,60,3,51,15,63},
		{32,16,44,28,35,19,47,31},
		{8,56,4,52,11,59,7,55},
		{40,24,36,20,43,27,39,23},
		{2,50,14,62,1,49,13,61},
		{34,18,46,30,33,17,45,29},
		{10,58,6,54,9,57,5,53},
		{42,26,38,22,41,25,37,21}
	};

	for (int i = 0; i < matrix.size(); ++i)
		for (int j = 0; j < matrix.front().size(); ++j)
			matrix[i][j] = (unsigned char)round((double)matrix[i][j] * 255.0 / 64.0);

	base_matrix_dithering(matrix, bit, gamma);
}

void p5_list::halftone_dithering(float bit, float gamma)
{
	vector<vector<unsigned char>> matrix =
	{
		{7,13,11,4},
		{12,16,14,8},
		{10,15,6,2},
		{5,9,3,1},
	};

	for (int i = 0; i < matrix.size(); ++i)
		for (int j = 0; j < matrix.front().size(); ++j)
			matrix[i][j] = (unsigned char)round((double)matrix[i][j] * 255.0 / 17.0);


	base_matrix_dithering(matrix, bit, gamma);
}

void p5_list::no_dithering(float bit, float gamma)
{
	vector<vector2d> offsets;
	base_dithering(offsets, bit, gamma);
}

void p5_list::offset(float value, float multiplier)
{
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			int new_pixel_value = ((int)pixels[i][j] - value) * multiplier;

			if (new_pixel_value > 255)
				new_pixel_value = 255;
			else if (new_pixel_value < 0)
				new_pixel_value = 0;

			pixels[i][j] = new_pixel_value;
		}
	}
}

vector<int> p5_list::calculate_distribution(float ignore_percent, int& calculated_offset, float& calculated_multiplier)
{
	int highest_pixel = 0;
	int lowest_pixel = 255;
	long long pixel_sum = 0;
	long long ignore_value = 0;
	int pixel_count = height * width;
	int pixels_for_remove = pixel_count * ignore_percent;

	vector<int> distribution(256);
	vector<int> on_remove(256);

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			pixel_sum += pixels[i][j];
			highest_pixel = max(highest_pixel, (int)pixels[i][j]);
			lowest_pixel = min(lowest_pixel, (int)pixels[i][j]);
			++distribution[pixels[i][j]];
		}
	}

	int highest_pixel_after_remove = highest_pixel;
	int lowest_pixel_after_remove = lowest_pixel;

	int highest_pixels_for_remove = pixels_for_remove;
	int lowest_pixels_for_remove = pixels_for_remove;

	for (int i = 0; i < 255; ++i)
	{
		if (distribution[i] > lowest_pixels_for_remove)
		{
			on_remove[i] += lowest_pixels_for_remove;
			lowest_pixel_after_remove = i;
			break;
		}
		else
		{
			lowest_pixels_for_remove -= distribution[i];
			on_remove[i] += distribution[i];
		}
	}

	for (int i = 255; i >= 0; --i)
	{
		if (distribution[i] > highest_pixels_for_remove)
		{
			on_remove[i] += highest_pixels_for_remove;
			highest_pixel_after_remove = i;
			break;
		}
		else
		{
			highest_pixels_for_remove -= distribution[i];
			on_remove[i] += distribution[i];
		}
	}

	if (highest_pixel_after_remove == lowest_pixel_after_remove)
	{
		calculated_offset = 0;
		calculated_multiplier = 1;
		return on_remove;
	}

	calculated_offset = lowest_pixel_after_remove;
	calculated_multiplier = 255 / (highest_pixel_after_remove - lowest_pixel_after_remove);

	return on_remove;
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
