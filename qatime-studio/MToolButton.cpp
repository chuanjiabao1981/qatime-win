/*
The MIT License (MIT)

Copyright (c) wenjie.zhao

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <QPainter>
#include <QMouseEvent>

#include "MToolButton.h"
//#include "mutility.h"
#include <QPixmap>
#define DEFAULT_FONT_COLOR "#646464"
#define SELECTED_FONT_COLOR "#fe4f00"

MToolButton::MToolButton(QWidget *parent)
    : QToolButton(parent)
    , alpha(0)
    , status(STATUS_NORMAL)
{
    setStyleSheet("QToolButton{border:0px;}");
    m_fontColor = DEFAULT_FONT_COLOR;
    m_cursor = cursor();
    timeLine.setDuration(200);
    connect(&timeLine, SIGNAL(frameChanged(int)), this, SLOT(frameChanged(int)));
    connect(&timeLine, SIGNAL(finished()), this, SLOT(finished()));
    timeLine.setFrameRange(0, 10);
}

MToolButton::~MToolButton()
{
    timeLine.stop();
}

void MToolButton::setPixmap(const QPixmap &pixmap, int num)
{
    int nPixWidth = pixmap.width();
    int nPixHeight = pixmap.height();

    m_normal = pixmap.copy( 0, 0, nPixWidth/num, nPixHeight);
    m_hover = pixmap.copy( nPixWidth/num , 0, nPixWidth/num, nPixHeight);
    m_pressed = pixmap.copy( nPixWidth/num * 2, 0, nPixWidth/num, nPixHeight);
    if(num == 4)
        m_disabled = pixmap.copy( nPixWidth/num * 3, 0, nPixWidth/num, nPixHeight);
    else if(num == 3)
    {
        m_disabled = convertGray(m_normal);
    }



    if(!m_normal.isNull())
    {
        setFixedSize((nPixWidth)/num, nPixHeight);
        setIcon(m_normal);
        setIconSize(QSize(nPixWidth/num, nPixHeight));
    }

    resultImage = m_hover.toImage();
    sourceImage = m_hover.toImage();

    setIcon(m_normal);
}

void MToolButton::setButtonStatus(int status)
{
    timeLine.stop();

    QPixmap bg;
    switch(status)
    {
    case STATUS_NORMAL:
        bg = m_normal;
        break;
    case STATUS_HOVE:
        bg = m_hover;
        break;
    case STATUS_PRESS:
        bg = m_pressed;
        break;
    case STATUS_DISABLED:
        bg = m_disabled;
        break;
    }

    setIcon(bg);
}
void MToolButton::setIsAllwaysStatus(bool status ,int nums)
{
    isAllwaysStatus = status;
    m_nums = nums;
}

void MToolButton::paintEvent(QPaintEvent *event)
{
    QToolButton::paintEvent(event);
    QPainter p(this);

    QPixmap bg(width(), height());
    bg.fill(QColor(255, 255, 255, 0));
    p.drawPixmap(rect(), bg);

    if(!text().isEmpty())
    {
        p.setPen(m_fontColor);
        p.setBrush(m_fontColor);
        p.drawText(rect(), Qt::AlignHCenter | Qt::AlignVCenter, text());
    }
}

void MToolButton::enterEvent(QEvent *event)
{
    if(isAllwaysStatus)
    {
        status = STATUS_HOVE;
        if(isEnabled())
        {
            setCursor(QCursor(Qt::PointingHandCursor));
            setIcon(m_hover);
            timeLine.stop();
            timeLine.setDirection(QTimeLine::Forward);
            //timeLine.start();
        }
        else
            setIcon(m_disabled);
    }
    QToolButton::enterEvent(event);
}

void MToolButton::mouseMoveEvent(QMouseEvent *event)
{
    event->accept();
}

void MToolButton::changeEvent(QEvent *e)
{
    if(isAllwaysStatus)
    {
        if(e->type() == QEvent::EnabledChange)
        {
            if(!this->isEnabled())
            {
                setIcon(m_disabled);
            }else
            {
                status = STATUS_NORMAL;
                setCursor(m_cursor);
                timeLine.stop();
                timeLine.setDirection(QTimeLine::Backward);
                //timeLine.start();
                setIcon(m_normal);

            }
        }
    }
}
QPixmap MToolButton :: setAlphaPixmap(const QPixmap &image, int alpha)
{
    QImage im = image.toImage();
    return setAlphaPixmap(im, alpha);
}
QPixmap MToolButton ::setAlphaPixmap(const QImage &image, int alpha)
{
    QImage resultImage=image;
    QImage sourceImage=image;

    QPainter painter(&resultImage);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(resultImage.rect(), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, sourceImage);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painter.fillRect(resultImage.rect(), QColor(0, 0, 0, alpha));
    painter.end();

    return QPixmap::fromImage(resultImage);
}

void MToolButton::frameChanged(int frame)
{
    setIcon(setAlphaPixmap(m_hover, frame * 25) );
}


void MToolButton::finished()
{
    int direction = timeLine.direction();
    switch(direction)
    {
    case QTimeLine::Backward:
        setIcon(m_normal);
    }
}

const QPixmap MToolButton::convertGray(const QPixmap &pix)
{
    QImage img = pix.toImage();
    int w = img.width();
    int h = img.height();
    QImage iGray(w,h,QImage::Format_ARGB32_Premultiplied);
    for(int i=0; i<w; i++)
    {
        for(int j=0; j<h; j++)
        {
            QRgb pixel = img.pixel(i,j);
            int gray = qGray(pixel);
            int alpha = qAlpha(pixel);
            QRgb grayPixel = qRgba(gray,gray,gray,alpha);
            iGray.setPixel(i, j, grayPixel);
        }
    }
    return QPixmap::fromImage(iGray);
}

void MToolButton::leaveEvent(QEvent *event)
{
    if(isAllwaysStatus)
    {
        status = STATUS_NORMAL;
        setCursor(m_cursor);
        if(isEnabled())
        {
            timeLine.stop();
            timeLine.setDirection(QTimeLine::Backward);
            //timeLine.start();
            setIcon(m_normal);
        }
        else
            setIcon(m_disabled);
    }
    QToolButton::leaveEvent(event);
}

void MToolButton::mousePressEvent(QMouseEvent *event)
{
    if(isAllwaysStatus)
    {
        timeLine.stop();
        status = STATUS_PRESS;
        if(isEnabled())
            setIcon(m_pressed);
        else
            setIcon(m_disabled);
    }
    QToolButton::mousePressEvent(event);
}

void MToolButton::mouseReleaseEvent(QMouseEvent *event)
{
    if(isAllwaysStatus)
    {
        status = STATUS_HOVE;
        if(isEnabled())
            setIcon(m_hover);
        else
            setIcon(m_disabled);
    }
    QToolButton::mouseReleaseEvent(event);
}
