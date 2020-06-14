#pragma once
#include "IPixel_list.h"
#include <vector>
#include "vector2d.h"

using namespace std;

class p5_list final : public IPixel_list
{
public:
	vector<vector<unsigned char>> pixels;
	
	p5_list(int width, int height);
	p5_list(vector<vector<unsigned char>> pixels);
	~p5_list() override;

	void turn_left() override;
	void turn_right() override;
	void horizontal_reflect() override;
	void vertical_reflect() override;
	void ez_line(double x1, double y1, double x2, double y2, float brightness);
	void ez_line_width(double x1, double y1, double x2, double y2, double width, float gamma);
	
	vector<vector<unsigned char>> fidelity_fx(float sharpness);
	int soft_min(int x, int y);
	int soft_max(int x, int y);
	int cross_sum(int x, int y);

	int mi(int x, int y);
	int ma(int x, int y);



	void inverse_pixel() override;

	ostream& operator<<(ostream& os) override;
	istream& operator>>(istream& is) override;
};
