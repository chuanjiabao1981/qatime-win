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

	QTreeWidget*					m_teacher_treewidget;	//�޸İ���ʦ���
private:
	Ui::UIAuxiliaryPanel ui;

	QString							mRemeberToken;
	QNetworkAccessManager			manager;
	QNetworkReply*					reply;

	UIMainWindow*					m_Parent;
	QString							m_teacherID;			//��ʦID
	QString							m_teacherName;			//��ʦ����
	QString							m_auxiliaryID;			//��ǰ������ID
	QString							m_lessonID;				//��ǰ�γ�ID
	QString							m_url;					//��ǰ��������ַ
	QString							m_CameraUrl;			//��ǰ������ͷ������ַ
	QTreeWidgetItem*				m_pTreeCurrentItem;		//��ǰ�γ̵���item
	bool							m_bPreview;				//��ǰ�Ƿ���ֱ��״̬(������ǰ�γ̵��л�)
	QString							m_lessonName;			//�γ�����
	QTreeWidgetItem*				m_pCurrenDoubTree;		//Ŀǰ˫�����б�
	QString							m_chatID;				//��ǰ�Ự����ID
	QTreeWidgetItem*				m_FirstLessonItem;		//��һ�ڿγ�

signals:
	void emitShowTip();

private slots:
	//TODO zp 
	void on_treeWidget_clicked(QTreeWidgetItem * item, int column);
	void on_DoubleClicked(QTreeWidgetItem* terrWidget, int index);
	void ShowTip();

private:
	void style(QTableWidget* pTableWidget);													//tableWidget��ʽ
	void GetItemColor(QString strStatus, QBrush& brush, QIcon& qIcon);						//����״̬���ƿγ̱���ɫ �� ͼ��

	/** @fn setCourseInfoToTree
	* �������Ϣ
	* @param[QJsonArray]	  �������µ����пγ���Ϣ
	* @param[QString]		  ������ַ
	* @param[QString]		  ��Ӧ��������
	* @param[QTreeWidgetItem] ���ؿγ̵Ŀؼ�
	* @param[QString]		  �Ự����ID
	* @param[QString]	      ������ID
	*/
	void setCourseInfoToTree(QJsonArray courses, QString url, QString tableName, QTreeWidgetItem* pTableWidget,
							 QString chatID, QString courseID, QString cameraUrl);		
	void GetItemTextColor(QString strStatus, QColor& qColor);
public:
	void init();
	void setAuxiliaryInfo(QJsonObject &obj);
	void setRemeberToken(const QString &token);
	void setTeacherID(const QString &teacherID);
	QString getLessonID();													// ��ȡ�γ�ID
	QString getURL();														// ��ȡ����url
	QString getCameraURL();													// ��ȡ����ͷ����url
	QString getCouresID();													// ��ȡ������ID
	QString getChatID();													// ��ȡ��ǰ�ỰID
	void setPreview(bool bPreview);
	QString getLessonName();
	void setParent(UIMainWindow* parent);

	void ergodicItem(QString lessonID,QString courseid);					// �����ڵ�
	void ChangeLessonStatus(QString sLessonID, QString Status);				// �ı䵱ǰ�γ�״̬
	SelLessonStatus SwitchLesson(QTreeWidgetItem* terrWidget);
};

#endif // UIAUXILIARYPANEL_H
