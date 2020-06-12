#include "pnm.h"
#include <iostream>
#include "p5_list.h"
#include "p6_list.h"

pnm::~pnm()
{
	delete table_data;
}

pnm::pnm(string version, int width, int height, int magic, IPixel_list* pixels)
{
	this->version = version;
	this->width = width;
	this->height = height;
	this->magic = magic;
	this->table_data = pixels;
}

pnm::pnm()
{
	table_data = new p5_list(0, 0);
}

void pnm::swap()
{
	std::swap(width, height);
	std::swap(table_data->height, table_data->width);
}

ostream& pnm::operator<<(ostream& os) const
{
	os << version << "\n" << width << " " << height << "\n" << magic << "\n";
	table_data->operator<<(os);
	return os;
}

istream& pnm::operator>>(istream& is)
{
	is >> version >> width >> height >> magic;

	if(version=="P5")
	{
		table_data = new p5_list(width, height);
	}
	else if (version=="P6")
	{
		table_data = new p6_list(width, height);
	}
	else
	{
		throw exception("invalid header");
	}

	if (magic != 255)
		throw exception("invalid magic value");
	
	table_data->operator>>(is);

	return is;
}