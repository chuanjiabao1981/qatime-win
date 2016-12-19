#include "imagewidget.h"
#include <QDesktopWidget>
#include <QPainter>

ImageWidget::ImageWidget(QWidget *parent) :
    QWidget(parent)
{
    QDesktopWidget desktop;
    pixmap = QPixmap(desktop.width(),desktop.height());
    scale = 1;
    angle = 0;
    bFit = true;
}

void ImageWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if(angle)
    {
        QPointF center(width()/2.0,height()/2.0);
        painter.translate(center);
        painter.rotate(angle);
        painter.translate(-center);
    }
    if(bFit)
    {
        QPixmap fitPixmap = pixmap.scaled(width(),height(),Qt::KeepAspectRatio);
        painter.drawPixmap(0,0,pixmap);
    }
    else
        painter.drawPixmap(0,0,pixmap);
}

void ImageWidget::setPixmap(QString fileName)
{
    pixmap.load(fileName);
    update();
}

QPixmap ImageWidget::getPixmap()
{
    return pixmap;
}

void ImageWidget::setAngle(qreal rotateAngle)
{
    angle += rotateAngle;
    update();
}
