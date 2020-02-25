#pragma once
#include "IPixel_list.h"
#include <vector>

using namespace std;

class p6_list final : public IPixel_list
{
public:
	vector<vector<tuple<char, char, char>>> pixels;
	
	p6_list(int width, int height);
	~p6_list() override;
	
	void turn_left() override;
	void turn_right() override;
	void horizontal_reflect() override;
	void vertical_reflect() override;
	void inverse_pixel() override;

	ostream& operator<<(ostream& os) override;
	istream& operator>>(istream& is) override;
};
