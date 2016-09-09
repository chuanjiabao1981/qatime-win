#include "UIAuxiliaryPanel.h"
#include <QPushButton>
#include <QToolBox>
#include <QVBoxLayout>
#include "course.h"
#include "lesson.h"
#include <QFile>
#include <QScrollBar>
#include "UIMessageBox.h"

#define QT_TOOLBOXINDEX			100				//QTableWidgetItem索引
#define QT_TOOLBOXCOURSEID		101				//Toolbox对应的辅导班ID
#define QT_TOOLBOXLESSONID		102				//Toolbox对应的课程ID
#define QT_TOOLBOXLESSONURL		103				//Toolbox对应的课程推流url
#define QT_TOOLBOXLITEMNAME		104				//QTableWidgetItem 对应的objectname
#define QT_TOOLBOXLITEMSTATUS	105				//课程状态	

UIAuxiliaryPanel::UIAuxiliaryPanel(QWidget *parent)
	: QWidget(parent)
	, m_teacher_toolBox(NULL)
	, m_pCurrentItem(NULL)
	, m_bPreview(false)
{
	ui.setupUi(this);
	setFocusPolicy(Qt::ClickFocus);
	
	init();
}

UIAuxiliaryPanel::~UIAuxiliaryPanel()
{

}

void UIAuxiliaryPanel::init()
{
	m_teacher_toolBox = new QToolBox(this);
	m_teacher_toolBox->setStyleSheet("QToolBoxButton{background:white; color:rgb(60, 60, 60);}"
		"QToolBoxButton{border:5px solid white}"
//		"QToolBox::tab:selected{color:red;}"
		"QToolBox{background:white;}");
//		"QToolBox{font:12px "Microsoft YaHei UI";}"

	connect(m_teacher_toolBox, SIGNAL(currentChanged(int)), this, SLOT(ChangedID(int)));
	connect(ui.drawBack_pushBtn, SIGNAL(clicked()), this, SLOT(DrawBack()));
}

void UIAuxiliaryPanel::setRemeberToken(const QString &token)
{
	mRemeberToken = token;;
}

void UIAuxiliaryPanel::setTeacherID(const QString &teacherID)
{
	m_teacherID = teacherID;
}

void UIAuxiliaryPanel::setTeacherName(const QString &teacherName)
{
	QString strTeacher = teacherName;
	strTeacher += "老师";
	ui.teacherName_label->setText(strTeacher);
}

void UIAuxiliaryPanel::setNetworkPic(const QString &szUrl)
{
	QUrl url(szUrl);
	QNetworkAccessManager manager;
	QEventLoop loop;

	QNetworkReply *reply = manager.get(QNetworkRequest(url));
	//请求结束并下载完成后，退出子事件循环 
	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	//开启子事件循环 
	loop.exec();

	QByteArray jpegData = reply->readAll();
	QPixmap pixmap;
	pixmap.loadFromData(jpegData);
	ui.teacherPhoto_Label->setPixmap(pixmap);
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
		pTableWidget->verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
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

void UIAuxiliaryPanel::ChangedID(int changeIndex)
{
	if (m_teacher_toolBox)
	{
		QIcon qIconBack("./images/course_back.png");
		int iCount = m_teacher_toolBox->count();
		for (int index = 0; index < iCount; index++)
		{
			m_teacher_toolBox->setItemIcon(index, qIconBack);
		}

		QIcon qIconClose("./images/course_expand.png");
		m_teacher_toolBox->setItemIcon(changeIndex, qIconClose);
		m_teacher_toolBox->update();
	}
}

void UIAuxiliaryPanel::DrawBack()
{
	this->setVisible(false);
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

		//向QToolBox中添加抽屉 
		m_teacher_toolBox->insertItem(nNum, widget, course->name());
		QIcon iconFriend("./images/course_back.png");
		
		if (nNum == 0)
		{
			iconFriend = QIcon("./images/course_expand.png");
		}
		//设置抽屉的图标  
		m_teacher_toolBox->setItemIcon(nNum, iconFriend);

		// 设置名字
		QString strItemName = "lesson_";
		strItemName += QString::number(nNum);
		pTable->setObjectName(strItemName);
		//展开直播中的课程
		bool bExpand = false;
		setCourseInfo(course->JsonLesson(), course->url(), strItemName, pTable, bExpand);
		if (bExpand)
		{
			m_teacher_toolBox->setCurrentIndex(nNum);
		}

		delete course;
		nNum++;
	}
	
	ui.teacher_verticalLayout->addWidget(m_teacher_toolBox);
}

