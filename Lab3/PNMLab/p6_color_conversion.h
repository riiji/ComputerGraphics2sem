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

	static p6_data rgb_to_hsv(p6_data value);
	static p6_data hsv_to_rgb(p6_data value);
	
	static p6_data rgb_to_cmy(p6_data value);
	static p6_data cmy_to_rgb(p6_data value);
	
	static p6_data rgb_to_ycocg(p6_data value);
	static p6_data ycocg_to_rgb(p6_data value);
	
	static p6_data rgb_to_ycbcr601(p6_data value);
	static p6_data ycbcr601_to_rgb(p6_data value);
	
	static p6_data rgb_to_ycbcr709(p6_data value);
	static p6_data ycbcr709_to_rgb(p6_data value);
};

p6_data p6_color_conversion::rgb_to_hsl(p6_data value)
{
	float r = get<0>(value) / 255.0;
	float g = get<1>(value) / 255.0;
	float b = get<2>(value) / 255.0;

	float mx = max(r, max(g, b));
	float mn = min(r, min(g, b));
	float h, s, l = (mx + mn) / 2;

	s = (mx - mn) / (1 - abs(1 - (mx + mn)));

	if (mx == mn)
		h = 0;
	else if (r == mx && g >= b)
		h = (g - b) / (mx - mn) * 60;
	else if (r == mx && g < b)
		h = (g - b) / (mx - mn) * 60 + 360;
	else if (mx == g)
		h = (b - r) / (mx - mn) * 60 + 120;
	else if (mx == b)
		h = (r - g) / (mx - mn) * 60 + 240;

	return p6_data(r, g, b);
}

inline p6_data p6_color_conversion::hsl_to_rgb(p6_data value)
{
	float h = get<0>(value) / 255.0;
	float s = get<1>(value) / 255.0;
	float l = get<2>(value) / 255.0;

	float q, p;
	
	if (l < 0.5)
		q = l * (s + 1.0);
	else
		q = l + s - (l * s);

	p = l * 2 - q;
	
	float tr = h + 1.0 / 3.0, tg = h, tb = h - 1.0 / 3.0;
	
	if (tr < 0) tr += 1.0;
	if (tr > 1.0) tr -= 1.0;
	if (tg < 0) tg += 1.0;
	if (tg > 1.0) tg -= 1.0;
	if (tb < 0) tb += 1.0;
	if (tb > 1.0) tb -= 1.0;
	
	float r, g, b;

	if (tr < 1.0 / 6.0)
		r = p + ((q - p) * 6.0 * tr);
	else if (tr >= 1.0 / 6.0 && tr < 0.5)
		r = q;
	else if (tr >= 0.5 && tr < 2.0 / 3.0)
		r = p + ((q - p) * (2.0 / 3.0 - tr) * 6.0);
	else
		r = p;

	if (tg < 1.0 / 6.0)
		g = p + ((q - p) * 6.0 * tg);
	else if (tg >= 1.0 / 6.0 && tg < 0.5)
		g = q;
	else if (tg >= 0.5 && tg < 2.0 / 3.0)
		g = p + ((q - p) * (2.0 / 3.0 - tg) * 6.0);
	else
		g = p;

	if (tb < 1.0 / 6.0)
		b = p + ((q - p) * 6.0 * tb);
	else if (tb >= 1.0 / 6.0 && tb < 0.5)
		b = q;
	else if (tb >= 0.5 && tb < 2.0 / 3.0)
		b = p + ((q - p) * (2.0 / 3.0 - tb) * 6.0);
	else
		b = p;

	return p6_data(r * 255, g * 255, b * 255);
}

inline p6_data p6_color_conversion::rgb_to_hsv(p6_data value)
{
	float r = get<0>(value) / 255.0;
	float g = get<1>(value) / 255.0;
	float b = get<2>(value) / 255.0;

	float mx = max(r, max(g, b));
	float mn = min(r, min(g, b));

	float h, s, v = mx, c = mx - mn;

	if (c == 0)
		h = 0;
	else if (v == r)
		h = (g - b) / c;
	else if (v == g)
		h = 2 + (b - r) / c;
	else
		h = 4 + (r - g) / c;

	h *= 60.0;

	if (h < 0)
		h += 360;

	s = (v == 0 ? 0 : c / v);

	h = h / 360.0 * 255.0;
	s = s * 255.0;
	v = v * 255.0;

	return p6_data(h, s, v);
}

inline p6_data p6_color_conversion::hsv_to_rgb(p6_data value)
{
	float h = get<0>(value) / 255.0 * 360.0;
	float s = get<1>(value) / 255.0;
	float v = get<2>(value) / 255.0;

	float c = v * s;
	float hh = h / 60;
	float x = c * (1 - abs(((int)hh) % 2 + (hh - (int)hh) - 1));
	float m = v - c;
	float r, g, b;

	if (h >= 0 && h <= 60)
		r = c, g = x, b = 0;
	else if (h >= 60 && h <= 120)
		r = x, g = c, b = 0;
	else if (h >= 120 && h <= 180)
		r = 0, g = c, b = x;
	else if (h >= 180 && h <= 240)
		r = 0, g = x, b = c;
	else if (h >= 240 && h <= 300)
		r = x, g = 0, b = c;
	else
		r = c, g = 0, b = x;

	int rr = (int)(round((r + m) * 255));
	int gg = (int)(round((g + m) * 255));
	int bb = (int)(round((b + m) * 255));

	if (rr < 0) rr = 0;
	if (rr > 255) rr = 255;
	if (bb < 0) bb = 0;
	if (bb > 255) bb = 255;
	if (gg < 0) gg = 0;
	if (gg > 255) gg = 255;

	return p6_data(rr, gg, bb);
}

