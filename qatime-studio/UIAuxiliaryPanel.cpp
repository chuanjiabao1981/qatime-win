#include "UIAuxiliaryPanel.h"
#include <QPushButton>
#include <QToolBox>
#include <QVBoxLayout>
#include "course.h"
#include "lesson.h"
#include <QFile>
#include <QScrollBar>

#define QT_TOOLBOXINDEX		100				//Toolbox索引
#define QT_TOOLBOXCOURSEID	101				//Toolbox对应的辅导班ID
#define QT_TOOLBOXLESSONID	102				//Toolbox对于的课程ID

UIAuxiliaryPanel::UIAuxiliaryPanel(QWidget *parent)
	: QWidget(parent)
	, m_teacher_toolBox(NULL)
{
	ui.setupUi(this);
	
	init();
}

UIAuxiliaryPanel::~UIAuxiliaryPanel()
{

}

void UIAuxiliaryPanel::init()
{
	m_teacher_toolBox = new QToolBox(this);
	m_teacher_toolBox->setStyleSheet("QToolBoxButton{background:white; color:rgb(60, 60, 60);}"
//		"QToolBox::tab:selected{color:red;}"
		"QToolBox{background:white;}");

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
	ui.teacherName_label->setText(teacherName);
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

 		QStringList header;
 		header << tr("编号") << tr("上课时间") << tr("课程名称") << tr("课程状态");
 		pTableWidget->setHorizontalHeaderLabels(header);

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
		pTableWidget->setStyleSheet("selection-background-color:lightblue;");  //设置选中背景色
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

void UIAuxiliaryPanel::ChangedID(int index)
{

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
		QIcon iconFriend("./images/username.png");
		//设置抽屉的图标  
		m_teacher_toolBox->setItemIcon(nNum, iconFriend);

		//展开直播中的课程
		bool bExpand = false;
		setCourseInfo(course->JsonLesson(), pTable, bExpand);
		if (bExpand)
		{
			m_teacher_toolBox->setCurrentIndex(nNum);
		}

		delete course;
		nNum++;
	}
	
	ui.teacher_verticalLayout->addWidget(m_teacher_toolBox);
}

void UIAuxiliaryPanel::setCourseInfo(QJsonArray courses, QTableWidget* pTableWidget, bool &bExpand)
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
		pItemNum->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		pItemNum->setBackground(brush);
		pTableWidget->setItem(nNum - 1, 1, pItemNum);

		// 上课时间
		QTableWidgetItem *pItemTime = new QTableWidgetItem(pLesson->LessonTime());
		pItemTime->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		pItemTime->setBackground(brush);
		pTableWidget->setItem(nNum - 1, 2, pItemTime);

		// 课程名字
		QTableWidgetItem *pItemName = new QTableWidgetItem(pLesson->name());
		pItemName->setBackground(brush);
		pTableWidget->setItem(nNum - 1, 3, pItemName);

		// 课程状态
		QTableWidgetItem *pItemStatus = new QTableWidgetItem(pLesson->ChinaLessonStatus());
		pItemStatus->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		pItemStatus->setBackground(brush);
		pTableWidget->setItem(nNum - 1, 4, pItemStatus);

		delete pLesson;
	}
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