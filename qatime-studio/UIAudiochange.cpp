#include "UIAudiochange.h"
#include <QPainter>

UIAudioChange::UIAudioChange(QWidget *parent)
	: QWidget(parent)
	, m_Parent(NULL)
{
	ui.setupUi(this);
	setAutoFillBackground(true);
	QPalette p = palette();
	p.setColor(QPalette::Window, QColor(255, 255, 255));
	setPalette(p);

	ui.close_pushButton->setStyleSheet("border-image: url(./images/btn_off.png);");
	connect(ui.close_pushButton, SIGNAL(clicked()), this, SLOT(clickClose()));

	QFont font;
	font.setPointSize(10);
	font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));

	QFile file("styles/ComboBox.qss");
	file.open(QFile::ReadOnly);
	QString styleSheet = file.readAll();

	ui.Audio_comboBox->setStyleSheet(styleSheet);
	ui.Audio_comboBox->setMinimumHeight(22);
	ui.Audio_comboBox->setFont(font);
	connect(ui.Audio_comboBox, SIGNAL(activated(int)), this, SLOT(AudioChanged(int)));

	ui.pushButton->setStyleSheet("border-image: url(./images/set_title.png);");
	ui.pushButton->installEventFilter(this);
}

UIAudioChange::~UIAudioChange()
{
	if (m_Parent)
	{
		m_Parent = NULL;
	}
}

void UIAudioChange::setAudioChange(UIWindowSet* Parent)
{
	m_Parent = Parent;
}


void UIAudioChange::setAudioParam(QString strName, QString path)
{
	ui.Audio_comboBox->addItem(strName, path);
}

void UIAudioChange::AudioChanged(int index)
{
	if (m_Parent)
		m_Parent->setAudioChangeIndex(index);
}

void UIAudioChange::paintEvent(QPaintEvent *event)
{
	QPainterPath path;
	QPainter painter(this);

	QColor color(217, 217, 217);

	path.addRect(0, 0, this->width() - 1, this->height() - 1);
	painter.setPen(color);
	painter.drawPath(path);
}

// 拖动标题做的处理
bool UIAudioChange::eventFilter(QObject *target, QEvent *event)
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

void UIAudioChange::clickClose()
{
	hide();
}

void UIAudioChange::focusOutEvent(QFocusEvent* e)
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