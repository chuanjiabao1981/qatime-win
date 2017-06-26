#include "DefineCalendar.h"
#include <QtWidgets/QtWidgets>

DefineCalendar::DefineCalendar(QWidget *parent)
	: QCalendarWidget(parent)
{
	setMinimumDate(QDate(2017,3,21));
	setMaximumDate(QDate(2050,1,1));
	InitWidgets();
	setMinimumHeight(224);
	setNavigationBarVisible(false);
	setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
	this->layout()->setSizeConstraint(QLayout::SetFixedSize);
	connect(this, SIGNAL(currentPageChanged(int,int)), this, SLOT(CurPageChange(int,int)));
	UpdateYear();

// 	QTextCharFormat format;
// 	QBrush brush(QColor(5, 157, 213));
// 	format.setBackground(brush);  //设置格式，颜色自选
// 	this->setWeekdayTextFormat(Qt::Sunday, format);
// 	this->setWeekdayTextFormat(Qt::Monday, format);
// 	this->setWeekdayTextFormat(Qt::Tuesday, format);
// 	this->setWeekdayTextFormat(Qt::Wednesday, format);
// 	this->setWeekdayTextFormat(Qt::Thursday, format);
// 	this->setWeekdayTextFormat(Qt::Friday, format);
// 	this->setWeekdayTextFormat(Qt::Saturday, format);
// 
// 	format.setForeground(Qt::white);
// 	this->setWeekdayTextFormat(Qt::Sunday, format);
// 	this->setWeekdayTextFormat(Qt::Monday, format);
// 	this->setWeekdayTextFormat(Qt::Tuesday, format);
// 	this->setWeekdayTextFormat(Qt::Wednesday, format);
// 	this->setWeekdayTextFormat(Qt::Thursday, format);
// 	this->setWeekdayTextFormat(Qt::Friday, format);
// 	this->setWeekdayTextFormat(Qt::Saturday, format);
}

DefineCalendar::~DefineCalendar()
{

}

void DefineCalendar::paintCell( QPainter *painter, const QRect &rect, const QDate &date ) const
{
 	if (date == this->selectedDate())
 	{
 		painter->save();
 		painter->setRenderHint(QPainter::Antialiasing);
 		painter->setPen(Qt::NoPen);
		painter->setBrush(QColor(5, 157, 213));
 		painter->drawRect(rect);
 		painter->setPen(Qt::white);
 		painter->drawText(rect,Qt::AlignCenter,QString::number(date.day()));
 		painter->restore();
 	}
 	else
	{
		QCalendarWidget::paintCell(painter,rect,date);
	}
	
}

void DefineCalendar::InitWidgets()
{
	//顶部
	widget_top = new QWidget(this);
	comboBox_Year = new QComboBox(this);
	comboBox_Month = new QComboBox(this);
	pushBtn_ML = new QPushButton(this);
	pushBtn_MR = new QPushButton(this);

	comboBox_Year->setView(new QListView());
	comboBox_Month->setView(new QListView());

	QStringList monthList;
	monthList << tr("一月") << tr("二月") << tr("三月") << tr("四月") << tr("五月") << tr("六月")
		<< tr("七月") << tr("八月") << tr("九月") << tr("十月") << tr("十一月") << tr("十二月");
	comboBox_Month->addItems(monthList);

	
	int nO = 24;
	int nI = 20;

	pushBtn_ML->setFixedSize(nO,nO);
	pushBtn_ML->setIconSize(QSize(nI,nI));
	pushBtn_ML->setIcon(QPixmap("images/al.png"));

	pushBtn_MR->setFixedSize(nO,nO);
	pushBtn_MR->setIconSize(QSize(nI,nI));
	pushBtn_MR->setIcon(QPixmap("images/ar.png"));

	QHBoxLayout *HTopLayout = new QHBoxLayout;
	HTopLayout->setContentsMargins(4,4,4,4);
	HTopLayout->setSpacing(0);
	widget_top->setLayout(HTopLayout);
	HTopLayout->addWidget(pushBtn_ML);
	HTopLayout->addStretch(1);
	HTopLayout->addWidget(comboBox_Year);
	HTopLayout->addStretch(1);
	HTopLayout->addWidget(comboBox_Month);
	HTopLayout->addStretch(1);
	HTopLayout->addWidget(pushBtn_MR);

	QVBoxLayout *VMainLayout = qobject_cast<QVBoxLayout *>(this->layout());
	VMainLayout->insertWidget(0,widget_top);

	//类名，信号和槽
	widget_top->setObjectName("CalTopWidget");
	comboBox_Year->setObjectName("CalComboBox");
	comboBox_Month->setObjectName("CalComboBox");
	pushBtn_ML->setObjectName("CalPushBtnT1");
	pushBtn_MR->setObjectName("CalPushBtnT1");

	connect(pushBtn_ML, SIGNAL(clicked()), this, SLOT(BtnSlots()));
	connect(pushBtn_MR, SIGNAL(clicked()), this, SLOT(BtnSlots()));

	connect(comboBox_Year, SIGNAL(currentIndexChanged(int)), this, SLOT(ComboBoxSlots(int)));
	connect(comboBox_Month, SIGNAL(currentIndexChanged(int)), this, SLOT(ComboBoxSlots(int)));
}

