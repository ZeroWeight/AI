#include "cvtemplatefinder.h"
Painter::Painter (QWidget* parent)
	: QGLWidget (parent) {}

Painter::~Painter () {}

void Painter::paintGL () {
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity ();
	glTranslatef (-16.0, 16.0, -40.0);

	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 32; j++) {
			if (ans[i][j]) {
				glBegin (GL_QUADS);
				glVertex3f (0, 1.0, 0.0);
				glVertex3f (1.0, 1.0, 0.0);
				glVertex3f (1.0, 0, 0.0);
				glVertex3f (0, 0, 0.0);
				glEnd ();
			}
			glTranslatef (1, 0, 0);
		}
		glTranslatef (-32.0, -1, 0);
	}
}

void Painter::initializeGL () {
	glShadeModel (GL_SMOOTH);
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glClearDepth (1.0);
	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LEQUAL);
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}
void Painter::resizeGL (int w, int h) {
	glViewport (0, 0, GLint (width ()), GLint (height ()));
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (45.0, GLfloat (width ()) / GLfloat (height ()), 0.1, 100.0);
	glMatrixMode (GL_MODELVIEW);

	glLoadIdentity ();
}