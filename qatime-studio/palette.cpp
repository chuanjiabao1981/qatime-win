#include "palette.h"
#include "ui_palette.h"

#include <QMoveEvent>
#include <QPainter>
#include <QDebug>
#include <QColorDialog>
#include <QTimer>
#include "shape.h"
#include "windows.h"

#define  COLOR_VALUE_BLACK QColor(0, 0, 0)
#define  COLOR_VALUE_RED QColor(204, 0, 0)
#define  COLOR_VALUE_YELLOW QColor(255, 204, 0)
#define  COLOR_VALUE_BLUE QColor(0, 153, 255)
#define  COLOR_VALUE_GREEN QColor(0, 153, 0)
#define  COLOR_VALUE_PURPLE QColor(255, 51, 255)

Palette::Palette(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Palette)
	, m_timer(NULL)
	, mStatus(0)
{
    ui->setupUi(this);
	setMouseTracking(true);
    mMouseIsPress = false;

    mPen.setColor(Qt::black);
    mPen.setWidth(2);

	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_onCountTimeout()));
}

void Palette::slot_onCountTimeout()
{
	
}

Palette::~Palette()
{
    delete ui;

    foreach(Shape *shape, mShapeStack)
    {
        delete shape;
    }

	foreach(Shape *shape, mSycnShapeStack)
	{
		delete shape;
	}
}

void Palette::setPenColor(const QColor &color)
{
    mPen.setColor(color);
}

QColor Palette::getPenColor()
{
	return mPen.color();
}

// 撤销
void Palette::revocation()
{
	if (mShapeStack.isEmpty())
	{
		return;
	}
	delete mShapeStack.last();
	mShapeStack.removeLast();

	QString strInfo;
	strInfo.append(QString("%1:%2,%3,%4;").arg(QString::number(DrawOpUndo)).arg("").arg("").arg(""));
	emit PicData(strInfo, "");
	update();
}

// 清空
void Palette::cleanUp()
{
	foreach(Shape *shape, mShapeStack)
	{
		delete shape;
	}
	mShapeStack.clear();

	foreach(Shape *shape, mSycnShapeStack)
	{
		delete shape;
	}
	mSycnShapeStack.clear();
	update();

	QString opType = QString::number(DrawOpClear);	//  消息类型
	QString strInfo;
	strInfo.append(QString("%1:%2,%3,%4;").arg(opType).arg("").arg("").arg(""));
	emit PicData(strInfo, "");
}

void Palette::setIsDraw(bool isDraw)
{
	mIsDraw = isDraw;
	setMouseStyle(mIsDraw);
}

void Palette::setMouseStyle(bool isDraw)
{
	if (isDraw)
	{

		QString opType = QString::number(kMultiBoardOpSignEnd);//  消息类型
		QString strInfo;
		strInfo.append(QString("%1:%2,%3,%4;").arg(opType).arg("").arg("").arg(""));
		emit PicData(strInfo,"");

		QCursor cursor;
		QPixmap pixmap(":/LoginWindow/images/pen.png");
		cursor = QCursor(pixmap, 0, pixmap.height());
		setCursor(cursor);
	}
	else
	{
		QCursor cursor;
		QPixmap pixmap("./images/pensign.png");
		cursor = QCursor(pixmap, 0, pixmap.height());
		setCursor(cursor);
	}
}

void Palette::paintEvent(QPaintEvent *)
{
	if (this->isVisible())
	{
		QPainter paint(this);
		paint.setBrush(Qt::white);
		paint.setPen(Qt::white);   //绘制画板区域
		QSize lsize = size();
		paint.drawRect(0, 0, lsize.width(), lsize.height());

		foreach(Shape *shape, mShapeStack)
		{
			shape->paint(paint, lsize);
		}

		foreach(Shape *shape, mSycnShapeStack)
		{
			shape->paint(paint, lsize);
		}
	}
}

