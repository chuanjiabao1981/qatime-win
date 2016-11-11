#include <QtGui>

 #include "scribblearea.h"

 ScribbleArea::ScribbleArea(QWidget *parent)
     : QWidget(parent)
 {    
     setWindowOpacity(0.0);
     setAttribute(Qt::WA_StaticContents);
     modified = false;
     scribbling = false;
     myPenWidth = 1;
     myPenColor = Qt::black;
     myPenBrush=Qt::black, Qt::BDiagPattern;
     imagePainter = new QPainter;
     status = null;
     roundRectR = 10;
     EraseColor =Qt::white;
     EraseBrush =Qt::white, Qt::BDiagPattern;
 }

 bool ScribbleArea::openImage(const QString &fileName)
 {
     QImage loadedImage;
     if (!loadedImage.load(fileName))
         return false;

     QSize newSize = loadedImage.size().expandedTo(size());
     resizeImage(&loadedImage, newSize);
     image = loadedImage;
     modified = false;
     update();
     return true;
 }

 bool ScribbleArea::saveImage(const QString &fileName, const char *fileFormat)
 {
     QImage visibleImage = image;
     resizeImage(&visibleImage, size());

     if (visibleImage.save(fileName, fileFormat)) {
         modified = false;
         return true;
     } else {
         return false;
     }
 }

 void ScribbleArea::setPenColor(const QColor &newColor)
 {
     myPenColor = newColor;
 }
void ScribbleArea::setPenBrush(const QBrush &newBrush)
 {
     myPenBrush = newBrush;
 }
 void ScribbleArea::setPenWidth(int newWidth)
 {
     myPenWidth = newWidth;
 }

 void ScribbleArea::clearImage()
 {
     image.fill(qRgb(255, 255, 255));
     image.scaled(QSize(1920,1080));
     modified = true;
	 p1.setX(0);
	 p1.setY(0);
	 p2.setX(0);
	 p2.setY(0);
     update();
 }

 void ScribbleArea::mousePressEvent(QMouseEvent *event)
 {
         if (scribbling)
         return;
         lastPoint = event->pos();
         p1.setX(event->pos().x());
         p1.setY(event->pos().y());
         scribbling = true;
 }

 void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
 {
	  if (!scribbling)
        return;
      p2.setX(event->pos().x());
      p2.setY(event->pos().y());
      if(status==null)
		drawLineTo(event->pos());
	  if(status==circleErase)
		drawLineTo(event->pos());
      if(status==rectErase)
		drawLineTo(event->pos());
      update();
 }

 void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
 {
		if (status == insertText)
		{
			drawLineTo(event->pos());
			scribbling = false;
			return;
		}
         if (!scribbling)
         return;
         if (p2.isNull())
         return;
         emit mouseReleased();
         drawLineTo(event->pos());
         scribbling = false;
 }


 void ScribbleArea::paintEvent(QPaintEvent * /* event */)
 {

     QPainter painter(this);
     painter.drawImage(QPoint(0, 0), image);
//	 image = image.scaled(width(), height());
//	 painter.drawImage(0, 0, image);
     QColor color(0, 0, 0);
     for (int i = 0; i < 1; i++)
     {
         QPainterPath path;
         path.setFillRule(Qt::WindingFill);

         for (int j = 0; j < 1; j++)
         {
             path.addRect(1 - i, j - i, this->width() - (1 - i) * 2, this->height() - (1 - i) * 2);
         }

         painter.setPen(color);
         painter.drawPath(path);
     }
	  
     paint(&painter);
}

 void ScribbleArea::resizeEvent(QResizeEvent *event)
 {
    if (width() > image.width() || height() > image.height())
    {
        int newWidth = qMax(width()+1920, image.width());
        int newHeight = qMax(height()+1080, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
 }

void ScribbleArea::paint(QPainter *painter)
{   
	switch (status) {
        case drawLine:
            painter->drawLine(p1,p2);
            break;
        case drawRect:
            if (p2.x()<p1.x() || p2.y()<p1.y())
                break;
            painter->drawRect(QRectF(p1,p2));
            break;
        case drawRoundRect:
            if (p2.x()<p1.x() || p2.y()<p1.y())
                break;
            painter->drawRoundedRect(QRectF(p1,p2),roundRectR,roundRectR);
            break;
        case drawEllipse:
            painter->drawEllipse(QRectF(p1,p2));
            break;
        case circleErase:
        painter->setPen(QPen(EraseColor, myPenWidth, Qt::SolidLine/*线*/, Qt::RoundCap,
                         Qt::RoundJoin));
        painter->setBrush(QBrush(EraseBrush/*颜色*/));
        painter->drawEllipse(p2,myPenWidth,myPenWidth);
        break;
        case rectErase:
        painter->setPen(QPen(EraseColor, myPenWidth, Qt::SolidLine/*线*/, Qt::RoundCap,
                         Qt::RoundJoin));
        painter->setBrush(QBrush(EraseBrush/*颜色*/));
            painter->drawRect(QRectF(p2,QSize(myPenWidth*2,myPenWidth*2)));
        case null:
            break;
// 		case insertText:
// 			painter->drawText(p1.x(), p1.y(), strText);
//			break;
            }
}

void ScribbleArea::drawLineTo(const QPoint &endPoint)
 {
     QPainter painter(&image);
//	 painter.setRenderHints(QPainter::Antialiasing, true);
	 painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine/*线*/, Qt::RoundCap,
                         Qt::RoundJoin));
     painter.setBrush(QBrush(myPenBrush/*颜色*/));

     switch (status) {
        case drawLine:
            painter.drawLine(lastPoint, endPoint);
            break;
        case drawRect:
            if (p2.x()<p1.x() || p2.y()<p1.y())
                break;
            painter.drawRect(QRectF(lastPoint, endPoint));
            break;
        case drawRoundRect:
            if (p2.x()<p1.x() || p2.y()<p1.y())
                break;
            painter.drawRoundedRect(QRectF(lastPoint, endPoint),roundRectR,roundRectR);
            break;
        case drawEllipse:
            painter.drawEllipse(QRectF(lastPoint, endPoint));
            break;
        case circleErase:
            painter.setPen(QPen(EraseColor, myPenWidth, Qt::SolidLine/*线*/, Qt::RoundCap,
                         Qt::RoundJoin));
            painter.setBrush(QBrush(EraseBrush/*颜色*/));
            painter.drawEllipse(QRectF(lastPoint, endPoint));
            break;
        case rectErase:
            painter.setPen(QPen(EraseColor, myPenWidth, Qt::SolidLine/*线*/, Qt::RoundCap,
                         Qt::RoundJoin));
            painter.setBrush(QBrush(EraseBrush/*颜色*/));
            painter.drawRect(QRectF(p2,QSize(myPenWidth*2,myPenWidth*2)));
        case null:
            painter.drawLine(lastPoint, endPoint);
			break;
		case insertText:
			{
				SetBoardText(painter);
//				painter.drawText(lastPoint.x(), lastPoint.y(), strText);
				break;
			}
			}
     modified = true;
     int rad = (myPenWidth / 2) + 2;
     update(QRect(lastPoint, endPoint).normalized()
                                      .adjusted(-rad, -rad, +rad, +rad));
     lastPoint = endPoint;
 }

