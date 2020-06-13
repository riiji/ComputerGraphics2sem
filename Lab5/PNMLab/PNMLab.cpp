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

	int fake = 0; float fake2 = 0;
	try {
		if (argc != 4 && argc != 6)
			throw exception("invalid argument count");

		string input_filename = string(argv[1]);
		string output_filename = string(argv[2]);
		int convertion = stoi(argv[3]);
		int offset = 0;
		float multiplier = 0;

		if (argc == 6)
		{
			offset = stoi(argv[4]);
			multiplier = stof(argv[5]);

			if (offset < -255 || offset>255)
				throw exception("invalid offset value");
			if (multiplier < (1.0f / 255.0f) || multiplier>255.0f)
				throw exception("invalid multiplier value");
		}

		if (argc == 6)
			if (convertion != 0 && convertion != 1)
				throw exception("invalid conversion value");

		if (argc == 4)
			if (convertion < 2 && convertion>5)
				throw exception("invalid conversion value");

		pnm pic = pnm();

		ifstream is(input_filename, ios::binary);

		if (!is.is_open())
			throw exception("can't open input file");

		pic.operator>>(is);

		if (convertion == 0)
		{
			if (pic.version == "P5")
			{
				auto p5 = (p5_list*)pic.table_data;

				auto pixels_for_remove = p5->calculate_distribution(0, fake, fake2);
				p5->offset(offset, multiplier);
			}
			else if (pic.version == "P6")
			{	
				auto p6 = (p6_list*)pic.table_data;
				auto pixels_for_remove = p6->calculate_distribution(false, 0, fake, fake2);
				p6->offset(offset, multiplier, false);
			}
		}

		if (convertion == 1)
		{
			if (pic.version == "P5")
			{
				auto p5 = (p5_list*)pic.table_data;
				auto pixels_for_remove = p5->calculate_distribution(0, fake, fake2);
				p5->offset(offset, multiplier);
			}
			else if (pic.version == "P6")
			{
				auto p6 = (p6_list*)pic.table_data;

				p6->convert("RGB", "YCbCr.601");

				auto pixels_for_remove = p6->calculate_distribution(true, 0, fake, fake2);
				p6->offset(offset, multiplier, true);

				p6->convert("YCbCr.601", "RGB");
			}
		}

		if (convertion == 2)
		{
			if (pic.version == "P5")
			{
				auto p5 = (p5_list*)pic.table_data;
				auto pixels_for_remove = p5->calculate_distribution(0, offset, multiplier);
				p5->offset(offset, multiplier);
			}
			else if (pic.version == "P6")
			{
				auto p6 = (p6_list*)pic.table_data;
				auto pixels_for_remove = p6->calculate_distribution(false, 0, offset, multiplier);
				p6->offset(offset, multiplier, false);
			}
		}

		if (convertion == 3)
		{
			if (pic.version == "P5")
			{
				auto p5 = (p5_list*)pic.table_data;
				auto pixels_for_remove = p5->calculate_distribution(0, offset, multiplier);
				p5->offset(offset, multiplier);
			}
			else if (pic.version == "P6")
			{
				auto p6 = (p6_list*)pic.table_data;
				p6->convert("RGB", "YCbCr.601");

				auto pixels_for_remove = p6->calculate_distribution(true, 0, offset, multiplier);
				p6->offset(offset, multiplier, true);	

				p6->convert("YCbCr.601", "RGB");
			}
		}

		if (convertion == 4)
		{
			if (pic.version == "P5")
			{
				auto p5 = (p5_list*)pic.table_data;
				auto pixels_for_remove = p5->calculate_distribution(0.0039f, offset, multiplier);
				p5->offset(offset, multiplier);
			}
			else if (pic.version == "P6")
			{
				auto p6 = (p6_list*)pic.table_data;
				auto pixels_for_remove = p6->calculate_distribution(false, 0.0039f, offset, multiplier);
				p6->offset(offset, multiplier, false);
			}
		}

		if (convertion == 5)
		{
			if (pic.version == "P5")
			{
				auto p5 = (p5_list*)pic.table_data;
				auto pixels_for_remove = p5->calculate_distribution(0.0039f, offset, multiplier);
				p5->offset(offset, multiplier);
			}
			else if (pic.version == "P6")
			{
				auto p6 = (p6_list*)pic.table_data;
				p6->convert("RGB", "YCbCr.601");
				
				auto pixels_for_remove = p6->calculate_distribution(true, 0.0039f, offset, multiplier);
				p6->offset(offset, multiplier, true);

				p6->convert("YCbCr.601", "RGB");
			}
		}


		ofstream os(output_filename, ios::binary);

		if (!os.is_open())
			throw exception("can't open output file");

		bool is_p5_ouput = false;

		if (output_filename[output_filename.size() - 2] == 'g')
			is_p5_ouput = true;

		if (pic.version == "P5")
		{
			if (is_p5_ouput)
				pic.operator<<(os);
			else
			{
				pic.version = "P6";
				auto p5 = ((p5_list*)pic.table_data)->pixels;
				pic.table_data = new p6_list(p6_list::combine(p5, p5, p5));
				pic.operator<<(os);
			}
		}
		else if (pic.version == "P6")
		{
			if (is_p5_ouput)
			{
				pic.version = "P5";

				auto p6 = (p6_list*)pic.table_data;
				auto p5 = p6_list::split(p6->pixels)[0];

				pic.table_data = new p5_list(p5);

				pic.operator<<(os);
			}
			else
				pic.operator<<(os);
		}

		cout << offset << " " << multiplier;
	}
	catch (exception e)
	{
		std::cerr << e.what();
		return 1;
	}
}

