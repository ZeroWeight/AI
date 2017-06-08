#include "cvtemplatefinder.h"
struct point {
	int x;
	int y;
};
int CvTemplateFinder::proc () {
	//basic search
	float tte[32][32] = { 0 };
	memcpy (paint_area->ans, tte, 32 * 32 * sizeof (float));
	int li, lj, lk;
	std::vector<std::vector<cv::Point> > contours;
	cv::findContours (mat_query.clone (), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	float st1mat[32][32] = { 0 };
	float core[32][32] = { 0 };
	for (std::vector<cv::Point> border : contours) {
		float temp[32][32] = { 0.0 };
		float ttemp[32][32];
		for (cv::Point point : border)
			temp[point.y][point.x] = 255;
		for (li = 0; li < 32; ++li)
			for (lj = 0; lj < 32; ++lj)
				if (temp[li][lj] == 0) {
					if (lj > 0 && lj < 31 && li > 0 && li < 31) {
						if (temp[li + 1][lj + 1] == 255 && temp[li - 1][lj - 1] == 255 && temp[li - 1][lj + 1] == 0 && temp[li + 1][lj - 1] == 0)
							temp[li][lj] = 255;
						if (temp[li - 1][lj + 1] == 255 && temp[li + 1][lj - 1] == 255 && temp[li + 1][lj + 1] == 0 && temp[li - 1][lj - 1] == 0)
							temp[li][lj] = 255;
						if (temp[li - 1][lj] == 255 && temp[li + 1][lj] == 255 && temp[li][lj + 1] == 0 && temp[li][lj - 1] == 0)
							temp[li][lj] = 255;
						if (temp[li - 1][lj] == 0 && temp[li + 1][lj] == 0 && temp[li][lj + 1] == 255 && temp[li][lj - 1] == 255)
							temp[li][lj] = 255;
					}
				}
		memcpy (ttemp, temp, 32 * 32 * sizeof (float));
		//connectivity check
		std::queue<point> Q;
		int count = 1;
		point head;
		Q.push (point ({ 0,0 }));
		temp[0][0] = 1;
		while (!Q.empty ()) {
			head = Q.front ();
			Q.pop ();
			if (head.x > 0) if (temp[head.x - 1][head.y] == 0) {
				temp[head.x - 1][head.y] = 1;
				Q.push (point ({ head.x - 1,head.y }));
				++count;
			}
			if (head.x < 31) if (temp[head.x + 1][head.y] == 0) {
				temp[head.x + 1][head.y] = 1;
				Q.push (point ({ head.x + 1,head.y }));
				++count;
			}
			if (head.y > 0) if (temp[head.x][head.y - 1] == 0) {
				temp[head.x][head.y - 1] = 1;
				Q.push (point ({ head.x,head.y - 1 }));
				++count;
			}
			if (head.y < 31) if (temp[head.x][head.y + 1] == 0) {
				temp[head.x][head.y + 1] = 1;
				Q.push (point ({ head.x,head.y + 1 }));
				++count;
			}
		}
		if (count + border.size () < 1000) {
#pragma  omp parallel for
			for (li = 0; li < 32; ++li)
				for (lj = 0; lj < 32; ++lj) {
					if (ttemp[li][lj])
						st1mat[li][lj] = 255;
					if (!temp[li][lj] || ttemp[li][lj])
						core[li][lj] = 255;
				}
		}
	}
	emit (get10 ());
	int count = 0;
	bool OK = true;
	while (OK) {
		OK = false;
		for (li = 0; li < 32; ++li)
			for (lj = 0; lj < 32; ++lj) {
				if (core[li][lj]) {
					count = 0;
					if (li > 0) {
						if (lj > 0) {
							count += (core[li - 1][lj - 1] > 0);
						}
						if (lj < 31) {
							count += (core[li - 1][lj + 1] > 0);
						}
						count += (core[li - 1][lj] > 0);
					}
					if (li < 31) {
						if (lj > 0) {
							count += (core[li + 1][lj - 1] > 0);
						}
						if (lj < 31) {
							count += (core[li + 1][lj + 1] > 0);
						}
						count += (core[li + 1][lj] > 0);
					}
					if (lj > 0) {
						count += (core[li][lj - 1] > 0);
					}
					if (lj < 31) {
						count += (core[li][lj + 1] > 0);
					}
					if (count <= 2) {
						OK = true;
						core[li][lj] = 0.0;
					}
				}
			}
	}
	emit (get10 ());
#pragma omp parallel for
	for (li = 0; li < 32; ++li) {
		for (lj = 0; lj < 32; ++lj)
			if (core[li][lj] && st1mat[li][lj])
				st1mat[li][lj] = 255;
			else st1mat[li][lj] = 0;
	}
	bool K = false;
	Eigen::Matrix3d C;
	C (0, 0) = C (0, 1) = C (0, 2) = C (1, 0) = C (1, 1) = C (1, 2) = -1;
	C (2, 0) = C (2, 1) = C (2, 2) = 1;
	for (li = 0; li < mat_template.cols; ++li)
		for (lj = 0; lj < mat_template.cols; ++lj)
			if (mat_template.ptr<uchar> (li)[lj]) {
				if (C (0, 0) < 0 || (C (0, 0) - C (1, 0)) < li - lj) {
					C (0, 0) = li;
					C (1, 0) = lj;
				}
				if (C (0, 1) < 0 || (C (0, 1) - C (1, 1)) > li - lj) {
					C (0, 1) = li;
					C (1, 1) = lj;
				}
				if (C (0, 2) < 0 || (C (1, 2) < lj&&abs (li - lj) > 5) || (C (1, 2) == lj&&C (0, 2) < li&&abs (li - lj) > 5)) {
					C (0, 2) = li;
					C (1, 2) = lj;
				}
			}
	count = 0;
	for (li = 0; li < 32; ++li)
		for (lj = 0; lj < 32; ++lj)
			if (st1mat[li][lj])
				++count;
	Eigen::MatrixXd matrix (3, count);
	for (li = 0; li < 32; ++li)
		for (lj = 0; lj < 32; ++lj)
			if (st1mat[li][lj]) {
				matrix (0, --count) = li;
				matrix (1, count) = lj;
				matrix (2, count) = 1;
			}
	count = 0;
	for (li = 0; li < mat_template.cols; ++li)
		for (lj = 0; lj < mat_template.cols; ++lj)
			if (mat_template.ptr<uchar> (li)[lj])
				++count;
	Eigen::MatrixXd Tmatrix (3, count);
	for (li = 0; li < mat_template.cols; ++li)
		for (lj = 0; lj < mat_template.cols; ++lj)
			if (mat_template.ptr<uchar> (li)[lj]) {
				Tmatrix (0, --count) = li;
				Tmatrix (1, count) = lj;
				Tmatrix (2, count) = 1;
			}
	Eigen::Matrix3d B;
	int cc;
	B (2, 0) = B (2, 1) = B (2, 2) = 1;
	emit (get10 ());
	for (li = 0; li < matrix.cols (); ++li)
		for (lj = 0; lj < matrix.cols (); ++lj)
			for (lk = 0; lk < matrix.cols (); ++lk) {
				if (li == lj || lj == lk || lk == li) continue;
				B (0, 0) = matrix (0, li);
				B (1, 0) = matrix (1, li);
				B (0, 1) = matrix (0, lj);
				B (1, 1) = matrix (1, lj);
				B (0, 2) = matrix (0, lk);
				B (1, 2) = matrix (1, lk);
				Eigen::MatrixXd X = B*(C.inverse ())*Tmatrix;
				count = 0;
				cc = 0;
				float t[32][32] = { 0 };
				float tt[32][32] = { 0 };
#pragma omp parallel for
				for (int lm = 0; lm < X.cols (); ++lm) {
					if (int (0.5 + (X (0, lm))) >= 0 && int (0.5 + (X (0, lm))) < 32 && int (0.5 + (X (1, lm))) >= 0 && int (0.5 + (X (1, lm))) < 32) {
						tt[int (0.5 + (X (0, lm)))][int (0.5 + (X (1, lm)))] = 255;
						if (mat_query.ptr<uchar> (int (0.5 + (X (0, lm))))[int (0.5 + (X (1, lm)))])
							t[int (0.5 + (X (0, lm)))][int (0.5 + (X (1, lm)))] = 255;
					}
				}
				for (int tli = 0; tli < 32; ++tli)
					for (int tlj = 0; tlj < 32; ++tlj) {
						if (tt[tli][tlj]) ++count;
						if (t[tli][tlj]) ++cc;
					}
				if (count - cc<5 && double (cc) / X.cols () > 0.7) {
					for (int tli = 0; tli < 32; ++tli)
						for (int tlj = 0; tlj < 32; ++tlj)
							if (t[tli][tlj]) {
								paint_area->ans[tli][tlj] = 255;
								K = true;
							}
				}
			}
	emit (get40 ());
	if (!K) {
		memcpy (paint_area->ans, st1mat, 32 * 32 * sizeof (float));
	}
	return 0;
}