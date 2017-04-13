#include "UIRatio.h"
#include <QPainter>

UIRatio::UIRatio(QWidget *parent)
	: QWidget(parent)
	, m_Parent(NULL)
{
	ui.setupUi(this);

	ui.close_pushButton->setStyleSheet("border-image: url(./images/btn_off.png);");
	connect(ui.close_pushButton, SIGNAL(clicked()), this, SLOT(clickClose()));

	QFont font;
	font.setPointSize(10);
	font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));

	QFile file("styles/ComboBox.qss");
	file.open(QFile::ReadOnly);
	QString styleSheet = file.readAll();

	ui.Ratio_comboBox->setStyleSheet(styleSheet);
	ui.Ratio_comboBox->setMinimumHeight(22);
	ui.Ratio_comboBox->setFont(font);
	connect(ui.Ratio_comboBox, SIGNAL(activated(int)), this, SLOT(RatioChanged(int)));

	ui.pushButton->setStyleSheet("border-image: url(./images/set_title.png);");
	ui.pushButton->installEventFilter(this);

	ui.Ratio_comboBox->addItem("高清", "高清");
	ui.Ratio_comboBox->addItem("标清", "标清");
}

UIRatio::~UIRatio()
{
	if (m_Parent)
	{
		m_Parent = NULL;
	}
}

void UIRatio::setVideoChange(UIWindowSet* Parent)
{
	m_Parent = Parent;
}

void UIRatio::clickClose()
{
	hide();
}

void UIRatio::RatioChanged(int i)
{
	if (i == 0)
	{
		m_Parent->setRatioChangeIndex("高清");
	}
	else if (i == 1)
	{
		m_Parent->setRatioChangeIndex("标清");
	}
}

void UIRatio::paintEvent(QPaintEvent *event)
{
	QPainterPath path;
	QPainter painter(this);

	QColor color(217, 217, 217);

	path.addRect(0, 0, this->width() - 1, this->height() - 1);
	painter.setPen(color);
	painter.drawPath(path);
}

// 拖动标题做的处理
bool UIRatio::eventFilter(QObject *target, QEvent *event)
{
	if (target == ui.pushButton)
	{
		QMouseEvent* pMe = static_cast<QMouseEvent*>(event);
		if (event->type() == QEvent::MouseButtonPress)
		{
			m_startPos = pMe->globalPos();
			m_WndCurPos = this->pos();
		}
		else if (event->type() == QEvent::MouseMove)
		{
			m_clickPos = pMe->globalPos();
			this->move(m_WndCurPos + (m_clickPos - m_startPos));
		}
	}
	return false;
}

void UIRatio::focusOutEvent(QFocusEvent* e)
{
	QPoint pt = mapFromGlobal(QCursor::pos());
	pt.setX(pt.x() + geometry().x());
	pt.setY(pt.y() + geometry().y());
	QRect rc = this->geometry();
	if (rc.contains(pt))
	{
		setFocus();
		return;
	}

	this->hide();
}