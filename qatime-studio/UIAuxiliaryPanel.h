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

	UIMainWindow*					m_Parent;
	QTreeWidget*					m_teacher_treewidget;	//�޸İ���ʦ���
	QString							m_teacherID;			//��ʦID
	QString							m_teacherName;			//��ʦ����
	QString							m_auxiliaryID;			//��ǰ������ID
	QString							m_lessonID;				//��ǰ�γ�ID
	QString							m_url;					//��ǰ��������ַ
	QTreeWidgetItem*				m_pTreeCurrentItem;		//��ǰ�γ̵���item
	bool							m_bPreview;				//��ǰ�Ƿ���ֱ��״̬(������ǰ�γ̵��л�)
	QString							m_lessonName;			//�γ�����
	QTreeWidgetItem*				m_pCurrenDoubTree;		//Ŀǰ˫�����б�
private slots:
	void DrawBack();
	//TODO zp 
	void on_treeWidget_clicked(QTreeWidgetItem * item, int column);
	void on_DoubleClicked(QTreeWidgetItem* terrWidget, int index);
	void on_itemExpanded(QTreeWidgetItem* terrWidget);
	void on_itemCollapsed(QTreeWidgetItem* terrWidget);
	void returnClick();

private:
	void style(QTableWidget* pTableWidget);													//tableWidget��ʽ
	void GetItemColor(QString strStatus, QBrush& brush, QIcon& qIcon);						//����״̬���ƿγ̱���ɫ �� ͼ��
	void setCourseInfoToTree(QJsonArray courses, QString url, QString tableName, QTreeWidgetItem* pTableWidget, bool &bExpand,int num);		//���ÿγ���ϢTODO
	void GetItemTextColor(QString strStatus, QColor& qColor);
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
	void setParent(UIMainWindow* parent);
};

#endif // UIAUXILIARYPANEL_H
