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
	void ez_line(double x1, double y1, double x2, double y2);
	void ez_line_width(double x1, double y1, double x2, double y2, double width);
	void draw_horizontal_gradient();
	void base_dithering(vector<vector2d> offsets, double bit);
	void floid_dithering(float bit);
	void jjn_dithering(float bit);
	void atkinson_dithering(float bit);
	void sierra_dithering(float bit);
	void random_dithering(float bit);
	void base_matrix_dithering(vector<vector<unsigned char>> matrix, float bit);
	void ordered_dithering(float bit);
	void halftone_dithering(float bit);
	void no_dithering(float bit);
	void gamma_correction(float gamma);

	void inverse_pixel() override;

	ostream& operator<<(ostream& os) override;
	istream& operator>>(istream& is) override;
};
