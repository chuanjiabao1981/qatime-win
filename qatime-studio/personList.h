#ifndef PERSONLIST_H
#define PERSONLIST_H
#include <QListWidget>
#include <QMenu>
#include <QMouseEvent>
#include <QLineEdit>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "personlistbuddy.h"
//�Զ���QListWidget
class personList : public QListWidget 
{
    Q_OBJECT
public:
    explicit personList(QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *event);		//������¼�
    void contextMenuEvent(QContextMenuEvent*);		//�˵��¼���Ϊ����ʾ�˵�
    void initMenu();//��ʼ���˵�
    QMenu *blankMenu;//����հ��ϵĲ˵�
    QMenu *groupMenu;//������ϵĲ˵�
    QMenu *personMenu;//������ϵĲ˵�
    QMap<QListWidgetItem*,QListWidgetItem*> groupMap;   // ������ - key:�� value:��
	QMap<QString, QListWidgetItem*> allStudents;
    QMap<QListWidgetItem*,bool> isHideMap;//�����жϸ����Ƿ�������
    QLineEdit *groupNameEdit;//������֣���������ʱ����Ҫ�õ�
    QListWidgetItem *currentItem;//��ǰ����
	void addStrdent(QString imagePath, QString stuName, QString ID);
	void initFronUi();
	void initSecUi();
	void setOlineNum(int olineNum, int allNum);//�������������Ľӿ�
	void cleanStudents();		//����б��е�����
signals:
	void signalChickChage(bool,QString, QString);
	void signalAllTalk(bool, QString);		//ȫ������ʱ�� IDΪ��0��
public slots:
    void slotAddGroup();   //�����
    void slotDelGroup();   //ɾ����
    void slotAddBuddy();   //��Ӻ���
    void slotDelBuddy();   //ɾ������
    void slotRename();     //��������
	void findName(QString name);//���Һ���
    void slotRenameEditFshed();//�������

	void chickChage(bool b,QString ID, QString name);	//���Ե�״̬�����ı�
	void stopAllTalk(bool, QString ID); //�������е�ѧ��
private:
	personListBuddy *buddyFirst;
	personListBuddy *buddySec;
    
};
#endif // PERSONLIST_H