#include "uimainwindow.h"

UIMainWindow::UIMainWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.mainmin_pushBtn, SIGNAL(clicked()), this, SLOT(MinDialog()));
	connect(ui.mainclose_pushBtn, SIGNAL(clicked()), this, SLOT(CloseDialog()));
}

UIMainWindow::~UIMainWindow()
{

}

void UIMainWindow::MinDialog()
{
	showMinimized();
}

void UIMainWindow::CloseDialog()
{
	close();
}