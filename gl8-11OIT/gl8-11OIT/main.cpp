#include "oit.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	OIT w;
	w.show();
	return a.exec();
}