inline p6_data p6_color_conversion::rgb_to_cmy(p6_data value)
{
	float h = get<0>(value);
	float s = get<1>(value);
	float v = get<2>(value);

	return p6_data(255 - h, 255 - s, 255 - v);
}

inline p6_data p6_color_conversion::rgb_to_ycocg(p6_data value)
{
	float r = get<0>(value) / 255.0;
	float g = get<1>(value) / 255.0;
	float b = get<2>(value) / 255.0;

	float y = r / 4 + g / 2 + b / 4;
	float co = r / 2 - b / 2 + 0.5;
	float cg = -r / 4 + g / 2 - b / 4 + 0.5;

	if (y > 1.0) y = 1.0;
	if (y < 0) y = 0;
	if (co > 1.0) co = 1.0;
	if (co < 0) co = 0;
	if (cg > 1.0) cg = 1.0;
	if (cg < 0) cg = 0;

	return p6_data(y * 255, co * 255, cg * 255);
}

inline p6_data p6_color_conversion::ycocg_to_rgb(p6_data value)
{
	float y = get<0>(value) / 255.0;
	float co = get<1>(value) / 255.0 - 0.5;
	float cg = get<2>(value) / 255.0 - 0.5;

	float r = y + co - cg;
	float g = y + cg;
	float b = y - co - cg;

	if (r > 1.0) r = 1.0;
	if (r < 0) r = 0;
	if (g > 1.0) g = 1.0;
	if (g < 0) g = 0;
	if (b > 1.0) b = 1.0;
	if (b < 0) b = 0;

	return p6_data(r * 255, g * 255, b * 255);
}

inline p6_data p6_color_conversion::rgb_to_ycbcr601(p6_data value)
{
	float r = get<0>(value) / 255.0;
	float g = get<1>(value) / 255.0;
	float b = get<2>(value) / 255.0;

	float kb = 0.299;
	float kr = 0.587;
	float kg = 0.114;

	float y = kr * r + kg * g + kb * b;
	float cb = (b - y) / (2 * (1 - kb));
	float cr = (r - y) / (2 * (1 - kr));

	int rr = round(y * 255), gg = round((cb + 0.5) * 255), bb = round((cr + 0.5) * 255);
	if (rr < 0) rr = 0;
	if (rr > 255) rr = 255;
	if (bb < 0) bb = 0;
	if (bb > 255) bb = 255;
	if (gg < 0) gg = 0;
	if (gg > 255) gg = 255;
	return p6_data(rr, gg, bb);
}

inline p6_data p6_color_conversion::ycbcr601_to_rgb(p6_data value)
{
	float y = get<0>(value) / 255.0;
	float cb = get<1>(value) / 255.0 - 0.5;
	float cr = get<2>(value) / 255.0 - 0.5;

	float kb = 0.299;
	float kr = 0.587;
	float kg = 0.114;

	float r = y + (2.0 - 2.0 * kr) * cr;
	float g = y - kb * (2.0 - 2.0 * kb) * cb / kg - kr * (2 - 2.0 * kr) * cr / kg;
	float b = y + (2.0 - 2.0 * kb) * cb;
	
	int rr = round(r * 255), gg = round(g * 255), bb = round(b * 255);

	if (rr > 255) rr = 255;
	if (rr < 0) rr = 0;
	if (gg > 255) gg = 255;
	if (gg < 0) gg = 0;
	if (bb > 255) bb = 255;
	if (bb < 0) bb = 0;

	return p6_data(rr, gg, bb);
}

inline p6_data p6_color_conversion::rgb_to_ycbcr709(p6_data value)
{
	float r = get<0>(value) / 255.0;
	float g = get<1>(value) / 255.0;
	float b = get<2>(value) / 255.0;

	float kb = 0.0722;
	float kr = 0.2126;
	float kg = 0.7152;

	float y = kr * r + kg * g + kb * b;
	float cb = (b - y) / (2 * (1 - kb));
	float cr = (r - y) / (2 * (1 - kr));

	int rr = round(y * 255), gg = round((cb + 0.5) * 255), bb = round((cr + 0.5) * 255);
	if (rr < 0) rr = 0;
	if (rr > 255) rr = 255;
	if (bb < 0) bb = 0;
	if (bb > 255) bb = 255;
	if (gg < 0) gg = 0;
	if (gg > 255) gg = 255;

	return p6_data(rr, gg, bb);
}

inline p6_data p6_color_conversion::ycbcr709_to_rgb(p6_data value)
{
	float y = get<0>(value) / 255.0;
	float cb = get<1>(value) / 255.0 - 0.5;
	float cr = get<2>(value) / 255.0 - 0.5;

	float kb = 0.0722;
	float kr = 0.2126;
	float kg = 0.7152;

	float r = y + (2 - 2 * kr) * cr;
	float g = y - kb * (2 - 2 * kb) * cb / kg - kr * (2 - 2 * kr) * cr / kg;
	float b = y + (2 - 2 * kb) * cb;
	
	int rr = round(r * 255), gg = round(g * 255), bb = round(b * 255);
	
	if (rr > 255) rr = 255;
	if (rr < 0) rr = 0;
	if (gg > 255) gg = 255;
	if (gg < 0) gg = 0;
	if (bb > 255) bb = 255;
	if (bb < 0) bb = 0;

	return p6_data(rr, gg, bb);
}

inline p6_data p6_color_conversion::cmy_to_rgb(p6_data value)
{
	float h = get<0>(value);
	float s = get<1>(value);
	float v = get<2>(value);

	return p6_data(255 - h, 255 - s, 255 - v);
}
