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
		if (argc != 11)
			throw exception("invalid argument count");

		string from_color_space = "";
		string to_color_space = "";
		int i_count = 0;
		int o_count = 0;
		string input_file = "";
		string output_file = "";


		for (int i = 1; i < argc - 1; ++i)
		{
			if (string(argv[i]) == "-f")
			{
				from_color_space = argv[i + 1];
			}

			if (string(argv[i]) == "-t")
			{
				to_color_space = argv[i + 1];
			}

			if (string(argv[i]) == "-i")
			{
				i_count = stoi(argv[i + 1]);

				if (i + 2 >= argc)
					throw exception("invalid args");

				input_file = argv[i + 2];
			}

			if (string(argv[i]) == "-o")
			{
				o_count = stoi(argv[i + 1]);

				if (i + 2 >= argc)
					throw exception("invalid args");

				output_file = argv[i + 2];
			}
		}

		if (from_color_space == "" || to_color_space == "" || i_count == 0 || o_count == 0 || input_file == "" || output_file == "")
			throw exception("invalid args");

		auto pic_i_1 = pnm();
		auto pic_i_2 = pnm();
		auto pic_i_3 = pnm();

		auto pic_o_1 = pnm();
		auto pic_o_2 = pnm();
		auto pic_o_3 = pnm();

		if (from_color_space != "RGB" && from_color_space != "HSL" && from_color_space != "HSV"
			&& from_color_space != "YCbCr.601" && from_color_space != "YCbCr.709"
			&& from_color_space != "YCoCg" && from_color_space != "CMY")
			throw exception("invalid from color space argument");

		if (to_color_space != "RGB" && to_color_space != "HSL" && to_color_space != "HSV"
			&& to_color_space != "YCbCr.601" && to_color_space != "YCbCr.709"
			&& to_color_space != "YCoCg" && to_color_space != "CMY")
			throw exception("invalid to color space argument");

		if (i_count != 1 && i_count != 3)
			throw exception("invalid input count argument");

		if (o_count != 1 && o_count != 3)
			throw exception("invalid output count argument");

		ifstream is1;
		ifstream is2;
		ifstream is3;

		ofstream os1;
		ofstream os2;
		ofstream os3;

		if (i_count == 1)
		{
			is1.open(input_file + ".ppm", ios::binary);

			if (!is1.is_open())
				throw exception("file not existed to open");

			pic_i_1.operator>>(is1);

			if (pic_i_1.version != "P6")
				throw exception("only p6 aviable");

			is1.close();
		}

		if (i_count == 3)
		{
			is1.open(input_file + "_1.pgm", ios::binary);
			is2.open(input_file + "_2.pgm", ios::binary);
			is3.open(input_file + "_3.pgm", ios::binary);

			if (!is1.is_open() || !is2.is_open() || !is3.is_open())
				throw exception("file not existed to open");

			pic_i_1.operator>>(is1);
			pic_i_2.operator>>(is2);
			pic_i_3.operator>>(is3);

			auto p6 = p6_list(p6_list::combine(((p5_list*)pic_i_1.table_data)->pixels, ((p5_list*)pic_i_2.table_data)->pixels, ((p5_list*)pic_i_3.table_data)->pixels));

			if (pic_i_1.version != "P5" || pic_i_2.version != "P5" || pic_i_3.version != "P5")
				throw exception("only p5 available");

			is1.close();
			is2.close();
			is3.close();

			pic_i_1.table_data = new p6_list(p6);
			pic_i_1.version = "P6";
		}

		p6_list* pix = (p6_list*)pic_i_1.table_data;

		pix->convert(from_color_space, to_color_space);

		pic_i_1.table_data = pix;

		if (o_count == 1)
		{
			os1.open(output_file + ".ppm", ios::binary);

			if (!os1.is_open())
				throw exception("can't open output file");

			pic_i_1.operator<<(os1);
		}

		if (o_count == 3)
		{
			os1.open(output_file + "_1.pgm", ios::binary);
			os2.open(output_file + "_2.pgm", ios::binary);
			os3.open(output_file + "_3.pgm", ios::binary);

			if (!os1.is_open() || !os2.is_open() || !os3.is_open())
				throw exception("can't open output file");

			auto splitted = (p6_list*)pic_i_1.table_data;
			auto vectors = p6_list::split(splitted->pixels);

			int he = vectors[0].size();
			int wi = vectors[0].front().size();

			pnm pic_o_1 = pnm("P5", wi, he, 255, new p5_list(vectors[0]));
			pnm pic_o_2 = pnm("P5", wi, he, 255, new p5_list(vectors[1]));
			pnm pic_o_3 = pnm("P5", wi, he, 255, new p5_list(vectors[2]));

			pic_o_1.operator<<(os1);
			pic_o_2.operator<<(os2);
			pic_o_3.operator<<(os3);

		}
	}
	catch (exception e)
	{
		std::cerr << e.what();
		return 1;
	}
}

