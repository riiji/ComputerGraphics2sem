#pragma once
#include <ostream>

using namespace std;

class IPixel
{
public:
	//dispose
	virtual ~IPixel() = default;
	
	// methods
	virtual void inverse() = 0;

	// extensions
	virtual ostream& operator<<(ostream& os) = 0;
	virtual istream& operator>>(istream& is) = 0;
};
