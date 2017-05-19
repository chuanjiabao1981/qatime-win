#include "UIVideochange.h"
#include <QPainter>
#include "UIWindowSet.h"

#define MAINWINDOW_X_MARGIN			0
#define MAINWINDOW_Y_MARGIN			0
#define MAINWINDOW_TITLE_HEIGHT		30
UIVideoChange::UIVideoChange(QWidget *parent)
	: QWidget(parent)
	, m_Parent(NULL)
	, iCount(0)
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

	ui.Video_comboBox->setStyleSheet(styleSheet);
	ui.Video_comboBox->setMinimumHeight(22);
	ui.Video_comboBox->setFont(font);
	connect(ui.Video_comboBox, SIGNAL(activated(int)), this, SLOT(AudioChanged(int)));

	ui.pushButton->setStyleSheet("border-image: url(./images/set_title.png);");
	ui.pushButton->installEventFilter(this);
}

UIVideoChange::~UIVideoChange()
{
	if (m_Parent)
	{
		m_Parent = NULL;
	}
}

void UIVideoChange::clickClose()
{
	hide();
}

void UIVideoChange::AudioChanged(int index)
{
	if (m_Parent)
		m_Parent->setAudioChangeIndex(index);
}

void UIVideoChange::setVideoChange(UIWindowSet* Parent)
{
	m_Parent = Parent;
}

void UIVideoChange::setVideoParam(QString name, QString path)
{
	ui.Video_comboBox->addItem(name, path);

	DevVideoInfo dev;
	dev.name = name;
	dev.path = path;
	dev.type = 3;
	mDeviceInfoMap.insert(path, dev);
}

const DeviceVideoInfoMap &UIVideoChange::getDeviceInfos()
{
	return mDeviceInfoMap;
}

void UIVideoChange::paintEvent(QPaintEvent *event)
{
	QPainterPath path;
	QPainter painter(this);

	QColor color(217, 217, 217);

	path.addRect(0, 0, this->width() - 1, this->height() - 1);
	painter.setPen(color);
	painter.drawPath(path);
}

// �϶��������Ĵ���
bool UIVideoChange::eventFilter(QObject *target, QEvent *event)
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

void UIVideoChange::focusOutEvent(QFocusEvent* e)
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