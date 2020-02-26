#include <iostream>
#include <fstream>
#include "pnm.h"
#include "operations.h"

using namespace std;

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 4)
			throw exception("invalid arguments count");

		pnm* pic = new pnm();

		ifstream is(argv[1], ios::binary);

		if (!is.is_open())
			throw exception("invalid filename input");

		pic->operator>>(is);

		const auto command_value = stoi(argv[3]);

		switch (command_value)
		{
		case 0:
			pic->table_data->inverse_pixel();
			break;
		case 1:
			pic->table_data->horizontal_reflect();
			break;
		case 2:
			pic->table_data->vertical_reflect();
			break;
		case 3:
			pic->table_data->turn_right();
			pic->swap();
			break;
		case 4:
			pic->table_data->turn_left();
			pic->swap();
			break;
		default: throw exception("invalid command input");
		}

		ofstream os(argv[2], ios::binary);

		if (!os.is_open())
			throw exception("can't create output file");

		pic->operator<<(os);
	}
	catch (exception e)
	{
		cout << e.what();
	}
}

