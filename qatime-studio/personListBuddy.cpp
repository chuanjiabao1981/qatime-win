#include "personlistbuddy.h"
#include <QPainter>
#include <QSize>
#pragma execution_character_set("utf-8")
personListBuddy::personListBuddy(QWidget *parent) :
    QWidget(parent)
{
 //   initUi();
	this->setFixedSize(QSize(300, 30));
}

personListBuddy::~personListBuddy()
{
}

void personListBuddy::initFirst()
{
	peopleNum = new QLabel(this);	
	peopleNum->move(95, 5);
	peopleNum->setText("��ǰ�ۿ����� 30");
}
void personListBuddy::initFindPeople()
{
	secLinEdit = new QLineEdit(this);
	connect(secLinEdit, SIGNAL(textChanged(const QString)), this, SLOT(findName(const QString)));
	secFindButton = new QPushButton(this);
	connect(secFindButton, SIGNAL(clicked()), this, SLOT(findName()));	
	secLinEdit->move(5, 5);
	secLinEdit->setFixedWidth(220);
	secLinEdit->setPlaceholderText("������Ҫ�������");
	secLinEdit->setAlignment(Qt::AlignCenter);//���ж���
	secFindButton->move(225, 4);
	secFindButton->setText("����");
}
void personListBuddy::initNotFind()
{
	notFind = new QLabel(this);
	notFind->setBaseSize(QSize(200, 25));
	notFind->move(95, 5);
	notFind->setText("û���ҵ��ú���");
	notFind->setAlignment(Qt::AlignCenter);//���ж���;//���ж���	
	notFind->setStyleSheet("color:red;");
	notFind->hide();
	firstButton = new QCheckBox(this);
	connect(firstButton, SIGNAL(clicked(bool)), this, SLOT(stopAllTalk(bool)));
	firstButton->move(250, 5);
	firstButton->setText("ȫ��");
}

//��ʼ��Ui
void personListBuddy::initUi(const QString &szUrl,QString strName,QString ID)
{
    //��ʼ��
//    head=new QWidget(this);
	head = new QLabel(this);
    name=new QLabel(this);
	m_ID = ID;
	button = new QCheckBox(this);
	connect(button, SIGNAL(stateChanged(int)), this, SLOT(radioChange(int)));
//    sign=new QLabel(this);
    //����ͷ���С
    head->setFixedSize(32,32);
    //���ø���ǩ������Ϊ��ɫ
    QPalette color;
    color.setColor(QPalette::Text,Qt::gray);
//    sign->setPalette(color);
    //����
    head->move(5,1);
    name->move(7+16+10,10);
	button->move(250, 5);
	button->setText("����");

	setNetworkPic(szUrl);
	name->setText(strName);
//    sign->move(54,27);
    //װ���¼�������
 //   head->installEventFilter(this);
}

void personListBuddy::setOlineNum(int olineNum,int allNum)
{
	peopleNum->setText("�ۿ����� " + QString::number(olineNum) + "/" + QString::number(allNum));
}

//�¼�����������Ҫ��Ϊ����ͼƬ�ܹ�ȫ�������head����
bool personListBuddy::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == head)
    {
        if(event->type() == QEvent::Paint)
        {
            QPainter painter(head);			
			painter.drawPixmap(head->rect(), headPath);
        }
    }
    return QWidget::eventFilter(obj, event);
}

void personListBuddy::radioChange(int b)
{
	emit emitRadioChange(b, m_ID, name->text());
}

void personListBuddy::stopAllTalk(bool b)
{
	emit signalStopAllTalk(b,"0");
}
//TODO ������Ӵ˹���
void personListBuddy::onlyOnline(bool b)
{

}

void personListBuddy::findName()
{
	QString name = secLinEdit->text();
	emit signalFindName(name);
}
//������ѧ���༭��ΪNULLʱ����ʾ����ѧ������
void personListBuddy::findName(const QString name)
{
	if (name.isEmpty())
	{
		emit signalFindNameNULL(name);
	}	
}

void personListBuddy::setNetworkPic(const QString &szUrl)
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
	QSize pixSize(32,32);
	bool b = pixmap.loadFromData(jpegData);
	QPixmap scaledPixmap = pixmap.scaled(pixSize, Qt::KeepAspectRatio);	
	head->setPixmap(scaledPixmap);
}
