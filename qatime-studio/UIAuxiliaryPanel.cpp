#include "UIAuxiliaryPanel.h"
#include <QPushButton>
#include <QToolBox>
#include <QVBoxLayout>
#include "course.h"
#include "lesson.h"
#include <QFile>
#include <QScrollBar>
#include "UIMessageBox.h"
#include <QToolTip>
#include "define.h"
#include <iosfwd>
#include <sstream>

#define QT_TOOLBOXINDEX			100				//QTableWidgetItem索引
#define QT_TOOLBOXCOURSEID		101				//Toolbox对应的辅导班ID
#define QT_TOOLBOXLESSONID		102				//Toolbox对应的课程ID
#define QT_TOOLBOXLESSONURL		103				//Toolbox对应的课程推流url
#define QT_TOOLBOXLITEMNAME		104				//QTableWidgetItem 对应的objectname
#define QT_TOOLBOXLITEMSTATUS	105				//课程状态	
#define QT_TOOLBOXLESSONNAME	106				//课程名字
#define QT_TOOLBOXCHATID		107				//云信会话ID（一个辅导班对应一个ID）
#define QT_TOOLBOXCAMERAURL		108				//Toolbox对应的摄像头推流url

UIAuxiliaryPanel::UIAuxiliaryPanel(QWidget *parent)
	: QWidget(parent)	
	, m_teacher_treewidget(NULL)	
	, m_pCurrenDoubTree(NULL)
	, m_pTreeCurrentItem(NULL)
	, m_bPreview(false)
	, m_Parent(NULL)
	, m_FirstLessonItem(NULL)
{
	ui.setupUi(this);
	setFocusPolicy(Qt::ClickFocus);

	setAutoFillBackground(true);;
	QPalette p = palette();
	p.setColor(QPalette::Window, QColor("white"));
	setPalette(p);
	
	init();
}

UIAuxiliaryPanel::~UIAuxiliaryPanel()
{
	if (m_Parent)
		m_Parent = NULL;
}

void UIAuxiliaryPanel::init()
{			
	m_teacher_treewidget = new QTreeWidget(this);
	connect(m_teacher_treewidget, SIGNAL(itemClicked(QTreeWidgetItem * item, int column)), this, SLOT(on_treeWidget_clicked(QTreeWidgetItem * item, int column)));
	connect(m_teacher_treewidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(on_DoubleClicked(QTreeWidgetItem*, int)));
	m_teacher_treewidget->setHeaderHidden(true);
}

void UIAuxiliaryPanel::setParent(UIMainNewWindow* parent)
{
	m_Parent = parent;
}

void UIAuxiliaryPanel::setRemeberToken(const QString &token)
{
	mRemeberToken = token;;
}

void UIAuxiliaryPanel::setTeacherID(const QString &teacherID)
{
	m_teacherID = teacherID;
}

void UIAuxiliaryPanel::style(QTableWidget* pTableWidget)
{
	if (pTableWidget)
	{
		pTableWidget->clearContents();

		pTableWidget->setColumnCount(5); //设置列数
		pTableWidget->setRowCount(1);
		pTableWidget->horizontalHeader()->setSectionsMovable(false);
		pTableWidget->horizontalHeader()->setSectionsClickable(false);

		pTableWidget->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
		pTableWidget->verticalHeader()->setDefaultSectionSize(25); //设置行高
		pTableWidget->setFrameShape(QFrame::NoFrame); //设置无边框
		pTableWidget->setShowGrid(false); //设置不显示格子线
		pTableWidget->verticalHeader()->setVisible(false); //设置垂直头不可见
		pTableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);  //可多选（Ctrl、Shift、  Ctrl+A都可以）
		pTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
		pTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
		pTableWidget->horizontalHeader()->resizeSection(0, 20); //设置表头第一列的宽度为60
		pTableWidget->horizontalHeader()->resizeSection(1, 20);
		pTableWidget->horizontalHeader()->resizeSection(2, 160);
		pTableWidget->horizontalHeader()->resizeSection(3, 230);
		pTableWidget->horizontalHeader()->resizeSection(4, 70);
		pTableWidget->horizontalHeader()->setFixedHeight(25); //设置表头的高度
		pTableWidget->setStyleSheet("selection-background-color:rgb(255, 243, 231);");  //设置选中背景色
		pTableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:lightgray;}"
			"QHeaderView::section{padding-left:4px; border:3px solid white; }"); //设置表头背景色

		// 设置滚动条样式
		m_teacher_treewidget->verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
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

		pTableWidget->setFocusPolicy(Qt::NoFocus);
		pTableWidget->horizontalHeader()->setVisible(false);
		pTableWidget->verticalHeader()->setVisible(false);
	}
}

