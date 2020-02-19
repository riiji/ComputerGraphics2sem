#include <iostream>
#include <fstream>
#include "base_pnm.h"
#include "p5_pixel.h"
#include "p6_pixel.h"

int main(int argc, char* argv[])
{
	base_pnm pnm;

	ifstream is(argv[1]);
	ofstream os(argv[2]);

	if (!is.is_open() || !os.is_open())
		throw exception("invalid filename input");
	
	pnm.operator>>(is);

	auto command_value = stoi(argv[3]);

	switch (command_value)
	{
	case 0:
		pnm.inverse_pixels();
		break;
	case 1:
		pnm.horizontal_reflect();
		break;
	case 2:
		pnm.vertical_reflect();
		break;
	case 3:
		pnm.turn_right();
		break;
	case 4:
		pnm.turn_left();
		break;
	default: throw exception("invalid command input");
	}

	pnm.operator<<(os);
}

