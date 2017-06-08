#ifndef CVTEMPLATEFINDER_H
#define CVTEMPLATEFINDER_H

#include <QtWidgets/QMainWindow>
#include <string>
#include <sstream>
#include <fstream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <Eigen/Dense>
#include <QtWidgets/QApplication>
#include <cmath>
#include <queue>
#include <QDesktopWidget>
#include <QApplication>
#include <QGLWidget>
#include <qgl.h>
#include <GL/GLU.h>
#include <QTime>
#include <QFileDialog>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QFile>
#include <QProgressBar>
#include <QTextStream>
#include <QMovie>
class Painter :public QGLWidget {
	Q_OBJECT
public:
	float ans[32][32]={0};
	Painter (QWidget* parent = 0);
	~Painter ();
protected:
	void resizeGL (int w, int h)override;
	void initializeGL ()override;
	void paintGL () override;
};

class CvTemplateFinder : public QMainWindow {
	Q_OBJECT

public:
	CvTemplateFinder (QWidget *parent = 0);
	~CvTemplateFinder ();

private:
	int sum;
	int finish;
	//main function
	void go ();
	//core element
	cv::Mat mat_template;
	cv::Mat mat_query;
	cv::Mat mat_proc;
	//paint element
	Painter* paint_area;
	//file element
	QPushButton* button_template;
	QFileDialog* file_template;
	QPushButton* button_query;
	QFileDialog* file_query;
	QPushButton* button_output;
	QFileDialog* file_output;
	QLabel* label_template;
	QLabel* label_output;
	QLabel* label_query;
	QLineEdit* edit_template;
	QLineEdit* edit_query;
	QLineEdit* edit_output;
	QStringList list;
	QPushButton* GO;
	QPushButton* reset;
	QProgressBar* bar;
	QLabel* label;
	//core function
	int load (const std::string strq, const std::string strt);
	int proc ();
	int save (const QString &strO);
	double check (const int i, const int j);
	double size;

	//paint function
	//aux function
	static void int2string (std::string & str, const int & i) {
		std::stringstream temp;
		temp << i;
		std::string t;
		temp >> t;
		str += t;
	}
	static std::vector<std::string> splitString (std::string srcStr, std::string delimStr, bool repeatedCharIgnored) {
		std::vector<std::string> resultStringVector;
		std::replace_if (srcStr.begin (), srcStr.end (),
			[&](const char& c) {
			if (delimStr.find (c) != std::string::npos) { return true; }
			else { return false; }
		}, delimStr.at (0));
		size_t pos = srcStr.find (delimStr.at (0));
		std::string addedString = "";
		while (pos != std::string::npos) {
			addedString = srcStr.substr (0, pos);
			if (!addedString.empty () || !repeatedCharIgnored) {
				resultStringVector.push_back (addedString);
			}
			srcStr.erase (srcStr.begin (), srcStr.begin () + pos + 1);
			pos = srcStr.find (delimStr.at (0));
		}
		addedString = srcStr;
		if (!addedString.empty () || !repeatedCharIgnored) {
			resultStringVector.push_back (addedString);
		}
		return resultStringVector;
	}
signals:
	void get10 ();
	void get40 ();
};

#endif // CVTEMPLATEFINDER_H
