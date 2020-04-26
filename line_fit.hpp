#include <iostream>
#include <string>
#include <list>
#include <array>
#include <random>
#include <assert.h>

//only support two dimension
template<int size =1000>
class LineFitter {
public:
	LineFitter();
	~LineFitter();
	std::pair<double, double> ransac_fit(std::array<double,size>x, std::array<double,size>y, int max_iteration = 100, int const& cal_size = -1,float inlierdist= 30, int const& count=50); 
private:
	std::array<double, size> m_arrXpos;
	std::array<double,size> m_arrYpos;
	int m_iMaxIteration;
	float m_fInlierDist;
	int m_iTrustedCount;
	std::pair<double, double > _ransac_run(int const& cal_size);
};


template<int size>
LineFitter<size>::LineFitter() {
}

template<int size>
LineFitter<size>::~LineFitter() {

}

template<int size>
std::pair<double, double> LineFitter<size>::ransac_fit(std::array<double,size>x, std::array<double, size>y, int max_iteration, int const& cal_size,float inlierdist, int const& count) {
	assert(x.size() == y.size());
	m_arrXpos = x;
	m_arrYpos = y;
	m_iMaxIteration = max_iteration;
	m_iTrustedCount = count;
	m_fInlierDist = inlierdist;
	///////////////////////////////////////////////
	//test para valid?
	return _ransac_run(cal_size);
}
template<int size>
std::pair<double, double> LineFitter<size>::_ransac_run(int const& cal_size) {
	struct point2d
	{
		point2d(double const& a, double const& b) :x(a), y(b) {}
		double x;
		double y;
	};
	double now_best_fit = 0;
	double now_best_k = 0;
	double now_best_b = 0;
	std::random_device rd;
	std::default_random_engine dre(rd());
	int length = m_arrXpos.size();
	if (cal_size > 0) length = cal_size;
	std::uniform_int_distribution<int>uniform_range(0, length - 1);
	std::cout << "total_length: " << length << std::endl;
	for (int i = 0; i < m_iMaxIteration; ++i) {
		int fir_idx = uniform_range(dre);
		int sec_idx = uniform_range(dre);
		point2d fir_point(m_arrXpos[fir_idx], m_arrYpos[fir_idx]);
		point2d sec_point(m_arrXpos[sec_idx], m_arrYpos[sec_idx]);
		if (fabs(fir_point.x - sec_point.x) < 1e-6)continue;
	    double k = (sec_point.y - fir_point.y) / (sec_point.x - fir_point.x);
		double b = sec_point.y - k * sec_point.x;
		//estimate
		int good_fit = 0;
		for (int j = 0; j < length; ++j) {
			point2d predict(m_arrXpos[j], m_arrXpos[j] * k + b);
			double dist = sqrt(powf(predict.x - m_arrXpos[j], 2) + pow(predict.y - m_arrYpos[j], 2));
			if (dist < m_fInlierDist) {
				good_fit++;
				if (good_fit > now_best_fit) {
					now_best_fit = good_fit;
					now_best_k = k;
					now_best_b = b;
				}
				if (good_fit >= m_iTrustedCount) {
					return std::make_pair(now_best_k, now_best_b);
				}

			}

		}
		
	}
	return std::make_pair(now_best_k, now_best_b);
}
