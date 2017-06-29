#include "DefDateTimeEdit.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>

DefDateTimeEdit::DefDateTimeEdit(QWidget *parent)
	: QDateTimeEdit(parent)
{
//	setFixedSize(1, 1);
	this->setCalendarPopup(true);
	m_DefCalendar = new DefineCalendar(this);
	m_DefCalendar->setLocale(QLocale(QLocale::Chinese));

	setCalendarWidget(m_DefCalendar);

//	m_DefCalendar->disconnect(SIGNAL(clicked(QDate)));
	connect(m_DefCalendar, SIGNAL(setFinished(QDateTime)), this, SLOT(getDateTime(QDateTime)));

	
	connect(m_DefCalendar, SIGNAL(clicked(QDate)), this, SLOT(CalendarClick(QDate)));
	
	setMyStytle();
	
	this->setDateTime(QDateTime::currentDateTime());

	this->setDisplayFormat("yyyy-MM-dd");
	//设置最大日期，超出日期不可选 add by zbc 20170628
	this->setMaximumDate(QDate::currentDate());	
}

DefDateTimeEdit::~DefDateTimeEdit()
{

}



void DefDateTimeEdit::CalendarClick(QDate date)
{
	emit sig_CalendarClick(date);
}

void DefDateTimeEdit::getDateTime( const QDateTime &dateTime )
{
	this->setDateTime(dateTime);
}

void DefDateTimeEdit::setMyStytle()
{
	QString strTemp;
	//QWidget
	strTemp.append("QWidget{font:normal 10pt Microsoft YaHei;}");
	strTemp.append("QWidget#CalTopWidget{background-color:rgb(5, 157, 213);}");
	strTemp.append("QWidget#CalBottomWidget{background-color:white;}");
	//QLabel
	strTemp.append("QLabel#CalLabel{border:1px solid lightgray; color:rgb(5, 157, 213);}");

	//QPushButton
	strTemp.append("QPushButton#CalPushBtnT1{border:0px;}");
	strTemp.append("QPushButton#CalPushBtnT1:hover,QPushButton#CalPushBtnT1:pressed\
				   {background-color:rgb(5, 115, 213);}");
	strTemp.append("QPushButton#CalPushBtnT2{border:1px solid lightgray; color:rgb(5, 157, 213);}");
	strTemp.append("QPushButton#CalPushBtnT2:hover,QPushButton#CalPushBtnT2:pressed\
				   {background-color:rgb(231, 231, 231);}");
	//QComboBox
	strTemp.append("QComboBox#CalComboBox{border:0px; background-color:rgb(5, 157, 213); \
				   color:white; height:24px; width:40px;}\
				   QComboBox#CalComboBox::down-arrow{\
				   image:url(images/ad.png);}\
				   QComboBox#CalComboBox::drop-down{width:14px; border:0px;}\
				   QComboBox#CalComboBox QAbstractItemView {\
				   color:rgb(5, 157, 213);\
				   border: 1px solid rgb(5, 157, 213);\
				   background-color:white;\
				   selection-color:white;\
				   selection-background-color: rgb(5, 115, 213);}");
	//QTimeEdit
	strTemp.append("QTimeEdit#CalTimeEdit{ color:rgb(5, 157, 213);}");
	strTemp.append("QTimeEdit#CalTimeEdit:!enabled{ background:rgb(65, 65, 65); color:rgb(90, 90, 90); border:0px; }");
	strTemp.append("QTimeEdit#CalTimeEdit::up-button{  background:rgb(5, 157, 213);width: 16px;  border-width: 1px;}");
	strTemp.append("QTimeEdit#CalTimeEdit::up-button:hover{ background:rgb(5, 115, 213); }");
	strTemp.append("QTimeEdit#CalTimeEdit::up-button:!enabled{ background:rgb(65, 65, 65); }");
	strTemp.append("QTimeEdit#CalTimeEdit::up-arrow{  image:url(images/au.png);}");

	strTemp.append("QTimeEdit#CalTimeEdit::down-button{  background:rgb(5, 157, 213); width: 16px;  border-width: 1px;}");
	strTemp.append("QTimeEdit#CalTimeEdit::down-button:hover{ background:rgb(5, 115, 213); }");
	strTemp.append("QTimeEdit#CalTimeEdit::down-button:!enabled{ background:rgb(65, 65, 65); }");
	strTemp.append("QTimeEdit#CalTimeEdit::down-arrow{  image:url(images/ad.png);}");

	//QDateTimeEdit
	strTemp.append("QDateTimeEdit{border:1px solid white;color:rgb(5,157,213); height:24px; }");
	strTemp.append("QDateTimeEdit::down-arrow{image:url(images/calendar.png);}");
	strTemp.append("QDateTimeEdit::drop-down{width:30px; border:0px solid red;\
				   subcontrol-origin: padding;subcontrol-position: top right;}");

	//QScrollBar
	strTemp.append("QScrollBar:vertical{background-color:white; width: 10px;}\
				   QScrollBar::handle:vertical{background-color:rgb(5, 157, 213); border:1px solid white;border-radius:2px; min-height:8px}\
				   QScrollBar::handle:vertical:hover{background-color:rgb(0, 102, 96);}\
				   QScrollBar::sub-line{background-color:white;}\
				   QScrollBar::add-line{background-color:white;}");

	this->setStyleSheet(strTemp);
}