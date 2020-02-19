#include "p5_pixel.h"
#include <iostream>
#include <climits>

p5_pixel::~p5_pixel()
{
	delete& amount;
}

void p5_pixel::inverse()
{
	amount = CHAR_MAX - amount;
}

ostream& p5_pixel::operator<<(ostream& os)
{
	os << amount;
	return os;
}

istream& p5_pixel::operator>>(istream& is)
{
	is >> amount;
	return is;
}
