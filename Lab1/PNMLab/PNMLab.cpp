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

	
	pic.height = 1200;
	pic.width = 700;
	pic.magic = 255;
	pic.version = "P5";

	vector<vector<unsigned char>> l(1200, vector<unsigned char>(700, 255));

	auto list = new p5_list(l);

	list->draw_horizontal_gradient();
	list->jjn_dithering(1);

	pic.table_data = new p5_list(*list);

	freopen("225HO.pgm", "wb", stdout);
	pic.operator<<(cout);
}

	