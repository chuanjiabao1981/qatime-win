#ifndef ANIMATEDTEXTBROWSER_H
#define ANIMATEDTEXTBROWSER_H

#include <QTextBrowser>
#include <QMouseEvent>
class AnimatedTextBrowser : public QTextBrowser
{
	Q_OBJECT

public:
	AnimatedTextBrowser(QWidget *parent);
	void addAnimation(const QUrl& url, const QString& fileName);
private slots:
	void animate(int);
signals:
	void colseCalendar();
	void colseBrow();
protected:
	void mousePressEvent(QMouseEvent *e);       //--鼠标按下事件 
private:
	QList<QUrl> lstUrl;
	QList<QMovie *> lstMovie;
	QHash<QMovie*, QUrl> urls;
};

#endif // ANIMATEDTEXTBROWSER_H
