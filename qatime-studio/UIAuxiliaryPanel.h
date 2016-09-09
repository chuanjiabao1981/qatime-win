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
	QString							m_url;					//当前的推流地址
	QTableWidgetItem*				m_pCurrentItem;			//当前课程item
	bool							m_bPreview;				//当前是否是直播状态(决定当前课程的切换)
	QString							m_lessonName;			//课程名字

private slots:
	void ChangedID(int index);
	void DrawBack();
	void SelectionChanged(QTableWidgetItem* pItem);		    //课程选择变化

private:
	void style(QTableWidget* pTableWidget);													//tableWidget样式
	void GetItemColor(QString strStatus, QBrush& brush, QIcon& qIcon);						//根据状态绘制课程背景色 和 图标
	void setCourseInfo(QJsonArray courses, QString url, QString tableName, QTableWidget* pTableWidget, bool &bExpand);		//设置课程信息
public:
	void init();
	void setAuxiliaryInfo(QJsonObject &obj);
	void setRemeberToken(const QString &token);
	void setTeacherID(const QString &teacherID);
	void setTeacherName(const QString &teacherName);
	void setNetworkPic(const QString &szUrl);												// 显示网络图片
	QString getLessonID();
	QString getURL();
	void setPreview(bool bPreview);
	QString getLessonName();
};

#endif // UIAUXILIARYPANEL_H
