#include "file_reader.hpp"
#include <fstream>
#include <array>
#include <string.h>
#include <iostream>
#include <assert.h>

iniFileReader::iniFileReader(path const& file_path):
m_filePath(file_path)
{
	std::cout << "go!" << std::endl;
	std::cout << "Conducted file: " << file_path << std::endl;

}

iniFileReader::~iniFileReader()
{

}

bool iniFileReader::file_exist(path const& file_path, file_status s)
{
	if (status_known(s) ? exists(s) : exists(file_path)) {
		return true;
	}
	else
	{
		return false;
	}
}

void iniFileReader::read()
{
	if (file_exist(m_filePath)) {
		std::ifstream ifstrm(m_filePath, std::ios::binary);
		if (!ifstrm.is_open()) {
			throw("fail to open...");
		}
		else {
			//开始读取文件 
			std::array<char, maxLineLen>single_line;
			std::array<char, maxLineLen>phase_cache;
			data sec_data;
			std::pair<std::string, std::string> data_pair;
			single_line.fill('\n');
			int phase_count = 0;
			while (ifstrm.getline(&single_line[0], maxLineLen, '\n'))
			{
				int line_len = strlen(&single_line[0]) + 1;
				//if (line_len < 3)continue;
				phase_count = 0;
				for (int pos = 0; pos < line_len; ++pos)
				{
					if(single_line[pos] == ' ' || single_line[pos] == '\r' || single_line[pos] == '\n')continue;
					//关于【】的处理
					if (single_line[pos] == '[') {	
						if (!sec_data.m_secName.empty() && !data_pair.first.empty() && !data_pair.second.empty())
						{
							m_phasedData.push_back(sec_data);
						}
						sec_data.m_secName.clear();
						sec_data.m_dataPair.clear();
						data_pair.first.clear();
						data_pair.second.clear();
						pos++;
						for (; pos < line_len; ++pos)
						{
							if (single_line[pos] == ' ' || single_line[pos] == '\r' || single_line[pos] == '\n')continue;
							if (single_line[pos] == ']')
							{
								sec_data.m_secName = std::string(phase_cache.begin(), phase_cache.begin() + phase_count);
								phase_count = 0;
								break;
							}
							else {
								phase_cache[phase_count++] = single_line[pos];
							}
						}
					}
					else {
						if (sec_data.m_secName.empty())throw("error: not a right datastruct...");
						if (single_line[pos] == '=')
						{
							pos++;
							data_pair.first = std::string(phase_cache.begin(), phase_cache.begin()+phase_count);
							phase_count = 0;
							for (; pos < line_len; ++pos) {
								if (single_line[pos] == ' ' || single_line[pos] == '\r' || single_line[pos] == '\n')continue;
								if (single_line[pos] == '\0' ) {
									if (phase_count == 0)continue;
									data_pair.second = std::string(phase_cache.begin(), phase_cache.begin() + phase_count);
									sec_data.m_dataPair.push_back(data_pair);
								}
								else
								{
									phase_cache[phase_count++] = single_line[pos];
								}
							}

						}
						else
						{
							if(single_line[pos] == '\r')throw("error: not a right datastruct...");
							phase_cache[phase_count++] = single_line[pos];
						}
					}

				}
			//	assert(phase_count == 0);
				single_line.fill('\n');
				
			}
			if (!sec_data.m_secName.empty() && !data_pair.first.empty() && !data_pair.second.empty())
			{
				m_phasedData.push_back(sec_data);
			}
		}

	}
	else {
		throw ("No such an target...");
	}
	std::cout << "mission out..." << std::endl;
}



void iniFileReader::show() {
	for (auto& i : m_phasedData)
	{
		std::cout << std::endl<< "Section name: " << i.m_secName << std::endl;
		for (auto& j : i.m_dataPair) {
			std::cout << j.first<< ": "<< j.second << std::endl;
		}

	}
}


int iniFileReader::get_int(std::string const& sec, std::string const& key) {
	auto data = data_exist(sec, key);
	if (data.first)
	{
		return atoi(data.second.c_str());
	}
	else {
		throw("Don't have such data...");
	}
}
std::string iniFileReader::get_string(std::string const& sec, std::string const& key) {
	auto data = data_exist(sec, key);
	if (data.first)
	{
		return data.second;
	}
	else {
		throw("Don't have such data...");
	}
}

double iniFileReader::get_float(std::string const& sec, std::string const& key) {
	auto data = data_exist(sec, key);
	if (data.first)
	{
		return atof(data.second.c_str());
	}
	else {
		throw("Don't have such data...");
	}
}

std::pair<bool,std::string> iniFileReader::data_exist(std::string const& sec, std::string const& key) {
	for(auto& memsec : m_phasedData)
	{
		if (memsec.m_secName == sec) {
			for(auto& mempair :memsec.m_dataPair)
			{
				if (mempair.first == key) {
					return std::make_pair(true, mempair.second);
				}
			}
		}
	}
	return std::make_pair(false, "");
}








