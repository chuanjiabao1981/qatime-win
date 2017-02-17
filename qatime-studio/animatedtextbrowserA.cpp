#include "AnimatedTextBrowserA.h"
#include <QFile>
#include <QMovie>
#include <QScrollBar>

AnimatedTextBrowserA::AnimatedTextBrowserA(QWidget *parent)
	:QTextBrowser(parent)
	, m_bTimer(false)
{

}

AnimatedTextBrowserA::AnimatedTextBrowserA(bool changed, QWidget *parent)
	: QTextBrowser(parent)
{
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
	}

	emit sig_scrollDown();
// 	if (m_parent)
// 		m_parent->ScrollDown();
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

void AnimatedTextBrowserA::resizeEvent(QResizeEvent *e)
{
// 	if (this->verticalScrollBar()->isVisible() && m_bTimer)
// 	{
// 		this->resize(width(), height() + 5);
// 	}
}

void AnimatedTextBrowserA::autoHeight()
{
	m_timer->start(200);
}