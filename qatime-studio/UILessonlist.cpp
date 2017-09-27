#include "UILessonlist.h"
#include <QPainter>
#include <QScrollBar>

UILessonList::UILessonList(QWidget *parent)
	: QWidget(parent)
	, m_vecBuddy(NULL)
	, m_spacer(NULL)
	, m_btn(NULL)
{
	ui.setupUi(this);
	style(ui.scrollArea);

	setAutoFillBackground(true);;
	QPalette p = palette();
	p.setColor(QPalette::Window, QColor("white"));
	setPalette(p);
}

UILessonList::~UILessonList()
{

}

void UILessonList::paintEvent(QPaintEvent *event)
{
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);

	QColor color(5, 157, 210);

		path.addRect(0, 0, this->width()-1, this->height()-1);

		painter.setPen(color);
		painter.drawPath(path);
}

void UILessonList::focusOutEvent(QFocusEvent* e)
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

	if (m_parent->IsHasFoucs(UIWindowSet::LessonWnd))
		return;

	m_parent->GetLessonBtn()->setStyleSheet("border-image: url(./images/lessonBtn_nor.png);");

	this->hide();
}



void UILessonList::AddLesson(QString number, QString strTime, QString strName, QString strStatus)
{
	UILessonBuddy* buddy = new UILessonBuddy();
	buddy->AddLesson(number, strTime, strName, strStatus);
	ui.verticalLayout->addWidget(buddy);

	m_vecBuddy.push_back(buddy);
	setFocus();

	// 添加到布局里
	if (m_spacer == NULL)
	{
		m_spacer = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
		ui.verticalLayout->addSpacerItem(m_spacer);
	}
	else
	{
		ui.verticalLayout->removeItem(m_spacer);
		m_spacer = NULL;
		m_spacer = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
		ui.verticalLayout->addSpacerItem(m_spacer);
	}

}

void UILessonList::DeleteLesson()
{
	DeleteLessonTitle();
	if (m_vecBuddy.size()>0)
	{
		std::vector<UILessonBuddy*>::iterator it;
		for (it = m_vecBuddy.begin(); it != m_vecBuddy.end(); it++)
		{
			UILessonBuddy* buddy = *it;
			buddy->close();
			ui.verticalLayout->removeWidget(buddy);
			buddy = NULL;
		}
	}
}

void UILessonList::style(QScrollArea *style)
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

void UILessonList::AddLessonTitle(QString mLessonTitle)
{
	QLabel *mLabelTitle = new QLabel();
	mLabelTitle->setObjectName(mLessonTitle);
	QFont mFont;
	mFont.setPointSize(12);
	mFont.setFamily(("微软雅黑"));
	mFont.setBold(true);
	mLabelTitle->setFont(mFont);
	mLabelTitle->setText(mLessonTitle);
	ui.verticalLayout->addWidget(mLabelTitle);
}

void UILessonList::DeleteLessonTitle()
{
	//删除课程标题
	QLabel * templabel = this->findChild<QLabel * >("线上直播");
	if (templabel != NULL)
	{
		
		delete  templabel;
		templabel = NULL;
	}
	templabel = this->findChild<QLabel * >("线下讲课");
	if (templabel != NULL)
	{
		delete  templabel;
		templabel = NULL;
	}
}

void UILessonList::setParentBtn(UIWindowSet* parent)
{
	m_parent = parent;
}