#pragma once
#include <ostream>
#include <vector>
using namespace std;
class IPixel_list
{
public:

	int width, height;
	
	virtual ~IPixel_list() = default;

	virtual void inverse_pixel() = 0;
	virtual void turn_left() = 0;
	virtual void turn_right() = 0;
	virtual void horizontal_reflect() = 0;
	virtual void vertical_reflect() = 0;
	
	virtual ostream& operator<<(ostream& os) = 0;
	virtual istream& operator>>(istream& is) = 0;
};
