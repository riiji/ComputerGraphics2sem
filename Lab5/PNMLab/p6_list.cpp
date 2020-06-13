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

void p6_list::convert(string from, string to)
{
	if (from == "HSL")
	{
		p6_list::hsl_to_rgb(pixels);
	}
	else if (from == "HSV")
	{
		p6_list::hsv_to_rgb(pixels);
	}
	else if (from == "YCbCr.601")
	{
		p6_list::ycbcr601_to_rgb(pixels);
	}
	else if (from == "YCbCr.709")
	{
		p6_list::ycbcr709_to_rgb(pixels);
	}
	else if (from == "YCoCg")
	{
		p6_list::ycocg_to_rgb(pixels);
	}
	else if (from == "CMY")
	{
		p6_list::cmy_to_rgb(pixels);
	}

	if (to == "HSL")
	{
		p6_list::rgb_to_hsl(pixels);
	}
	else if (to == "HSV")
	{
		p6_list::rgb_to_hsv(pixels);
	}
	else if (to == "YCbCr.601")
	{
		p6_list::rgb_to_ycbcr601(pixels);
	}
	else if (to == "YCbCr.709")
	{
		p6_list::rgb_to_ycbcr709(pixels);
	}
	else if (to == "YCoCg")
	{
		p6_list::rgb_to_ycocg(pixels);
	}
	else if (to == "CMY")
	{
		p6_list::rgb_to_cmy(pixels);
	}
}

void p6_list::offset(float off, float multiplier, bool YCbCr)
{
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			if (YCbCr)
			{
				int brightness = get<0>(pixels[i][j]);

				int y = get<0>(pixels[i][j]);
				int cb = get<1>(pixels[i][j]);
				int cr = get<2>(pixels[i][j]);

				int new_y_value = ((int)y - off) * multiplier;

				if (new_y_value > 255) new_y_value = 255;

				if (new_y_value < 0) new_y_value = 0;

				pixels[i][j] = p6_data(new_y_value, cb, cr);
			}
			else
			{
				int brightness = max(get<0>(pixels[i][j]), max(get<1>(pixels[i][j]), get<2>(pixels[i][j])));

				int r = get<0>(pixels[i][j]);
				int g = get<1>(pixels[i][j]);
				int b = get<2>(pixels[i][j]);

				int new_r_value = ((int)r - off) * multiplier;
				int new_g_value = ((int)g - off) * multiplier;
				int new_b_value = ((int)b - off) * multiplier;

				if (new_r_value > 255) new_r_value = 255;
				if (new_g_value > 255) new_g_value = 255;
				if (new_b_value > 255) new_b_value = 255;

				if (new_r_value < 0) new_r_value = 0;
				if (new_g_value < 0) new_g_value = 0;
				if (new_b_value < 0) new_b_value = 0;

				pixels[i][j] = p6_data(new_r_value, new_g_value, new_b_value);
			}
		}
	}

}

vector<int> p6_list::calculate_distribution(bool YCbCr, float ignore_percent, int& calculated_offset, float& calculated_multiplier)
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
			if (YCbCr)
			{
				pixel_sum += get<0>(pixels[i][j]);
				highest_pixel = max(highest_pixel, (int)get<0>(pixels[i][j]));
				lowest_pixel = min(lowest_pixel, (int)get<0>(pixels[i][j]));
				++distribution[get<0>(pixels[i][j])];
			}

			else
			{
				int ma = max(get<0>(pixels[i][j]), max(get<1>(pixels[i][j]), get<2>(pixels[i][j])));
				int mi = min(get<0>(pixels[i][j]), min(get<1>(pixels[i][j]), get<2>(pixels[i][j])));

				pixel_sum += ma;
				highest_pixel = max(highest_pixel, ma);
				lowest_pixel = min(lowest_pixel, mi);

				++distribution[ma];
			}
		}
	}

	int highest_pixel_after_remove = highest_pixel;
	int lowest_pixel_after_remove = lowest_pixel;

	int highest_pixels_for_remove = pixels_for_remove;
	int lowest_pixels_for_remove = pixels_for_remove;

	if (lowest_pixels_for_remove > 0)
	{
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
	}

	if (highest_pixel_after_remove == lowest_pixel_after_remove)
	{
		calculated_offset = 0;
		calculated_multiplier = 1;
		return on_remove;
	}

	calculated_offset = lowest_pixel_after_remove;
	calculated_multiplier = 255.0f / ((float)highest_pixel_after_remove - (float)lowest_pixel_after_remove);

	return on_remove;
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