void UIAuxiliaryPanel::setAuxiliaryInfo(QJsonObject &obj)
{
	int nNum = 0;
	QJsonArray courses = obj["data"].toArray();
	foreach(const QJsonValue & value, courses)
	{
		QJsonObject obj = value.toObject();
		Course *course = new Course();
		course->readJson(value.toObject());

		//新建一个QWidget 添加到QToolBox中  
		QWidget *widget = new QWidget();
		QTableWidget* pTable = new QTableWidget();
		style(pTable);
		//设置当前索引
		pTable->setAccessibleDescription(QString::number(nNum));

		QVBoxLayout *layout = new QVBoxLayout();
		layout->addWidget(pTable);
		widget->setLayout(layout);
		
		QIcon iconFriend("./images/course_back.png");
		if (nNum == 0)
		{
			iconFriend = QIcon("./images/course_expand.png");
		}
		// 设置名字
		QString strItemName = "lesson_";
		strItemName += QString::number(nNum);
		pTable->setObjectName(strItemName);
		//展开直播中的课程
		bool bExpand = false;
		
		QTreeWidgetItem *imageItem1 = new QTreeWidgetItem(m_teacher_treewidget, QStringList(QString(course->name())));
		m_teacher_treewidget->setColumnCount(3);
		m_teacher_treewidget->setWindowIcon(iconFriend);
		imageItem1->setText(2, course->progress());
		imageItem1->setTextAlignment(2, Qt::AlignRight | Qt::AlignVCenter);
		imageItem1->setTextAlignment(0, Qt::AlignLeft | Qt::AlignVCenter);
		m_teacher_treewidget->setColumnWidth(0, 205);
		m_teacher_treewidget->setColumnWidth(1, 78);
		m_teacher_treewidget->setColumnWidth(2, 40);
		imageItem1->setSizeHint(0, QSize(200, 25));	

		QFont font;
		font.setPointSize(10);
		font.setFamily(("微软雅黑"));
		imageItem1->setFont(0, font);
		imageItem1->setFont(2, font);

		setCourseInfoToTree(course->JsonLesson(), course->url(), strItemName, imageItem1,course->ChatId(),course->id(),course->CameraURL());
		
		delete course;
		nNum++;		
	}
	ui.teacher_verticalLayout->addWidget(m_teacher_treewidget);
}

