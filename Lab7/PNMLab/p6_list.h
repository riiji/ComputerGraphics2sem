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

	static vector<vector<p6_data>> combine(vector<vector<vector<p5_data>>>& xyz);
	static vector<vector<p6_data>> combine(vector<vector<p5_data>>& x, vector<vector<p5_data>>& y, vector<vector<p5_data>>& z);
	static vector<vector<vector<p5_data>>> split(vector<vector<p6_data>>& x);

	void fidelity_fx(float sharpness);

	ostream& operator<<(ostream& os) override;
	istream& operator>>(istream& is) override;
};
