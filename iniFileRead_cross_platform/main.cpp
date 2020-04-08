#include <iostream>
#include "file_reader.hpp"
#include <iomanip>



int main(int arcv, char** arc) {
	try
	{
		std::cout << std::setprecision(10);
		auto test = iniFileReader(current_path().parent_path().append("ini_like.txt"));
		test.read();
		test.show();
		std::cout << "*************************search_test*******************" << std::endl;
		std::cout << "william-1111: "<<test.get_string("william", "1111") << std::endl;
		std::cout << "jack-json: " << test.get_float("jack", "json") << std::endl;
		std::cout << "fuckend-jjj: "<<test.get_int("fuck_end", "jjj") << std::endl;
		std::cout << test.get_int("fuck_end22", "jjj") << std::endl;
	}
	catch (...)
	{

	}

}