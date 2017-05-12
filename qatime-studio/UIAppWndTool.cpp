#include "UIAppWndTool.h"

UIAppWndTool::UIAppWndTool(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.shift_pushButton, SIGNAL(clicked()), this, SLOT(clickShift()));
	connect(ui.close_pushButton, SIGNAL(clicked()), this, SLOT(clickClose()));

	ui.widget->setStyleSheet("border-image:url(./images/whiteBoardBack.png)");
	ui.shift_pushButton->setStyleSheet("QPushButton{border-image:url(./images/appShift_nor.png);}"
		"QPushButton:hover{border-image:url(./images/appShift_hor.png);}"
		"QPushButton:pressed{border-image:url(./images/appShift_hor.png);}");
	ui.close_pushButton->setStyleSheet("QPushButton{border-image:url(./images/appClose_nor.png);}"
		"QPushButton:hover{border-image:url(./images/appClose_hor.png);}"
		"QPushButton:pressed{border-image:url(./images/appClose_hor.png);}");
}

UIAppWndTool::~UIAppWndTool()
{

}

void UIAppWndTool::clickShift()
{
	emit sig_shiftWnd();
}

void UIAppWndTool::clickClose()
{
	emit sig_CloseWnd();
}