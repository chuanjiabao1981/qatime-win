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
	peopleNum->setText("当前观看人数 30");
}
void personListBuddy::initFindPeople()
{
	secLinEdit = new QLineEdit(this);
	connect(secLinEdit, SIGNAL(textChanged(const QString)), this, SLOT(findName(const QString)));
	secFindButton = new QPushButton(this);
	connect(secFindButton, SIGNAL(clicked()), this, SLOT(findName()));	
	secLinEdit->move(5, 5);
	secLinEdit->setFixedWidth(220);
	secLinEdit->setPlaceholderText("请输入要查的姓名");
	secLinEdit->setAlignment(Qt::AlignCenter);//居中对齐
	secFindButton->move(225, 4);
	secFindButton->setText("查找");
}
void personListBuddy::initNotFind()
{
	notFind = new QLabel(this);
	notFind->setBaseSize(QSize(200, 25));
	notFind->move(95, 5);
	notFind->setText("没有找到该好友");
	notFind->setAlignment(Qt::AlignCenter);//居中对齐;//居中对齐	
	notFind->setStyleSheet("color:red;");
	notFind->hide();
	firstButton = new QCheckBox(this);
	connect(firstButton, SIGNAL(clicked(bool)), this, SLOT(stopAllTalk(bool)));
	firstButton->move(250, 5);
	firstButton->setText("全禁");
}

//初始化Ui
void personListBuddy::initUi(const QString &szUrl,QString strName,QString ID)
{
    //初始化
//    head=new QWidget(this);
	head = new QLabel(this);
    name=new QLabel(this);
	m_ID = ID;
	button = new QCheckBox(this);
	connect(button, SIGNAL(stateChanged(int)), this, SLOT(radioChange(int)));
//    sign=new QLabel(this);
    //设置头像大小
    head->setFixedSize(32,32);
    //设置个性签名字体为灰色
    QPalette color;
    color.setColor(QPalette::Text,Qt::gray);
//    sign->setPalette(color);
    //布局
    head->move(5,1);
    name->move(7+16+10,10);
	button->move(250, 5);
	button->setText("禁言");

	setNetworkPic(szUrl);
	name->setText(strName);
//    sign->move(54,27);
    //装载事件过滤器
 //   head->installEventFilter(this);
}

void personListBuddy::setOlineNum(int olineNum,int allNum)
{
	peopleNum->setText("观看人数 " + QString::number(olineNum) + "/" + QString::number(allNum));
}

//事件过滤器，主要是为了让图片能够全部填充在head里面
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
//TODO 后续添加此功能
void personListBuddy::onlyOnline(bool b)
{

}

void personListBuddy::findName()
{
	QString name = secLinEdit->text();
	emit signalFindName(name);
}
//当查找学生编辑框为NULL时候，显示所有学生名字
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
	//请求结束并下载完成后，退出子事件循环 
	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	//开启子事件循环 
	loop.exec();
	QByteArray jpegData = reply->readAll();
	QPixmap pixmap;
	QSize pixSize(32,32);
	bool b = pixmap.loadFromData(jpegData);
	QPixmap scaledPixmap = pixmap.scaled(pixSize, Qt::KeepAspectRatio);	
	head->setPixmap(scaledPixmap);
}
