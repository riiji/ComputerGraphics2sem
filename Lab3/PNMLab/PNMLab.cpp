#include <iostream>
#include <fstream>
#include <tuple>

#include "pnm.h"
#include "operations.h"
#include "extensions.h"
#include "p5_list.h"
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
	pnm pic = pnm();

	try {
		if (argc != 7)
			throw exception("invalid argument count");

		ifstream is(argv[1], ios::binary);

		if (!is.is_open())
			throw exception("invalid filename input");

		pic.operator>>(is);

		if (pic.version != "P5")
			throw exception("only p5 version available");

		int gradient = stoi(argv[3]);
		int dithering = stoi(argv[4]);
		int bit = stoi(argv[5]);
		float gamma = stoi(argv[6]);

		if (bit < 1 || bit>8)
			throw exception("invalid bit value");

		p5_list* p5 = (p5_list*)pic.table_data;

		switch (gradient)
		{
		case 0: break;
		case 1: p5->draw_horizontal_gradient(gamma); break;
		default: throw exception("invalid gradient value"); break;
		}

		switch (dithering)
		{
		case 0: p5->no_dithering(bit, gamma); break;
		case 1: p5->ordered_dithering(bit, gamma); break;
		case 2: p5->random_dithering(bit, gamma); break;
		case 3: p5->floid_dithering(bit, gamma); break;
		case 4: p5->jjn_dithering(bit, gamma); break;
		case 5: p5->sierra_dithering(bit, gamma); break;
		case 6: p5->atkinson_dithering(bit, gamma); break;
		case 7: p5->halftone_dithering(bit, gamma); break;
		default: throw exception("invalid dither type value"); break;
		}

		ofstream os(argv[2], ios::binary);

		if (!os.is_open())
			throw exception("invalid filename output");

		pic.operator<<(os);
	}
	catch (exception e)
	{
		std::cerr << e.what();
	}
}

