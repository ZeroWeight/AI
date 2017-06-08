#include "cvtemplatefinder.h"

double CvTemplateFinder::check (const int i, const int j) {
	std::string strA = "../data/gt_";
	int2string (strA, i);
	strA += '_';
	int2string (strA, j);
	strA += ".txt";
	cv::Mat asw;
	std::ifstream inFile (strA, std::ios_base::in);
	if (!inFile.is_open ()) {
		qWarning ("file read fail");
		return -1;
	}
	std::istream_iterator<float> begin (inFile);
	std::istream_iterator<float> end;
	std::vector<float> inData (begin, end);
	cv::Mat (inData).reshape (1, 32).assignTo (mat_proc, CV_8UC1);
	cv::threshold (mat_proc, asw, 0.5, 255, CV_8UC1);
	double f1 = 0, f2 = 0, f3 = 0;
	for (int li = 0; li < 32; li++) {
		for (int lj = 0; lj < 32; lj++) {
			if (paint_area->ans[li][lj]) ++f1;
			if (asw.ptr<uchar> (li)[lj]) ++f2;
			if (paint_area->ans[li][lj] && asw.ptr<uchar> (li)[lj]) ++f3;
		}
	}
	std::cout << f1 << '\t' << f2 << '\t' << f3 << '\t' << 2 * f3 / f1 * f3 / f2 / (f3 / f1 + f3 / f2) << std::endl;
	return 2 * f3 / f1 * f3 / f2 / (f3 / f1 + f3 / f2);
}