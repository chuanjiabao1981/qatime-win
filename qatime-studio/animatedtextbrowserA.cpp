#include "AnimatedTextBrowserA.h"
#include <QFile>
#include <QMovie>
#include <QScrollBar>

AnimatedTextBrowserA::AnimatedTextBrowserA(QWidget *parent)
	:QTextBrowser(parent)
	, m_bTimer(false)
{
	m_bAutoLeft = false;
}

AnimatedTextBrowserA::AnimatedTextBrowserA(bool changed, QWidget *parent)
	: QTextBrowser(parent)
{
	m_bAutoLeft = false;
	m_bar = this->verticalScrollBar();
	m_bTimer = changed;
	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_onTimeout()));
}

void AnimatedTextBrowserA::slot_onTimeout()
{
	m_timer->stop();
	
 	if (this->verticalScrollBar()->isVisible())
	{
		this->setFixedHeight(height() + 100);
		m_timer->start(50);
		this->moveCursor(QTextCursor::End);
 	}
	else
	{
		int h = this->document()->size().rheight();
		this->setFixedHeight(h);
		m_bTimer = false;  // 如果滚动条已消失，则设置为不需要再改变自身状态 add by zbc 20171013
	}

	emit sig_scrollDown();
}

void AnimatedTextBrowserA::addAnimation(const QUrl& url, const QString& fileName)
{
	QFile *file = new QFile(fileName);
	if (!file->open(QIODevice::ReadOnly)){

	}

	if (lstUrl.contains(url)){ //同一个gif 使用同一个movie
		return;
	}
	else{
		lstUrl.append(url);
	}

	QMovie* movie = new QMovie(this);
	movie->setFileName(fileName);
	movie->setScaledSize(QSize(24, 24));
	movie->setCacheMode(QMovie::CacheNone);


	lstMovie.append(movie);   //获取该movie,以便删除
	urls.insert(movie, url);

	//换帧时刷新
	connect(movie, SIGNAL(frameChanged(int)), this, SLOT(animate(int)));
	movie->start();
	file->close();
	delete file;
}

void AnimatedTextBrowserA::animate(int)
{
	// qDebug()<<"hash count is "<<urls.count();
	if (QMovie* movie = qobject_cast<QMovie*>(sender()))
	{

		// qDebug()<<"No."<<lstMovie.indexOf(movie)<<a<<"time is"<<QTime::currentTime();
		document()->addResource(QTextDocument::ImageResource,   //替换图片为当前帧
			urls.value(movie), movie->currentPixmap());

		setLineWrapColumnOrWidth(lineWrapColumnOrWidth()); // ..刷新显示
	}

}

void AnimatedTextBrowserA::mousePressEvent(QMouseEvent *e)
{
	if (Qt::LeftButton == e->button())
	{
		emit colseCalendar();
		emit colseBrow();
	}
}

void AnimatedTextBrowserA::autoHeight()
{
	m_timer->start(200);
}

void AnimatedTextBrowserA::paintEvent(QPaintEvent *e)
{
	QTextBrowser::paintEvent(e);

	if (m_bAutoLeft)
	{
		int fontSize = this->fontMetrics().width(toPlainText());//获取之前设置的字符串的像素大小
		if (fontSize + 10 >= width())
			this->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		else
			this->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	}

	if (!m_bTimer)
		return;

	static int iCount = 0;
	if (m_bar->isVisible())
	{
		this->setFixedHeight(height() + 100);
		this->moveCursor(QTextCursor::End);
	}
	else
	{
		int h = this->document()->size().rheight();
		this->setFixedHeight(h);
	}
}

void AnimatedTextBrowserA::AutoLeftOrEnter()
{
	m_bAutoLeft = true;
}