#ifndef UIMAINWINDOW_H
#define UIMAINWINDOW_H

#include <QWidget>
#include "ui_uimainwindow.h"
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "user.h"
#include "UIOtherApp.h"
#include "UIVideo.h"
#include <windows.h>
#include "UIAuxiliaryPanel.h"
#include "UIAudiochange.h"
#include "UIVideochange.h"
#include "UIRatio.h"
#include "UIChatRoom.h"
#include "loginwindow.h"
#include "UILessontable.h"
#include "UIMenuTool.h"
#include "UICamera.h"
#include <QMutex>
#include "Rangecapture.h"
#include "UISetparam.h"
#include "UIHoverwindow.h"
#include "UISeelivewnd.h"
#include "UIScreentip.h"
#include "UIPersonNum.h"
#pragma execution_character_set("utf-8")
#define STARTLS_ASYNC

class LoginWindow;
class UIAuxiliaryPanel;
class UIAudioChange;
class UIVideoChange;
class UIRatio;
class UILessonTable;
class UIVideo;
class UICamera;
class RangeCapture;
class UISetParam;
class UIHoverWindow;
class UISeeLiveWnd;
class UIScreenTip;
class UIPersonNum;

struct StructVideo
{
	std::string url;
	HWND hwnd;
};
struct MEDIA_DEVICE_DRIVE_INFO1
{
	char* device_path_;
	char* friendly_name_;
};

class UIMainWindow : public QWidget
{
	Q_OBJECT

public:
	UIMainWindow(QWidget *parent = 0);
	~UIMainWindow();

private:
	Ui::UIMainWindow				ui;
	QString							mRemeberToken;
	QNetworkAccessManager			manager;
	QNetworkReply*					reply;


	UIChatRoom*						m_charRoom;			//������
	LoginWindow*					m_LoginWindow;

	UIAuxiliaryPanel*				m_AuxiliaryPanel;	// ��ʦ�γ����
	UIOtherApp*					    m_OtherAppInfo;		// ����Ӧ����Ϣ����
	UILessonTable*					m_LessonTable;		// �γ̱���
	UIVideo*						m_VideoInfo;		// ֱ������
	UICamera*						m_CameraInfo;		// ����ͷ����
	UISetParam*						m_SetParam;			// ���ò�������
	UIHoverWindow*					m_HoverWnd;			// ��������
	UISeeLiveWnd*					m_SeeLiveWnd;		// ֱ�����ڷŴ�
	UIScreenTip*					m_ScreenTip;		// ȫ����ʾ��
	UIPersonNum*					m_PersonNum;		// �ۿ�����

	QString							m_teacherID;		// ��ʦID
	QTimer*							m_CountTimer;		// ��ʱ��
	QTimer*							m_HeartTimer;		// ����
	QTimer*							m_ShowVideoTimer;	// ��ʾ��Ƶ
	QTimer*							m_ShowCameraTimer;	// ��ʾ����ͷ
	INT64							m_iTimerCount;		// ��ʱ������
	bool							m_bOtherApp;		// ��������Ӧ��

	UIAudioChange*					m_AudioChangeInfo;	// ��˷�ѡ�񴰿�
	UIVideoChange*					m_VideoChangeInfo;	// ����ͷѡ�񴰿�
	UIRatio*						m_RatioChangeInfo;	// �ֱ��ʴ���
	HWND							m_VideoWnd;			// ��Ƶ���ھ��
	HWND							m_CameraWnd;		// ����ͷ���ھ��
	QString							m_liveToken;		// ֱ������ʱ��Ҫ����ʼֱ��ʱ����token��

	UIMenuTool*						m_MenuTool;			// ���߰�ť
	QTimer*							m_tempTimers;		// ��ʱ�Ķ�ʱ��������ǵùص�
	QTimer*							m_ScreenTipTimer;	// ����ȫ����ʾ
	RangeCapture*					m_RangeCapture;		// ����ץȡ����
	int a;
	int chat_X;	//����λ��x
	int chat_Y; //����λ��y
	int video_Width; //��������С
	int video_Heigth;//��������С
	int chat_Width;//��������С���
	int chat_Heigth;//��������С �߶�
	QMutex	m_mutex;
	bool  showMax;

	int								m_SwichScreenTimerId;	// ������ʱ��
	int                             m_ShowChatRoomTimerId;	// ��ʾ�Ự���ڶ�ʱ��
	QRect							m_videoRect;
	QRect							m_cameraRect;
	QWidget*						m_VideoOrCamera;		// ��ʼ����װ�
	QWidget*						m_CameraOrVideo;		// ��ʼ��������ͷ

