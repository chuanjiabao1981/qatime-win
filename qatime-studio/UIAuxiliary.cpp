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
	int nNum = 0;	// ���
	QJsonArray courses = obj["data"].toArray();
	foreach(const QJsonValue & value, courses)
	{
		QJsonObject obj = value.toObject();
		Course *course = new Course();
		course->readJson(value.toObject());

		ui.Auxiliary_tableWidget->insertRow(nNum);
		nNum++;

		// ��ʾ���
		QString strNum = QString::number(nNum);
		QTableWidgetItem *pItemNum = new QTableWidgetItem(strNum);
		pItemNum->setData(QT_USERDATA, course->id());
		pItemNum->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		ui.Auxiliary_tableWidget->setItem(nNum - 1, 0, pItemNum);

		// ��ʾ����������
		QString strName = course->name();
		QTableWidgetItem *pItemName = new QTableWidgetItem(strName);
		ui.Auxiliary_tableWidget->setItem(nNum - 1, 1, pItemName);

		// ����
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

	ui.Auxiliary_tableWidget->setColumnCount(3); //��������
	ui.Auxiliary_tableWidget->setRowCount(1);
	ui.Auxiliary_tableWidget->horizontalHeader()->setSectionsMovable(false);
	ui.Auxiliary_tableWidget->horizontalHeader()->setSectionsClickable(false);

	QStringList header;
	header << tr("���") << tr("��������Ϣ") << tr("����");
	ui.Auxiliary_tableWidget->setHorizontalHeaderLabels(header);

	ui.Auxiliary_tableWidget->horizontalHeader()->setStretchLastSection(true); //���ó�������
	ui.Auxiliary_tableWidget->verticalHeader()->setDefaultSectionSize(25); //�����и�
	ui.Auxiliary_tableWidget->setFrameShape(QFrame::NoFrame); //�����ޱ߿�
	ui.Auxiliary_tableWidget->setShowGrid(false); //���ò���ʾ������
	ui.Auxiliary_tableWidget->verticalHeader()->setVisible(false); //���ô�ֱͷ���ɼ�
	ui.Auxiliary_tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);  //�ɶ�ѡ��Ctrl��Shift��  Ctrl+A�����ԣ�
	ui.Auxiliary_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //����ѡ����Ϊʱÿ��ѡ��һ��
	ui.Auxiliary_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //���ò��ɱ༭
	ui.Auxiliary_tableWidget->horizontalHeader()->resizeSection(0, 60); //���ñ�ͷ��һ�еĿ��Ϊ60
	ui.Auxiliary_tableWidget->horizontalHeader()->resizeSection(1, 300);
	ui.Auxiliary_tableWidget->horizontalHeader()->resizeSection(2, 70);
	ui.Auxiliary_tableWidget->horizontalHeader()->setFixedHeight(25); //���ñ�ͷ�ĸ߶�
	ui.Auxiliary_tableWidget->setStyleSheet("selection-background-color:lightblue;");  //����ѡ�б���ɫ
	ui.Auxiliary_tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:lightgray;}"
		"QHeaderView::section{padding-left:4px; border:3px solid white; }"); //���ñ�ͷ����ɫ
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
