#pragma once
#include "IPixel_list.h"
#include <vector>
typedef unsigned char p5_data;
typedef tuple<unsigned char, unsigned char, unsigned char> p6_data;
using namespace std;

class p6_list final : public IPixel_list
{
public:
	vector<vector<p6_data>> pixels;
	
	p6_list(vector<vector<p6_data>> pixels);

	p6_list(int width, int height);
	~p6_list() override;
	
	void turn_left() override;
	void turn_right() override;
	void horizontal_reflect() override;
	void vertical_reflect() override;
	void inverse_pixel() override;
	void convert(string from, string to);
	void offset(float off, float multiplier, bool only_first, float ignore_percent, bool auto_contrast);

	static vector<vector<p6_data>> combine(vector<vector<vector<p5_data>>>& xyz);
	static vector<vector<p6_data>> combine(vector<vector<p5_data>>& x, vector<vector<p5_data>>& y, vector<vector<p5_data>>& z);
	static vector<vector<vector<p5_data>>> split(vector<vector<p6_data>>& x);

	static p6_data rgb_to_hsl(p6_data value);
	static void rgb_to_hsl(vector<vector<p6_data>>& rgb);
	static p6_data hsl_to_rgb(p6_data value);
	static void hsl_to_rgb(vector<vector<p6_data>>& hsl);

	static p6_data rgb_to_hsv(p6_data value);
	static void rgb_to_hsv(vector<vector<p6_data>>& rgb);
	static p6_data hsv_to_rgb(p6_data value);
	static void hsv_to_rgb(vector<vector<p6_data>>& hsv);

	static p6_data rgb_to_cmy(p6_data value);
	static void rgb_to_cmy(vector<vector<p6_data>>& rgb);
	static p6_data cmy_to_rgb(p6_data value);
	static void cmy_to_rgb(vector<vector<p6_data>>& cmy);

	static p6_data rgb_to_ycocg(p6_data value);
	static void rgb_to_ycocg(vector<vector<p6_data>>& rgb);
	static p6_data ycocg_to_rgb(p6_data value);
	static void ycocg_to_rgb(vector<vector<p6_data>>& ycocg);

	static p6_data rgb_to_ycbcr601(p6_data value);
	static void rgb_to_ycbcr601(vector<vector<p6_data>>& rgb);
	static p6_data ycbcr601_to_rgb(p6_data value);
	static void ycbcr601_to_rgb(vector<vector<p6_data>>& ycbcr601);

	static p6_data rgb_to_ycbcr709(p6_data value);
	static void rgb_to_ycbcr709(vector<vector<p6_data>>& rgb);
	static p6_data ycbcr709_to_rgb(p6_data value);
	static void ycbcr709_to_rgb(vector<vector<p6_data>>& ycbcr709);

	ostream& operator<<(ostream& os) override;
	istream& operator>>(istream& is) override;
};
