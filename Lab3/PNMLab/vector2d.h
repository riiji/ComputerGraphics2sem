#pragma once

class vector2d {
public:
	vector2d(int x, int y, int dith)
	{
		this->x = x;
		this->y = y;
		this->dith = dith;
	}

	int x, y;
	int dith;
};