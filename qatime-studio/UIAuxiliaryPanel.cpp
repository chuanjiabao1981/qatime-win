#include "UIAuxiliaryPanel.h"
#include <QPushButton>
#include <QToolBox>
#include <QVBoxLayout>
#include "course.h"
#include "lesson.h"
#include <QFile>
#include <QScrollBar>
#include "UIMessageBox.h"

#define QT_TOOLBOXINDEX			100				//QTableWidgetItem����
#define QT_TOOLBOXCOURSEID		101				//Toolbox��Ӧ�ĸ�����ID
#define QT_TOOLBOXLESSONID		102				//Toolbox��Ӧ�Ŀγ�ID
#define QT_TOOLBOXLESSONURL		103				//Toolbox��Ӧ�Ŀγ�����url
#define QT_TOOLBOXLITEMNAME		104				//QTableWidgetItem ��Ӧ��objectname
#define QT_TOOLBOXLITEMSTATUS	105				//�γ�״̬	
#define QT_TOOLBOXLESSONNAME	106				//�γ�����

UIAuxiliaryPanel::UIAuxiliaryPanel(QWidget *parent)
	: QWidget(parent)	
	, m_teacher_treewidget(NULL)	
	, m_pCurrenDoubTree(NULL)
	, m_pTreeCurrentItem(NULL)
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
	connect(ui.drawBack_pushBtn, SIGNAL(Clicked(QTreeWidgetItem *item,)), this, SLOT(DrawBack()));	
	m_teacher_treewidget = new QTreeWidget(this);
	connect(m_teacher_treewidget, SIGNAL(itemClicked(QTreeWidgetItem * item, int column)), this, SLOT(on_treeWidget_clicked(QTreeWidgetItem * item, int column)));
	connect(m_teacher_treewidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(on_DoubleClicked(QTreeWidgetItem*, int)));
	m_teacher_treewidget->setHeaderHidden(true);
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
	strTeacher += "��ʦ";
	ui.teacherName_label->setText(strTeacher);
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
		pTableWidget->setStyleSheet("selection-background-color:rgb(255, 243, 231);");  //����ѡ�б���ɫ
		pTableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:lightgray;}"
			"QHeaderView::section{padding-left:4px; border:3px solid white; }"); //���ñ�ͷ����ɫ

		// ���ù�������ʽ
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
		
		QIcon iconFriend("./images/course_back.png");
		if (nNum == 0)
		{
			iconFriend = QIcon("./images/course_expand.png");
		}
		// ��������
		QString strItemName = "lesson_";
		strItemName += QString::number(nNum);
		pTable->setObjectName(strItemName);
		//չ��ֱ���еĿγ�
		bool bExpand = false;
		
		QTreeWidgetItem *imageItem1 = new QTreeWidgetItem(m_teacher_treewidget, QStringList(QString(course->name())));
		m_teacher_treewidget->setColumnCount(3);
		m_teacher_treewidget->setWindowIcon(iconFriend);
		QString str = "�γ̽��� ";
		imageItem1->setText(3, str+course->progress());
		imageItem1->setTextAlignment(3, Qt::AlignRight);
		imageItem1->setTextAlignment(0, Qt::AlignLeft );
		m_teacher_treewidget->setColumnWidth(0, 250);
		m_teacher_treewidget->setColumnWidth(1, 150);
		imageItem1->setSizeHint(0, QSize(200, 25));		
		setCourseInfoToTree(course->JsonLesson(), course->url(), strItemName, imageItem1, bExpand, nNum);
		//չ��ֱ���еĿγ�
		if (bExpand)
		{
//			m_teacher_treewidget->setCurrentIndex(nNum);
		}
		
		delete course;
		nNum++;		
	}
	ui.teacher_verticalLayout->addWidget(m_teacher_treewidget);
}

//TODO  �������Ϣ

