#pragma once
#include <iostream>
#include "p6_list.h"
#include <math.h>

template <class T>
class p5_color_conversion 
{
public:
	static T rgb_to_hsl(T value);
	static T hsl_to_rgb(T value);

	static T rgb_to_hsv(T value);
	static T hsv_to_rgb(T value);

	static T rgb_to_ycbcr601(T value);
	static T ycbcr601_to_rgb(T value);
	
	static T rgb_to_ycbcr709(T value);
	static T ycbcr709_to_rgb(T value);
	
	static T rgb_to_ycocg(T value);
	static T ycocg_to_rgb(T value);

	static T rgb_to_cmy(T value);
	static T cmy_to_rgb(T value);

};

template<class T>
inline T p5_color_conversion<T>::rgb_to_hsl(T value)
{
	return round(value / 255.0);
}

template<class T>
inline T p5_color_conversion<T>::hsl_to_rgb(T value)
{
	return value * 255.0;
}

template<class T>
inline T p5_color_conversion<T>::rgb_to_hsv(T value)
{
	return round(value / 255.0);
}

template<class T>
inline T p5_color_conversion<T>::hsv_to_rgb(T value)
{
	return value * 255.0;
}

template<class T>
inline T p5_color_conversion<T>::rgb_to_cmy(T value)
{
	return value / 255.0 * 100.0;
}

template<class T>
inline T p5_color_conversion<T>::cmy_to_rgb(T value)
{
	return value * 100.0 / 255.0;
}