p6_data p6_list::rgb_to_hsl(p6_data value)
{
	float r = get<0>(value) / 255.0;
	float g = get<1>(value) / 255.0;
	float b = get<2>(value) / 255.0;

	float cmax = max(r, max(g, b));
	float cmin = min(r, min(g, b));
	float delta = cmax - cmin;

	float h = 0, s = 0, l = (cmax + cmin) / 2;

	if (delta == 0)
		h = 0;

	else if (cmax == r && g >= b)
		h = ((g - b) / delta) * 60 + 0;
	else if (cmax == r && g <= b)
		h = ((g - b) / delta) * 60 + 360;
	else if (cmax == g)
		h = ((b - r) / delta) * 60 + 120;
	else if (cmax == b)
		h = ((r - g) / delta) * 60 + 240;

	s = (delta) / (1 - abs(1 - 2 * l));

	h = roundf(h * 255.0f / 360.0f);
	s = roundf(s * 255.0f);
	l = roundf(l * 255.0f);

	if (h > 255)
		h = 255;
	if (h < 0)
		h = 0;

	if (s > 255)
		s = 255;
	if (s < 0)
		s = 0;

	if (l > 255)
		l = 255;
	if (l < 0)
		l = 0;

	return p6_data(h, s, l);
}

void p6_list::rgb_to_hsl(vector<vector<p6_data>>& rgb)
{
	int height = rgb.size();
	int width = rgb.front().size();

	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			rgb[i][j] = rgb_to_hsl(rgb[i][j]);

}

p6_data p6_list::hsl_to_rgb(p6_data value)
{
	float h = get<0>(value) / 255.0;
	float s = get<1>(value) / 255.0;
	float l = get<2>(value) / 255.0;

	float q = 0;

	if (l < 0.5)
		q = l * (1 + s);
	else if (l > 0.5)
		q = l + s - (l * s);

	float p = 2 * l - q;

	float tr = h + (1.0 / 3.0);
	float tg = h;
	float tb = h - (1.0 / 3.0);

	if (tr < 0.0)
		tr += 1.0;
	if (tr > 1.0)
		tr -= 1.0;

	if (tg < 0.0)
		tg += 1.0;
	if (tg > 1.0)
		tg -= 1.0;

	if (tb < 0.0)
		tb += 1.0;
	if (tb > 1.0)
		tb -= 1.0;

	float r, g, b;

	if (tr < (1.0 / 6.0))
		r = p + ((q - p) * 6 * tr);
	if (tg < (1.0 / 6.0))
		g = p + ((q - p) * 6 * tg);
	if (tb < (1.0 / 6.0))
		b = p + ((q - p) * 6 * tb);

	if (tr >= (1.0 / 6.0) && tr < (1.0 / 2.0))
		r = q;
	if (tg >= (1.0 / 6.0) && tg < (1.0 / 2.0))
		g = q;
	if (tb >= (1.0 / 6.0) && tb < (1.0 / 2.0))
		b = q;

	if (tr >= (1.0 / 2.0) && tr < (2.0 / 3.0))
		r = p + ((q - p) * 6.0 * ((2.0 / 3.0) - tr));
	if (tg >= (1.0 / 2.0) && tg < (2.0 / 3.0))
		g = p + ((q - p) * 6.0 * ((2.0 / 3.0) - tg));
	if (tb >= (1.0 / 2.0) && tb < (2.0 / 3.0))
		b = p + ((q - p) * 6.0 * ((2.0 / 3.0) - tb));

	if (tr >= (2.0 / 3.0))
		r = p;
	if (tg >= (2.0 / 3.0))
		g = p;
	if (tb >= (2.0 / 3.0))
		b = p;

	r = roundf(r * 255);
	g = roundf(g * 255);
	b = roundf(b * 255);

	if (r > 255)
		r = 255;
	if (r < 0)
		r = 0;
	if (g > 255)
		g = 255;
	if (g < 0)
		g = 0;
	if (b > 255)
		b = 255;
	if (b < 0)
		b = 255;


	return p6_data(r, g, b);
}

