#ifndef _LS_UI_UITextEdit_APP_H_
#define _LS_UI_UITextEdit_APP_H_
#include <QtWidgets/QTextEdit>
#pragma execution_character_set("utf-8")

class UITextEdit : public QTextEdit
{
	Q_OBJECT

public:
	UITextEdit(QWidget *parent = 0);
	~UITextEdit();

	QTimer*			m_timer;
	int				m_i;
private slots:
	void textAreaChanged();
	void timeout();

protected:
	virtual void paintEvent(QPaintEvent *e);
};

#endif