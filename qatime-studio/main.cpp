#include "loginwindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QIcon qIcon("./images/favicon.png");
	a.setWindowIcon(qIcon);

	LoginWindow w;
	w.setWindowFlags(Qt::FramelessWindowHint);
	w.ReadSetting();

	if (w.IsAutoLogin())
		w.hide();
	else
		w.show();
	return a.exec();
}
