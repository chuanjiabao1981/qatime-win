#ifndef AnimatedTextBrowserAA_H
#define AnimatedTextBrowserAA_H

#include <QTextBrowser>
#include <QMouseEvent>
#include <QTimer>

class AnimatedTextBrowserA : public QTextBrowser
{
	Q_OBJECT

public:
	AnimatedTextBrowserA(QWidget *parent);
	AnimatedTextBrowserA(bool changed, QWidget *parent); //是否需要改变自身高度
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
	virtual void resizeEvent(QResizeEvent *);	//窗口改变
private:
	QList<QUrl> lstUrl;
	QList<QMovie *> lstMovie;
	QHash<QMovie*, QUrl> urls;
private:
	QTimer* m_timer; // 改变自身高度
	bool    m_bTimer;// 是否需要改变自身高度
};

#endif // AnimatedTextBrowserA_H
