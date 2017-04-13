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
	QColor				m_TeacherColor;		//老师本人颜色
	QColor				m_StudentColor;		//学生颜色
	QColor				m_ContentColor;		//内容颜色
	QTimer*				m_Timer;
	QTimer*				m_delayTimer;		//延迟显示
	int					m_iFontHeight;		//字体高度
	bool				m_bTalk;
	int					m_iDelay;			//延迟时间
	bool				m_bBtnTrigger;		//是否按钮触发背景
	int					m_iTitle;			//可拖动高度
	QString				m_chatID;			//会话ID

protected:
	virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result); // 添加caption
	virtual void paintEvent(QPaintEvent *event);
	virtual void focusInEvent(QFocusEvent *);
	virtual bool eventFilter(QObject *target, QEvent *event);
	virtual void resizeEvent(QResizeEvent *);						// 设置窗口圆角

private slots:
	void CloseDialog();
	void SetDialog();
	void slot_onCountTimeout();
	void slot_onDelayTimeout();
	void ClickTalk();

private:
	void DeleteTalkData();
	void stringToHtml(QString &str, QColor crl);		//颜色转换
	void stringToHtmlFilter(QString &str);				//html转换

public:
	void setMainWindow(UIWindowSet* parent);
	void SetContentColor(QColor color);
	void SetFontSize(int iSize);
	void ReciverStudent(QString name, QString content, QString chatid);
	void ReciverTeacher(QString name, QString content, QString chatid);
	void showDialog(QString chatid);
	void WidgetHide(bool b);
	bool windowChanged();
	void setBulletTriggerType(bool bType);				//设置弹幕触发类型
	void setTriggerDelay(int iDelay);					//设置弹幕触发延时时间
};

#endif // UIBULLETSCREEN_H
