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
#include "UIMainwindow.h"

#pragma execution_character_set("utf-8")

class UIMainWindow;

enum SelLessonStatus {
	Finished,
	Closed,
	Ready,
	NullStatus
};
class UIAuxiliaryPanel : public QWidget
{
	Q_OBJECT

public:
	UIAuxiliaryPanel(QWidget *parent = 0);
	~UIAuxiliaryPanel();

	QTreeWidget*					m_teacher_treewidget;	//修改版老师面板
private:
	Ui::UIAuxiliaryPanel ui;

	QString							mRemeberToken;
	QNetworkAccessManager			manager;
	QNetworkReply*					reply;

	UIMainWindow*					m_Parent;
	QString							m_teacherID;			//老师ID
	QString							m_teacherName;			//老师名字
	QString							m_auxiliaryID;			//当前辅导班ID
	QString							m_lessonID;				//当前课程ID
	QString							m_url;					//当前的推流地址
	QString							m_CameraUrl;			//当前的摄像头推流地址
	QTreeWidgetItem*				m_pTreeCurrentItem;		//当前课程的树item
	bool							m_bPreview;				//当前是否是直播状态(决定当前课程的切换)
	QString							m_lessonName;			//课程名字
	QTreeWidgetItem*				m_pCurrenDoubTree;		//目前双击的列表
	QString							m_chatID;				//当前会话窗口ID
	QTreeWidgetItem*				m_FirstLessonItem;		//上一节课程

signals:
	void emitShowTip();

private slots:
	//TODO zp 
	void on_treeWidget_clicked(QTreeWidgetItem * item, int column);
	void on_DoubleClicked(QTreeWidgetItem* terrWidget, int index);
	void ShowTip();

private:
	void style(QTableWidget* pTableWidget);													//tableWidget样式
	void GetItemColor(QString strStatus, QBrush& brush, QIcon& qIcon);						//根据状态绘制课程背景色 和 图标

	/** @fn setCourseInfoToTree
	* 添加树信息
	* @param[QJsonArray]	  辅导班下的所有课程信息
	* @param[QString]		  推流地址
	* @param[QString]		  对应的树名称
	* @param[QTreeWidgetItem] 加载课程的控件
	* @param[QString]		  会话窗口ID
	* @param[QString]	      辅导班ID
	*/
	void setCourseInfoToTree(QJsonArray courses, QString url, QString tableName, QTreeWidgetItem* pTableWidget,
							 QString chatID, QString courseID, QString cameraUrl);		
	void GetItemTextColor(QString strStatus, QColor& qColor);
public:
	void init();
	void setAuxiliaryInfo(QJsonObject &obj);
	void setRemeberToken(const QString &token);
	void setTeacherID(const QString &teacherID);
	QString getLessonID();													// 获取课程ID
	QString getURL();														// 获取推流url
	QString getCameraURL();													// 获取摄像头推流url
	QString getCouresID();													// 获取辅导班ID
	QString getChatID();													// 获取当前会话ID
	void setPreview(bool bPreview);
	QString getLessonName();
	void setParent(UIMainWindow* parent);

	void ergodicItem(QString lessonID,QString courseid);					// 遍历节点
	void ChangeLessonStatus(QString sLessonID, QString Status);				// 改变当前课程状态
	SelLessonStatus SwitchLesson(QTreeWidgetItem* terrWidget);
};

#endif // UIAUXILIARYPANEL_H
