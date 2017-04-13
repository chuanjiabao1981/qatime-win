#include "UIBulletparam.h"
#include <QFile>
#include <QPainter>

UIBulletParam::UIBulletParam(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	QFont font;
	font.setPixelSize(12);
	font.setFamily(("微软雅黑"));

	QFile file("styles/ComboBox.qss");
	file.open(QFile::ReadOnly);
	QString styleSheet = file.readAll();

	ui.delay_comboBox->setStyleSheet(styleSheet);
	ui.delay_comboBox->setMinimumHeight(22);
	ui.delay_comboBox->setFont(font);
	connect(ui.delay_comboBox, SIGNAL(activated(int)), this, SLOT(DelayChanged(int)));

	// 整体触发
	ui.all_radioButton->setStyleSheet("QRadioButton::indicator{width: 70px;height: 13px;}"
		"QRadioButton::indicator:unchecked{image: url(./images/allTirgger_nor.png);}"
		"QRadioButton::indicator:checked{image: url(./images/allTirgger_sel.png);}");
	connect(ui.all_radioButton, SIGNAL(clicked(bool)), this, SLOT(AllRidio(bool)));

	// 按钮触发
	ui.btn_radioButton->setStyleSheet("QRadioButton::indicator{width: 70px;height: 13px;}"
		"QRadioButton::indicator:unchecked{image: url(./images/btnTirgger_nor.png);}"
		"QRadioButton::indicator:checked{image: url(./images/btnTirgger_sel.png);}");
	connect(ui.btn_radioButton, SIGNAL(clicked(bool)), this, SLOT(BtnRidio(bool)));

	ui.close_pushButton->setStyleSheet("border-image: url(./images/btn_off.png);");
	connect(ui.close_pushButton, SIGNAL(clicked()), this, SLOT(CloseDialog()));
	ui.delay_comboBox->addItem("0秒", "0");
	ui.delay_comboBox->addItem("1秒", "1");
	ui.delay_comboBox->addItem("2秒", "2");
	ui.delay_comboBox->addItem("3秒", "3");
	ui.delay_comboBox->setCurrentIndex(0);

	ui.pushButton->setStyleSheet("border-image: url(./images/set_title.png);");
	ui.pushButton->installEventFilter(this);
	ui.tirgger_groupBox->setStyleSheet("border: 1px solid white;");
	ui.all_radioButton->setChecked(true);
}

UIBulletParam::~UIBulletParam()
{

}

void UIBulletParam::CloseDialog()
{
	setVisible(false);
}

void UIBulletParam::AllRidio(bool b)
{
	if (b && m_Parent)
	{
		m_Parent->setTriggerType(true);
	}
}

void UIBulletParam::BtnRidio(bool b)
{
	if (b && m_Parent)
	{
		m_Parent->setTriggerType(false);
	}
}

void UIBulletParam::DelayChanged(int index)
{
	if (m_Parent)
	{
		m_Parent->BulletDelay(index);
	}
}

void UIBulletParam::setParent(UIWindowSet* parent)
{
	m_Parent = parent;
}

void UIBulletParam::paintEvent(QPaintEvent *event)
{
	QPainterPath path;
	QPainter painter(this);

	QColor color(217, 217, 217);

	path.addRect(0, 0, this->width() - 1, this->height() - 1);
	painter.setPen(color);
	painter.drawPath(path);
}

// 拖动标题做的处理
bool UIBulletParam::eventFilter(QObject *target, QEvent *event)
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

void UIBulletParam::focusOutEvent(QFocusEvent* e)
{
	QPoint pt = mapFromGlobal(QCursor::pos());
	pt.setX(pt.x() + geometry().x());
	pt.setY(pt.y() + geometry().y());
	QRect rc = QRect(x(),y(),width(),height()+50);
	if (rc.contains(pt))
	{
		setFocus();
		return;
	}

	this->hide();
}