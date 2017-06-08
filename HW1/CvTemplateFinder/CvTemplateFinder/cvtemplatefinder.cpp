#include "cvtemplatefinder.h"
static const std::string awkard = "/";
CvTemplateFinder::CvTemplateFinder (QWidget *parent)
	: QMainWindow (parent) {
	size = QApplication::desktop ()->height () / 50;
	this->setFixedHeight (size * 22.5);
	this->setFixedWidth (size * 40);
	paint_area = new Painter (this);
	paint_area->setGeometry (22 * size, 2 * size, 16 * size, 16 * size);
	file_template = new QFileDialog (this, "files for template", ".", "*.txt");
	button_template = new QPushButton (this);
	button_template->setText ("...");
	file_query = new QFileDialog (this, "files for query", ".", "*.txt");
	file_query->setFileMode (QFileDialog::ExistingFiles);
	button_query = new QPushButton (this);
	button_query->setText ("...");
	file_output = new QFileDialog (this, "output directory", ".");
	file_output->setFileMode (QFileDialog::Directory);
	file_output->setOption (QFileDialog::ShowDirsOnly);
	file_output->setViewMode (QFileDialog::List);
	file_template->setViewMode (QFileDialog::List);
	file_query->setViewMode (QFileDialog::List);
	button_output = new QPushButton (this);
	button_output->setText ("...");
	label_template = new QLabel (this);
	label_output = new QLabel (this);
	label_query = new QLabel (this);
	label_template->setText ("template\r\nfile");
	label_output->setText ("output\r\ndirectory");
	label_query->setText ("query\r\nfiles");
	edit_template = new QLineEdit (this);
	edit_query = new QLineEdit (this);
	edit_output = new QLineEdit (this);
	edit_query->setGeometry (8 * size, 2 * size, 9 * size, 2 * size);
	edit_template->setGeometry (8 * size, 6 * size, 9 * size, 2 * size);
	edit_output->setGeometry (8 * size, 10 * size, 9 * size, 2 * size);
	label_query->setGeometry (2 * size, 1 * size, 6 * size, 4 * size);
	label_template->setGeometry (2 * size, 5 * size, 6 * size, 4 * size);
	label_output->setGeometry (2 * size, 9 * size, 6 * size, 4 * size);
	button_query->setGeometry (18 * size, 2 * size, 2 * size, 2 * size);
	button_template->setGeometry (18 * size, 6 * size, 2 * size, 2 * size);
	button_output->setGeometry (18 * size, 10 * size, 2 * size, 2 * size);
	connect (file_template, &QFileDialog::fileSelected, edit_template, &QLineEdit::setText);
	connect (button_template, &QPushButton::clicked, file_template, &QFileDialog::show);
	connect (file_query, &QFileDialog::filesSelected, [=](const QStringList &selected) {
		list = selected;
		QString temp;
		temp.clear ();
		foreach (QString s, selected) {
			temp += s;
			temp += ',';
		}
		edit_query->setText (temp);
	});
	connect (file_output, &QFileDialog::fileSelected, edit_output, &QLineEdit::setText);
	connect (button_query, &QPushButton::clicked, file_query, &QFileDialog::show);
	connect (button_output, &QPushButton::clicked, file_output, &QFileDialog::show);
	reset = new QPushButton (this);
	reset->setText ("reset");
	reset->setGeometry (12 * size, 14 * size, 6 * size, 4 * size);
	connect (reset, &QPushButton::clicked, [=](void) {
		list.clear ();
		edit_query->clear ();
		edit_template->clear ();
		edit_output->clear ();
		edit_query->setEnabled (true);
		edit_template->setEnabled (true);
		edit_output->setEnabled (true);
		button_query->setEnabled (true);
		button_template->setEnabled (true);
		button_output->setEnabled (true);
	});
	GO = new QPushButton (this);
	GO->setText ("GO!");
	GO->setGeometry (4 * size, 14 * size, 6 * size, 4 * size);
	connect (GO, &QPushButton::clicked, [=](void) {
		edit_query->setEnabled (false);
		edit_template->setEnabled (false);
		edit_output->setEnabled (false);
		button_query->setEnabled (false);
		button_template->setEnabled (false);
		button_output->setEnabled (false);
		reset->setEnabled (false);
		GO->setEnabled (false);
		go ();
	});
	bar = new QProgressBar (this);
	bar->setMaximum (1000);
	bar->setMinimum (0);
	bar->setGeometry (8 * size, 19.5 * size, 30 * size, 2 * size);
	bar->setValue (0);
	connect (this, &CvTemplateFinder::get10, [=](void) {
		bar->setValue (bar->value () + 1000 / sum*0.1);
	});
	connect (this, &CvTemplateFinder::get40, [=](void) {
		bar->setValue (bar->value () + 1000 / sum*0.4);
	});
	label = new QLabel (this);
	label->setGeometry (2 * size, 19.5 * size, 5 * size, 2 * size);
	label->setText ("Ready");
	button_template->setObjectName ("button_template");
	file_template->setObjectName ("file_template");
	button_query->setObjectName ("button_query");
	file_query->setObjectName ("file_query");
	button_output->setObjectName ("button_output");
	file_output->setObjectName ("file_output");
	label_template->setObjectName ("label_template");
	label_output->setObjectName ("label_output");
	label_query->setObjectName ("label_query");
	edit_template->setObjectName ("edit_template");
	edit_query->setObjectName ("edit_query");
	edit_output->setObjectName ("edit_output");
	GO->setObjectName ("GO");
	reset->setObjectName ("reset");
	bar->setObjectName ("bar");
	label->setObjectName ("label");
}

CvTemplateFinder::~CvTemplateFinder () {}

void CvTemplateFinder::go () {
	label->setText ("Processing");
	bar->setValue (0);
	sum = list.size ();
	finish = 0;
	foreach (QString name, list) {
		load (name.toStdString (), edit_template->text ().toStdString ());
		bar->setValue (bar->value () + 1000 / sum*0.1);
		proc ();
		paint_area->updateGL ();
		if (edit_output->text ().size ()) {
			std::string _name = *(splitString (name.toStdString (), awkard, true).end () - 1);
			_name[0] = 'u';
			_name[1] = 't';
			_name[2] = 'p';
			_name[3] = 'u';
			_name[4] = 't';
			save (edit_output->text () +
				QString (awkard.data ()) + "o" +
				QString (_name.data ()));
		}
		bar->setValue (bar->value () + 1000 / sum*0.1);
		QTime t;
		t.start ();
		while (t.elapsed () < 1000)
			QCoreApplication::processEvents ();
		++finish;
		bar->setValue (1000 * finish / sum);
	}
	edit_query->setEnabled (true);
	edit_template->setEnabled (true);
	edit_output->setEnabled (true);
	button_query->setEnabled (true);
	button_template->setEnabled (true);
	button_output->setEnabled (true);
	GO->setEnabled (true);
	reset->setEnabled (true);
	bar->setValue (1000);
	label->setText ("Ready");
}