void Palette::mousePressEvent(QMouseEvent *event)
{
	if (!mIsDraw)
	{
		return;
	}
    switch (event->button()) {
    case Qt::LeftButton:
    {
        mMouseIsPress = true;
        Shape *shape = new Shape();
        mShapeStack.append(shape);

		QPoint point = event->pos();
		QSize lSize = size();
		QPointF pointf(point.x() * 1.0 / lSize.width(), point.y() * 1.0 / lSize.height());

		shape->append(pointf);
        shape->setPen(mPen);
        update();

		float x = (float)(point.x() * 1.0 / lSize.width());
		float y = (float)(point.y() * 1.0 / lSize.height());
		QString ptX = QString("%1").arg(x);				//	x的相对坐标
		QString ptY = QString("%1").arg(y);				//  y的相对坐标
		QString opType = QString::number(DrawOpStart);	//  消息类型
		DWORD clr = colorConvert(getPenColor());
		QString sClr = QString::number(clr);			//  颜色类型
		QString strInfo;
		strInfo.append(QString("%1:%2,%3,%4;").arg(opType).arg(ptX).arg(ptY).arg(sClr));
		emit PicData(strInfo,"");
    }
        break;
    default:
        break;
    }	
}

void Palette::mouseMoveEvent(QMouseEvent *event)
{
	if (!mIsDraw)
	{
		QPoint point = event->pos();
		QSize lSize = size();

		update();

		float x = (float)(point.x() * 1.0 / lSize.width());
		float y = (float)(point.y() * 1.0 / lSize.height());
		QString ptX = QString("%1").arg(x);				//	x的相对坐标
		QString ptY = QString("%1").arg(y);				//  y的相对坐标
		QString opType = QString::number(kMultiBoardOpSign);//  消息类型
		DWORD clr = colorConvert(getPenColor());
		QString sClr = QString::number(clr);			//  颜色类型
		QString strInfo;
		strInfo.append(QString("%1:%2,%3,%4;").arg(opType).arg(ptX).arg(ptY).arg(sClr));
		qDebug() << strInfo;
		emit PicData(strInfo, "");
		return;
	}

    if(mMouseIsPress)
    {
		QPoint point = event->pos();
		QSize lSize = size();
		QPointF pointf(point.x() * 1.0 / lSize.width(), point.y() * 1.0 / lSize.height());
		mShapeStack.last()->append(pointf);
        update();

		float x = (float)(point.x() * 1.0 / lSize.width());
		float y = (float)(point.y() * 1.0 / lSize.height());
		QString ptX = QString("%1").arg(x);				//	x的相对坐标
		QString ptY = QString("%1").arg(y);				//  y的相对坐标
		QString opType = QString::number(DrawOpMove);	//  消息类型
		DWORD clr = colorConvert(getPenColor());
		QString sClr = QString::number(clr);			//  颜色类型
		QString strInfo;
		strInfo.append(QString("%1:%2,%3,%4;").arg(opType).arg(ptX).arg(ptY).arg(sClr));
		qDebug() << strInfo;
		emit PicData(strInfo, "");
    }
}

void Palette::mouseReleaseEvent(QMouseEvent *event)
{
	if (!mIsDraw)
	{
		return;
	}
    if(Qt::LeftButton == event->button())
    {
        mMouseIsPress = false;
        update();

		QPoint point = event->pos();
		QSize lSize = size();
		float x = (float)(point.x() * 1.0 / lSize.width());
		float y = (float)(point.y() * 1.0 / lSize.height());
		QString ptX = QString("%1").arg(x);				//	x的相对坐标
		QString ptY = QString("%1").arg(y);				//  y的相对坐标
		QString opType = QString::number(DrawOpEnd);	//  消息类型
		DWORD clr = colorConvert(getPenColor());
		QString sClr = QString::number(clr);			//  颜色类型
		QString strInfo;
		strInfo.append(QString("%1:%2,%3,%4;").arg(opType).arg(ptX).arg(ptY).arg(sClr));
		emit PicData(strInfo, "");
    }
}

