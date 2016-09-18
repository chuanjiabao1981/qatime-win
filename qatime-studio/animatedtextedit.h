#ifndef ANIMATEDTEXTEDIT_H
#define ANIMATEDTEXTEDIT_H

#include <QTextEdit>

class AnimatedTextEdit : public QTextEdit
{
	Q_OBJECT

public:
	AnimatedTextEdit(QWidget * p = 0);
	// QTextEdit *textEdit;

	void addAnimation(const QUrl& url, const QString& fileName);


	private slots:
	void animate(int);


private:
	QList<QUrl> lstUrl;
	QList<QMovie *> lstMovie;
	QHash<QMovie*, QUrl> urls;
	
};

#endif // ANIMATEDTEXTEDIT_H
