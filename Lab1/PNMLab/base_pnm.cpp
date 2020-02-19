#include "base_pnm.h"
#include <iostream>
#include "IPixel.h"
#include "p5_pixel.h"
#include "p6_pixel.h"

base_pnm::~base_pnm()
{
	delete& magic;
	delete& version;
	delete& height;
	delete& width;
}

void base_pnm::inverse_pixels()
{
	for (auto &pixel_row : table_data)
	{
		for (auto& pixel : pixel_row)
			pixel->inverse();
	}
}

void base_pnm::turn_left()
{
	turn_right();
	turn_right();
	turn_right();
}

void base_pnm::turn_right()
{
	auto new_position = vector<vector<IPixel*>>(width, vector<IPixel*>(height));

	for (auto i = 0; i < table_data.size(); ++i)
	{
		for (auto j = 0; j < table_data[i].size(); ++j)
		{
			new_position[j][i] = table_data[i][j];
		}
	}

	table_data = new_position;
	swap(width, height);

	horizontal_reflect();
	 
}

void base_pnm::horizontal_reflect()
{
	for (auto& row_pixel : table_data)
		reverse(row_pixel.begin(), row_pixel.end());
}

void base_pnm::vertical_reflect()
{
	turn_right();
	turn_right();
	horizontal_reflect();
}

ostream& base_pnm::operator<<(ostream& os)
{
	os << version << endl << width << " " << height << endl << magic << endl;

	for (auto &pixel_row : table_data)
	{
		for (auto &pixel : pixel_row)
		{
			pixel->operator<<(os);
		}
	}
	return os;
}

istream& base_pnm::operator>>(istream& is)
{
	is >> version >> width >> height >> magic;

	table_data = vector<vector<IPixel*>>(height, vector<IPixel*>(width));
	
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			if (version == "P5")
			{
				const auto p5 = new p5_pixel;
				(*p5).operator>>(is);
				table_data[i][j] = p5;
			}
			else if (version == "P6")
			{
				const auto p6 = new p6_pixel;
				(*p6).operator>>(is);

			}
			else
			{
				throw exception("invalid version");
			}

		}
	}

	return is;
}
