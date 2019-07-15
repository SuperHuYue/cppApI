#include <iostream>
#include <string>
#include <array>
#include <stdexcept>
using namespace std;
template<typename T = int, int C = 100>
void split(string str, std::array<T, C> & out, const char label)
{
	//初始化
	size_t array_cur_index = 0;
	size_t index = 0;
	size_t index_count = 0;
	int len = str.length();
	for (auto& i : str)
	{

		if (i == label || len - 1 == index_count)
		{
			if (i == label)	i = '\0';
			if (std::is_same<T, float>::value)
			{
				out[array_cur_index++] = std::stof(str.data() + index);
			}
			else if (std::is_same<T, int>::value)
			{
				out[array_cur_index++] = std::stoi(str.data() + index);
			}
			else {
				throw std::runtime_error("wrong type....");
			}
			index = index_count + 1;
		}
		index_count++;
	}
}



template<int C = 100>
void split(string str, std::array<std::string, C> & out, const char label)
{
	//初始化
	size_t array_cur_index = 0;
	size_t index = 0;
	size_t index_count = 0;
	int len = str.length();
	for (auto& i : str)
	{

		if (i == label || len - 1 == index_count)
		{
			out[array_cur_index++] = str.substr(index, index_count - index);
			index = index_count + 1;
		}
		index_count++;
	}
}

int main() {

	string a = "12,3,444,5453 , 12334,  54363 ";
	array<std::string, 200> b{"0"};
	split(a, b, ',');
	for (auto i :b)
	{
		cout << " " << i << "";
	}
	cout << endl;
	return 0;
}




//plan B 运用stringstream库完成目的
// #include <sstream>
// #include <iostream>
// #include <vector>
// #include <array>
// using namespace  std;
// #define check ','
// int main() {
// 	string test = "12,3,444,5453 , 12334,  54363 ";
// 	vector<array<char, 10>> out;
// 	stringstream ss{test};
// 	for (array<char,10> inner{'0'}; ss.getline(&inner[0], 10, check);inner.fill('0'))
// 	{
// 		out.push_back(inner);
// 	}

// 	for (auto &itr : out)
// 	{
// 		cout << itr.data() << endl;
// 	}

// 	return 0;
// }

