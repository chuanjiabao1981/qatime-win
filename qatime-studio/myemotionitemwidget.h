#ifndef MYEMOTIONITEMWIDGET_H
#define MYEMOTIONITEMWIDGET_H

#include <QLabel>

class MyEmotionItemWidget : public QLabel
{
	Q_OBJECT

public:
	MyEmotionItemWidget(QString fileName, QSize emotionMoiveSize);
	~MyEmotionItemWidget();

private:
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
protected:
	void mousePressEvent(QMouseEvent *event);
signals:
	void imageName(QString);
private:
	QString m_fileName;
	
};

#endif // MYEMOTIONITEMWIDGET_H
