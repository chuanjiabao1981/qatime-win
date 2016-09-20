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
	firstButton = new QCheckBox(this);	
	connect(firstButton, SIGNAL(clicked(bool)), this, SLOT(stopAllTalk(bool)));
	peopleNum->move(5, 5);
	peopleNum->setText("�ۿ����� 0/0");
	firstButton->move(225, 5);
	firstButton->setText("ȫ������");	
}
void personListBuddy::initFindPeople()
{
	secLinEdit = new QLineEdit(this);
	secFindButton = new QPushButton(this);
	connect(secFindButton, SIGNAL(clicked()), this, SLOT(findName()));	
	secLinEdit->move(5, 5);
	secLinEdit->setFixedWidth(220);
	secLinEdit->setPlaceholderText("������Ҫ�������");
	secLinEdit->setAlignment(Qt::AlignCenter);//���ж���
	secFindButton->move(225, 4);
	secFindButton->setText("����");
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
	connect(button, SIGNAL(clicked(bool)), this, SLOT(radioChange(bool)));
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

void personListBuddy::radioChange(bool b)
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