void UIAuxiliaryPanel::setCourseInfoToTree(QJsonArray courses, QString url, QString tableName, QTreeWidgetItem* pTableWidget, bool &bExpand,int num)
{
	int nNum = 0;	// ���
//	pTableWidget->setSizeHint(0, QSize(50, 40));
	foreach(const QJsonValue & value, courses)
	{
		QJsonObject obj = value.toObject();
		Lesson *pLesson = new Lesson();
		pLesson->readJson(value.toObject());				
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

		// ��ʾ���
		QString strNum = QString::number(nNum);		
		QTreeWidgetItem *imageItem1 = new QTreeWidgetItem(pTableWidget, QStringList(strNum + "     " + pLesson->name()));
		for (int i = 0; i < 3; i++)
		{			
			imageItem1->setData(0,QT_TOOLBOXLESSONID, pLesson->LessonID());
			imageItem1->setData(0,QT_TOOLBOXLESSONURL, url);
			imageItem1->setData(0,QT_TOOLBOXLITEMNAME, tableName);
			imageItem1->setData(0,QT_TOOLBOXLITEMSTATUS, pLesson->LessonStatus());
			imageItem1->setTextAlignment(i, Qt::AlignHCenter | Qt::AlignVCenter);
			imageItem1->setBackground(i, brush);
		}
		imageItem1->setText(1, pLesson->LessonTime());
//		imageItem1->setText(1, pLesson->name());
		imageItem1->setText(2, pLesson->ChinaLessonStatus());	
		imageItem1->setSizeHint(0, QSize(20, 20));
		imageItem1->setTextAlignment(3, Qt::AlignRight);
		imageItem1->setTextAlignment(2, Qt::AlignLeft);
		imageItem1->setTextAlignment(1, Qt::AlignLeft);
		imageItem1->setTextAlignment(0, Qt::AlignLeft);
		QIcon qIconOld("./images/empty.png");
		imageItem1->setIcon(0, qIconOld);
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
	else if (strStatus == "billing")
	{
		brush = QColor::fromRgb(255, 255, 255);
		qIcon = QIcon("./images/empty.png");
	}
	else if (strStatus == "completed")
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
void UIAuxiliaryPanel::on_treeWidget_clicked(QTreeWidgetItem * item, int column)
{
	//TODO ���������ĺ�������ʱ����
	return;
}
void UIAuxiliaryPanel::on_DoubleClicked(QTreeWidgetItem* terrWidget, int index)
{
	//TODO ˫�������ĺ���
	QString status = (QString)terrWidget->data(0,QT_TOOLBOXLITEMSTATUS).toString();
	if (status.isNull())
	{
		if (!m_pCurrenDoubTree)
		{				
				for (int i = 0; i < terrWidget->childCount(); i++)
				{
					for (int j = 0; j <= 3; j++)
					{
						terrWidget->child(i)->setBackgroundColor(j, QColor("#FFFF00"));
					}
				}	
				m_pCurrenDoubTree = terrWidget;
				return;
		}
		if (terrWidget == m_pCurrenDoubTree)
		{
			return;
		}
		else
		{			
			for (int i = 0; i < m_pCurrenDoubTree->childCount(); i++)
			{
				for (int j = 0; j <= 3; j++)
				{					
					m_pCurrenDoubTree->child(i)->setBackgroundColor(j, QColor("#FFFFFF"));
				}
			}
			for (int i = 0; i < terrWidget->childCount(); i++)	//���µ�ѡ���������ӵ�ͼ��ɫ
			{
				for (int j = 0; j <= 3; j++)
				{
					terrWidget->child(i)->setBackgroundColor(j, QColor("#FFFF00"));
				}
			}

			m_pCurrenDoubTree = terrWidget;
		}
		return;
	}
	if (status == "finished" || status == "billing" || status == "completed" || status == "init")
	{
		CMessageBox::showMessage(
			QString("����ʱ��"),
			QString("�����л����ѽ���/δ��ʼ�Ŀγ̣�"),
			QString("ȷ��"),
			QString());
		return;
	}
	// ֱ���в�׼�л��γ�
	if (m_bPreview)
	{
		CMessageBox::showMessage(
			QString("����ʱ��"),
			QString("���Ƚ�����ǰֱ�����ٽ����л�/�رգ�"),
			QString("ȷ��"),
			QString());
		return;
	}
	else
	{
		m_lessonID = (QString)terrWidget->data(0,QT_TOOLBOXLESSONID).toString();
		m_url = (QString)terrWidget->data(0, QT_TOOLBOXLESSONURL).toString();


		QString tableName = (QString)terrWidget->data(0,QT_TOOLBOXLITEMNAME).toString();
		QString tabText = terrWidget->parent()->text(0);
		
				// ȥ����һ�ڿεļ�ͷ
				if (m_pTreeCurrentItem)
				{
					QIcon qIconOld("./images/empty.png");
					m_pTreeCurrentItem->setIcon(0,qIconOld);
					m_pTreeCurrentItem->parent()->setTextColor(0, QColor("#000000"));
					m_pTreeCurrentItem->parent()->setTextColor(3, QColor("#000000"));
				}

				// ��ӵ�ǰ�εļ�ͷ
				QIcon qIcon("./images/teaching.png");
				terrWidget->setIcon(0, qIcon);	
				// ���õ�ǰitem
				m_pTreeCurrentItem = terrWidget;	
				m_pTreeCurrentItem->parent()->setTextColor(0, QColor("#FF0000"));
				m_pTreeCurrentItem->parent()->setTextColor(3, QColor("#FF0000"));
			
	}
	setFocus();
}
