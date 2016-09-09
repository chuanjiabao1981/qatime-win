#ifndef UIAUXILIARYPANEL_H
#define UIAUXILIARYPANEL_H

#include <QWidget>
#include "ui_UIAuxiliaryPanel.h"
#include <QTableWidget>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QVBoxLayout>
#include <QToolBox>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#pragma execution_character_set("utf-8")

class UIAuxiliaryPanel : public QWidget
{
	Q_OBJECT

public:
	UIAuxiliaryPanel(QWidget *parent = 0);
	~UIAuxiliaryPanel();

private:
	Ui::UIAuxiliaryPanel ui;

	QString							mRemeberToken;
	QNetworkAccessManager			manager;
	QNetworkReply*					reply;

	QToolBox*						m_teacher_toolBox;		//��ʦ��� 
	QString							m_teacherID;			//��ʦID
	QString							m_teacherName;			//��ʦ����
	QString							m_auxiliaryID;			//��ǰ������ID
	QString							m_lessonID;				//��ǰ�γ�ID
	QString							m_url;					//��ǰ��������ַ
	QTableWidgetItem*				m_pCurrentItem;			//��ǰ�γ�item
	bool							m_bPreview;				//��ǰ�Ƿ���ֱ��״̬(������ǰ�γ̵��л�)
	QString							m_lessonName;			//�γ�����

private slots:
	void ChangedID(int index);
	void DrawBack();
	void SelectionChanged(QTableWidgetItem* pItem);		    //�γ�ѡ��仯

private:
	void style(QTableWidget* pTableWidget);													//tableWidget��ʽ
	void GetItemColor(QString strStatus, QBrush& brush, QIcon& qIcon);						//����״̬���ƿγ̱���ɫ �� ͼ��
	void setCourseInfo(QJsonArray courses, QString url, QString tableName, QTableWidget* pTableWidget, bool &bExpand);		//���ÿγ���Ϣ
public:
	void init();
	void setAuxiliaryInfo(QJsonObject &obj);
	void setRemeberToken(const QString &token);
	void setTeacherID(const QString &teacherID);
	void setTeacherName(const QString &teacherName);
	void setNetworkPic(const QString &szUrl);												// ��ʾ����ͼƬ
	QString getLessonID();
	QString getURL();
	void setPreview(bool bPreview);
	QString getLessonName();
};

#endif // UIAUXILIARYPANEL_H