void p6_list::hsl_to_rgb(vector<vector<p6_data>>& hsl)
{
	int height = hsl.size();
	int width = hsl.front().size();

	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			hsl[i][j] = hsl_to_rgb(hsl[i][j]);
}

p6_data p6_list::rgb_to_hsv(p6_data value)
{
	float r = get<0>(value) / 255.0;
	float g = get<1>(value) / 255.0;
	float b = get<2>(value) / 255.0;

	float cmax = max(r, max(g, b));
	float cmin = min(r, min(g, b));
	float delta = cmax - cmin;

	float h = 0, s = 0, v = cmax;

	if (delta == 0)
		h = 0;

	else if (cmax == r && g >= b)
		h = ((g - b) / delta) * 60 + 0;
	else if (cmax == r && g <= b)
		h = ((g - b) / delta) * 60 + 360;
	else if (cmax == g)
		h = ((b - r) / delta) * 60 + 120;
	else if (cmax == b)
		h = ((r - g) / delta) * 60 + 240;

	if (cmax == 0)
		s = 0;
	else
		s = 1 - cmin / cmax;

	h = roundf(h * 255.0f / 360.0f);
	s = roundf(s * 255.0f);
	v = roundf(v * cmax * 255.0f);

	if (h > 255)
		h = 255;
	if (h < 0)
		h = 0;

	if (s > 255)
		s = 255;
	if (s < 0)
		s = 0;

	if (v > 255)
		v = 255;
	if (v < 0)
		v = 0;

	return p6_data(h, s, v);
}

void p6_list::rgb_to_hsv(vector<vector<p6_data>>& rgb)
{
	int height = rgb.size();
	int width = rgb.front().size();

	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			rgb[i][j] = rgb_to_hsv(rgb[i][j]);
}

p6_data p6_list::hsv_to_rgb(p6_data value)
{
	float h = get<0>(value) / 255.0 * 360.0;
	float s = get<1>(value) / 255.0;
	float v = get<2>(value) / 255.0;

	float c = v * s;
	float x = c * (1 - abs(((int)(h / 60)) % 2 + ((h / 60) - (int)(h / 60)) - 1));
	float m = v - c;

	float rf = 0;
	float gf = 0;
	float bf = 0;

	if (h >= 0 && h < 60)
	{
		rf = c; gf = x; bf = 0;
	}
	else if (h >= 60 && h < 120)
	{
		rf = x; gf = c; bf = 0;
	}
	else if (h >= 120 && h < 180)
	{
		rf = 0; gf = c; bf = x;
	}
	else if (h >= 180 && h < 240)
	{
		rf = 0; gf = x; bf = c;
	}
	else if (h >= 240 && h < 300)
	{
		rf = x; gf = 0; bf = c;
	}
	else
	{
		rf = c; gf = 0; bf = x;
	}

	float r, g, b;

	r = roundf((rf + m) * 255.0f);
	g = roundf((gf + m) * 255.0f);
	b = roundf((bf + m) * 255.0f);

	if (r > 255) r = 255;
	if (r < 0) r = 0;
	if (b > 255) b = 255;
	if (b < 0) b = 0;
	if (g > 255) g = 255;
	if (g < 0) g = 0;

	return p6_data(r, g, b);
}

void p6_list::hsv_to_rgb(vector<vector<p6_data>>& hsv)
{
	int height = hsv.size();
	int width = hsv.front().size();

	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			hsv[i][j] = hsv_to_rgb(hsv[i][j]);
}

