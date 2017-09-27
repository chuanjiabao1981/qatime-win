#ifndef UIAUXILIARYWND_H
#define UIAUXILIARYWND_H

#include <QWidget>
#include "ui_UIAuxiliarywnd.h"
#include "UIWorkPicThread.h"
#include "UIAuxiliarytoday.h"
#include "UIAuxiliarylist.h"
#include "UIMainNewWindow.h"
#include "UIMenu.h"

#pragma execution_character_set("utf-8")

class UIMenu;
class UIMainNewWindow;
class UIAuxiliaryWnd : public QWidget
{
	Q_OBJECT

public:
	UIAuxiliaryWnd(QWidget *parent = 0);
	~UIAuxiliaryWnd();

	// ���岿�ֹ��б���
	QString								m_TeacherID; // ��ʦID

	// ��갴���϶��Ĳ���
	QPoint								m_startPos;
	QPoint								m_clickPos;
	QPoint								m_WndCurPos;

	typedef enum enum_LESSON_TYPE{
		EN_TODAY_LESSON = 0,                  //!< ����ֱ��
		EN_ALL_LESSON,                       //!< ȫ������
		EN_1V1_LESSON,                       //!< 1��1ֱ��
		EN_EXCLUSIVE_LESSON					 //!< ר����
	}EN_LESSON_TYPE;
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

	// ר���γ̲���
	QWidget*							m_ExclusiveView;
	QVBoxLayout*						m_VerExclusive;
	QSpacerItem*						m_spacerExclusive;

	// ����ͼƬ�߳�
	WorkerPic*							m_pWorker;		// ����ȫ��������ͼƬ
	WorkerPic*							m_pWorker1v1;	// ����1v1������ͼƬ
	WorkerPic						   *m_pWorkerExclusive;	//����ר����ͼƬ

	QMap<QString, UIAuxiliaryList*>		m_mapAuxiliaryChatID;	// �Ự����ID��������
	QMap<QString, UIAuxiliaryList*>		m_mapAuxiliaryCourseID;	// ������ID��  ������
	QMap<QString, UIAuxiliaryList*>		m_map1v1CourseID;		// 1v1 ������ID
	QMap<QString, UIAuxiliaryList*>		m_mapExclusiveCourseID;	// ר���� ������ID
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
	void	sig_StartExclusiveLoading();
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
	void	clickExculsive();

public:
	void	setMainWindow(UIMainNewWindow* parent);
	void	AddTodayAuxiliary(QString lessonName, QString courseID, QString courseName, QString time, QString status, QString lessonid, int mLessonType);
	void    AddAuxiliary(QString picUrl, QString courseName, QString grade, QString teacherName, QString chatID, QString courseID, QString teacherID, QString token,
		QString studentName, std::string AudioPath, QString status, QString url, QString cameraUrl);
	void    Add1v1Auxiliary(QString picUrl, QString courseName, QString grade, QString teacherName, QString chatID, QString courseID, QString teacherID, QString token,
		QString studentName, std::string AudioPath, QString status);
	void	AddExclusive(QString picUrl, QString courseName, QString grade, QString teacherName, QString chatID, QString courseID, QString teacherID, QString token,
		QString studentName, std::string AudioPath, QString status, QString url, QString cameraUrl);

	void	SetToken(QString mRemeberToken);
	QPixmap setTeacherUrl(QString url);
	void	setTeacherName(QString TeacherName);
	void    AddTodayNoLesson(EN_LESSON_TYPE type);
	void    LoadPic();
	void	LoadPic1v1();
	void	LoadPicExclusive();
	void	ReturnAccount();												// �л��˺�
	void    setVersion(QString version);									// ���ð汾��
	void	SetEnvironmental(bool bType, QString homePage);					// ���õ�ǰ����
	void	ReceiverNumber(QString chatID);									// ������Ϣ
	void    ChangeAuxiliaryTodayStatus(QString lessonid, QString status);	// ��ʼֱ����ʱ�򣬸ı��Ͽ�״̬
	void	ProcedureClearLayout(QVBoxLayout *tLayout);						// ���layout
	void	AddWriteBoard(EN_LESSON_TYPE type);								// ��������layoutָ��

};

#endif // UIAUXILIARYWND_H