void UIAuxiliaryPanel::setCourseInfoToTree(QJsonArray courses, QString url, QString tableName, QTreeWidgetItem* pTableWidget, QString chatID, QString courseID, QString cameraUrl)
{
	int nNum = 0;	// 编号
	foreach(const QJsonValue & value, courses)
	{
		QJsonObject obj = value.toObject();
		Lesson *pLesson = new Lesson();
		pLesson->readJson(value.toObject());				
		nNum++;
		//设置当前课程的背景颜色和箭头图标
		QBrush brush;
		QIcon qIcon;
		QColor qColor;
		GetItemColor(pLesson->LessonStatus(), brush, qIcon);
		GetItemTextColor(pLesson->LessonStatus(), qColor);	

		// 显示编号
		QString strNum = QString().sprintf("%02d", nNum);
		strNum += "  ";
		strNum += pLesson->name();
		QTreeWidgetItem *imageItem1 = new QTreeWidgetItem(pTableWidget, QStringList(strNum));
		QFont font;
		font.setPointSize(9);
		font.setFamily(("微软雅黑"));

		for (int i = 0; i <= 3; i++)
		{			
			imageItem1->setData(0,QT_TOOLBOXLESSONID, pLesson->LessonID());
			imageItem1->setData(0,QT_TOOLBOXLESSONURL, url);
			imageItem1->setData(0,QT_TOOLBOXLITEMNAME, tableName);
			imageItem1->setData(0,QT_TOOLBOXLITEMSTATUS, pLesson->LessonStatus());
			imageItem1->setData(0, QT_TOOLBOXCHATID, chatID);
			imageItem1->setData(0, QT_TOOLBOXCOURSEID, courseID);
			imageItem1->setData(0, QT_TOOLBOXLESSONNAME, pLesson->name());
			imageItem1->setData(0, QT_TOOLBOXCAMERAURL, cameraUrl);
			imageItem1->setData(0, QT_TOOLBOXINDEX, QString().sprintf("%02d  ", nNum));
			imageItem1->setTextAlignment(i, Qt::AlignHCenter | Qt::AlignVCenter);
			imageItem1->setBackground(i, brush);
			imageItem1->setToolTip(0,pLesson->name());
			imageItem1->setToolTip(1, pLesson->LessonTime());
			imageItem1->setToolTip(2, pLesson->ChinaLessonStatus());
			imageItem1->setFont(i,font);
		}

		QString sLesson = pLesson->LessonTime();
		sLesson.remove(0, 11);
		imageItem1->setText(1, sLesson);
		imageItem1->setText(2, pLesson->ChinaLessonStatus());	
		imageItem1->setSizeHint(0, QSize(20, 20));
		imageItem1->setTextAlignment(2, Qt::AlignRight | Qt::AlignVCenter);
		imageItem1->setTextAlignment(1, Qt::AlignLeft | Qt::AlignVCenter);
		imageItem1->setTextAlignment(0, Qt::AlignLeft | Qt::AlignVCenter);
		imageItem1->setTextColor(0, qColor);
		imageItem1->setTextColor(1, qColor);
		imageItem1->setTextColor(2, qColor);
//		imageItem1->setIcon(0, qIcon);

		// 异常退出状态
		if (pLesson->LessonStatus() == "paused")
		{
			m_pTreeCurrentItem = imageItem1;

			m_lessonID = pLesson->LessonID();
			m_url = url;
			m_auxiliaryID = courseID;
			m_lessonName = pLesson->name();
			m_CameraUrl = cameraUrl;

			//设置父节点颜色
			m_pTreeCurrentItem->parent()->setTextColor(0, QColor("#FF0000"));
			m_pTreeCurrentItem->parent()->setTextColor(2, QColor("#FF0000"));
			m_pTreeCurrentItem->parent()->setExpanded(true);
			imageItem1->setIcon(0, qIcon);
			imageItem1->setText(0, m_lessonName);

			//进入聊天室
			m_chatID = chatID;
			m_Parent->setCurChatRoom(m_chatID, m_auxiliaryID);
			m_Parent->setVideoLesson(m_lessonName);
			m_Parent->setPausedBtn();
		}
		delete pLesson;
	}
}