void Palette::RecData(const std::string& data, const std::string &uid)
{
	m_SenderUid =  uid;
	qDebug() << data.c_str();
	float x;
	float y;
	DWORD clr_;
	int draw_op_type_;
	int pos = data.find(';');
	if (pos != -1)
	{
		std::string	cur_data;
		int pos_type = data.find(":");
		draw_op_type_ = atoi(data.substr(0, pos_type).c_str());
		cur_data = data.substr(pos_type + 1);

		if (draw_op_type_ == kMultiBoardOpSyncQuery)//同步消息
		{
			LONG64 timeX;
// 			std::list<std::string> param_list = BoardStringTokenize(cur_data.c_str(), ",");
// 			if (param_list.size() > 0)
// 			{
// 				auto it = param_list.begin();
// 				timeX = atoll(it->c_str());
// 			}
			SendSyncDraw(timeX);
			return;
		}
					 
		std::list<std::string> param_list = BoardStringTokenize(cur_data.c_str(), ",");
		if (param_list.size() > 0)
		{
			auto it = param_list.begin();
			switch (draw_op_type_)
			{
			case kMultiBoardOpStart:
			case kMultiBoardOpMove:
			case kMultiBoardOpEnd:
			case kMultiBoardOpSign:
				if (param_list.size() >= 3)
				{
					x = atof(it->c_str());
					++it;
					y = atof(it->c_str());
					++it;
					clr_ = atof(it->c_str());
				}
				break;
			case kMultiBoardOpClear:
			case kMultiBoardOpSignEnd:
				break;
			case  kMultiBoardOpUndo:
				{
					DrawUndo();
					return;
				}
			}
		}
	}
//	qDebug() << "x; " << QString::number(pt.x()) << "y: " << QString::number(pt.y());
	QPointF pt(x, y);
	if (mSycnShapeStack.size()>0 && draw_op_type_ != kMultiBoardOpStart)
	{
		mSycnShapeStack.last()->append(pt);
		update();
	}
	else
	{
		Shape *shape = new Shape();
		mSycnShapeStack.append(shape);
		shape->append(pt);
		QPen pen;
		pen.setWidth(2);
		pen.setColor(QColor(QRgb(clr_)));
		shape->setPen(pen);
		update();
	}
}

void Palette::DrawUndo()
{
	if (mSycnShapeStack.isEmpty())
	{
		return;
	}
	delete mSycnShapeStack.last();
	mSycnShapeStack.removeLast();
	update();
}

// 解析数据
std::list<std::string> Palette::BoardStringTokenize(const char* input, const char* delimitor)
{
	std::list<std::string> ret_list;
	std::string data = input;
	int pos = data.find(delimitor);
	while (pos != -1)
	{
		ret_list.push_back(data.substr(0, pos));
		data = data.substr(pos + 1);
		pos = data.find(delimitor);
	}
	ret_list.push_back(data);
	return ret_list;
}

int Palette::colorConvert(QColor color)
{
	int clr;
	if (color == COLOR_VALUE_BLACK)
		clr = 0x000000;
	else if (color == COLOR_VALUE_RED)
		clr = 0xcc0000;
	else if (color == COLOR_VALUE_YELLOW)
		clr = 0xffcc00;
	else if (color == COLOR_VALUE_BLUE)
		clr = 0x228bf8;
	else if (color == COLOR_VALUE_GREEN)
		clr = 0x009900;
	else if (color == COLOR_VALUE_PURPLE)
		clr = 0xff33ff;
	else if (color == 0x7dd21f)
		clr = 0x7dd21f;
	else if (color == 0x228bf7)
		clr = 0x228bf7;
	else if (color == 0xd1021c)
		clr = 0xd1021c;
	else if (color == 0xfddc01)
		clr = 0xfddc01;
	else if (color == 0x9b0df5)
		clr = 0x9b0df5;

	return clr;
}