void DefineCalendar::UpdateYear()
{
	comboBox_Year->clear();
	QDate d1 = this->minimumDate();
	QDate d2 = this->maximumDate();
	for (int i = d1.year(); i<= d2.year(); i++)
	{
		comboBox_Year->addItem(tr("%1").arg(i));
	}
}

void DefineCalendar::SetToday()
{
	QDate curDate = QDate::currentDate();
	int year = curDate.year();
	int month = curDate.month();
	this->setSelectedDate(curDate);
	comboBox_Year->setCurrentText(QString::number(year));
	comboBox_Month->setCurrentIndex(month-1);
	timeEdit->setTime(QTime::currentTime());
}

void DefineCalendar::ClearTime()
{
	timeEdit->setTime(QTime(0,0,0));
}

void DefineCalendar::BtnSlots()
{
	QPushButton *pBtn = qobject_cast<QPushButton *>(sender());
	if (pBtn == pushBtn_Ok)
	{
		QDateTime curDateTime;
		curDateTime.setDate(this->selectedDate());
		curDateTime.setTime(timeEdit->time());
		emit setFinished(curDateTime);
		emit clicked(curDateTime.date());
		emit activated(curDateTime.date());
	}
	else
	{
		if (pBtn == pushBtn_YL)
		{
			int curInt = comboBox_Year->currentIndex()-1;
			if (curInt<=0)
			{
				curInt = 0;
			}
			comboBox_Year->setCurrentIndex(curInt);
		}
		else if (pBtn == pushBtn_YR)
		{
			int curInt = comboBox_Year->currentIndex()+1;
			if (curInt > comboBox_Year->count()-1)
			{
				curInt = comboBox_Year->count()-1;
			}
			comboBox_Year->setCurrentIndex(curInt);
		}
		else if (pBtn == pushBtn_ML)
		{
			int curInt = comboBox_Month->currentIndex()-1;
			if (curInt<=0)
			{
				curInt = 0;
			}
			comboBox_Month->setCurrentIndex(curInt);
		}
		else if (pBtn == pushBtn_MR)
		{
			int curInt = comboBox_Month->currentIndex()+1;
			if (curInt > comboBox_Month->count()-1)
			{
				curInt = comboBox_Month->count()-1;
			}
			comboBox_Month->setCurrentIndex(curInt);
		}
		UpdatePage();
	}
}

void DefineCalendar::ComboBoxSlots( int index )
{
	UpdatePage();
}

void DefineCalendar::UpdatePage()
{
	int nYear = comboBox_Year->currentText().toInt();
	int nMonth = comboBox_Month->currentIndex()+1;
	this->setCurrentPage(nYear,nMonth);
}

void DefineCalendar::CurPageChange( int year, int month )
{
	comboBox_Year->setCurrentText(QString::number(year));
	comboBox_Month->setCurrentIndex(month-1);
}


