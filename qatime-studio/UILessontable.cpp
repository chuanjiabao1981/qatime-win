#include "UILessontable.h"
#include <QScrollBar>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define QT_TOOLBOXCOURSEID		101				//对应的辅导班ID
#define QT_TOOLBOXLESSONID		102				//对应的课程ID
#define QT_TOOLBOXSTATUS		103				//对应的课程状态

#ifdef TEST
	#define _DEBUG
#else
#endif

UILessonTable::UILessonTable(QWidget *parent)
	: QWidget(parent)
	, m_bInit(false)
	, m_MainWindow(NULL)
{
	ui.setupUi(this);
	setFocusPolicy(Qt::ClickFocus);
	ui.lessonTable_tableWidget->setFocusPolicy(Qt::NoFocus);
	
	connect(ui.lessonTable_tableWidget, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(LessonDoubleClicked(int,int)));
	connect(ui.ok_pushButton, SIGNAL(clicked()), this, SLOT(SelectItem()));
	connect(ui.cancel_pushButton, SIGNAL(clicked()), this, SLOT(CancelSelect()));

	init();
}

UILessonTable::~UILessonTable()
{
	if (m_MainWindow)
		m_MainWindow = NULL;
}

void UILessonTable::init()
{
	ui.lessonTable_tableWidget->setColumnCount(4); //设置列数
	ui.lessonTable_tableWidget->setRowCount(1);
	ui.lessonTable_tableWidget->horizontalHeader()->setSectionsMovable(false);
	ui.lessonTable_tableWidget->horizontalHeader()->setSectionsClickable(false);

	ui.lessonTable_tableWidget->verticalHeader()->setDefaultSectionSize(30); //设置行高
//	ui.lessonTable_tableWidget->setFrameShape(QFrame::NoFrame); //设置无边框
	ui.lessonTable_tableWidget->setShowGrid(false); //设置不显示格子线
	ui.lessonTable_tableWidget->verticalHeader()->setVisible(false); //设置垂直头不可见
	ui.lessonTable_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);  //可多选（Ctrl、Shift、  Ctrl+A都可以）
	ui.lessonTable_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
	ui.lessonTable_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
	ui.lessonTable_tableWidget->horizontalHeader()->resizeSection(0, 5); //设置表头第一列的宽度为30
	ui.lessonTable_tableWidget->horizontalHeader()->resizeSection(1, 25); //设置表头第一列的宽度为30
	ui.lessonTable_tableWidget->horizontalHeader()->resizeSection(2, 90); //设置表头第二列的宽度为70
	ui.lessonTable_tableWidget->horizontalHeader()->resizeSection(3, 190); //设置表头第三列的宽度为200
	ui.lessonTable_tableWidget->horizontalHeader()->setFixedHeight(30); //设置表头的高度
	ui.lessonTable_tableWidget->setStyleSheet("selection-background-color:lightblue;");  //设置选中背景色
	ui.lessonTable_tableWidget->horizontalHeader()->setVisible(false);

	// 设置滚动条样式
	ui.lessonTable_tableWidget->horizontalScrollBar()->setVisible(false);
	ui.lessonTable_tableWidget->verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
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

void UILessonTable::RequestLesson()
{
	if (m_bInit)
		return;
	
	QString strUrl;
#ifdef _DEBUG
	strUrl = "http://testing.qatime.cn/api/v1/live_studio/teachers/{teacher_id}/schedule";
	strUrl.replace("{teacher_id}", m_teacherID);
#else
	strUrl = "https://qatime.cn/api/v1/live_studio/teachers/{teacher_id}/schedule";
	strUrl.replace("{teacher_id}", m_teacherID);
#endif

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);
	QString str = this->mRemeberToken;

	request.setRawHeader("Remember-Token", this->mRemeberToken.toUtf8());
	reply = manager.get(request);
	connect(reply, &QNetworkReply::finished, this, &UILessonTable::returnLesson);
}

