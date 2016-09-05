#include "UIAuxiliaryPanel.h"
#include <QPushButton>
#include <QToolBox>
#include <QVBoxLayout>
#include "course.h"
#include "lesson.h"
#include <QFile>
#include <QScrollBar>

#define QT_TOOLBOXINDEX		100				//Toolbox����
#define QT_TOOLBOXCOURSEID	101				//Toolbox��Ӧ�ĸ�����ID
#define QT_TOOLBOXLESSONID	102				//Toolbox���ڵĿγ�ID

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
	//���������������ɺ��˳����¼�ѭ�� 
	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	//�������¼�ѭ�� 
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

		pTableWidget->setColumnCount(5); //��������
		pTableWidget->setRowCount(1);
		pTableWidget->horizontalHeader()->setSectionsMovable(false);
		pTableWidget->horizontalHeader()->setSectionsClickable(false);

 		QStringList header;
 		header << tr("���") << tr("�Ͽ�ʱ��") << tr("�γ�����") << tr("�γ�״̬");
 		pTableWidget->setHorizontalHeaderLabels(header);

		pTableWidget->horizontalHeader()->setStretchLastSection(true); //���ó�������
		pTableWidget->verticalHeader()->setDefaultSectionSize(25); //�����и�
		pTableWidget->setFrameShape(QFrame::NoFrame); //�����ޱ߿�
		pTableWidget->setShowGrid(false); //���ò���ʾ������
		pTableWidget->verticalHeader()->setVisible(false); //���ô�ֱͷ���ɼ�
		pTableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);  //�ɶ�ѡ��Ctrl��Shift��  Ctrl+A�����ԣ�
		pTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //����ѡ����Ϊʱÿ��ѡ��һ��
		pTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //���ò��ɱ༭
		pTableWidget->horizontalHeader()->resizeSection(0, 20); //���ñ�ͷ��һ�еĿ��Ϊ60
		pTableWidget->horizontalHeader()->resizeSection(1, 20);
		pTableWidget->horizontalHeader()->resizeSection(2, 160);
		pTableWidget->horizontalHeader()->resizeSection(3, 230);
		pTableWidget->horizontalHeader()->resizeSection(4, 70);
		pTableWidget->horizontalHeader()->setFixedHeight(25); //���ñ�ͷ�ĸ߶�
		pTableWidget->setStyleSheet("selection-background-color:lightblue;");  //����ѡ�б���ɫ
		pTableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:lightgray;}"
			"QHeaderView::section{padding-left:4px; border:3px solid white; }"); //���ñ�ͷ����ɫ

		// ���ù�������ʽ
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

		//�½�һ��QWidget ��ӵ�QToolBox��  
		QWidget *widget = new QWidget();
		QTableWidget* pTable = new QTableWidget();
		style(pTable);

		//���õ�ǰ����
		pTable->setAccessibleDescription(QString::number(nNum));

		QVBoxLayout *layout = new QVBoxLayout();
		layout->addWidget(pTable);
		widget->setLayout(layout);

		//��QToolBox����ӳ��� 
		m_teacher_toolBox->insertItem(nNum, widget, course->name());
		QIcon iconFriend("./images/username.png");
		//���ó����ͼ��  
		m_teacher_toolBox->setItemIcon(nNum, iconFriend);

		//չ��ֱ���еĿγ�
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
	int nNum = 0;	// ���
	foreach(const QJsonValue & value, courses)
	{
		QJsonObject obj = value.toObject();
		Lesson *pLesson = new Lesson();
		pLesson->readJson(value.toObject());

		pTableWidget->insertRow(nNum);
		nNum++;

		//���õ�ǰ�γ̵ı�����ɫ�ͼ�ͷͼ��
		QBrush brush;
		QIcon qIcon;
		GetItemColor(pLesson->LessonStatus(), brush, qIcon);

		if (pLesson->LessonStatus() == "teaching")
		{
			bExpand = true;
			m_lessonID = pLesson->LessonID();
		}

		// ��ʾ��ͷͼ��
		QTableWidgetItem *pItemIcon = new QTableWidgetItem(qIcon, "");
		pItemIcon->setBackground(brush);
		pTableWidget->setItem(nNum - 1, 0, pItemIcon);

		// ��ʾ���
		QString strNum = QString::number(nNum);
		QTableWidgetItem *pItemNum = new QTableWidgetItem(strNum);
		pItemNum->setData(QT_TOOLBOXLESSONID, pLesson->LessonID());
		pItemNum->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		pItemNum->setBackground(brush);
		pTableWidget->setItem(nNum - 1, 1, pItemNum);

		// �Ͽ�ʱ��
		QTableWidgetItem *pItemTime = new QTableWidgetItem(pLesson->LessonTime());
		pItemTime->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		pItemTime->setBackground(brush);
		pTableWidget->setItem(nNum - 1, 2, pItemTime);

		// �γ�����
		QTableWidgetItem *pItemName = new QTableWidgetItem(pLesson->name());
		pItemName->setBackground(brush);
		pTableWidget->setItem(nNum - 1, 3, pItemName);

		// �γ�״̬
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