void Palette::SendSyncDraw(LONG64 timeX)
{
//	ReturnSync(timeX);

	SendFullScreen(mStatus);
	
	foreach(Shape *shape, mShapeStack)
	{
		QString strMoveInfo;
		foreach(QPointF pt, shape->PointVec())
		{
			QString ptX, ptY, opType, sClr, strInfo;
			DWORD clr;
			if (pt == shape->PointVec().first())
			{
				ptX = QString("%1").arg(pt.x());				//	x的相对坐标
				ptY = QString("%1").arg(pt.y());				//  y的相对坐标
				opType = QString::number(DrawOpStart);			//  消息类型
				clr = colorConvert(shape->penColor());
				sClr = QString::number(clr);			//  颜色类型
				strInfo.append(QString("%1:%2,%3,%4;").arg(opType).arg(ptX).arg(ptY).arg(sClr));
				emit PicData(strInfo, "");
			}
			else if (pt == shape->PointVec().last())
			{
				emit PicData(strMoveInfo, "");
				ptX = QString("%1").arg(pt.x());				//	x的相对坐标
				ptY = QString("%1").arg(pt.y());				//  y的相对坐标
				opType = QString::number(DrawOpEnd);			//  消息类型
				clr = colorConvert(shape->penColor());
				sClr = QString::number(clr);			//  颜色类型
				strInfo.append(QString("%1:%2,%3,%4;").arg(opType).arg(ptX).arg(ptY).arg(sClr));
				emit PicData(strInfo, "");
			}
			else
			{
				ptX = QString("%1").arg(pt.x());				//	x的相对坐标
				ptY = QString("%1").arg(pt.y());				//  y的相对坐标
				opType = QString::number(DrawOpMove);			//  消息类型
				clr = colorConvert(shape->penColor());
				sClr = QString::number(clr);			//  颜色类型
				strInfo.append(QString("%1:%2,%3,%4;").arg(opType).arg(ptX).arg(ptY).arg(sClr));

				// 把移动点存起来，再结束之前一起发送
				strMoveInfo += strInfo;
			}
		}
	}

	foreach(Shape *shape, mSycnShapeStack)
	{
		QString strMoveInfo;
		QString strSync;
		strSync.append(QString("%1:%2,%3;").arg(kMultiBoardOpSync).arg(QString::fromStdString(m_SenderUid)).arg(0));
		strMoveInfo += strSync;
		foreach(QPointF pt, shape->PointVec())
		{
			QString ptX, ptY, opType, sClr, strInfo;
			DWORD clr;
			if (pt == shape->PointVec().first())
			{
				ptX = QString("%1").arg(pt.x());				//	x的相对坐标
				ptY = QString("%1").arg(pt.y());				//  y的相对坐标
				opType = QString::number(DrawOpStart);			//  消息类型
				clr = colorConvert(shape->penColor());
				sClr = QString::number(clr);					//  颜色类型
				strInfo.append(QString("%1:%2,%3,%4;").arg(opType).arg(ptX).arg(ptY).arg(sClr));
				strMoveInfo += strInfo;
			}
			else if (pt == shape->PointVec().last())
			{
				ptX = QString("%1").arg(pt.x());				//	x的相对坐标
				ptY = QString("%1").arg(pt.y());				//  y的相对坐标
				opType = QString::number(DrawOpEnd);			//  消息类型
				clr = colorConvert(shape->penColor());
				sClr = QString::number(clr);					//  颜色类型
				strInfo.append(QString("%1:%2,%3,%4;").arg(opType).arg(ptX).arg(ptY).arg(sClr));
				strMoveInfo += strInfo;
			}
			else
			{
				ptX = QString("%1").arg(pt.x());				//	x的相对坐标
				ptY = QString("%1").arg(pt.y());				//  y的相对坐标
				opType = QString::number(DrawOpMove);			//  消息类型
				clr = colorConvert(shape->penColor());
				sClr = QString::number(clr);
				strInfo.append(QString("%1:%2,%3,%4;").arg(opType).arg(ptX).arg(ptY).arg(sClr));

				// 把移动点存起来，再结束之前一起发送
				strMoveInfo += strInfo;
			}
		}

		emit PicData(strMoveInfo, "");
		qDebug() << strMoveInfo;
	}
}

void Palette::SendFullScreen(int iOpen)
{
	mStatus = iOpen;
	QString strInfo;
	strInfo.append(QString("%1:%2,%3,%4;").arg(kMultiBoardOpFullScreen).arg(iOpen).arg("0").arg("0"));
	emit PicData(strInfo, "");
}

void Palette::ReturnSync(LONG64 timeX)
{
	QString strInfo;
	strInfo.append(QString("%1:%2,%3,%4;").arg(kMultiBoardOpSyncTime).arg(timeX).arg("0").arg("0"));
	emit PicData(strInfo, QString::fromStdString(m_SenderUid));
}