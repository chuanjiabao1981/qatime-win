#ifndef UIAUXILIARYWND_H
#define UIAUXILIARYWND_H

#include <QWidget>
#include "ui_UIAuxiliarywnd.h"
#include "UIWorkPicThread.h"
#include "UIAuxiliarytoday.h"
#include "UIAuxiliarylist.h"
#include "UIMainNewWindow.h"
#include "UIMenu.h"

class UIMenu;
class UIMainNewWindow;
class UIAuxiliaryWnd : public QWidget
{
	Q_OBJECT

public:
	UIAuxiliaryWnd(QWidget *parent = 0);
	~UIAuxiliaryWnd();

	// 鼠标按下拖动的操作
	QPoint								m_startPos;
	QPoint								m_clickPos;
	QPoint								m_WndCurPos;
private:
	Ui::UIAuxiliaryWnd ui;
	UIMainNewWindow*					m_parent;
	// 全部辅导布局
	QWidget*							m_mainAllView;
	QVBoxLayout*						m_VerAll;
	QSpacerItem*						m_spacerAll;

	// 今日课程布局
	QWidget*							m_mainTodayView;
	QVBoxLayout*						m_VerToday;
	QSpacerItem*						m_spacerToday;

	// 1对1辅导布局
	QWidget*							m_main1v1View;
	QVBoxLayout*						m_Ver1v1;
	QSpacerItem*						m_spacer1v1;

	// 加载图片线程
	WorkerPic*							m_pWorker;		// 加载全部辅导班图片
	WorkerPic*							m_pWorker1v1;	// 加载1v1辅导班图片

	QMap<QString, UIAuxiliaryList*>		m_mapAuxiliaryChatID;	// 会话窗口ID、辅导班
	QMap<QString, UIAuxiliaryList*>		m_mapAuxiliaryCourseID;	// 辅导班ID、  辅导班
	QMap<QString, UIAuxiliaryToday*>	m_mapAuxiliaryLesson;	// 课程ID，	   课程表

	UIMenu*								m_UIMenu;				// 弹出的窗口
protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual bool eventFilter(QObject *target, QEvent *event);

private:
	void style(QScrollArea *style);
signals:
	void	sig_StartLoading();
	void	sig_Start1v1Loading();
	void	sig_Close();

private slots:
	void	clickAuxiliary(UIAuxiliaryList*);
	void    clickAuxiliaryToday(UIAuxiliaryToday*);
	void    clickToday();
	void    clickAll();
	void    click1V1();
	void	MinDialog();
	void	CloseDialog();
	void    clickPic();

public:
	void	setMainWindow(UIMainNewWindow* parent);
	void	AddTodayAuxiliary(QString lessonName, QString courseID, QString courseName, QString time, QString status, QString lessonid);
	void    AddAuxiliary(QString picUrl, QString courseName, QString grade, QString teacherName, QString chatID, QString courseID, QString teacherID, QString token,
							QString studentName, std::string AudioPath, QString status);
	void    Add1v1Auxiliary(QString picUrl, QString courseName, QString grade, QString teacherName, QString chatID, QString courseID, QString teacherID, QString token,
		QString studentName, std::string AudioPath, QString status);
	void	SetToken(QString mRemeberToken);
	QPixmap setTeacherUrl(QString url);
	void	setTeacherName(QString TeacherName);
	void    AddTodayNoLesson();
	void    LoadPic();
	void	LoadPic1v1();
	void	ReturnAccount();						// 切换账号
	void    setVersion(QString version);			// 设置版本号
	void	SetEnvironmental(bool bType);			// 设置当前环境
	void	ReceiverNumber(QString chatID);			// 接收消息
	void    ChangeAuxiliaryTodayStatus(QString lessonid, QString status);	// 开始直播的时候，改变上课状态
};

#endif // UIAUXILIARYWND_H
