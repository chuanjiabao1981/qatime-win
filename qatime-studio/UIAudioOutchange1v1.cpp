#include "UIAudioOutChange1v1.h"
#include <QPainter>

UIAudioOutChange1v1::UIAudioOutChange1v1(QWidget *parent)
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
	setSliderStyle();
}

UIAudioOutChange1v1::~UIAudioOutChange1v1()
{
	if (m_Parent)
	{
		m_Parent = NULL;
	}
}

void UIAudioOutChange1v1::setAudioChange(UIWindowSet* Parent)
{
	m_Parent = Parent;
}


void UIAudioOutChange1v1::setAudioParam(QString strName, QString path)
{
	ui.Audio_comboBox->addItem(strName, path);
}

void UIAudioOutChange1v1::AudioChanged(int index)
{
	QString path = GetCurPath();
	if (m_Parent)
		m_Parent->setAudioChange1v1(path);
}

void UIAudioOutChange1v1::paintEvent(QPaintEvent *event)
{
	QPainterPath path;
	QPainter painter(this);

	QColor color(217, 217, 217);

	path.addRect(0, 0, this->width() - 1, this->height() - 1);
	painter.setPen(color);
	painter.drawPath(path);
}

// 拖动标题做的处理
bool UIAudioOutChange1v1::eventFilter(QObject *target, QEvent *event)
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

void UIAudioOutChange1v1::clickClose()
{
	hide();
}

void UIAudioOutChange1v1::focusOutEvent(QFocusEvent* e)
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

QString UIAudioOutChange1v1::GetCurPath()
{
	QString path = ui.Audio_comboBox->currentData().toString();
	return path;
}

void UIAudioOutChange1v1::setSliderStyle()
{
	ui.audio1v1_label->setStyleSheet("border-image: url(./images/audioout_open.png);");
	ui.audio_horizontalSlider->setStyleSheet("QSlider::add - page:horizontal{background - color: rgb(87, 97, 106);height:4px;}"
			"QSlider::sub - page:horizontal{background - color:qlineargradient(spread : pad, x1 : 0, y1 : 0, x2 : 1, y2 : 0, stop : 0 rgba(231, 80, 229, 255), stop : 1 rgba(7, 208, 255, 255));height:4px;}"
			"QSlider::groove:horizontal{background:transparent;height:6px;}"
			"QSlider::handle:horizontal{height: 30px;width:8px;border - image: url(: / images / pos.png); margin: -8 0px;}"); 

	connect(ui.audio_horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));
}

void UIAudioOutChange1v1::valueChanged(int i)
{
	if (m_Parent)
		m_Parent->setValueChange1v1(i,false);
}