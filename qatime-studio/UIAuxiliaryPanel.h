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
#include <QTreeWidget>
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

	QTreeWidget*					m_teacher_treewidget;	//修改版老师面板
	QString							m_teacherID;			//老师ID
	QString							m_teacherName;			//老师名字
	QString							m_auxiliaryID;			//当前辅导班ID
	QString							m_lessonID;				//当前课程ID
	QString							m_url;					//当前的推流地址
	QTreeWidgetItem*				m_pTreeCurrentItem;		//当前课程的树item
	bool							m_bPreview;				//当前是否是直播状态(决定当前课程的切换)
	QString							m_lessonName;			//课程名字
	QTreeWidgetItem*				m_pCurrenDoubTree;		//目前双击的列表
private slots:
	void DrawBack();
	//TODO zp 
	void on_treeWidget_clicked(QTreeWidgetItem * item, int column);
	void on_DoubleClicked(QTreeWidgetItem* terrWidget, int index);
private:
	void style(QTableWidget* pTableWidget);													//tableWidget样式
	void GetItemColor(QString strStatus, QBrush& brush, QIcon& qIcon);						//根据状态绘制课程背景色 和 图标
	void setCourseInfoToTree(QJsonArray courses, QString url, QString tableName, QTreeWidgetItem* pTableWidget, bool &bExpand,int num);		//设置课程信息TODO
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