p6_data p6_list::rgb_to_cmy(p6_data value)
{
	float h = get<0>(value);
	float s = get<1>(value);
	float v = get<2>(value);

	return p6_data(255 - h, 255 - s, 255 - v);
}

void p6_list::rgb_to_cmy(vector<vector<p6_data>>& rgb)
{
	int height = rgb.size();
	int width = rgb.front().size();

	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			rgb[i][j] = rgb_to_cmy(rgb[i][j]);
}

p6_data p6_list::rgb_to_ycocg(p6_data value)
{
	float r = get<0>(value) / 255.0;
	float g = get<1>(value) / 255.0;
	float b = get<2>(value) / 255.0;

	float y = r / 4 + g / 2 + b / 4;
	float co = r / 2 - b / 2 + 0.5;
	float cg = -r / 4 + g / 2 - b / 4 + 0.5;

	y = roundf(y * 255.0f);
	co = roundf(co * 255.0f);
	cg = roundf(cg * 255.0f);

	if (y > 255)
		y = 255;
	else if (co > 255)
		co = 255;
	else if (cg > 255)
		cg = 255;

	if (y < 0)
		y = 0;
	else if (co < 0)
		co = 0;
	else if (cg < 0)
		cg = 0;

	return p6_data(y, co, cg);
}

void p6_list::rgb_to_ycocg(vector<vector<p6_data>>& rgb)
{
	int height = rgb.size();
	int width = rgb.front().size();

	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			rgb[i][j] = rgb_to_ycocg(rgb[i][j]);
}

p6_data p6_list::ycocg_to_rgb(p6_data value)
{
	float y = get<0>(value) / 255.0;
	float co = get<1>(value) / 255.0 - 0.5;
	float cg = get<2>(value) / 255.0 - 0.5;

	float r = y + co - cg;
	float g = y + cg;
	float b = y - co - cg;

	r = roundf(r * 255.0f);
	g = roundf(g * 255.0f);
	b = roundf(b * 255.0f);

	if (r > 255)
		r = 255;
	if (r < 0)
		r = 0;

	if (g > 255)
		g = 255;
	if (g < 0)
		g = 0;

	if (b > 255)
		b = 255;
	if (b < 0)
		b = 0;

	return p6_data(r, g, b);
}

void p6_list::ycocg_to_rgb(vector<vector<p6_data>>& ycocg)
{
	int height = ycocg.size();
	int width = ycocg.front().size();

	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			ycocg[i][j] = ycocg_to_rgb(ycocg[i][j]);
}

p6_data p6_list::rgb_to_ycbcr601(p6_data value)
{
	float r = get<0>(value) / 255.0f;
	float g = get<1>(value) / 255.0f;
	float b = get<2>(value) / 255.0f;

	float kb = 0.299f;
	float kr = 0.587f;
	float kg = 0.114f;

	float y, cb, cr;

	y = kr * r + kg * g + kb * b;
	cb = (b - y) / (2 * (1 - kb));
	cr = (r - y) / (2 * (1 - kr));

	y = roundf(255.0f * y);
	cb = roundf(255.0f * (cb + 0.5f));
	cr = roundf(255.0f * (cr + 0.5f));

	if (y > 255)
		y = 255;
	if (y < 0)
		y = 0;

	if (cb > 255)
		cb = 255;
	if (cb < 0)
		cb = 0;

	if (cr > 255)
		cr = 255;
	if (cr < 0)
		cr = 0;

	return p6_data(y, cb, cr);
}

void p6_list::rgb_to_ycbcr601(vector<vector<p6_data>>& rgb)
{
	int height = rgb.size();
	int width = rgb.front().size();

	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			rgb[i][j] = rgb_to_ycbcr601(rgb[i][j]);
}

