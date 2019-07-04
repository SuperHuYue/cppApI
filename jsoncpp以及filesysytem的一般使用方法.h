#include <iostream>
#include <filesystem>
#include <json/json.h>
#include <fstream>
using namespace  std;
namespace fs = std::filesystem;
int main() {
	Json::Value root,jout;
	Json::CharReaderBuilder rbuilder;
	std::unique_ptr<Json::CharReader> const reader(rbuilder.newCharReader());
	std::string errs;
	fs::path json_path = { fs::current_path().parent_path() / "json.txt" };
	std::string filename = json_path.string();
   //reader->parse(start, stop, &root, &errs);
	ofstream ostrm(filename, std::ios::binary);
	for(int i = 0; i < 2; ++i)
	{
		
		jout["DBID"] = Json::Value(i);
		jout["ID"] = Json::Value("huyue");
		jout["SCORE"] = Json::Value("100fen");
		jout["PICNAME"] = Json::Value("test");
		jout["PICURL"] = Json::Value("%$^$%^$1111");
		root["COMPARERESULT"].append(jout);
	}
	ostrm << root;
	return 0;
}
