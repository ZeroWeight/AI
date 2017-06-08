#include "cvtemplatefinder.h"
int CvTemplateFinder::load (const std::string strq, const std::string strt) {
	std::ifstream inFile (strq, std::ios_base::in);
	if (!inFile.is_open ()) {
		qWarning ("file read fail");
		return -1;
	}
	std::istream_iterator<float> begin (inFile);
	std::istream_iterator<float> end;
	std::vector<float> inData (begin, end);
	cv::Mat (inData).reshape (1, 32).assignTo (mat_proc, CV_8UC1);
	cv::threshold (mat_proc, mat_query, 0.5, 255, CV_8UC1);
	std::ifstream inFileT (strt, std::ios_base::in);
	if (!inFileT.is_open ()) {
		qWarning ("file read fail");
		return -1;
	}
	std::istream_iterator<float> beginT (inFileT);
	std::istream_iterator<float> endT;
	std::vector<float> inDataT (beginT, endT);
	cv::Mat (inDataT).reshape (1, int (sqrt (inDataT.size ()))).assignTo (mat_proc, CV_8UC1);
	cv::threshold (mat_proc, mat_template, 0.5, 255, CV_8UC1);
	mat_template.assignTo (mat_proc, CV_8UC1);
	return 0;
}

int CvTemplateFinder::save (const QString& str) {
	QFile file (str);
	file.open (QIODevice::WriteOnly);
	QTextStream stream (&file);
	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 32; j++)
			stream << (paint_area->ans[i][j] > 0) << ' ';
		stream << '\n';
	}
	return 0;
}