#pragma once
#include <tuple>

template <class T>
std::tuple<T> operator+(std::tuple<T> const& l, std::tuple<T> r)
{
	int l_size = std::tuple_size<T>::value;
	int r_size = std::tuple_size<T>::value;

	if (l_size != r_size)
		throw std::exception("invalid tuples sizes");

	std::tuple<T> result(l_size);
	
	for (int i = 0; i < l_size; ++i)
	{
		auto temp = std::get<i>(l) + std::get<i>(r);
		result = make_tuple(temp);
	}

	return result;
}
