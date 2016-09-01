#ifndef UIMAINWINDOW_H
#define UIMAINWINDOW_H

#include <QWidget>
#include "ui_uimainwindow.h"
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "user.h"
#include "UIAuxiliary.h"
#include "UILesson.h"
#include <QMouseEvent>
#include "nlss_type.h"
#include "nlss_api.h"
#include <QTimer>
#include "UIViedeo.h"

#pragma execution_character_set("utf-8")
#define STARTLS_ASYNC

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
	QPoint							last;

	UIAuxiliary*					m_AuxiliaryInfo;	// ��������Ϣ����
	UILesson*						m_LessonInfo;		// �γ���Ϣ����
	UIViedeo*						m_VideoInfo;		// ֱ������
	QString							m_teacherID;		// ��ʦID

	_HNLSSERVICE					m_hNlssService;

private slots :
	void MinDialog();							// ��С���Ի���
	void CloseDialog();							// �رնԻ���
	void ShowAuxiliary();						// ��ʾ������
	void ShowLesson();							// ��ʾ�γ�
	void setNetworkPic(const QString &szUrl);	// ��ʾ����ͼƬ
	void Expansion();							// �������
	void slot_startOrStopLiveStream();			// ֱ��

protected:
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);

public:
	void setTeacherInfo(QJsonObject& data);					// ������ʦ��Ϣ
	void setRemeberToken(const QString &name);				// ����Token
	void AuxiliaryRequestFinished();						// ������http����
	void LessonRequestFinished();							// �γ�http����
	void SetNlsService(_HNLSSERVICE hNlssService);
};

#endif // UIMAINWINDOW_H
