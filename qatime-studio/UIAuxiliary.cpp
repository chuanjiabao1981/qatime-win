#include "UIAuxiliary.h"
#include <QJsonObject>
#include "course.h"
#include <QJsonArray>
#include "windows.h"

#define QT_USERDATA			100

UIAuxiliary::UIAuxiliary(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.Auxiliary_frame->setStyleSheet("border: 1px solid white;border-radius:8px");
	ui.Auxiliary_tableWidget->setFocusPolicy(Qt::NoFocus);

	bool bsuc = connect(ui.Auxiliary_tableWidget, SIGNAL(itemSelectionChanged()), this, SLOT(setLessonID()));

	bFirstInit = false;
}

UIAuxiliary::~UIAuxiliary()
{

}

void UIAuxiliary::setAuxiliaryInfo(QJsonObject &obj)
{
	int nNum = 0;	// 编号
	QJsonArray courses = obj["data"].toArray();
	foreach(const QJsonValue & value, courses)
	{
		QJsonObject obj = value.toObject();
		Course *course = new Course();
		course->readJson(value.toObject());

		ui.Auxiliary_tableWidget->insertRow(nNum);
		nNum++;

		// 显示编号
		QString strNum = QString::number(nNum);
		QTableWidgetItem *pItemNum = new QTableWidgetItem(strNum);
		pItemNum->setData(QT_USERDATA, course->id());
		pItemNum->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		ui.Auxiliary_tableWidget->setItem(nNum - 1, 0, pItemNum);

		// 显示辅导班名字
		QString strName = course->name();
		QTableWidgetItem *pItemName = new QTableWidgetItem(strName);
		ui.Auxiliary_tableWidget->setItem(nNum - 1, 1, pItemName);

		// 进度
		QString strProgress = course->progress();
		QTableWidgetItem *pItemProgress = new QTableWidgetItem(strProgress);
		pItemProgress->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		ui.Auxiliary_tableWidget->setItem(nNum - 1, 2, pItemProgress);
	}

	ui.Auxiliary_tableWidget->removeRow(nNum);
}

void UIAuxiliary::initCourses()
{
	bFirstInit = true;

	ui.Auxiliary_tableWidget->setColumnCount(3); //设置列数
	ui.Auxiliary_tableWidget->setRowCount(1);
	ui.Auxiliary_tableWidget->horizontalHeader()->setSectionsMovable(false);
	ui.Auxiliary_tableWidget->horizontalHeader()->setSectionsClickable(false);

	QStringList header;
	header << tr("编号") << tr("辅导班信息") << tr("进度");
	ui.Auxiliary_tableWidget->setHorizontalHeaderLabels(header);

	ui.Auxiliary_tableWidget->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
	ui.Auxiliary_tableWidget->verticalHeader()->setDefaultSectionSize(25); //设置行高
	ui.Auxiliary_tableWidget->setFrameShape(QFrame::NoFrame); //设置无边框
	ui.Auxiliary_tableWidget->setShowGrid(false); //设置不显示格子线
	ui.Auxiliary_tableWidget->verticalHeader()->setVisible(false); //设置垂直头不可见
	ui.Auxiliary_tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);  //可多选（Ctrl、Shift、  Ctrl+A都可以）
	ui.Auxiliary_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
	ui.Auxiliary_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
	ui.Auxiliary_tableWidget->horizontalHeader()->resizeSection(0, 60); //设置表头第一列的宽度为60
	ui.Auxiliary_tableWidget->horizontalHeader()->resizeSection(1, 300);
	ui.Auxiliary_tableWidget->horizontalHeader()->resizeSection(2, 70);
	ui.Auxiliary_tableWidget->horizontalHeader()->setFixedHeight(25); //设置表头的高度
	ui.Auxiliary_tableWidget->setStyleSheet("selection-background-color:lightblue;");  //设置选中背景色
	ui.Auxiliary_tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:lightgray;}"
		"QHeaderView::section{padding-left:4px; border:3px solid white; }"); //设置表头背景色
}

void UIAuxiliary::setLessonID()
{
	int row = ui.Auxiliary_tableWidget->currentRow();
	QTableWidgetItem* CurItem = ui.Auxiliary_tableWidget->item(row, 0);
	if (CurItem)
	{
		mLessonID = CurItem->data(QT_USERDATA).toString();
	}
}

QString UIAuxiliary::getLessonID()
{
	return mLessonID;
}

bool UIAuxiliary::IsInit()
{
	return bFirstInit;
}
