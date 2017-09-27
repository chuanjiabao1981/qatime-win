#include "UILivelessonwnd.h"
#include <QScrollBar>
#include <QPainter>
#include <QMouseEvent>
#include <QToolTip>
#include "ZPublicDefine.h"
#include "ZWeb.h"

UILiveLessonWnd::UILiveLessonWnd(QWidget *parent)
	: QWidget(parent)
	, m_vecLessonItem(NULL)
	, m_CurrentLessonType(1)
{
	ui.setupUi(this);

	// 设置垂直布局
	m_mainAllView = new QWidget();
	ui.scrollArea->setWidget(m_mainAllView);
	m_VerAll = new QVBoxLayout(m_mainAllView);
	m_VerAll->setSpacing(0);
	m_VerAll->setContentsMargins(0, 0, 0, 0);
	ui.scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	style(ui.scrollArea);

	ui.pushButton->setStyleSheet("border-image: url(./images/btn_off.png);");
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(clickClose()));

	ui.title_pushButton->setStyleSheet("border-image: url(./images/set_title.png);");
	ui.title_pushButton->installEventFilter(this);
}

UILiveLessonWnd::~UILiveLessonWnd()
{

}

// 拖动标题做的处理
bool UILiveLessonWnd::eventFilter(QObject *target, QEvent *event)
{
	if (target == ui.title_pushButton)
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



void UILiveLessonWnd::AddEveryTimeLesson()
{
	UILiveLessonItem* item = new UILiveLessonItem(this);
	item->AddAuToDefineLesson("", m_CourseID, m_BoardURL, m_CameraURL, "互动答疑", "", "", 0, d_ExclusiveLesson);
	connect(item, SIGNAL(sig_doubleclick(QString, QString, QString, QString, int, QString, bool)), this, SLOT(slot_dbClick(QString, QString, QString, QString, int, QString, bool)));
	m_VerAll->addWidget(item);

	m_vecLessonItem.push_back(item);
	// 添加到布局里
	if (m_spacerAll == NULL)
	{
		m_spacerAll = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
		m_VerAll->addSpacerItem(m_spacerAll);
	}
	else
	{
		m_VerAll->removeItem(m_spacerAll);
		m_spacerAll = NULL;
		m_spacerAll = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
		m_VerAll->addSpacerItem(m_spacerAll);
	}
}

void UILiveLessonWnd::setCourseID(QString courseid, int mLessonType)
{
	m_CurrentLessonType = mLessonType;
	m_CourseID = courseid;
	int i = 0;
	if (mLessonType == d_ExclusiveLesson)
	{
		AddEveryTimeLesson();
		i = i + 1;
	}
	if (m_vecLessonInfo.size() > 0)
	{	
		std::vector<LessonInfo>::iterator it;
		for (it = m_vecLessonInfo.begin(); it != m_vecLessonInfo.end(); it++)
		{
			LessonInfo tags = *it;
			if (courseid == tags.m_CourseID)
			{
				if (tags.m_status == "待上课" || tags.m_status == "暂停中" || tags.m_status == "已直播" || tags.m_status == "直播中")
				{
					UILiveLessonItem* item = new UILiveLessonItem(this);
					item->AddLesson(tags.m_LessonID, tags.m_CourseID, tags.m_BoardUrl, tags.m_CameraUrl, tags.m_time, tags.m_status, tags.m_name, i);
					connect(item, SIGNAL(sig_doubleclick(QString, QString, QString, QString, int, QString, bool)), this, SLOT(slot_dbClick(QString, QString, QString, QString, int, QString, bool)));
					m_VerAll->addWidget(item);

					m_vecLessonItem.push_back(item);
					// 添加到布局里
					if (m_spacerAll == NULL)
					{
						m_spacerAll = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
						m_VerAll->addSpacerItem(m_spacerAll);
					}
					else
					{
						m_VerAll->removeItem(m_spacerAll);
						m_spacerAll = NULL;
						m_spacerAll = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
						m_VerAll->addSpacerItem(m_spacerAll);
					}
					i++;
				}
			}
		}
	}
}

void UILiveLessonWnd::DeleteItem()
{
	if (m_vecLessonItem.size() > 0)
	{
		std::vector<UILiveLessonItem*>::iterator it;
		for (it = m_vecLessonItem.begin(); it != m_vecLessonItem.end(); it++)
		{
			UILiveLessonItem* item = *it;
			delete item;
			item = NULL;
		}
	}

	m_vecLessonItem.clear();
	m_vecLessonInfo.clear();
}

void UILiveLessonWnd::addLesson(QString  id, QString courseid, QString boardUrl, QString cameraUrl, QString time, QString status, QString name)
{
	LessonInfo info;
	info.m_LessonID = id;
	info.m_CourseID = courseid;
	info.m_BoardUrl = boardUrl;
	info.m_CameraUrl = cameraUrl;
	info.m_time = time;
	info.m_status = status;
	info.m_name = name;

	m_vecLessonInfo.push_back(info);
}

void UILiveLessonWnd::style(QScrollArea *style)
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

void UILiveLessonWnd::clickClose()
{
	hide();
}

void UILiveLessonWnd::paintEvent(QPaintEvent *event)
{
	QPainterPath path;
	QPainter painter(this);

	QColor color(217, 217, 217);
	for (int i = 0; i < 45; i++)
		path.addRect(0, i, this->width() - 1, this->height() - i - 1);

	painter.setPen(color);
	painter.drawPath(path);
}

void UILiveLessonWnd::SetURLBasicInfo(QString mHomePage, QString mTeacherToken, QString mBoardURL, QString mCameraURL)
{
	m_homePage = mHomePage;
	m_TeacherToken = mTeacherToken;
	m_BoardURL = mBoardURL;
	m_CameraURL = mCameraURL;
}

void UILiveLessonWnd::slot_dbClick(QString id, QString courseid, QString boardurl, QString cameraurl, int index, QString name, bool bEveryTime)
{
	// 如果点击的是即时直播
	if (bEveryTime == true)
	{
		ZWeb* mZWeb = new ZWeb;
		QString strUrl;
		QJsonObject mUrlJson;

		strUrl += m_homePage;
		strUrl += "/api/v1/live_studio/customized_groups/{mGroupID}/instant_lessons";
		strUrl.replace("{mGroupID}", courseid);
		bool bPostUrl = false;
		//bPostUrl = mZWeb.PostURLInfo(strUrl, mUrlJson, m_TeacherToken);
		bPostUrl = mZWeb->PostURLInfo(strUrl, mUrlJson, m_TeacherToken);
		if (bPostUrl == false)
		{
			//delete mZWeb;
			return;
		}
		id = QString::number(mUrlJson["data"].toObject()["id"].toInt());
		emit sig_PullStreaming(id, courseid, boardurl, cameraurl, name, m_CurrentLessonType, bEveryTime);
		hide();
		//delete mZWeb;
		return;
	}
	emit sig_PullStreaming(id, courseid, boardurl, cameraurl, name, m_CurrentLessonType, false);
	hide();
/*	// 如果是第一节课直接推流 限制直播课顺序代码
	if (index == 0)
	{
		emit sig_PullStreaming(id, courseid, boardurl, cameraurl,name,m_CurrentLessonType);
		hide();
	}
	else
	{
		if (m_vecLessonItem.size() > 0)
		{
			std::vector<UILiveLessonItem*>::iterator it;
			for (it = m_vecLessonItem.begin(); it != m_vecLessonItem.end(); it++)
			{
				UILiveLessonItem* item = *it;
				// 查找双击课程的上一节课的状态
				if (item && item->m_index == index-1)
				{
					if (item->m_status == "待上课" || item->m_status == "暂停中" || item->m_status == "直播中")
					{
						QToolTip::showText(QCursor::pos(), "请完成上一节课程！");
					}
					else if (item->m_status == "已直播")
					{
						// 结束已直播的课程，以及修改其状态
						changeStatus(item->m_id);

						// 开始直播
						emit sig_PullStreaming(id, courseid, boardurl, cameraurl, name, m_CurrentLessonType);

						hide();
					}
				}
			}
		}
	}
*/
}

void UILiveLessonWnd::changeStatus(QString id)
{
	if (m_vecLessonInfo.size() > 0)
	{
		std::vector<LessonInfo>::iterator it;
		for (it = m_vecLessonInfo.begin(); it != m_vecLessonInfo.end(); it++)
		{
			LessonInfo info = *it;
			if (id == info.m_LessonID)
			{
				QString status = "已结束";
				(*it).m_status = status;
				emit sig_changeLessonStatus(id, status);
				return;
			}
		}
	}
}

void UILiveLessonWnd::focusOutEvent(QFocusEvent* e)
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