void UIAuxiliaryPanel::setCourseInfo(QJsonArray courses, QString url,QString tableName, QTableWidget* pTableWidget, bool &bExpand)
{
	int nNum = 0;	// 编号
	foreach(const QJsonValue & value, courses)
	{
		QJsonObject obj = value.toObject();
		Lesson *pLesson = new Lesson();
		pLesson->readJson(value.toObject());

		pTableWidget->insertRow(nNum);
		nNum++;

		//设置当前课程的背景颜色和箭头图标
		QBrush brush;
		QIcon qIcon;
		GetItemColor(pLesson->LessonStatus(), brush, qIcon);

		if (pLesson->LessonStatus() == "teaching")
		{
			bExpand = true;
			m_lessonID = pLesson->LessonID();
		}

		// 显示箭头图标
		QTableWidgetItem *pItemIcon = new QTableWidgetItem(qIcon, "");
		pItemIcon->setBackground(brush);
		pTableWidget->setItem(nNum - 1, 0, pItemIcon);

		// 显示编号
		QString strNum = QString::number(nNum);
		QTableWidgetItem *pItemNum = new QTableWidgetItem(strNum);
		pItemNum->setData(QT_TOOLBOXLESSONID, pLesson->LessonID());
		pItemNum->setData(QT_TOOLBOXLESSONURL, url);
		pItemNum->setData(QT_TOOLBOXLITEMNAME, tableName);
		pItemNum->setData(QT_TOOLBOXLITEMSTATUS, pLesson->LessonStatus());
		pItemNum->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		pItemNum->setBackground(brush);
		pTableWidget->setItem(nNum - 1, 1, pItemNum);

		// 上课时间
		QTableWidgetItem *pItemTime = new QTableWidgetItem(pLesson->LessonTime());
		pItemTime->setData(QT_TOOLBOXLESSONID, pLesson->LessonID());
		pItemTime->setData(QT_TOOLBOXLESSONURL, url);
		pItemTime->setData(QT_TOOLBOXLITEMNAME, tableName);
		pItemTime->setData(QT_TOOLBOXLITEMSTATUS, pLesson->LessonStatus());
		pItemTime->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		pItemTime->setBackground(brush);
		pTableWidget->setItem(nNum - 1, 2, pItemTime);

		// 课程名字
		QTableWidgetItem *pItemName = new QTableWidgetItem(pLesson->name());
		pItemName->setData(QT_TOOLBOXLESSONID, pLesson->LessonID());
		pItemName->setData(QT_TOOLBOXLESSONURL, url);
		pItemName->setData(QT_TOOLBOXLITEMNAME, tableName);
		pItemName->setData(QT_TOOLBOXLITEMSTATUS, pLesson->LessonStatus());
		pItemName->setBackground(brush);
		pTableWidget->setItem(nNum - 1, 3, pItemName);

		// 课程状态
		QTableWidgetItem *pItemStatus = new QTableWidgetItem(pLesson->ChinaLessonStatus());
		pItemStatus->setData(QT_TOOLBOXLESSONID, pLesson->LessonID());
		pItemStatus->setData(QT_TOOLBOXLESSONURL, url);
		pItemStatus->setData(QT_TOOLBOXLITEMNAME, tableName);
		pItemStatus->setData(QT_TOOLBOXLITEMSTATUS, pLesson->LessonStatus());
		pItemStatus->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		pItemStatus->setBackground(brush);
		pTableWidget->setItem(nNum - 1, 4, pItemStatus);

		delete pLesson;
	}

	connect(pTableWidget, SIGNAL(itemClicked(QTableWidgetItem *)), this, SLOT(SelectionChanged(QTableWidgetItem*)));
}

