#include "overdrawcount.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	OverDrawCount w;
	w.show();
	return a.exec();
}
