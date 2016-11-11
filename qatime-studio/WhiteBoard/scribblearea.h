#ifndef SCRIBBLEAREA_H
 #define SCRIBBLEAREA_H

 #include <QColor>
 #include <QImage>
 #include <QPoint>
 #include <QWidget>
 #include <QPainter>
 #include <QInputDialog>

 class ScribbleArea : public QWidget
 {
     Q_OBJECT
enum status {null,drawLine,drawRect,drawRoundRect,drawEllipse,circleErase,rectErase, insertText};

 public:
     ScribbleArea(QWidget *parent = 0);

     bool openImage(const QString &fileName);
     bool saveImage(const QString &fileName, const char *fileFormat);

     void setPenColor(const QColor &newColor);
     void setPenWidth(int newWidth);
     void setPenBrush(const QBrush &newBrush);

     bool isModified() const { return modified; }
     QColor penColor() const { return myPenColor; }
     int penWidth() const { return myPenWidth; }




     //void setPen(QPainter *painter);
     //void setBrush(QPainter *painter);
     status status;
 public slots:
     void clearImage();
     void setStatusNull();
     void setStatusDrawLine();
     void setStatusDrawRect();
     void setStatusDrawRoundRect();
     void setStatusDrawEllipse();
     void setStatusCircleErase();
     void setStatusRectErase();
	 void setStatusText();

  signals:
    void setP1(QPointF);
    void setP2(QPointF);
    void mousePressed();
    void mouseReleased();
    void mouseMoved();


 protected:
     void mousePressEvent(QMouseEvent *event);
     void mouseMoveEvent(QMouseEvent *event);
     void mouseReleaseEvent(QMouseEvent *event);
     void paintEvent(QPaintEvent *event);
     void resizeEvent(QResizeEvent *event);

 private:
     void paint(QPainter *painter);


     void drawLineTo(const QPoint &endPoint);
     void resizeImage(QImage *image, const QSize &newSize);
	 void SetBoardText(QPainter& painter);
     QPainter *imagePainter;
     QPointF p1;
     QPointF p2;

     bool modified;
     bool scribbling;
     int myPenWidth;
     QColor myPenColor;
     QBrush myPenBrush;
     QColor EraseColor;
     QBrush EraseBrush;
     QImage image;
     QPoint lastPoint;
     QPoint endPoint;
     double roundRectR;
	 QString strText;
 };

 #endif
