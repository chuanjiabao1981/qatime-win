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
	int nNum = 0;	// ���
	QJsonArray courses = obj["lessons"].toArray();
	foreach(const QJsonValue & value, courses)
	{
		QJsonObject obj = value.toObject();
		Lesson *pLesson = new Lesson();
		pLesson->readJson(value.toObject());

		ui.Lesson_tableWidget->insertRow(nNum);
		nNum++;

		// ��ʾ���
		QString strNum = QString::number(nNum);
		QTableWidgetItem *pItemNum = new QTableWidgetItem(strNum);
		pItemNum->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		ui.Lesson_tableWidget->setItem(nNum - 1, 0, pItemNum);

		// �Ͽ�ʱ��
		QTableWidgetItem *pItemTime = new QTableWidgetItem(pLesson->LessonTime());
		pItemTime->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		ui.Lesson_tableWidget->setItem(nNum - 1, 1, pItemTime);

		// �γ�����
		QTableWidgetItem *pItemName = new QTableWidgetItem(pLesson->name());
		ui.Lesson_tableWidget->setItem(nNum - 1, 2, pItemName);

		// �γ�״̬
		QTableWidgetItem *pItemStatus = new QTableWidgetItem(pLesson->ChinaLessonStatus());
		pItemStatus->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		ui.Lesson_tableWidget->setItem(nNum - 1, 3, pItemStatus);
	}

	ui.Lesson_tableWidget->removeRow(nNum);
}

void UILesson::initLesson()
{
	ui.Lesson_tableWidget->clearContents();

	ui.Lesson_tableWidget->setColumnCount(4); //��������
	ui.Lesson_tableWidget->setRowCount(1);
	ui.Lesson_tableWidget->horizontalHeader()->setSectionsMovable(false);
	ui.Lesson_tableWidget->horizontalHeader()->setSectionsClickable(false);

	QStringList header;
	header << tr("���") << tr("�Ͽ�ʱ��") << tr("�γ�����") << tr("�γ�״̬");
	ui.Lesson_tableWidget->setHorizontalHeaderLabels(header);

	ui.Lesson_tableWidget->horizontalHeader()->setStretchLastSection(true); //���ó�������
	ui.Lesson_tableWidget->verticalHeader()->setDefaultSectionSize(25); //�����и�
	ui.Lesson_tableWidget->setFrameShape(QFrame::NoFrame); //�����ޱ߿�
	ui.Lesson_tableWidget->setShowGrid(false); //���ò���ʾ������
	ui.Lesson_tableWidget->verticalHeader()->setVisible(false); //���ô�ֱͷ���ɼ�
	ui.Lesson_tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);  //�ɶ�ѡ��Ctrl��Shift��  Ctrl+A�����ԣ�
	ui.Lesson_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //����ѡ����Ϊʱÿ��ѡ��һ��
	ui.Lesson_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //���ò��ɱ༭
	ui.Lesson_tableWidget->horizontalHeader()->resizeSection(0, 60); //���ñ�ͷ��һ�еĿ��Ϊ60
	ui.Lesson_tableWidget->horizontalHeader()->resizeSection(1, 200);
	ui.Lesson_tableWidget->horizontalHeader()->resizeSection(2, 170);
	ui.Lesson_tableWidget->horizontalHeader()->resizeSection(3, 70);
	ui.Lesson_tableWidget->horizontalHeader()->setFixedHeight(25); //���ñ�ͷ�ĸ߶�
	ui.Lesson_tableWidget->setStyleSheet("selection-background-color:lightblue;");  //����ѡ�б���ɫ
	ui.Lesson_tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:lightgray;}"
		"QHeaderView::section{padding-left:4px; border:3px solid white; }"); //���ñ�ͷ����ɫ
}

void UILesson::Test()
{
	MessageBox(NULL, L"", L"", MB_OK);
}