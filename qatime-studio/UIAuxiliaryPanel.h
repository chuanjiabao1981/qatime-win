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

	QToolBox*						m_teacher_toolBox;		//老师面板 
	QString							m_teacherID;			//老师ID
	QString							m_teacherName;			//老师名字
	QString							m_auxiliaryID;			//当前辅导班ID
	QString							m_lessonID;				//当前课程ID

private slots:
	void ChangedID(int index);
	void DrawBack();

private:
	void style(QTableWidget* pTableWidget);													//tableWidget样式
	void GetItemColor(QString strStatus, QBrush& brush, QIcon& qIcon);						//根据状态绘制课程背景色 和 图标
	void setCourseInfo(QJsonArray courses, QTableWidget* pTableWidget, bool &bExpand);		//设置课程信息
public:
	void init();
	void setAuxiliaryInfo(QJsonObject &obj);
	void setRemeberToken(const QString &token);
	void setTeacherID(const QString &teacherID);
	void setTeacherName(const QString &teacherName);
	void setNetworkPic(const QString &szUrl);												// 显示网络图片
	QString getLessonID();
};

#endif // UIAUXILIARYPANEL_H
