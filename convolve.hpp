#include <iostream>
#include <list>
#include <algorithm>

//�������һά����ľ�������
template<size_t data_size, size_t kernel_size>
class convolve_master {
public:
	convolve_master(std::array<double, data_size> data, std::array<double, kernel_size> kernel, std::string type = "full");
	~convolve_master() {};
	std::list<double> run();
private:
	void check_valid();//����쳣�������������������׳��쳣,����������������������ò���
	 std::array<double, data_size> m_arrData;
	 std::array<double, kernel_size> m_arrKernel;
   	 std::string m_strType;
////////////////////////////////////////
	int m_nOutsize;
////////////////////////////////////////
};


template<size_t data_size, size_t kernel_size>
convolve_master<data_size, kernel_size>::convolve_master(std::array<double, data_size> data, std::array<double, kernel_size> kernel, std::string type):
m_arrData(data),m_strType(type){
	//based on the convolve math the kernel need filps first
	for (int i = 0; i < kernel_size; ++i) {
		m_arrKernel[i] = kernel[kernel_size - i - 1];
	}

}

template<size_t data_size, size_t kernel_size>
void convolve_master<data_size, kernel_size>::check_valid() {
	if (data_size == 0 || kernel_size == 0 || kernel_size > data_size || kernel_size % 2 == 0) {
		throw std::invalid_argument("invalid para, please check..");
	}
}

template<size_t data_size, size_t kernel_size>
std::list<double> convolve_master<data_size, kernel_size>::run() {
	check_valid();
	std::list<double> out;
	if (m_strType == "full") {
		std::array<double, data_size + kernel_size - 1> tmp_out_arr{0};
		constexpr size_t one_side_dilate = kernel_size - 1;
		std::array<double, data_size + one_side_dilate * 2> back{ 0 };
		for (int i = one_side_dilate; i < back.size() - one_side_dilate; ++i) {
			back[i] = m_arrData[i - one_side_dilate];
		}
		for (int i = 0; i < tmp_out_arr.size(); ++i) {
			int tmp_data = 0;
			for (int j = 0; j < kernel_size; ++j) {
				tmp_data += m_arrKernel[j] * back[size_t(i + j)];
			}
			tmp_out_arr[i] = tmp_data;
			out.push_back(tmp_out_arr[i]);
		}
	}
	if (m_strType == "same") {
		std::array<double, std::max(data_size, kernel_size)>tmp_out_arr{ 0 };
		constexpr size_t one_side_dilate = (kernel_size -1) / 2;
		std::array<double, data_size + one_side_dilate * 2> back{ 0 };
		for (int i = one_side_dilate; i < back.size() - one_side_dilate; ++i) {
			back[i] = m_arrData[i - one_side_dilate];
		}

		for (int i = 0; i < tmp_out_arr.size(); ++i) {
			double tmp_data = 0;
			for (int j = 0; j < kernel_size; ++j) {
				tmp_data += back[i + j] * m_arrKernel[j];
			}
			tmp_out_arr[i] = tmp_data;
			out.push_back(tmp_out_arr[i]);
		}
	}
	if (m_strType == "valid") {
		//std::array<double, xx> tmp_arr{0};
		std::array<double, std::max(data_size, kernel_size) - std::min(data_size, kernel_size) + 1>tmp_out_arr{ 0 };
		for (int i = 0; i < tmp_out_arr.size(); ++i) {
			double tmp_data = 0;
			for (int j = 0; j < kernel_size; ++j) {
				tmp_data += m_arrData[i + j] * m_arrKernel[j];
			}
			tmp_out_arr[i] = tmp_data;
			out.push_back(tmp_out_arr[i]);
		}

	}
	return out;
}

