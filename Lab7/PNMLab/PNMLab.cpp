#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
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

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

struct Pod {
	int x;
};

int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
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

			delete p5_1;
			delete p5_2;
			delete p5_3;

			((p6_list*)pic.table_data)->pixels = p6_sharpness;
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

		_CrtDumpMemoryLeaks();
	}
	catch (exception e)
	{
		std::cerr << e.what();
		return 1;
	}
}

