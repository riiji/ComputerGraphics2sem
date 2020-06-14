#include <iostream>
#include <fstream>
#include <tuple>
#include "pnm.h"
#include "operations.h"
#include "extensions.h"
#include "p5_list.h"
#include <string>
#include "p6_list.h"

using namespace std;

int main(int argc, char* argv[])
{
	try {
		if (argc != 4)
			throw exception("invalid argument count");

		string input_file = string(argv[1]);
		string ouptut_file = string(argv[2]);

		float sharpness = stof(argv[3]);

		if (sharpness < 0 || sharpness>1.0f)
			throw exception("invalid sharpness value");

		ifstream is(input_file, ios::binary);

		if (!is.is_open())
			throw exception("invalid input file");

		pnm pic;

		pic.operator>>(is);

		if (pic.version == "P6")
		{
			auto p6 = ((p6_list*)pic.table_data);
			auto p5_splitted = p6_list::split(p6->pixels);
			
			auto p5_1 = new p5_list(p5_splitted[0]);
			auto p5_2 = new p5_list(p5_splitted[1]);
			auto p5_3 = new p5_list(p5_splitted[2]);

			p5_1->pixels = p5_1->fidelity_fx(sharpness);
			p5_2->pixels = p5_2->fidelity_fx(sharpness);
			p5_3->pixels = p5_3->fidelity_fx(sharpness);

			auto p6_sharpness = p6_list::combine(p5_1->pixels, p5_2->pixels, p5_3->pixels);

			pic.table_data = new p6_list(p6_sharpness);
		}
		else
		{
			auto p5 = ((p5_list*)pic.table_data);
			p5->pixels = p5->fidelity_fx(sharpness);
		}

		ofstream os(ouptut_file, ios::binary);

		if (!os.is_open())
			throw exception("invalid output file");

		pic.operator<<(os);
	}
	catch (exception e)
	{
		std::cerr << e.what();
		return 1;
	}
}