void UIAuxiliaryPanel::GetItemColor(QString strStatus, QBrush& brush, QIcon& qIcon)
{
	QString icon = "./images/empty1.png";
	QString icon1 = "./images/empty2.png";
	QString icon2 = "./images/teaching.png";
	if (strStatus == "init")
	{
		brush = QColor::fromRgb(255, 255, 255);
		qIcon = QIcon(icon);
	}
	else if (strStatus == "ready")
	{
		brush = QColor::fromRgb(255, 243, 200);
		qIcon = QIcon(icon1);
	}
	else if (strStatus == "teaching")
	{
		brush = QColor::fromRgb(255, 243, 200);
		qIcon = QIcon(icon1);
	}
	else if (strStatus == "paused")
	{
		brush = QColor::fromRgb(255, 243, 200);
		qIcon = QIcon(icon2);
	}
	else if (strStatus == "closed")
	{
		brush = QColor::fromRgb(255, 243, 200);
		qIcon = QIcon(icon1);
	}
	else if (strStatus == "finished")
	{
		brush = QColor::fromRgb(255, 255, 255);
		qIcon = QIcon(icon);
	}
	else if (strStatus == "billing")
	{
		brush = QColor::fromRgb(255, 255, 255);
		qIcon = QIcon(icon);
	}
	else if (strStatus == "completed")
	{
		brush = QColor::fromRgb(255, 255, 255);
		qIcon = QIcon(icon);
	}
	else if (strStatus == "missed")
	{
		brush = QColor::fromRgb(255, 255, 255);
		qIcon = QIcon(icon);
	}
}

void UIAuxiliaryPanel::GetItemTextColor(QString strStatus, QColor& qColor)
{
	if (strStatus == "init")
	{
		qColor = QColor::fromRgb(145, 145, 145);
	}
	else if (strStatus == "ready")
	{
		qColor = QColor::fromRgb(0, 0, 0);
	}
	else if (strStatus == "teaching")
	{
		qColor = QColor::fromRgb(0, 0, 0);
	}
	else if (strStatus == "paused")
	{
		qColor = QColor::fromRgb(0, 0, 0);
	}
	else if (strStatus == "closed")
	{
		qColor = QColor::fromRgb(0, 0, 0);
	}
	else if (strStatus == "finished")
	{
		qColor = QColor::fromRgb(145, 145, 145);
	}
	else if (strStatus == "billing")
	{
		qColor = QColor::fromRgb(145, 145, 145);
	}
	else if (strStatus == "completed")
	{
		qColor = QColor::fromRgb(145, 145, 145);
	}
	else if (strStatus == "missed")
	{
		qColor = QColor::fromRgb(145, 145, 145);
	}
}

QString UIAuxiliaryPanel::getLessonID()
{
	return m_lessonID;
}

QString UIAuxiliaryPanel::getURL()
{
	return m_url;
}

QString UIAuxiliaryPanel::getCameraURL()
{
	return m_CameraUrl;
}

QString UIAuxiliaryPanel::getCouresID()
{
	return m_auxiliaryID;
}

QString UIAuxiliaryPanel::getChatID()
{
	return m_chatID;
}

void UIAuxiliaryPanel::setPreview(bool bPreview)
{
	m_bPreview = bPreview;
}

