﻿#pragma once
#include <string>
#include "IPixel_list.h"
#pragma once

using namespace std;

class pnm
{
public:
	// constr/dectr
	~pnm();
	pnm(string version, int width, int height, int magic, IPixel_list* pixels);
	pnm();
	// fields
	string version;
	int width, height;
	int magic;
	IPixel_list* table_data;

	void swap();
	
	// extensions
	ostream& operator<<(ostream& os) const;
	istream& operator>>(istream& is);
};