void UILessonTable::returnLesson()
{
	QFont font;
	font.setPointSize(10);
	font.setFamily(("微软雅黑"));

	QByteArray result = reply->readAll();
	QJsonDocument document(QJsonDocument::fromJson(result));
	QJsonObject obj = document.object();

	int jiejf = obj["status"].toInt();
	QJsonArray data = obj["data"].toArray();
	QJsonObject error = obj["error"].toObject();
	if (obj["status"].toInt() == 1)
	{
		m_bInit = true;
		int i = 0;
		foreach(const QJsonValue & value, data)
		{
			QDateTime CurTime(QDateTime::currentDateTime());
			QString sCurTime = CurTime.toString("yyyy-MM-dd");

			QJsonObject Lesson = value.toObject();
			QString date = Lesson["date"].toString();
			if (date == sCurTime)
			{
				QJsonArray lessonArray = Lesson["lessons"].toArray();
				foreach(const QJsonValue & value, lessonArray)
				{
					// 读取课程数据
					QJsonObject objLesson = value.toObject();
					m_LessonID = QString::number(objLesson["id"].toInt());
					m_CourseID = objLesson["course_id"].toString();
					m_LessonTime = objLesson["live_time"].toString();
					m_Status = objLesson["status"].toString();
					m_LessonName = objLesson["name"].toString();
					ui.lessonTable_tableWidget->insertRow(i + 1);

					// 根据状态显示不同样式
					QColor textColor;
					QIcon icon;
					if (m_Status == "finish")
					{
						textColor = QColor::fromRgb(145, 145, 145);
						icon = QIcon("./images/lessonStatus_1.png");
						icon.actualSize(QSize(12, 12));
					}
					else if (m_Status == "close")
					{
						textColor = QColor::fromRgb(50, 50, 50);
						icon = QIcon("./images/lessonStatus_1.png");
						icon.actualSize(QSize(12, 12));
					}
					else
					{
						textColor = QColor::fromRgb(50, 50, 50);
						icon = QIcon("./images/lessonStatus_2.png");
						icon.actualSize(QSize(12, 12));
					}

					QTableWidgetItem *pItemIcon = new QTableWidgetItem();
					pItemIcon->setIcon(icon);
					pItemIcon->setData(QT_TOOLBOXLESSONID, m_LessonID);
					pItemIcon->setData(QT_TOOLBOXCOURSEID, m_CourseID);
					pItemIcon->setData(QT_TOOLBOXSTATUS, m_Status);
					ui.lessonTable_tableWidget->setItem(i, 1, pItemIcon);

					QTableWidgetItem *pItemNum = new QTableWidgetItem(m_LessonTime);
					pItemNum->setTextColor(textColor);
					pItemNum->setData(QT_TOOLBOXLESSONID, m_LessonID);
					pItemNum->setData(QT_TOOLBOXCOURSEID, m_CourseID);
					pItemNum->setData(QT_TOOLBOXSTATUS, m_Status);
					pItemNum->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
					pItemNum->setFont(font);
					ui.lessonTable_tableWidget->setItem(i, 2, pItemNum);

					QTableWidgetItem *pItemName = new QTableWidgetItem(m_LessonName);
					pItemName->setTextColor(textColor);
					pItemName->setData(QT_TOOLBOXLESSONID, m_LessonID);
					pItemName->setData(QT_TOOLBOXCOURSEID, m_CourseID);
					pItemName->setData(QT_TOOLBOXSTATUS, m_Status);
					pItemName->setFont(font);
					ui.lessonTable_tableWidget->setItem(i, 3, pItemName);
					i++;
				}
			}
		}
	}
	else if (obj["status"].toInt() == 0)
	{
		m_MainWindow->RequestError(error);
		return;
	}

	int iCount = ui.lessonTable_tableWidget->rowCount();
	if (iCount > 0)
	{
		ui.lessonTable_tableWidget->removeRow(iCount - 1);
	}
}

void UILessonTable::InitToken(QString sToken, QString sTeacherID)
{
	m_teacherID = sTeacherID;
	mRemeberToken = sToken;
}

void UILessonTable::LessonDoubleClicked(int row, int column)
{
	QTableWidgetItem *pItem = ui.lessonTable_tableWidget->item(row, column);
	if (pItem)
	{
		QString Lessonid = pItem->data(QT_TOOLBOXLESSONID).toString();
		QString Courseid = pItem->data(QT_TOOLBOXCOURSEID).toString();
		QString status = pItem->data(QT_TOOLBOXSTATUS).toString();
		if (status == "finish")
			return;

		m_MainWindow->LessonTable_Auxiliary(Lessonid, Courseid);
	}
	hide();
}

void UILessonTable::SelectItem()
{
	QTableWidgetItem* pItem = ui.lessonTable_tableWidget->currentItem();
	if (pItem)
	{
		QString Lessonid = pItem->data(QT_TOOLBOXLESSONID).toString();
		QString Courseid = pItem->data(QT_TOOLBOXCOURSEID).toString();
		QString status = pItem->data(QT_TOOLBOXSTATUS).toString();
		if (status == "finish")
			return;

		m_MainWindow->LessonTable_Auxiliary(Lessonid, Courseid);
	}
	hide();
}

void UILessonTable::CancelSelect()
{
	hide();
}

void UILessonTable::SetMainWindow(UIMainWindow* mainWnd)
{
	m_MainWindow = mainWnd;
}

void UILessonTable::setResize(int iWidth)
{
	resize(width() + iWidth, height());
	ui.cancel_pushButton->move(ui.cancel_pushButton->pos().x() + iWidth, ui.cancel_pushButton->pos().y());
	ui.ok_pushButton->move(ui.ok_pushButton->pos().x() + iWidth, ui.ok_pushButton->pos().y());
	ui.lessonTable_tableWidget->setFixedWidth(ui.lessonTable_tableWidget->width()+iWidth);
	ui.label_2->setFixedWidth(ui.label_2->width() + iWidth);
}