void UIAuxiliaryPanel::GetItemColor(QString strStatus, QBrush& brush, QIcon& qIcon)
{
	if (strStatus == "init")
	{
		brush = QColor::fromRgb(255, 243, 231);
		qIcon = QIcon("./images/empty.png");
	}
	else if (strStatus == "ready")
	{
		brush = QColor::fromRgb(255, 243, 231);
		qIcon = QIcon("./images/empty.png");
	}
	else if (strStatus == "teaching")
	{
		brush = QColor::fromRgb(255, 243, 231);
		qIcon = QIcon("./images/teaching.png");
	}
	else if (strStatus == "paused")
	{
		brush = QColor::fromRgb(255, 243, 231);
		qIcon = QIcon("./images/empty.png");
	}
	else if (strStatus == "closed")
	{
		brush = QColor::fromRgb(255, 243, 231);
		qIcon = QIcon("./images/empty.png");
	}
	else if (strStatus == "finished")
	{
		brush = QColor::fromRgb(255, 255, 255);
		qIcon = QIcon("./images/empty.png");
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

void UIAuxiliaryPanel::setPreview(bool bPreview)
{
	m_bPreview = bPreview;
}

QString UIAuxiliaryPanel::getLessonName()
{
	return m_lessonName;
}

void UIAuxiliaryPanel::SelectionChanged(QTableWidgetItem* pItem)
{
	// 课程为finished状态则不可点击
	QString status = (QString)pItem->data(QT_TOOLBOXLITEMSTATUS).toString();
	if (status == "finished")
	{
		CMessageBox::showMessage(
			QString("答疑时间"),
			QString("已完成的课程不可再选！"),
			QString("确定"),
			QString());
		return;
	}

	// 直播中不准切换课程
	if (m_bPreview)
	{
		CMessageBox::showMessage(
			QString("答疑时间"),
			QString("正在直播中，请结束直播后再切换课程！"),
			QString("确定"),
			QString());
		return;
	}
	else
	{
		m_lessonID = (QString)pItem->data(QT_TOOLBOXLESSONID).toString();
		m_url = (QString)pItem->data(QT_TOOLBOXLESSONURL).toString();

		QString tableName = (QString)pItem->data(QT_TOOLBOXLITEMNAME).toString();
		QTableWidget* pTable = this->findChild<QTableWidget*>(tableName);
		if (pTable)
		{
			int iRow = pTable->row(pItem);
			QTableWidgetItem* item = pTable->takeItem(iRow, 0);
			if (item)
			{
				// 去掉上一节课的箭头
				if (m_pCurrentItem)
				{
					QIcon qIconOld("./images/empty.png");
					m_pCurrentItem->setIcon(qIconOld);

					QString tableNameOld = (QString)m_pCurrentItem->data(QT_TOOLBOXLITEMNAME).toString();
					QTableWidget* pTableOld = this->findChild<QTableWidget*>(tableNameOld);
					if (pTableOld)
					{
						int iRowOld = pTableOld->row(m_pCurrentItem);
						pTableOld->setItem(iRowOld, 0, m_pCurrentItem);
					}
				}

				// 添加当前课的箭头
				QIcon qIcon("./images/teaching.png");
				item->setIcon(qIcon);
				pTable->setItem(iRow, 0, item);

				// 设置当前item
				m_pCurrentItem = item;

// 				// 重新设置选中行颜色
// 				QTableWidgetItem* itemColor;
// 				for (int i = 1; i < 4; i++)
// 				{
// 					itemColor  = pTable->takeItem(iRow, i);
// 					if (itemColor)
// 					{
// 						itemColor->setBackgroundColor(QColor(255, 243, 231));
// 						pTable->setItem(iRow, i, itemColor);
// 					}
// 				}
			}
		}
	}

	setFocus();
}