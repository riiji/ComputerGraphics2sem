#pragma once
#include "IPixel.h"

class p5_pixel : public IPixel
{
public:
	//dispose
	~p5_pixel() override;

	// fields
	char amount;

	// methods
	void inverse() override;

	// extensions
	ostream& operator<<(ostream& os) override;
	istream& operator>>(istream& is) override;
};
