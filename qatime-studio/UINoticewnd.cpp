#include "UINoticewnd.h"
#include <QPainter>
#include <QMouseEvent>
#include <QObject>
#include <QScrollBar>

UINoticeWnd::UINoticeWnd(QWidget *parent)
	: QWidget(parent)
	, m_mainAllView(NULL)
	, m_VerAll(NULL)
	, m_spacer(NULL)
	, m_veclabel(NULL)
	, m_parent(NULL)
{
	ui.setupUi(this);

	setAutoFillBackground(true);
	QPalette p = palette();
	p.setColor(QPalette::Window, QColor("white"));
	setPalette(p);

	m_mainAllView = new QWidget();
	ui.scrollArea->setWidget(m_mainAllView);
	m_VerAll = new QVBoxLayout(m_mainAllView);
	m_VerAll->setSpacing(0);
	m_VerAll->setContentsMargins(5, 5, 5, 5);
	ui.scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_mainAllView->show();
	 
	style(ui.scrollArea);

	QFont font = ui.label->font();
	font.setPixelSize(13);
	ui.label->setFont(font);
	ui.announce_pushButton->setFont(font);
	ui.announce1_pushButton->setFont(font);
	ui.cancel_pushButton->setFont(font);
	ui.textEdit->setFont(font);
	ui.textEdit->document()->setMaximumBlockCount(5);

	ui.A_widget->setVisible(false);

	connect(ui.announce_pushButton, SIGNAL(clicked()), this, SLOT(clickShowNotice()));
	connect(ui.announce1_pushButton, SIGNAL(clicked()), this, SLOT(clickSendAnnounce()));
	connect(ui.cancel_pushButton, SIGNAL(clicked()), this, SLOT(clickCancel()));
}

UINoticeWnd::~UINoticeWnd()
{

}

void UINoticeWnd::clickShowNotice()
{
	ui.announce_widget->setVisible(false);
	ui.A_widget->setVisible(true);
	ui.textEdit->setText("");
	ui.textEdit->setFocus();
	QTextCursor textCursor = ui.textEdit->textCursor();
	textCursor.movePosition(QTextCursor::Start);
}

void UINoticeWnd::clickSendAnnounce()
{
	ui.announce_widget->setVisible(true);
	ui.A_widget->setVisible(false);

	QString text = ui.textEdit->toPlainText();
	if (!text.isEmpty())
	{
		QDateTime time = QDateTime::currentDateTime();
		QString curTime = time.toString("yyyy-MM-dd hh:mm:ss");
		AddNotic(text, curTime, true);
		m_parent->OnSendAnnouncements(text);
	}
	setFocus();
}

void UINoticeWnd::clickCancel()
{
	ui.announce_widget->setVisible(true);
	ui.A_widget->setVisible(false);
	setFocus();
}

void UINoticeWnd::paintEvent(QPaintEvent *event)
{
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);

	QColor color(5, 157, 210);

	path.setFillRule(Qt::WindingFill);

	path.addRect(0, 0, this->width() - 1, this->height() - 1);

	painter.setPen(color);
	painter.drawPath(path);
}

void UINoticeWnd::focusOutEvent(QFocusEvent* e)
{
	QPoint pt = mapFromGlobal(QCursor::pos());
	QRect rcBtn = ui.announce_pushButton->geometry();
	if (rcBtn.contains(pt))
		return;

	pt = mapFromGlobal(QCursor::pos());
	pt.setY(pt.y() + geometry().y());
	QRect rc = this->geometry();
	if (rc.contains(pt))
	{
		setFocus();
		return;
	}

	if (m_parent->IsHasFoucs(UIWindowSet::NoticeWnd))
		return;

	m_parent->GetNoticeBtn()->setStyleSheet("border-image: url(./images/notice_nor.png);");
	this->hide();
}

