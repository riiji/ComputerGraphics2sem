#pragma once
#include "IPixel.h"

class p6_pixel : public IPixel
{
public:
	//dispose
	~p6_pixel() override;

	// fields
	char r, g, b;

	// methods
	void inverse() override;

	// extensions
	ostream& operator<<(ostream& os) override;
	istream& operator>>(istream& is) override;
};