	// ��갴���϶��Ĳ���
	QPoint							m_startPos;
	QPoint							m_clickPos;
	QPoint							m_WndCurPos;
private slots :
	void MinDialog();									// ��С���Ի���
	void CloseDialog();									// �رնԻ���
	void slot_startOrStopLiveStream();					// ��ʼֱ��
	void VideoStatus(int iStatus);						// ��Ƶ״̬��ֱ������ͣ�������Ŀ��ƣ�
	void AudioStatus(int iStatus);						// ����״̬��ֱ������ͣ�������Ŀ��ƣ�
	void SwitchScreenStatus(int iStatus);					// �л�ȫ����ƵԴ
	void slot_onCountTimeout();							// ��ʱ�� �ı�ֱ��ʱ��
	void slot_onHeartTimeout();							// 5��һ�Σ���������
	void slot_onTempTimeout();							// ��ʱӦ��
	void slot_ScreenTipTimeout();
	void SetParamWindow();								// ���ò�������
	void clickLessonList();								// �����γ̱�
	void setVideoPos();									// ������Ƶλ��
	void setCameraPos();								// ������Ƶͷλ��
	void HideSideScreen();								// ��������ͷ��Ļ
	void WhiteBoard();									// �л��װ�
	void returnClick();									// �л��˺�

private:
	void setNetworkPic(const QString &szUrl);			// ��ʾ����ͼƬ

protected:
	virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result); // ���caption
	virtual void resizeEvent(QResizeEvent *);			// ���ô���Բ��
	virtual void paintEvent(QPaintEvent *event);
	virtual void closeEvent(QCloseEvent *e);
	virtual void focusInEvent(QFocusEvent *e);
	virtual void timerEvent(QTimerEvent *event);
	virtual bool eventFilter(QObject *target, QEvent *event);
	virtual void enterEvent(QEvent *);
	virtual void leaveEvent(QEvent *);

public:
	void setTeacherInfo(QJsonObject& data);					// ������ʦ��Ϣ
	void setAutoTeacherInfo(QString teacherID, QString teacherName, 
		 QString teacherUrl, QString accid, QString token); // ��ס����ʱʹ�õķ���
	void setRemeberToken(const QString &name);				// ����Token
	void AuxiliaryRequestFinished();						// ������http����
	void LessonRequestFinished();							// �γ�http����
	void EnumAvailableMediaDevices();						// ö���豸
	void SendStartLiveHttpMsg();							// ������������ֱ����ʼ��Ϣ
	void SendStopLiveHttpMsg(bool bConnect=true);			// ������������ֱ��ֹͣ��Ϣ
	void SendHeartBeatHttpMsg();							// ������������ֱ��������Ϣ��5����һ�Σ�
	void ShowAuxiliary();									// ��ʾ������
	void setAudioChangeIndex(QString path);					//  �ı���˷�
	void setVideoChangeIndex(QString path);					//  �ı���Ƶͷ
	void setRatioChangeIndex(int index);					//  �ı�ֱ���


	void HideOtherUI( QWidget* self=NULL);					//  ������������
	void setLoginWindow(LoginWindow* parent);
	void setCurChatRoom(QString chatID, QString courseid);	// ����������
	void RequestKey();										// ��������key
	void returnKey();										// ����key
	void RequestMember();									// ����Ⱥ��Ա
	void returnMember();									// ����Ⱥ��Ա
	void setVideoLesson(QString lessonName);				// ������Ƶ����ʾ�Ŀγ�
	void showChatRoomWnd();									// ��ʾ����Ự
	void LessonTable_Auxiliary(QString sLessonID, QString sCourseID); //�̱���ѡ��γ̡���������������
	void SendVideoMsg(UINT iMsg);							// ��win_video������Ϣ
	void SendCameraMsg(UINT iMsg);							// ��camera_video������Ϣ
	void FinishStartLive();									// ���ؿ�ʼֱ�������token
	void FinishStopLive();									// ���ؽ���ֱ�������״̬
	void RequestError(QJsonObject& error, bool bTrue=true);	// ���󷵻ش�����ʾ
	void SendRequestStatus();
	void RequestStatus();
	void SendChangeStatusMsg(QString id);
	void returnChangeStatus();	
	void setPausedBtn();									// �ı�ֱ����ťͼƬ
	void LivePage();										// �л���ֱ��ҳ
	void setComeBack();										// �ӷŴ�Ĵ��ڷ��ص�С����
};

#endif // UIMAINWINDOW_H
