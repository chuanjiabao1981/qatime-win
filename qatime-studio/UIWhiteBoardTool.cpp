#include "UIWhiteBoardTool.h"
#include <QPainter>

#define  COLOR_VALUE_BLACK QColor(0, 0, 0)
#define  COLOR_VALUE_RED QColor(204, 0, 0)
#define  COLOR_VALUE_YELLOW QColor(255, 204, 0)
#define  COLOR_VALUE_BLUE QColor(0, 153, 255)
#define  COLOR_VALUE_GREEN QColor(0, 153, 0)
#define  COLOR_VALUE_PURPLE QColor(255, 51, 255)

UIWhiteBoardTool::UIWhiteBoardTool(QWidget *parent)
	: QWidget(parent)
	, mColorPicker(NULL)
{
	ui.setupUi(this);
	ui.widget->setStyleSheet("border-image:url(./images/whiteBoardBack.png)");

	ui.return_pushButton->setStyleSheet("QPushButton{border-image:url(./images/return_nor.png);}"
										"QPushButton:hover{border-image:url(./images/return_hor.png);}"
										"QPushButton:pressed{border-image:url(./images/return_hor.png);}");
	ui.delete_pushButton->setStyleSheet("QPushButton{border-image:url(./images/delete_nor.png);}"
										"QPushButton:hover{border-image:url(./images/delete_hor.png);}"
										"QPushButton:pressed{border-image:url(./images/delete_hor.png);}");
	ui.color_pushButton->setStyleSheet("QPushButton{border-image:url(./images/color_m0_nor.png);}"
										"QPushButton:hover{border-image:url(./images/color_m0_hor.png);}"
										"QPushButton:pressed{border-image:url(./images/color_m0_hor.png);}");

	connect(ui.color_pushButton, SIGNAL(clicked()), this, SLOT(colorClicked()));
	connect(ui.return_pushButton, SIGNAL(clicked()), this, SLOT(returnClicked()));
	connect(ui.delete_pushButton, SIGNAL(clicked()), this, SLOT(deleteClicked()));
	connect(ui.laser_radioButton, SIGNAL(clicked()), this, SLOT(laserClicked()));
	connect(ui.draw_radioButton, SIGNAL(clicked()), this, SLOT(drawClicked()));

	initColorPicker();
}

UIWhiteBoardTool::~UIWhiteBoardTool()
{

}

void UIWhiteBoardTool::initColorPicker()
{
	mColorPicker = new ColorPicker();
	connect(mColorPicker, SIGNAL(selectColor(int)), this, SLOT(setPenColor(int)));
}

void UIWhiteBoardTool::colorClicked()
{
	if (mColorPicker)
	{
		mColorPicker->show();

		QPoint point = QCursor::pos();
		int height = mColorPicker->height();
		mColorPicker->move(point.x(), point.y() - height);
		mColorPicker->setFocus();
	}
}

void UIWhiteBoardTool::setPenColor(int color)
{
	switch (color)
	{
	case ColorPicker::color_black:
		emit selectColor(COLOR_VALUE_BLACK);
		break;
	case ColorPicker::color_red:
		emit selectColor(COLOR_VALUE_RED);
		break;
	case ColorPicker::color_yellow:
		emit selectColor(COLOR_VALUE_YELLOW);
		break;
	case ColorPicker::color_green:
		emit selectColor(COLOR_VALUE_GREEN);
		break;
	case ColorPicker::color_blue:
		emit selectColor(COLOR_VALUE_BLUE);
		break;
	case ColorPicker::color_purple:
		emit selectColor(COLOR_VALUE_PURPLE);
		break;
	default:
		break;
	}

	QString style = "QPushButton{border-image:url(./images/color_m{id}_nor.png);}"
		"QPushButton:hover{border-image:url(./images/color_m{id}_hor.png);}"
		"QPushButton:pressed{border-image:url(./images/color_m{id}_hor.png);}";
	
	style.replace("{id}", QString::number(color));
	ui.color_pushButton->setStyleSheet(style);
}

void UIWhiteBoardTool::returnClicked()
{
	emit returnClick();
}

void UIWhiteBoardTool::deleteClicked()
{
	emit deleteClick();
}

void UIWhiteBoardTool::laserClicked()
{
	emit laserClick();
}

void UIWhiteBoardTool::drawClicked()
{
	emit drawClick();
}

void UIWhiteBoardTool::paintEvent(QPaintEvent *event)
{
	QPainterPath path;
	QPainter painter(this);

	QColor color(255, 255, 255);
	path.addRect(0, 0, this->width()-1, this->height());

	painter.setPen(color);
	painter.drawPath(path);
}