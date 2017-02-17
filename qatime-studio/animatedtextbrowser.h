#ifndef ANIMATEDTEXTBROWSER_H
#define ANIMATEDTEXTBROWSER_H

#include <QTextBrowser>
#include <QMouseEvent>
#include <QTimer>
class AnimatedTextBrowser : public QTextBrowser
{
	Q_OBJECT

public:
	AnimatedTextBrowser(QWidget *parent);
	void addAnimation(const QUrl& url, const QString& fileName);
	void autoHeight();
private slots:
	void animate(int);
	void slot_onTimeout();
signals:
	void colseCalendar();
	void colseBrow();
	void sig_scrollDown();
protected:
	void mousePressEvent(QMouseEvent *e);       //--鼠标按下事件 
private:
	QList<QUrl> lstUrl;
	QList<QMovie *> lstMovie;
	QHash<QMovie*, QUrl> urls;
	QTimer* m_timer;
};

#endif // ANIMATEDTEXTBROWSER_H
