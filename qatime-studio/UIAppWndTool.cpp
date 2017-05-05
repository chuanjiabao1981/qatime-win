#include "UIAppWndTool.h"

UIAppWndTool::UIAppWndTool(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.shift_pushButton, SIGNAL(clicked()), this, SLOT(clickShift()));
	connect(ui.close_pushButton, SIGNAL(clicked()), this, SLOT(clickClose()));
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