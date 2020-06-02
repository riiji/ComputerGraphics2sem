#include <math.h>
#pragma once

unsigned char to_srgb(unsigned char value)
{
	float srgb;
	if (value <= 0.0031308f) {
		srgb = value * 12.92f;
	}
	else {
		srgb = 1.055f * std::powf(value, 1.0f / 2.4f) - 0.055f;
	}
	return srgb * 255.f;
}

unsigned char from_srgb(unsigned char srgb) {
	auto linear = srgb / 255.0f;
	if (linear <= 0.04045f) {
		linear = linear / 12.92f;
	}
	else {
		linear = std::powf((linear + 0.055f) / 1.055f, 2.4f);
	}
	return linear;
}
