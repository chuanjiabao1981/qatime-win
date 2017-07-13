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
	void AutoLeftOrEnter();
private slots:
	void animate(int);
	void slot_onTimeout();
signals:
	void colseCalendar();
	void colseBrow();
	void sig_scrollDown();
protected:
	void mousePressEvent(QMouseEvent *e);       //--鼠标按下事件 
	virtual void paintEvent(QPaintEvent *e);
private:
	QList<QUrl> lstUrl;
	QList<QMovie *> lstMovie;
	QHash<QMovie*, QUrl> urls;
	QScrollBar* m_bar;
private:
	QTimer* m_timer; // 改变自身高度
	bool    m_bTimer;// 是否需要改变自身高度
	bool	m_bAutoLeft;//当超过一行时，自动判断左对齐或居中对齐
};

#endif // AnimatedTextBrowserA_H
