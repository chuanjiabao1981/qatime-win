#include "loginwindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QIcon qIcon("./images/logo.png");
	a.setWindowIcon(qIcon);

	LoginWindow w;
	w.setWindowFlags(Qt::FramelessWindowHint);

	w.setAutoFillBackground(true);
	QPalette palette;
	QPixmap pixmap("./images/bk.png");
	palette.setBrush(QPalette::Window, QBrush(pixmap));
	w.setPalette(palette);
	w.show();
	return a.exec();
}
