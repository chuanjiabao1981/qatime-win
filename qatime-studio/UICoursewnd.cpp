#include "UICoursewnd.h"
#include <QUrl>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QNetworkRequest>
#include <QObject>
#include <QNetworkReply>
#include <QPainter>
#include <QScrollBar>

extern QString mPublicTeacherPicturePath;	// 保存教师头像路径的全局变量
extern QString mPublicGender;				// 保存用户性别

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
/*
	font = ui.grade_label->font();
	font.setPixelSize(13);
	ui.grade_label->setFont(font);
*/
	font = ui.time_label->font();
	font.setPixelSize(13);
	ui.time_label->setFont(font);
	/*
	font = ui.label->font();
	font.setPixelSize(13);
	ui.label->setFont(font);
	*/
	font = ui.desc_label->font();
	font.setPixelSize(13);
	ui.desc_label->setFont(font);
	
	ui.desc_label->setWordWrap(true);
	ui.lbl_CourseTarget->setWordWrap(true);
	ui.lbl_FitPeople->setWordWrap(true);

	
	ui.lbl_CourseTarget->setMaximumWidth(300);
	ui.lbl_FitPeople->setMaximumWidth(300);
	ui.desc_label->setMaximumWidth(300);
	ui.scrollArea->setFixedWidth(408);
}


UICourseWnd::~UICourseWnd()
{

}

void UICourseWnd::CleanCourseInfo()
{
	// 设置Tag标签最初为隐藏状态
	ui.lblTagTitle->setVisible(false);
	ui.lblSpace->setVisible(false);
	QLabel *mCurrentLabel = new QLabel;
	QString mTagObjectName = "";
	int i = 0;
	for (i = 1; i < 7; i++)
	{
		mTagObjectName = "lbl_Tag";
		mTagObjectName += QString::number(i);
		mCurrentLabel = this->findChild<QLabel *>(mTagObjectName);
		if (mCurrentLabel != NULL)
		{
			mCurrentLabel->setVisible(false);
			mCurrentLabel->setText("");
		}
	}
	mCurrentLabel = NULL;
	delete mCurrentLabel;
}

void UICourseWnd::AddCourseTag(QJsonArray mJsonTags)
{
	ui.lblTagTitle->setVisible(true);
	ui.lblSpace->setVisible(true);
	int i = 1;
	QString mTagName = "";
	QLabel *mCurrentLabel = new QLabel;
	QString mTagObjectName = "";
	foreach(QJsonValue &mValue, mJsonTags)
	{
		mTagName = mValue.toString();
		mTagObjectName = "lbl_Tag";
		mTagObjectName += QString::number(i);
		mCurrentLabel = this->findChild<QLabel *>(mTagObjectName);
		if (mCurrentLabel != NULL)
		{
			mCurrentLabel->setVisible(true);
			mCurrentLabel->setText(mTagName);
			mCurrentLabel->setStyleSheet("border-width:1px;border-style:solid;border-color:rgb(204, 204, 204);color:rgb(102, 102, 102);");
			mCurrentLabel->setWindowFlags(Qt::WindowStaysOnTopHint);

		}
		i++;
		
	}
	mCurrentLabel = NULL;
	delete mCurrentLabel;
	
}

void UICourseWnd::AddInfo(QString pic, QString courseName, QString grade, QString prosses, QString time, QString desc, QString mCourseTarget, QString mFitPeople, QString mCourseProcesses, QString mName, QString mClassName)
{
	setNetworkPic(pic);
	ui.name_label->setText(mClassName);
	ui.lbl_CourseName->setText(courseName);
	ui.lbl_Grade->setText(grade);
	ui.lbl_StatusAndProcess->setText(prosses);
	ui.lbl_Time->setText(time);
	ui.desc_label->setText(desc);
	ui.lbl_TotalCourseCount->setText("共" + mCourseProcesses + "节课");

	ui.lbl_CourseTarget->setText(mCourseTarget);
	ui.lbl_FitPeople->setText(mFitPeople);

	QString mHeadPortriatPath = mPublicTeacherPicturePath;
	QFile file(mHeadPortriatPath);
	bool mIsPicExist = false;
	QPixmap mPixmap, scaledPixmap;
	QSize pixSize(20, 20);
	mIsPicExist = (file.exists() == true ? true : false);
	if (mIsPicExist)
	{
		mPixmap = QPixmap(mHeadPortriatPath);
		QPixmap scaledPixmap;
		scaledPixmap = mPixmap.scaled(pixSize, Qt::IgnoreAspectRatio);
		//mPixmap.scaled(pixSize, Qt::IgnoreAspectRatio);
		ui.lbl_HeadPortrait->setPixmap(scaledPixmap);
	}
	ui.lbl_Name->setText(mName);
	if (mPublicGender == "male")
	{
		ui.lbl_SexPic->setStyleSheet("image: url(:/LoginWindow/images/male.png);");
	}
	else
	{
		ui.lbl_SexPic->setStyleSheet("image: url(:/LoginWindow/images/woman.png);");
	}

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
		QString sUrl = "./images/course_default.png";
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