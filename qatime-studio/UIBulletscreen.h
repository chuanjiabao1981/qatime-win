#ifndef UIBULLETSCREEN_H
#define UIBULLETSCREEN_H

#include <QWidget>
#include "ui_UIBulletscreen.h"
#include "UIBulletset.h"
#include "UIMainwindow.h"

class UIMainWindow;
class UIBulletSet;
class UIBulletScreen : public QWidget
{
	Q_OBJECT

public:
	UIBulletScreen(QWidget *parent = 0);
	~UIBulletScreen();

private:
	Ui::UIBulletScreen ui;
	UIMainWindow*		m_parent;
	UIBulletSet*		m_BulletSet;
	QColor				m_TeacherColor;		//��ʦ������ɫ
	QColor				m_StudentColor;		//ѧ����ɫ
	QColor				m_ContentColor;		//������ɫ
	QTimer*				m_Timer;
	QTimer*				m_delayTimer;		//�ӳ���ʾ

protected:
	virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result); // ���caption
	virtual void paintEvent(QPaintEvent *event);
	virtual void focusInEvent(QFocusEvent *);

private slots:
	void CloseDialog();
	void SetDialog();
	void slot_onCountTimeout();
	void slot_onDelayTimeout();

private:
	void DeleteTalkData();
	void stringToHtml(QString &str, QColor crl);		//��ɫת��
	void stringToHtmlFilter(QString &str);				//htmlת��

public:
	void setMainWindow(UIMainWindow* parent);
	void SetContentColor(QColor color);
	void SetFontSize(int iSize);
	void ReciverStudent(QString name, QString content);
	void ReciverTeacher(QString name, QString content);
	void showDialog();
};

#endif // UIBULLETSCREEN_H
