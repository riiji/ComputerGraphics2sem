#include "p6_pixel.h"
#include <iostream>

p6_pixel::~p6_pixel()
{
	delete& r;
	delete& g;
	delete& b;
}

void p6_pixel::inverse()
{
	r = CHAR_MAX - r;
	g = CHAR_MAX - g;
	b = CHAR_MAX - b;
}

ostream& p6_pixel::operator<<(ostream& os)
{
	os << r << g  << b;
	return os;
}

istream& p6_pixel::operator>>(istream& is)
{
	is >> r >> g >> b;
	return is;
}
