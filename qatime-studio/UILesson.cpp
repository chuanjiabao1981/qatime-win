#include "UILesson.h"
#include <QJsonObject>
#include "lesson.h"
#include <QJsonArray>
#include "windows.h"

UILesson::UILesson(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.Lesson_frame->setStyleSheet("border: 1px solid white;border-radius:8px");
	ui.Lesson_tableWidget->setFocusPolicy(Qt::NoFocus);

	bool bsuc = connect(ui.Lesson_tableWidget, SIGNAL(itemSelectionChanged()), this, SLOT(Test()));
}

UILesson::~UILesson()
{

}

void UILesson::setLessonInfo(QJsonObject &objBig)
{
	QJsonObject obj = objBig["data"].toObject();
	int nNum = 0;	// 编号
	QJsonArray courses = obj["lessons"].toArray();
	foreach(const QJsonValue & value, courses)
	{
		QJsonObject obj = value.toObject();
		Lesson *pLesson = new Lesson();
		pLesson->readJson(value.toObject());

		ui.Lesson_tableWidget->insertRow(nNum);
		nNum++;

		// 显示编号
		QString strNum = QString::number(nNum);
		QTableWidgetItem *pItemNum = new QTableWidgetItem(strNum);
		pItemNum->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		ui.Lesson_tableWidget->setItem(nNum - 1, 0, pItemNum);

		// 上课时间
		QTableWidgetItem *pItemTime = new QTableWidgetItem(pLesson->LessonTime());
		pItemTime->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		ui.Lesson_tableWidget->setItem(nNum - 1, 1, pItemTime);

		// 课程名字
		QTableWidgetItem *pItemName = new QTableWidgetItem(pLesson->name());
		ui.Lesson_tableWidget->setItem(nNum - 1, 2, pItemName);

		// 课程状态
		QTableWidgetItem *pItemStatus = new QTableWidgetItem(pLesson->ChinaLessonStatus());
		pItemStatus->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		ui.Lesson_tableWidget->setItem(nNum - 1, 3, pItemStatus);
	}

	ui.Lesson_tableWidget->removeRow(nNum);
}

void UILesson::initLesson()
{
	ui.Lesson_tableWidget->clearContents();

	ui.Lesson_tableWidget->setColumnCount(4); //设置列数
	ui.Lesson_tableWidget->setRowCount(1);
	ui.Lesson_tableWidget->horizontalHeader()->setSectionsMovable(false);
	ui.Lesson_tableWidget->horizontalHeader()->setSectionsClickable(false);

	QStringList header;
	header << tr("编号") << tr("上课时间") << tr("课程名称") << tr("课程状态");
	ui.Lesson_tableWidget->setHorizontalHeaderLabels(header);

	ui.Lesson_tableWidget->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
	ui.Lesson_tableWidget->verticalHeader()->setDefaultSectionSize(25); //设置行高
	ui.Lesson_tableWidget->setFrameShape(QFrame::NoFrame); //设置无边框
	ui.Lesson_tableWidget->setShowGrid(false); //设置不显示格子线
	ui.Lesson_tableWidget->verticalHeader()->setVisible(false); //设置垂直头不可见
	ui.Lesson_tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);  //可多选（Ctrl、Shift、  Ctrl+A都可以）
	ui.Lesson_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
	ui.Lesson_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
	ui.Lesson_tableWidget->horizontalHeader()->resizeSection(0, 60); //设置表头第一列的宽度为60
	ui.Lesson_tableWidget->horizontalHeader()->resizeSection(1, 200);
	ui.Lesson_tableWidget->horizontalHeader()->resizeSection(2, 170);
	ui.Lesson_tableWidget->horizontalHeader()->resizeSection(3, 70);
	ui.Lesson_tableWidget->horizontalHeader()->setFixedHeight(25); //设置表头的高度
	ui.Lesson_tableWidget->setStyleSheet("selection-background-color:lightblue;");  //设置选中背景色
	ui.Lesson_tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:lightgray;}"
		"QHeaderView::section{padding-left:4px; border:3px solid white; }"); //设置表头背景色
}

void UILesson::Test()
{
	MessageBox(NULL, L"", L"", MB_OK);
}