#include "myLabel.h"
#include <QMouseEvent>
#include <QPainter>
#include <QPalette>
void myLabel::mousePressEvent(QMouseEvent *event)
{
    //如果单击了就触发clicked信号
    if (event->button() == Qt::LeftButton)
    {
        //触发clicked信号 
		emit Clicked();
    }
    //将该事件传给父类处理
    QLabel::mousePressEvent(event);
}