void ScribbleArea::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
         return;
    image->scaled(newSize);
    static bool bInit=true;
    if(bInit)
    {
     QImage newImage(newSize, QImage::Format_RGB32);
     newImage.fill(qRgb(255, 255, 255));
//     newImage.fill(QColor(0, 0, 255, 127));
//     QPainter painter(this);
//     painter.drawImage(QPoint(0, 0), *image);
//     painter.setBrush(QColor(255, 0, 0, 0));
//     painter.drawRect(0, 0, 500,500);
     *image = newImage;
    }
    bInit = false;
}


void ScribbleArea::setStatusDrawLine()
{
    status = drawLine;
}

void ScribbleArea::setStatusDrawRect()
{
      status = drawRect;
}

void ScribbleArea::setStatusDrawRoundRect()
{
      status = drawRoundRect;
}
void ScribbleArea::setStatusDrawEllipse()
{
       status = drawEllipse;
}

void ScribbleArea::setStatusNull()
{
       status = null;
}
void  ScribbleArea::setStatusCircleErase()
{

      status = circleErase;
}
void ScribbleArea::setStatusRectErase()
{
      status = rectErase;
}

void ScribbleArea::setStatusText()
{
	status = insertText;
}

void ScribbleArea::SetBoardText(QPainter& painter)
{
	bool ok;
	QString sText = QInputDialog::getText(this, tr("文字 "),
		tr(" 内容: "), QLineEdit::Normal, "", &ok, Qt::WindowCloseButtonHint);
	if (ok)
	{
		strText = sText;
		painter.drawText(p1.x(), p1.y(), strText);
	}
}