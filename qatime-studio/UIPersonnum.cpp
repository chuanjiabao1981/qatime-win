#include "UIPersonnum.h"

UIPersonNum::UIPersonNum(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

UIPersonNum::~UIPersonNum()
{

}

void UIPersonNum::AddPersonInfo(QPixmap pix, QString name, QString id, Qt::CheckState state)
{
	m_id = id;

	QFont font;
	font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
	font.setPixelSize(13);

	ui.pic_label->setPixmap(pix);
	ui.name_label->setFont(font);
	ui.name_label->setText(name);

	// 加省略号
	int fontSize = ui.name_label->fontMetrics().width(name);//获取之前设置的字符串的像素大小
	if (fontSize >= ui.name_label->width()) //与label自身相比较
	{
		QString  tmpStr = ui.name_label->fontMetrics().elidedText(name, Qt::ElideRight, ui.name_label->width());
		ui.name_label->setText(tmpStr);
		ui.name_label->setToolTip(name);
	}

	ui.checkBox->setCheckState(state);
	connect(ui.checkBox, SIGNAL(stateChanged(int)), this, SLOT(stateChanged(int)));
}

void UIPersonNum::stateChanged(int i)
{

}