void UINoticeWnd::AddNotic(QString text, QString time, bool bInsert)
{
	ui.label->setVisible(true);
	ui.scrollArea->setVisible(true);

	QFont font;
	font.setPixelSize(13);
	font.setFamily(("微软雅黑"));

	QLabel* LText = new QLabel();
	LText->setWordWrap(true);
	LText->setFont(font);
	LText->setText(text);

	QLabel* LTime = new QLabel();
	LTime->setFont(font);
	LTime->setAlignment(Qt::AlignRight);
	LTime->setText("发布时间：" + time);
	LTime->setStyleSheet("color:rgb(153,153,153)");

	QLabel* LSpec = new QLabel();
	LSpec->setFixedHeight(20);

	if (bInsert)
	{
		m_VerAll->insertWidget(0,LSpec);
		m_VerAll->insertWidget(0,LTime);
		m_VerAll->insertWidget(0,LText);
	}
	else
	{
		m_VerAll->addWidget(LText);
		m_VerAll->addWidget(LTime);
		m_VerAll->addWidget(LSpec);
	}

	m_veclabel.push_back(LText);
	m_veclabel.push_back(LTime);
	m_veclabel.push_back(LSpec);

	// 添加到布局里
	if (m_spacer == NULL)
	{
		m_spacer = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
		m_VerAll->addSpacerItem(m_spacer);
	}
	else
	{
		m_VerAll->removeItem(m_spacer);
		m_spacer = NULL;
		m_spacer = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
		m_VerAll->addSpacerItem(m_spacer);
	}

	setFocus();
	ui.label->setVisible(true);
	ui.scrollArea->setStyleSheet("QScrollArea{border: 1px solid #ffffff;background-color: rgb(255, 255, 255);border-image: url(:/LoginWindow/images/AuxiliaryPanelBack.png);}");
}

void UINoticeWnd::DeleteNotice()
{
	if (m_veclabel.size()>0)
	{
		std::vector<QLabel*>::iterator it;
		for (it = m_veclabel.begin(); it != m_veclabel.end(); it++)
		{
			QLabel* label = *it;
			m_VerAll->removeWidget(label);
			delete label;
			label = NULL;
		}
	}
	
	m_veclabel.clear();
}

void UINoticeWnd::style(QScrollArea *style)
{
	if (style)
	{
		// 设置滚动条样式
		style->verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
			"{"
			"width:8px;"
			"background:rgba(0,0,0,0%);"
			"margin:0px,0px,0px,0px;"
			"padding-top:9px;"
			"padding-bottom:9px;"
			"}"
			"QScrollBar::handle:vertical"
			"{"
			"width:8px;"
			"background:rgba(0,0,0,25%);"
			" border-radius:4px;"
			"min-height:20;"
			"}"
			"QScrollBar::handle:vertical:hover"
			"{"
			"width:8px;"
			"background:rgba(0,0,0,50%);"
			" border-radius:4px;"
			"min-height:20;"
			"}"
			"QScrollBar::add-line:vertical"
			"{"
			"height:9px;width:8px;"
			"border-image:url(:/images/a/3.png);"
			"subcontrol-position:bottom;"
			"}"
			"QScrollBar::sub-line:vertical"
			"{"
			"height:9px;width:8px;"
			"border-image:url(:/images/a/1.png);"
			"subcontrol-position:top;"
			"}"
			"QScrollBar::add-line:vertical:hover"
			"{"
			"height:9px;width:8px;"
			"border-image:url(:/images/a/4.png);"
			"subcontrol-position:bottom;"
			"}"
			"QScrollBar::sub-line:vertical:hover"
			"{"
			"height:9px;width:8px;"
			"border-image:url(:/images/a/2.png);"
			"subcontrol-position:top;"
			"}"
			"QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
			"{"
			"background:rgba(0,0,0,10%);"
			"border-radius:4px;"
			"}"
			);
	}
}

void UINoticeWnd::setParentBtn(UIWindowSet* parent)
{
	m_parent = parent;
}

void UINoticeWnd::initDate()
{
	ui.A_widget->setVisible(false);
	ui.announce_widget->setVisible(true);
	ui.label->setVisible(false);
	ui.scrollArea->setStyleSheet("border-image:url(./images/notice.png)");
}