QString UIAuxiliaryPanel::getLessonName()
{
	return m_lessonName;
}
void UIAuxiliaryPanel::on_treeWidget_clicked(QTreeWidgetItem * item, int column)
{
	//TODO 单击触发的函数，暂时屏蔽
	return;
}
void UIAuxiliaryPanel::on_DoubleClicked(QTreeWidgetItem* terrWidget, int index)
{
	//TODO 双击触发的函数
	QString status = (QString)terrWidget->data(0,QT_TOOLBOXLITEMSTATUS).toString();
	if (status.isNull())
	{
		return;
	}
	if (status == "finished" || status == "billing" || status == "completed" || status == "init" || status =="missed")
	{
		CMessageBox::showMessage(
			QString("答疑时间"),
			QString("不能切换至该课程！"),
			QString("确定"),
			QString());
		return;
	}
	if (m_pTreeCurrentItem!=NULL && (QString)m_pTreeCurrentItem->data(0, QT_TOOLBOXLITEMSTATUS).toString() == "paused")
	{
		CMessageBox::showMessage(
			QString("答疑时间"),
			QString("请先完成当前直播！"),
			QString("确定"),
			QString());
		return;
	}
	// 直播中不准切换课程
	if (m_bPreview)
	{
		CMessageBox::showMessage(
			QString("答疑时间"),
			QString("请先结束当前直播，再进行切换/关闭！"),
			QString("确定"),
			QString());
		return;
	}

	SelLessonStatus StatueType = SwitchLesson(terrWidget);
	if (StatueType == Ready)
	{
		CMessageBox::showMessage(
			QString("答疑时间"),
			QString("请按顺序直播课程！"),
			QString("确定"),
			QString());
		return;
	}
	else if (StatueType == Closed)
	{
		int iStatus = CMessageBox::showMessage(
			QString("答疑时间"),
			QString("是否结束上一节课，切换到本课程！"),
			QString("确定"),
			QString("取消"));
		if (iStatus == 1)
		{
			if (m_FirstLessonItem)
			{
				QString status = "finished";
				QBrush brush;
				QIcon qIcon;
				QColor qColor;
				GetItemColor(status, brush, qIcon);
				GetItemTextColor(status, qColor);

				QString sID = (QString)m_FirstLessonItem->data(0, QT_TOOLBOXLESSONID).toString();
				m_FirstLessonItem->setBackground(0,brush);
				m_FirstLessonItem->setBackground(1, brush);
				m_FirstLessonItem->setBackground(2, brush);
				m_FirstLessonItem->setTextColor(0, qColor);
				m_FirstLessonItem->setTextColor(1, qColor);
				m_FirstLessonItem->setTextColor(2, qColor);

				m_FirstLessonItem->setData(0, QT_TOOLBOXLITEMSTATUS, status);
				m_FirstLessonItem->setText(2, "已结束");
				m_FirstLessonItem->setIcon(0, qIcon);
				m_Parent->SendChangeStatusMsg(sID);
			}
		}
		else
			return;
	}
	
	m_lessonID = (QString)terrWidget->data(0,QT_TOOLBOXLESSONID).toString();
	m_url = (QString)terrWidget->data(0, QT_TOOLBOXLESSONURL).toString();
	m_auxiliaryID = (QString)terrWidget->data(0, QT_TOOLBOXCOURSEID).toString();
	m_lessonName = (QString)terrWidget->data(0, QT_TOOLBOXLESSONNAME).toString();
	m_CameraUrl = (QString)terrWidget->data(0, QT_TOOLBOXCAMERAURL).toString();

	QString tableName = (QString)terrWidget->data(0,QT_TOOLBOXLITEMNAME).toString();
	QString tabText = terrWidget->parent()->text(0);
		
	// 去掉上一节课的箭头
	if (m_pTreeCurrentItem)
	{
		QIcon qIcon;
		m_pTreeCurrentItem->setIcon(0, qIcon);

		QString sIndex = (QString)m_pTreeCurrentItem->data(0, QT_TOOLBOXINDEX).toString();
		QString sCurlessonName = (QString)m_pTreeCurrentItem->data(0, QT_TOOLBOXLESSONNAME).toString();
		QString sLessonName = sIndex;
		sLessonName += sCurlessonName;
		m_pTreeCurrentItem->setText(0, sLessonName);
		m_pTreeCurrentItem->parent()->setTextColor(0, QColor("#000000"));
		m_pTreeCurrentItem->parent()->setTextColor(2, QColor("#000000"));
	}

	// 添加当前课的箭头
	QIcon qIcon("./images/teaching.png");
	terrWidget->setIcon(0, qIcon);	
	// 修改显示的课程名称，去掉索引号
	QString strlessonName = (QString)terrWidget->data(0, QT_TOOLBOXLESSONNAME).toString();
	terrWidget->setText(0, strlessonName);
	// 设置当前item
	m_pTreeCurrentItem = terrWidget;	
	m_pTreeCurrentItem->parent()->setTextColor(0, QColor("#FF0000"));
	m_pTreeCurrentItem->parent()->setTextColor(2, QColor("#FF0000"));

	//进入聊天室
	m_Parent->LivePage();
	m_chatID = (QString)terrWidget->data(0, QT_TOOLBOXCHATID).toString();
	m_Parent->setCurChatRoom(m_chatID, m_auxiliaryID);
	m_Parent->setVideoLesson(m_lessonName);
}

