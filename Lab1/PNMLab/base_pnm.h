#include <string>
#include <vector>

#include "IPixel.h"

#pragma once

using namespace std;

class base_pnm
{
public:
	// constr/dectr
	~base_pnm();
	
	// fields
	string version;
	int width, height;
	int magic;
	vector<vector<IPixel*>> table_data;

	// methods
	void inverse_pixels();
	void turn_left();
	void turn_right();
	void horizontal_reflect();
	void vertical_reflect();

	// extensions
	ostream& operator<<(ostream& os);
	istream& operator>>(istream& is);
};
