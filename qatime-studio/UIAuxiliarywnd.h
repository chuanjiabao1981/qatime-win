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

	// ��갴���϶��Ĳ���
	QPoint								m_startPos;
	QPoint								m_clickPos;
	QPoint								m_WndCurPos;
private:
	Ui::UIAuxiliaryWnd ui;
	UIMainNewWindow*					m_parent;
	// ȫ����������
	QWidget*							m_mainAllView;
	QVBoxLayout*						m_VerAll;
	QSpacerItem*						m_spacerAll;

	// ���տγ̲���
	QWidget*							m_mainTodayView;
	QVBoxLayout*						m_VerToday;
	QSpacerItem*						m_spacerToday;

	// 1��1��������
	QWidget*							m_main1v1View;
	QVBoxLayout*						m_Ver1v1;
	QSpacerItem*						m_spacer1v1;

	// ����ͼƬ�߳�
	WorkerPic*							m_pWorker;		// ����ȫ��������ͼƬ
	WorkerPic*							m_pWorker1v1;	// ����1v1������ͼƬ

	QMap<QString, UIAuxiliaryList*>		m_mapAuxiliaryChatID;	// �Ự����ID��������
	QMap<QString, UIAuxiliaryList*>		m_mapAuxiliaryCourseID;	// ������ID��  ������
	QMap<QString, UIAuxiliaryToday*>	m_mapAuxiliaryLesson;	// �γ�ID��	   �γ̱�

	UIMenu*								m_UIMenu;				// �����Ĵ���
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
	void	ReturnAccount();						// �л��˺�
	void    setVersion(QString version);			// ���ð汾��
	void	SetEnvironmental(bool bType);			// ���õ�ǰ����
	void	ReceiverNumber(QString chatID);			// ������Ϣ
	void    ChangeAuxiliaryTodayStatus(QString lessonid, QString status);	// ��ʼֱ����ʱ�򣬸ı��Ͽ�״̬
};

#endif // UIAUXILIARYWND_H
