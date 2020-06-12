#pragma once
#include <vector>
using namespace std;
template <class T, typename fun>
class helper final
{
public:
	static void from_color_to_rgb(vector<vector<T>>& obj, T(*fun)(T));
	static void from_rgb_to_color(vector<vector<T>>& obj, T(*fun)(T));
};

template<class T, typename fun>
inline void helper<T, fun>::from_color_to_rgb(vector<vector<T>>& obj, T(*fun)(T))
{
	int height = obj.size();
	int width = obj.front().size();
	
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			obj[i][j] = color(obj[i][j]);
		}
	}
}

template<class T, typename fun>
inline void helper<T, fun>::from_rgb_to_color(vector<vector<T>>& obj, T(*fun)(T))
{
	int height = obj.size();
	int width = obj.front().size();

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			obj[i][j] = color(obj[i][j]);
		}
	}
}