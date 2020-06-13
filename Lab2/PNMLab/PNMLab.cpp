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
		if (argc != 10)
			throw exception("invalid argument count");

		string input_file = string(argv[1]);
		string output_file = string(argv[2]);
		float brightness = stof(argv[3]);
		float thikness = stof(argv[4]);

		float horizontal_start_pos = stof(argv[5]);
		float vertical_start_pos = stof(argv[6]);

		float horizontal_end_pos = stof(argv[7]);
		float vertical_end_pos = stof(argv[8]);

		float gamma = stof(argv[9]);

		if (thikness != 1)
			throw exception("only thiknesss 1 supported");
		if (gamma != 2)
			throw exception("only gamma 2 supported");

		pnm pic;

		ifstream is(input_file, ios::binary);

		if (!is.is_open())
			throw exception("invalid input file");

		pic.operator>>(is);
		
		((p5_list*)pic.table_data)->ez_line(horizontal_start_pos, vertical_start_pos, horizontal_end_pos, vertical_end_pos, brightness, gamma);

		ofstream os(output_file, ios::binary);

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

