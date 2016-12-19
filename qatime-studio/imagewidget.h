#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>

class ImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ImageWidget(QWidget *parent = 0);

    bool bFit;
    qreal scale;

    void setPixmap(QString fileName);
    QPixmap getPixmap();
    void setAngle(qreal rotateAngle);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QPixmap pixmap;
    qreal angle;

};

#endif // IMAGEWIDGET_H
