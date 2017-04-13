#ifndef UIBULLETSCREEN_H
#define UIBULLETSCREEN_H

#include <QWidget>
#include "ui_UIBulletscreen.h"
#include "UIBulletset.h"
#include "UIWindowset.h"

class UIWindowSet;
class UIBulletSet;
class UIBulletScreen : public QWidget
{
	Q_OBJECT

public:
	UIBulletScreen(QWidget *parent = 0);
	~UIBulletScreen();

private:
	Ui::UIBulletScreen ui;
	UIWindowSet*		m_parent;
	UIBulletSet*		m_BulletSet;
	QColor				m_TeacherColor;		//��ʦ������ɫ
	QColor				m_StudentColor;		//ѧ����ɫ
	QColor				m_ContentColor;		//������ɫ
	QTimer*				m_Timer;
	QTimer*				m_delayTimer;		//�ӳ���ʾ
	int					m_iFontHeight;		//����߶�
	bool				m_bTalk;
	int					m_iDelay;			//�ӳ�ʱ��
	bool				m_bBtnTrigger;		//�Ƿ�ť��������
	int					m_iTitle;			//���϶��߶�
	QString				m_chatID;			//�ỰID

protected:
	virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result); // ���caption
	virtual void paintEvent(QPaintEvent *event);
	virtual void focusInEvent(QFocusEvent *);
	virtual bool eventFilter(QObject *target, QEvent *event);
	virtual void resizeEvent(QResizeEvent *);						// ���ô���Բ��

private slots:
	void CloseDialog();
	void SetDialog();
	void slot_onCountTimeout();
	void slot_onDelayTimeout();
	void ClickTalk();

private:
	void DeleteTalkData();
	void stringToHtml(QString &str, QColor crl);		//��ɫת��
	void stringToHtmlFilter(QString &str);				//htmlת��

public:
	void setMainWindow(UIWindowSet* parent);
	void SetContentColor(QColor color);
	void SetFontSize(int iSize);
	void ReciverStudent(QString name, QString content, QString chatid);
	void ReciverTeacher(QString name, QString content, QString chatid);
	void showDialog(QString chatid);
	void WidgetHide(bool b);
	bool windowChanged();
	void setBulletTriggerType(bool bType);				//���õ�Ļ��������
	void setTriggerDelay(int iDelay);					//���õ�Ļ������ʱʱ��
};

#endif // UIBULLETSCREEN_H
