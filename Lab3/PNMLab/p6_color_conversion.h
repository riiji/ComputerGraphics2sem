#pragma once
#include <tuple>
#include <algorithm>
#include <iostream>
using namespace std;
typedef tuple<unsigned char, unsigned char, unsigned char> p6_data;

class p6_color_conversion
{
public:
	static p6_data rgb_to_hsl(p6_data value);
	static p6_data hsl_to_rgb(p6_data value);

};

p6_data p6_color_conversion::rgb_to_hsl(p6_data value)
{
	unsigned char r = get<0>(value);
	unsigned char g = get<1>(value);
	unsigned char b = get<2>(value);

	float rf = (float)r / 255.0;
	float gf = (float)g / 255.0;
	float bf = (float)b / 255.0;

	float max_v = max(rf, max(gf, bf));
	float min_v = min(rf, min(gf, bf));

	float del = max_v - min_v;
	float l = (max_v + min_v) / 2;

	float h = 0;

	float s = 0;

	if (del != 0)
		s = del / (1 - abs(2 * l - 1));

	if (del != 0)
	{
		if (max_v == rf)
		{
			h = 60 * ((int)((gf - bf) / del) % 6);
		}
		else if (max_v == gf)
		{
			h = 60 * ((int)((bf - rf) / del) + 2);
		}
		else if (max_v == bf)
		{
			h = 60 * ((int)((rf - gf) / del) + 4);
		}
	}

	return tuple<unsigned char, unsigned char, unsigned char>{h / 360.0 * 250.0, s, l};
}