void UIAuxiliaryPanel::ShowTip()
{
	QToolTip::showText(QCursor::pos(), "您已成功进入直播教室！");
}

// 遍历节点
void UIAuxiliaryPanel::ergodicItem(QString sLessonID, QString sCourseid)
{
	QTreeWidgetItemIterator it(m_teacher_treewidget);
	while (*it)
	{
		QTreeWidgetItem* terrWidget = *it;
		QString lessonID = (QString)terrWidget->data(0, QT_TOOLBOXLESSONID).toString();
		QString CourseID = (QString)terrWidget->data(0, QT_TOOLBOXCOURSEID).toString();
		if (sLessonID == lessonID && sCourseid == CourseID)
		{
			QString status = (QString)terrWidget->data(0, QT_TOOLBOXLITEMSTATUS).toString();
			if (status.isNull() || status == "finish")
				return;
			
			if (sLessonID == m_lessonID)
				return;
			
			emit m_teacher_treewidget->itemDoubleClicked(terrWidget, 0);
			return;
		}

		++it;
	}
}

void UIAuxiliaryPanel::ChangeLessonStatus(QString sLessonID, QString Status)
{
	QTreeWidgetItem* pItem = m_pTreeCurrentItem->parent();
	if (pItem)
	{
		QTreeWidgetItemIterator it(pItem);
		while (*it)
		{
			QTreeWidgetItem* terrWidget = *it;
			QString lessonID = (QString)terrWidget->data(0, QT_TOOLBOXLESSONID).toString();
			if (sLessonID == lessonID)
			{
				QString strChinaStatus;
				if (Status == "init")
					strChinaStatus = MSG_LESSON_STATUS_INIT;
				else if (Status == "ready")
					strChinaStatus = MSG_LESSON_STATUS_READY;
				else if (Status == "teaching")
					strChinaStatus = MSG_LESSON_STATUS_THEACHING;
				else if (Status == "paused")
					strChinaStatus = MSG_LESSON_STATUS_PAUSED;
				else if (Status == "closed")
					strChinaStatus = MSG_LESSON_STATUS_CLOSED;
				else if (Status == "finished")
					strChinaStatus = MSG_LESSON_STATUS_FINISHED;
				else if (Status == "billing")
					strChinaStatus = MSG_LESSON_STATUS_BILLING;
				else if (Status == "completed")
					strChinaStatus = MSG_LESSON_STATUS_COMPLETED;
				else if (Status == "missed")
					strChinaStatus = MSG_LESSON_STATUS_MISSED;

				terrWidget->setText(2, strChinaStatus);
				terrWidget->setData(0, QT_TOOLBOXLITEMSTATUS, Status);
				return;
			}
			++it;
		}
	}
}

SelLessonStatus UIAuxiliaryPanel::SwitchLesson(QTreeWidgetItem* terrWidget)
{
	QTreeWidgetItem* pParentItem = terrWidget->parent();
	QTreeWidgetItemIterator it(pParentItem);

	// 查找当前节点，在父节点中的位置
	int index = pParentItem->indexOfChild(terrWidget);

	// 查找选择节点的上一个节点
	QTreeWidgetItem * pFirstItem = pParentItem->child(index - 1);
	if (pFirstItem == NULL)
	{
		return NullStatus;
	}
	else
	{
		QString status = (QString)pFirstItem->data(0, QT_TOOLBOXLITEMSTATUS).toString();
		m_FirstLessonItem = pFirstItem;
		if (status == "finished")
		{
			return Finished;
		}
		else if ( status == "closed")
		{
			return Closed;
		}
		else if (status == "ready")
		{
			return Ready;
		}
	}

	return NullStatus;
}