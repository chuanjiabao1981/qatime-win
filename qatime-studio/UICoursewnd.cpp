#include "UICoursewnd.h"
#include <QUrl>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QNetworkRequest>
#include <QObject>
#include <QNetworkReply>
#include <QPainter>
#include <QScrollBar>

UICourseWnd::UICourseWnd(QWidget *parent)
	: QWidget(parent)
	, m_btn(NULL)
{
	ui.setupUi(this);
	style(ui.scrollArea);

	QFont font = ui.time_label->font();
	font.setPixelSize(13);
	ui.time_label->setFont(font);

	font = ui.name_label->font();
	font.setPixelSize(15);
	ui.name_label->setFont(font);

	font = ui.press_label->font();
	font.setPixelSize(13);
	ui.press_label->setFont(font);

	font = ui.grade_label->font();
	font.setPixelSize(13);
	ui.grade_label->setFont(font);

	font = ui.time_label->font();
	font.setPixelSize(13);
	ui.time_label->setFont(font);

	font = ui.label->font();
	font.setPixelSize(13);
	ui.label->setFont(font);

	font = ui.desc_label->font();
	font.setPixelSize(13);
	ui.desc_label->setFont(font);
}

UICourseWnd::~UICourseWnd()
{

}

void UICourseWnd::AddInfo(QString pic, QString courseName, QString grade, QString prosses, QString time, QString desc)
{
	setNetworkPic(pic);
	ui.name_label->setText(courseName);
	ui.grade_label->setText(grade);
	ui.press_label->setText(prosses);
	ui.time_label->setText(time);
	ui.desc_label->setText(desc);
	ui.desc_label->setWordWrap(true);

	int fontSize = ui.name_label->fontMetrics().width(courseName);//获取之前设置的字符串的像素大小
	if (fontSize >= ui.name_label->width()) //与label自身相比较
	{
		QString  tmpStr = ui.name_label->fontMetrics().elidedText(courseName, Qt::ElideRight, ui.name_label->width());
		ui.name_label->setText(tmpStr);
		ui.name_label->setToolTip(courseName);
	}

	setFocus();
}

void UICourseWnd::setNetworkPic(const QString &szUrl)
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
	QSize pixSize(ui.pic_label->width(), ui.pic_label->height());
	// 加载成功则显示
	if (pixmap.loadFromData(jpegData))
	{
		QPixmap scaledPixmap = pixmap.scaled(pixSize, Qt::IgnoreAspectRatio);
		ui.pic_label->setPixmap(scaledPixmap);
	}
	else // 否则显示备用图片
	{
		QString sUrl = "./images/teacherPhoto.png";
		pixmap = QPixmap(sUrl);
		QPixmap scaledPixmap = pixmap.scaled(pixSize, Qt::IgnoreAspectRatio);
		ui.pic_label->setPixmap(scaledPixmap);
	}
}

void UICourseWnd::paintEvent(QPaintEvent *event)
{
	QPainterPath path;
	QPainter painter(this);

	QColor color(5, 157, 210);

	path.addRect(0, 0, this->width()-1, this->height()-1);
	painter.setPen(color);
	painter.drawPath(path);
}

void UICourseWnd::focusOutEvent(QFocusEvent* e)
{
	QPoint pt = mapFromGlobal(QCursor::pos());
	pt.setY(pt.y() + geometry().y());
	QRect rc = this->geometry();
	if (rc.contains(pt))
	{
		setFocus();
		return;
	}

	if (m_parent->IsHasFoucs(UIWindowSet::CourseWnd))
		return;

	m_parent->GetCourseBtn()->setStyleSheet("border-image: url(./images/courseBtn_nor.png);");
	this->hide();
}

void UICourseWnd::style(QScrollArea *style)
{
	if (style)
	{
		// 设置滚动条样式
		style->verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
			"{"
			"width:8px;"
			"background:rgba(0,0,0,0%);"
			"margin:0px,0px,0px,0px;"
			"padding-top:9px;"
			"padding-bottom:9px;"
			"}"
			"QScrollBar::handle:vertical"
			"{"
			"width:8px;"
			"background:rgba(0,0,0,25%);"
			" border-radius:4px;"
			"min-height:20;"
			"}"
			"QScrollBar::handle:vertical:hover"
			"{"
			"width:8px;"
			"background:rgba(0,0,0,50%);"
			" border-radius:4px;"
			"min-height:20;"
			"}"
			"QScrollBar::add-line:vertical"
			"{"
			"height:9px;width:8px;"
			"border-image:url(:/images/a/3.png);"
			"subcontrol-position:bottom;"
			"}"
			"QScrollBar::sub-line:vertical"
			"{"
			"height:9px;width:8px;"
			"border-image:url(:/images/a/1.png);"
			"subcontrol-position:top;"
			"}"
			"QScrollBar::add-line:vertical:hover"
			"{"
			"height:9px;width:8px;"
			"border-image:url(:/images/a/4.png);"
			"subcontrol-position:bottom;"
			"}"
			"QScrollBar::sub-line:vertical:hover"
			"{"
			"height:9px;width:8px;"
			"border-image:url(:/images/a/2.png);"
			"subcontrol-position:top;"
			"}"
			"QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
			"{"
			"background:rgba(0,0,0,10%);"
			"border-radius:4px;"
			"}"
			);
	}
}

void UICourseWnd::setParentBtn(UIWindowSet* parent)
{
	m_parent = parent;
}