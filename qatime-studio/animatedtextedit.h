#ifndef ANIMATEDTEXTEDIT_H
#define ANIMATEDTEXTEDIT_H

#include <QTextEdit>
#include <QMouseEvent>
class AnimatedTextEdit : public QTextEdit
{
	Q_OBJECT

public:
	AnimatedTextEdit(QWidget * p = 0);
	// QTextEdit *textEdit;

	void addAnimation(const QUrl& url, const QString& fileName);


	private slots:
	void animate(int);

signals:
	void colseBrow();  //关闭表情
protected:
	void mousePressEvent(QMouseEvent *e);       //--鼠标按下事件 
private:
	QList<QUrl> lstUrl;
	QList<QMovie *> lstMovie;
	QHash<QMovie*, QUrl> urls;
	
};

#endif // ANIMATEDTEXTEDIT_H