p6_data p6_list::ycbcr601_to_rgb(p6_data value)
{
	float y = get<0>(value) / 255.0f;
	float cb = get<1>(value) / 255.0f - 0.5f;
	float cr = get<2>(value) / 255.0f - 0.5f;

	float kb = 0.299;
	float kr = 0.587;
	float kg = 0.114;

	float r, g, b;

	r = y + cr * (2 - 2 * kr);
	g = y - kb * (2 - 2 * kb) / kg * cb - kr * (2 - 2 * kr) / kg * cr;
	b = y + cb * (2 - 2 * kb);

	r = roundf(255.0f * r);
	g = roundf(255.0f * g);
	b = roundf(255.0f * b);

	if (r > 255)
		r = 255;
	else if (r < 0)
		r = 0;

	if (g > 255)
		g = 255;
	else if (g < 0)
		g = 0;

	if (b > 255)
		b = 255;
	else if (b < 0)
		b = 0;

	return p6_data(r, g, b);
}

void p6_list::ycbcr601_to_rgb(vector<vector<p6_data>>& ycbcr601)
{
	int height = ycbcr601.size();
	int width = ycbcr601.front().size();

	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			ycbcr601[i][j] = ycbcr601_to_rgb(ycbcr601[i][j]);
}

p6_data p6_list::rgb_to_ycbcr709(p6_data value)
{
	float r = get<0>(value) / 255.0f;
	float g = get<1>(value) / 255.0f;
	float b = get<2>(value) / 255.0f;

	float kb = 0.0722f;
	float kr = 0.2126f;
	float kg = 0.7152f;

	float y, cb, cr;

	y = kr * r + kg * g + kb * b;
	cb = (b - y) / (2 * (1 - kb));
	cr = (r - y) / (2 * (1 - kr));

	y = roundf(255.0f * y);
	cb = roundf(255.0f * (cb + 0.5f));
	cr = roundf(255.0f * (cr + 0.5f));

	if (y > 255)
		y = 255;
	if (y < 0)
		y = 0;

	if (cb > 255)
		cb = 255;
	if (cb < 0)
		cb = 0;

	if (cr > 255)
		cr = 255;
	if (cr < 0)
		cr = 0;

	return p6_data(y, cb, cr);
}

void p6_list::rgb_to_ycbcr709(vector<vector<p6_data>>& rgb)
{
	int height = rgb.size();
	int width = rgb.front().size();

	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			rgb[i][j] = rgb_to_ycbcr709(rgb[i][j]);
}

p6_data p6_list::ycbcr709_to_rgb(p6_data value)
{
	float y = get<0>(value) / 255.0f;
	float cb = get<1>(value) / 255.0f - 0.5f;
	float cr = get<2>(value) / 255.0f - 0.5f;

	float kb = 0.0722f;
	float kr = 0.2126f;
	float kg = 0.7152f;

	float r, g, b;

	r = y + cr * (2 - 2 * kr);
	g = y - kb * (2 - 2 * kb) / kg * cb - kr * (2 - 2 * kr) / kg * cr;
	b = y + cb * (2 - 2 * kb);

	r = roundf(255.0f * r);
	g = roundf(255.0f * g);
	b = roundf(255.0f * b);

	if (r > 255)
		r = 255;
	else if (r < 0)
		r = 0;

	if (g > 255)
		g = 255;
	else if (g < 0)
		g = 0;

	if (b > 255)
		b = 255;
	else if (b < 0)
		b = 0;

	return p6_data(r, g, b);
}

void p6_list::ycbcr709_to_rgb(vector<vector<p6_data>>& ycbcr709)
{
	int height = ycbcr709.size();
	int width = ycbcr709.front().size();

	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			ycbcr709[i][j] = ycbcr709_to_rgb(ycbcr709[i][j]);
}

p6_data p6_list::cmy_to_rgb(p6_data value)
{
	float h = get<0>(value);
	float s = get<1>(value);
	float v = get<2>(value);

	return p6_data(255 - h, 255 - s, 255 - v);
}

void p6_list::cmy_to_rgb(vector<vector<p6_data>>& cmy)
{
	int height = cmy.size();
	int width = cmy.front().size();

	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			cmy[i][j] = cmy_to_rgb(cmy[i][j]);
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
