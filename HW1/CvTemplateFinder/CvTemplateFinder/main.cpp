#define _CRT_SECURE_NO_WARNINGS
#include "cvtemplatefinder.h"
int main (int argc, char *argv[]) {
	QTime t;
	QApplication a (argc, argv);
	QMovie *movie;
	movie = new QMovie ("loading.gif");
	QLabel *label = new QLabel ();
	label->setGeometry (300, 300, 200, 200);
	label->setMovie (movie);
	label->setScaledContents (true);
	label->setWindowFlags (Qt::FramelessWindowHint);
	movie->start ();
	label->show ();
	label->move ((QApplication::desktop ()->screenGeometry (0).width () - label->width ()) / 2,
		(QApplication::desktop ()->screenGeometry (0).height () - label->height ()) / 2);
	t.start ();
	while (t.elapsed () < 1000)
		QApplication::processEvents ();
	QFile styleSheet ("UIstyle.qss");
	if (!styleSheet.open (QIODevice::ReadOnly)) {
		qWarning ("Can't open the style sheet file.");
	}
	else {
		qApp->setStyleSheet (styleSheet.readAll ());
		styleSheet.close ();
	}
	CvTemplateFinder w;
	w.show ();
	w.move ((QApplication::desktop ()->screenGeometry (0).width () - w.width ()) / 2,
		(QApplication::desktop ()->screenGeometry (0).height () - w.height ()) / 2);
	label->close ();
	return a.exec ();
}