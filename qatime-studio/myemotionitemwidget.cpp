#include "myemotionitemwidget.h"
#include <QMovie>
#include <QDebug>
#include <QLibrary> 

MyEmotionItemWidget::MyEmotionItemWidget(QString fileName , QSize emotionMoiveSize)
	: QLabel(NULL)
{
	//首先构造函数中进行初始化;	
	QMovie* iconMovie = new QMovie;
	iconMovie->setFileName(fileName);
	m_fileName = fileName;
	this->setMovie(iconMovie);
	setContentsMargins(0, 0, 0, 0);
	iconMovie->setScaledSize(QSize(emotionMoiveSize.width(), emotionMoiveSize.height()));
	//为了动态图片正常显示，（不加上start图片不显示，不加上stop图片一种处于动态效果）
	iconMovie->start();
	iconMovie->stop();
	setStyleSheet("QLabel:hover{border: 1px solid rgb(111, 156, 207);\
								color: rgb(2, 65, 132);\
								background: rgba(255, 255, 255, 200);}");
}

MyEmotionItemWidget::~MyEmotionItemWidget()
{}

// 鼠标进入Label事件
void MyEmotionItemWidget::enterEvent(QEvent* event)
{
	QMovie* movie = this->movie();
	movie->start();
	setContentsMargins(4, 2, 2, 4);
	return __super::enterEvent(event);
}
// 鼠标离开Label事件
void MyEmotionItemWidget::leaveEvent(QEvent* event)
{
	QMovie* movie = this->movie();
	movie->jumpToFrame(0);
	movie->stop();
	setContentsMargins(3, 3, 3, 3);
	return __super::leaveEvent(event);
}
void MyEmotionItemWidget::mousePressEvent(QMouseEvent *event)
{	
	emit imageName(m_fileName);
	return;
}
