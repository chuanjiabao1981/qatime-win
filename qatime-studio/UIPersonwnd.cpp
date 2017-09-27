#include "UIPersonwnd.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QScrollBar>

UIPersonWnd::UIPersonWnd(QWidget *parent)
	: QWidget(parent)
	, m_spacer(NULL)
	, m_vecLayout(NULL)
	, m_btn(NULL)
	, m_vecLayoutCheck(NULL)
{
	ui.setupUi(this);
	style(ui.scrollArea);

	setAutoFillBackground(true);;
	QPalette p = palette();
	p.setColor(QPalette::Window, QColor("white"));
	setPalette(p);
}

UIPersonWnd::~UIPersonWnd()
{

}

void UIPersonWnd::AddPerson(std::vector<personListBuddy*> vecBuddy, QString chatID)
{
	QFont font;
	font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
	font.setPixelSize(13);

	int iCount = 0;
	if (vecBuddy.size() > 0 )
	{
		std::vector<personListBuddy*>::iterator it;
		for (it = vecBuddy.begin(); it != vecBuddy.end(); it++)
		{
			personListBuddy* buddy = *it;
			if (buddy)
			{
// 				QPixmap* pix = NULL;
// 				pix = (QPixmap*)buddy->head->pixmap();
// 				QPixmap pixscaled = pix->scaled(30, 30);
// 				QString name = buddy->name->text();
// 				QString id = buddy->m_ID;
				if (buddy->button->checkState() == Qt::Checked)
					iCount++;

				UIPerson* person = new UIPerson(this);
				person->AddPersonInfo(buddy, chatID);
				connect(person, SIGNAL(sig_change(bool, QString, QString)), this, SLOT(slot_change(bool, QString, QString)));
				ui.verticalLayout->addWidget(person);

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

				m_vecPerson.push_back(person);
			}

		}
	}

	int iAllCount = m_vecPerson.size();
	if (iAllCount == iCount)
		AddAllMute(chatID, true);
	else
		AddAllMute(chatID, false);
	// 设置人员列表框高度自适应
	if (vecBuddy.size() != 0 && (vecBuddy.size() * 42 < 500))
	{
		setFixedHeight((vecBuddy.size()+1) * 45 );
	}
		
	setFocus();
}

void UIPersonWnd::slot_change(bool state, QString id, QString chatid)
{
	emit sig_stateChange(state,id,chatid);
}

void UIPersonWnd::paintEvent(QPaintEvent *event)
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

void UIPersonWnd::focusOutEvent(QFocusEvent* e)
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

	if (m_parent->IsHasFoucs(UIWindowSet::PersonWnd))
		return;

	m_parent->GetPersonBtn()->setStyleSheet("border-image: url(./images/personBtn_nor.png);");
	this->hide();
}

void UIPersonWnd::DeletePerson()
{
	if (m_vecPerson.size() > 0)
	{
		std::vector<UIPerson*>::iterator it;
		for (it = m_vecPerson.begin(); it != m_vecPerson.end(); it++)
		{
			UIPerson* layout = *it;
			disconnect(layout, SIGNAL(stateChanged(int)), this, SLOT(stateChanged(int)));
			if (layout)
			{
				delete layout;
				layout = NULL;
			}
		}
	}

	m_vecPerson.clear();
}

void UIPersonWnd::style(QScrollArea *style)
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

void UIPersonWnd::setParentBtn(UIWindowSet* parent)
{
	m_parent = parent;
}

void UIPersonWnd::AddAllMute(QString chatID, bool bAllMute)
{
	UIPerson* person = new UIPerson(this);
	person->AddPersonInfo(NULL, chatID);
	person->setMuteState(bAllMute);
	connect(person, SIGNAL(sig_allChange(bool, QString)), this, SLOT(slot_allChange(bool, QString)));
	ui.verticalLayout->insertWidget(0,person);

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

	m_vecPerson.push_back(person);
}

void UIPersonWnd::slot_allChange(bool state, QString chatid)
{
	if (m_vecPerson.size() > 0)
	{
		std::vector<UIPerson*>::iterator it;
		for (it = m_vecPerson.begin(); it != m_vecPerson.end(); it++)
		{
			UIPerson* layout = *it;
			if (layout->m_personBuddy != NULL)
			{
				layout->setMuteState(state);
			}
		}
	}
}