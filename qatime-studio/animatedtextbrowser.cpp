#include "animatedtextbrowser.h"
#include <QFile>
#include <QMovie>

AnimatedTextBrowser::AnimatedTextBrowser(QWidget *parent)
	:QTextBrowser(parent)
{

}

void AnimatedTextBrowser::addAnimation(const QUrl& url, const QString& fileName)
{
	QFile *file = new QFile(fileName);
	if (!file->open(QIODevice::ReadOnly)){

	}

	if (lstUrl.contains(url)){ //ͬһ��gif ʹ��ͬһ��movie
		return;
	}
	else{
		lstUrl.append(url);
	}

	QMovie* movie = new QMovie(this);
	movie->setFileName(fileName);
	movie->setScaledSize(QSize(24, 24));
	movie->setCacheMode(QMovie::CacheNone);


	lstMovie.append(movie);   //��ȡ��movie,�Ա�ɾ��
	urls.insert(movie, url);

	//��֡ʱˢ��
	connect(movie, SIGNAL(frameChanged(int)), this, SLOT(animate(int)));
	movie->start();
	file->close();
	delete file;
}

void AnimatedTextBrowser::animate(int)
{
	// qDebug()<<"hash count is "<<urls.count();
	if (QMovie* movie = qobject_cast<QMovie*>(sender()))
	{

		// qDebug()<<"No."<<lstMovie.indexOf(movie)<<a<<"time is"<<QTime::currentTime();
		document()->addResource(QTextDocument::ImageResource,   //�滻ͼƬΪ��ǰ֡
			urls.value(movie), movie->currentPixmap());

		setLineWrapColumnOrWidth(lineWrapColumnOrWidth()); // ..ˢ����ʾ
	}

}

void AnimatedTextBrowser::mousePressEvent(QMouseEvent *e)
{
	if (Qt::LeftButton == e->button())
	{
		emit colseCalendar();
		emit colseBrow();
	}
}
