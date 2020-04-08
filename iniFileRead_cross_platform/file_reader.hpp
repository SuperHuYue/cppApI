#include <utility>
#include <string>
#include <list>
#ifdef _MSC_VER
	#include <filesystem>
	using namespace std::filesystem;
#else
	#include <experimental/filesystem>
	using namespace std::experimental::filesystem;
#endif



constexpr int maxLineLen = 100;
class iniFileReader {
public:
	iniFileReader(path const& path);
	~iniFileReader();
	int get_int(std::string const& sec, std::string const& key);
	double get_float(std::string const& sec, std::string const& key);
	std::string get_string(std::string const& sec, std::string const& key);
	void read();
	void show();

private:
	struct data {
		std::string m_secName;
		std::list<std::pair <std::string, std::string>> m_dataPair;
	};
	std::list<data> m_phasedData;
	path const m_filePath;
	bool file_exist(path const& file_path, file_status s = file_status{});
	std::pair<bool, std::string> data_exist(std::string const& sec, std::string const& key);
};